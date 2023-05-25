/**
 * VUT FIT ISA 2022 project - feed reader.
 *
 * @file XML file processing
 * @author Dmytro Sadovskyi <xsadov06@stud.fit.vutbr.cz>
 */

#ifndef FEEDPARSER_HPP
#define FEEDPARSER_HPP
#include <string>
#include <libxml/parser.h>

#include "argsParser.hpp"

class feedParser
{
public:
    /**
	 * Parses XML feed.
	 *
	 * @param xml Data to be parsed.
     * @param url Feed URL.
	 * @param argsParser ArgumentProcessor object with processed arguments.
	 * @return True if XML has been parsed successfully, otherwise - false.
	 */
    static bool parseFeed(std::string xml, std::string url, argsParser *argsParser);
    
    /**
	 * Signature of the function to use when there is an error and
	 * no parsing or validity context available.
	 *
	 * @param ctx A parsing context.
	 * @param msg The message.
	 * @param ... The extra arguments.
	 */
    static void GEFunc(void *ctx, const char *msg, ...);
    
    /**
	 * Signature of the function to use when there is an error and
	 * the module handles the new error reporting mechanism.
	 *
	 * @param userData User provided data for the error callback.
	 * @param error The error being raised.
	 */
    static void SEFunc(void *userData, xmlErrorPtr error);
private:
    /**
	 * Parse RSS2 feed.
	 *
	 * @param root Root element of feed.
	 * @param argumentProcessor ArgumentProcessor object with processed arguments.
	 * @param url Feed URL.
	 * @return True if XML has been parsed successfully, otherwise - false.
	 */
    static bool parseRSS2(xmlNodePtr root, std::string url, argsParser *argsParser);
    
    /**
	 * Parse ATOM feed.
	 *
	 * @param root Root element of feed.
	 * @param argumentProcessor ArgumentProcessor object with processed arguments.
	 * @param url Feed URL.
	 * @return True if XML has been parsed successfully, otherwise - false.
	 */
    static bool parseATOM(xmlNodePtr root, std::string url, argsParser *argsParser);
};

#endif //FEEDREADER_XMLPARSER_HPP