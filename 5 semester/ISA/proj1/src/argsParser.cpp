/**
 * VUT FIT ISA 2022 project - feed reader.
 *
 * @file Parsing arguments
 * @author Dmytro Sadovskyi <xsadov06@stud.fit.vutbr.cz>
 */

#include <cstdio>
#include <cctype>
#include <getopt.h>
#include "../headers/argsParser.hpp"

argsParser::~argsParser()
{
	delete url;
	delete feedFile;
	delete certFile;
	delete certDir;
}

bool argsParser::getArguments(int argc, char *argv[])
{
	const char *sArgs = "f:c:C:Tauh";
	const struct option lArgs[] = {
		{"help", 0, nullptr, 'h'},
		{nullptr, 0, nullptr, 0},
	};
	int opt;
	while ((opt = getopt_long(argc, argv, sArgs, lArgs, nullptr)) != -1)
	{
		switch (opt)
		{
			case 'f':
				feedFile = new std::string(optarg);
				break;

			case 'c':
				certFile = new std::string(optarg);
				break;

			case 'C':
				certDir = new std::string(optarg);
				break;

			case 'T':
				time = true;
				break;

			case 'a':
				author = true;
				break;

			case 'u':
				associatedUrl = true;
				break;

			case 'h':
				help = true;
				printHelp();
				return true;

			case '?':
				if (optopt == 'f' || optopt == 'c' || optopt == 'C')
				{
					fprintf(stderr, "Option '-%c' requires an argument.", optopt);
				}
				else if (isprint(optopt))
				{
					fprintf(stderr, "Unknown option '-%c'.", optopt);
				}
				else
				{
					fprintf(stderr, "Unknown option '\\x%x'.", optopt);
				}

				printHelp();
				return false;

			default:
				fprintf(stderr, "Processing program arguments failed."); //CHANGE
				printHelp();
				return false;
		}
	}

	if (optind < argc)
	{
		if (argc - optind > 1)
		{
			std::string unknownArguments; //RENAME
			for (int i = optind + 1; i < argc; i++)
			{
				unknownArguments += argv[i];
				if (i == argc - 1)
				{
					unknownArguments += ".";
				}
				else
				{
					unknownArguments += ", ";
				}
			}
			fprintf(stderr,"Unknown arguments: %s", unknownArguments.c_str());
			printHelp();

			return false;
		}

		url = new std::string(argv[optind]);
	}

	if (!url && !feedFile)
	{
		fprintf(stderr,	"Argument 'url' or '-f <feedfile>' is required.\n"); //CHANGE
		return false;
	}

	if (url && feedFile)
	{
		fprintf(stderr,	"Argument 'url' or '-f <feedfile>' is required but not both.\n"); //CHANGE
		return false;
	}

	return true;
}

std::string *argsParser::getUrl()
{
	return url;
}


std::string *argsParser::getFeedFile()
{
	return feedFile;
}


std::string *argsParser::getCertFile()
{
	return certFile;
}


std::string *argsParser::getCertDir()
{
	return certDir;
}


bool argsParser::getTime()
{
	return time;
}


bool argsParser::getAuthor()
{
	return author;
}


bool argsParser::getAssociatedUrl()
{
	return associatedUrl;
}


bool argsParser::getHelp()
{
	return help;
}


void argsParser::printHelp()
{
	printf("%s\n", HELP.c_str());
}