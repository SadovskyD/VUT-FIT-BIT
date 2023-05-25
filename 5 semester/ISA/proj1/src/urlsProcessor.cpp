/**
 * VUT FIT ISA 2022 project - feed reader.
 *
 * @file Urls processing
 * @author Dmytro Sadovskyi <xsadov06@stud.fit.vutbr.cz>
 */

#include <fstream>

#include"../headers/urlsProcessor.hpp"
#include <regex>

urlsProcessor::~urlsProcessor()
{

}

bool urlsProcessor::urlsFromFile(std::string feedFile)
{
	std::ifstream file(feedFile);
	if (!file.is_open())
	{
		fprintf(stderr,"Unable to open file '%s'.", feedFile.c_str());
		return false;
	}

	std::string line;
	while (getline(file, line))
	{
		if (line.empty() || !line.compare(0, 1, "#"))
		{
			continue;
		}
		processURL(line);
	}
	file.close();

	return true;
}


bool urlsProcessor::url(std::string line)
{
    processURL(line);
    return true;
}

bool urlsProcessor::processURL(std::string line)
{
    processedUrl url;
    
    std::regex regex( R"(^(https?)://([^/?#:]+)(:([0-9]+))?(.*)$)", std::regex::extended);
    std::smatch matches;
    
    if (!std::regex_match(line, matches, regex)) return false;

    if (matches[1] == "http") url.https=false;
    else url.https=true;
    url.path = new std::string(matches[5]);
    url.authority=new std::string(matches[2]);
    if (std::string (matches[4]).empty())
    {
        if (url.https) *url.authority+=":443";
        else *url.authority+=":80";
    }
    else *url.authority+=matches[3];
    urlList.push_back(url);
    return true;
}

std::vector<processedUrl> urlsProcessor::getUrlsList(){
    return urlList;
}