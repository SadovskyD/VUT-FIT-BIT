/**
 * VUT FIT ISA 2022 project - feed reader.
 *
 * @file Parsing arguments
 * @author Dmytro Sadovskyi <xsadov06@stud.fit.vutbr.cz>
 */

#ifndef ARGPARSER_HPP
#define ARGPARSER_HPP

#include <string>

class argsParser
{
public:
	/**
	 * Object destructor
	 */
	~argsParser();

	/**
	 * Processes program arguments
	 * @param argc Number of arguments
	 * @param argv Array of arguments
	 * @return True if processiong is succesfull, otherwise - false.
	 */
	bool getArguments(int argc, char *argv[]);

	/**
	 * @return Resource URL.
	 */
	std::string *getUrl();

	/**
	 * @return Location of feed file.
	 */
	std::string *getFeedFile();

	/**
	 * @return Location of cerftificates file?
	 */
	std::string *getCertFile();

	/**
	 * @return Location of cerftificates directory?
	 */
	std::string *getCertDir();

	/**
	 * @return Display creation time?
	 */
	bool getTime();

	/**
	 * @return Display author name?
	 */
	bool getAuthor();

	/**
	 * @return Display associated url?
	 */
	bool getAssociatedUrl();

	/**
	 * @return Display help?
	 */
	bool getHelp();

private:
	/**
	 * String to show help
	 */
	const std::string HELP =
		"Atom and RSS FEDREADER usage:\n"
		"feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u]\n"
		"	url           - Resource URL\n"
		"	-f <feedFile> - \"feedfile\" location\n"
		"	-c <certFile> - certificates file location\n"
		"	-C <certDir>  - certificates directory location\n"
		"	-T            - display modification or creation time\n"
		"	-a            - display author name or email\n"
		"	-u            - display associated URL\n";

	/**
	 * Resource URL
	 */
	std::string *url = nullptr;

	/**
	 * Location of feed file
	 */
	std::string *feedFile = nullptr;

	/**
	 * Location of certificates file
	 */
	std::string *certFile = nullptr;

	/**
	 * Location of certificates directory
	 */
	std::string *certDir = nullptr;

	/**
	 * Display creation time?
	 */
	bool time = false;

	/**
	 * Display autor name?
	 */
	bool author = false;

	/**
	 * Display associated url?
	 */
	bool associatedUrl = false;

	/**
	 * Display help?
	 */
	bool help = false;

	/**
	 * Prints help
	 */
	void printHelp();

	/**
	 * Сhecks if the required parameters are set
	 *
	 * @return 
	 */
	bool checkRequiredArguments();
};


#endif //ARGPARSER_HPP