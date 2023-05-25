/**
 * VUT FIT ISA 2022 project - feed reader.
 *
 * @file Main file
 * @author Dmytro Sadovskyi <xsadov06@stud.fit.vutbr.cz>
 */
#include <cstdlib>
#include <iostream> //REMOVE
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <regex>

#include "../headers/argsParser.hpp"
#include "../headers/urlsProcessor.hpp"
#include "../headers/feedParser.hpp"

#define READ_BUFFER_SIZE 4096

int main(int argc, char *argv[])
{
    argsParser argsParser;

    if (!argsParser.getArguments(argc, argv)) return EXIT_FAILURE;
    if (argsParser.getHelp()) return EXIT_SUCCESS;

    urlsProcessor urlsProcessor;
    if (argsParser.getUrl()!=NULL)
    {
        if (!urlsProcessor.url(*argsParser.getUrl()))
        {
            fprintf(stderr, "Error in url");
            return EXIT_FAILURE;
        }
    }
    else
    {
        if (!urlsProcessor.urlsFromFile(*argsParser.getFeedFile())) 
        {
            fprintf(stderr, "Error in url");
            return EXIT_FAILURE;
        }
    }
    std::vector<processedUrl> urlList = urlsProcessor.getUrlsList();

    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();

    for (const auto &url : urlList)
	{
        urlList.pop_back();
        BIO *bio = nullptr;
		SSL_CTX *ctx = nullptr;

		if (!url.https)
		{
			bio = BIO_new_connect(url.authority->c_str());
		}
        else
        {
            ctx = SSL_CTX_new(SSLv23_client_method());
            const char* certDir = argsParser.getCertDir() ? argsParser.getCertDir()->c_str() : nullptr;
            const char* certFile = argsParser.getCertFile() ? argsParser.getCertFile()->c_str() : nullptr;
            int verify = 0;
            if (!certDir && !certFile)
            {
                verify = SSL_CTX_set_default_verify_paths(ctx);
            }
            else
            {
                verify = SSL_CTX_load_verify_locations(ctx,	certFile, certDir);
            }
            if(!verify)
            {
                fprintf(stderr, "Error during sertificate verefication: %s%s\n", url.authority->c_str(), url.path->c_str());
                SSL_CTX_free(ctx);
                continue;
            }
            bio = BIO_new_ssl_connect(ctx);
        }
        if (!bio)
        {
            fprintf(stderr, "Filled connection to: %s%s\n", url.authority->c_str(), url.path->c_str());
            if (bio) BIO_free_all(bio);
		    if (ctx) SSL_CTX_free(ctx);
            continue;
        }
        SSL *ssl = nullptr;
		if (url.https)
		{
			BIO_get_ssl(bio, &ssl);
			SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
			BIO_set_conn_hostname(bio, url.authority->c_str());
		}

		if (BIO_do_connect(bio) <= 0)
		{   
			fprintf(stderr,"Connection failed:%s%s\n", url.authority->c_str(), url.path->c_str());
			if (bio) BIO_free_all(bio);
		    if (ctx) SSL_CTX_free(ctx);
            continue;
		}

		if (ssl && SSL_get_verify_result(ssl) != X509_V_OK)
		{
			fprintf(stderr, "Error during sertificate verefication: %s%s\n", url.authority->c_str(), url.path->c_str());
			if (bio) BIO_free_all(bio);
		    if (ctx) SSL_CTX_free(ctx);
            continue;
		}

        std::string request(
			"GET " + *url.path + " HTTP/1.0\r\n"
			"Host: " + *url.authority + "\r\n"
			"Connection: Close\r\n"
			"User-Agent: Mozilla/5.0\r\n"
			"\r\n");
        
        auto writeDataSize = static_cast<int>(request.size());
		bool firstWrite = true, writeDone = false;
		while (firstWrite || BIO_should_retry(bio))
		{
			firstWrite = false;
			if (BIO_write(bio, request.c_str(), writeDataSize))
			{
				writeDone = true;
				break;
			}
		}
		if (!writeDone)
		{
			fprintf(stderr, "BIO_write error.");
			if (bio) BIO_free_all(bio);
		    if (ctx) SSL_CTX_free(ctx);
            continue;
		}

		char responseBuffer[READ_BUFFER_SIZE] = {'\0'};
		std::string response;
		int readResult = 0;
		do
		{
			bool firstRead = true, readDone = false;
			while (firstRead || BIO_should_retry(bio))
			{
				firstRead = false;
				readResult =
					BIO_read(bio, responseBuffer, READ_BUFFER_SIZE - 1);
				if (readResult >= 0)
				{
					if (readResult > 0)
					{
						responseBuffer[readResult] = '\0';
						response += responseBuffer;
					}

					readDone = true;
					break;
				}
			}
			if (!readDone)
			{
                fprintf(stderr, "BIO_write error.");
                if (bio) BIO_free_all(bio);
                if (ctx) SSL_CTX_free(ctx);
                return EXIT_FAILURE;
			}
		}
		while (readResult != 0);
        
        size_t endOfHeaderPosition = response.find("\r\n\r\n");
        if (endOfHeaderPosition == std::string::npos)
        {
            endOfHeaderPosition = response.find("\n\n");
            if (endOfHeaderPosition == std::string::npos)
            {
                if (bio) BIO_free_all(bio);
                if (ctx) SSL_CTX_free(ctx);
                fprintf(stderr, "Invalid HTTP response from %s%s\n",url.authority->c_str(), url.path->c_str());
                continue;
            }
            endOfHeaderPosition += 2;
        }
        else
        {
            endOfHeaderPosition += 4;
        }
        std::string responseBody;
        std::string responseHead = response.substr(0, endOfHeaderPosition);
        std::regex regex(
            R"(^HTTP/[0-9]\.[0-9] 2[0-9][0-9].*$)",
            std::regex::extended
        );
        if (!std::regex_match(responseHead, regex))
        {
			if (bio) BIO_free_all(bio);
		    if (ctx) SSL_CTX_free(ctx);
            fprintf(stderr, "Invalid HTTP response from %s%s\n",url.authority->c_str(), url.path->c_str());
            continue;
        }

        responseBody = response.substr(endOfHeaderPosition);
        std::string stru=*url.authority+*url.path;
		if (!feedParser::parseFeed(responseBody, stru, &argsParser))
		{
            if (bio) BIO_free_all(bio);
            if (ctx) SSL_CTX_free(ctx);
            continue;
		}

		if (bio) BIO_free_all(bio);
		if (ctx) SSL_CTX_free(ctx);
        if ((!urlList.empty()) && !(argsParser.getAssociatedUrl() || argsParser.getAuthor() || argsParser.getTime())) printf("\n");
	}

	return EXIT_SUCCESS;
}
