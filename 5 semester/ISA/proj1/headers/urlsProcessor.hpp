/**
 * VUT FIT ISA 2022 project - feed reader.
 *
 * @file Urls processing
 * @author Dmytro Sadovskyi <xsadov06@stud.fit.vutbr.cz>
 */
#ifndef URLSPROCESSOR_HPP
#define URLSPROCESSOR_HPP
#include <string>
#include <vector>

typedef struct processedUrl
{
	std::string *authority;
	std::string *path;
    bool https;
}processedUrl;


class urlsProcessor
{
public:
	/**
	 * Object destructor
	 */
	~urlsProcessor();

    /**
	 * Parse urls from file
	 * @param feedFile location of feed file
	 * @return True if success, otherwise - false
	 */
	bool urlsFromFile(std::string feedFile);

    /**
	 * Parse url
	 * @param line source url
	 * @return True if success, otherwise - false
	 */
	bool url(std::string line);

	/**
	 * @return List of processed urls
	 */
    std::vector<processedUrl> getUrlsList();

private:
    /**
	 * List of processed urls
	 */
	std::vector<processedUrl> urlList;
	
	/**
	 * Proces given url
	 * @param line source url
	 * @return True if success, otherwise - false
	 */
    bool processURL(std::string line);

};
#endif //URLSPROCESSOR_HPP