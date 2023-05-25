/**
 * VUT FIT ISA 2022 project - feed reader.
 *
 * @file XML file processing
 * @author Dmytro Sadovskyi <xsadov06@stud.fit.vutbr.cz>
 */
#include "../headers/feedParser.hpp"

bool feedParser::parseFeed(std::string xml, std::string url, argsParser *argsParser){
    xmlSetGenericErrorFunc(nullptr, feedParser::GEFunc);
	xmlSetStructuredErrorFunc(nullptr, feedParser::SEFunc);
    xmlDocPtr doc = xmlReadDoc ((const xmlChar *) xml.c_str(), NULL, NULL, XML_PARSE_NOBLANKS);
    if (!doc)
	{
		fprintf(stderr, "Uncorrect format on '%s'.", url.c_str());
		xmlCleanupParser();
		return false;
	}
    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (!root)
	{
		fprintf(stderr, "Empty file on '%s'.", url.c_str());
		xmlFreeDoc(doc);
		xmlCleanupParser();
		return false;
	}
    bool result = false;

    if (!xmlStrcasecmp(root->name, (xmlChar *) "rss")) result = parseRSS2(root, url, argsParser);
    else if (!xmlStrcasecmp(root->name, (xmlChar *) "feed")) result = parseATOM(root, url, argsParser);
	else fprintf(stderr, "The file format is different from the RSS2.0 or ATOM '%s'.", url.c_str());

	xmlFreeDoc(doc);
	xmlCleanupParser();
	return result;
}

bool feedParser::parseRSS2(xmlNodePtr root, std::string url, argsParser *argsParser)
{
    argsParser=argsParser;
    std::string title;
    auto currNode = root->children;
	while (currNode)
	{
		if (!xmlStrcasecmp(currNode->name, (xmlChar *) "channel"))
		{
            auto channelNode = currNode->children;
			while(channelNode)
			{
				if (!xmlStrcasecmp(channelNode->name, (xmlChar *) "title"))
				{
					title = (char *) xmlNodeGetContent(channelNode);
					break;
				}
                channelNode = channelNode->next;
			}
			break;
		}
        currNode = currNode->next;
	}
	if (title.empty())
	{
		fprintf(stderr, "No title in '%s\n", url.c_str());
		return false;
	}

    currNode = currNode->children;
    printf("*** %s ***\n", title.c_str());
    while(currNode)
    {
        if (!xmlStrcasecmp(currNode->name, (xmlChar *) "item"))
        {
            auto itemChild = currNode->children;
            std::string title, date, creator, link;

            while(itemChild)
            {
                if (!xmlStrcasecmp(itemChild->name, (xmlChar *) "title"))
				{
					title = (char *) xmlNodeGetContent(itemChild);
				}
                if (!xmlStrcasecmp(itemChild->name, (xmlChar *) "pubDate"))
				{
					date = (char *) xmlNodeGetContent(itemChild);
				}
                if (!xmlStrcasecmp(itemChild->name, (xmlChar *) "managingEditor"))
				{
					creator = (char *) xmlNodeGetContent(itemChild);
				}
                if (!xmlStrcasecmp(itemChild->name, (xmlChar *) "link"))
				{
					link = (char *) xmlNodeGetContent(itemChild);
				}
                itemChild=itemChild->next;
            }
            printf("%s\n", title.c_str());
            if (argsParser->getTime() && !date.empty()) printf("Time: %s\n", date.c_str());
            if (argsParser->getAuthor() && !creator.empty()) printf("Creator: %s\n", creator.c_str());
            if (argsParser->getAssociatedUrl() && !link.empty()) printf("URL: %s\n", link.c_str());
            if (argsParser->getAssociatedUrl() || argsParser->getAuthor() || argsParser->getTime()) printf("\n");
        }
        currNode = currNode->next;
    }
    return true;
}

bool feedParser::parseATOM(xmlNodePtr root, std::string url, argsParser *argsParser)
{
    argsParser=argsParser;
    auto currNode = root->children;
    std::string title;
    while(currNode)
	{
		if (!xmlStrcasecmp(currNode->name, (xmlChar *) "title"))
		{
			title = (char *) xmlNodeGetContent(currNode);
			break;
		}
        currNode=currNode->next;
	}
    if (title.empty())
    {
        fprintf(stderr, "No title in'%s'.", url.c_str());
        return false;
    }
    printf("*** %s ***\n", title.c_str());
    while(currNode)
    {
        if (!xmlStrcasecmp(currNode->name, (xmlChar *) "entry"))
        {
            auto itemChild = currNode->children;
            std::string title, date, creator, link;
            while(itemChild)
            {
                if (!xmlStrcasecmp(itemChild->name, (xmlChar *) "title"))
                {
                    title = (char *) xmlNodeGetContent(itemChild);
                }
                if (!xmlStrcasecmp(itemChild->name, (xmlChar *) "updated"))
                {
                    date = (char *) xmlNodeGetContent(itemChild);
                }
                if (!xmlStrcasecmp(itemChild->name, (xmlChar *) "author"))
                {
                    auto creatorChild=itemChild->children;
                    while (creatorChild)
                    {
                        if (!xmlStrcasecmp(creatorChild->name, (xmlChar *) "name"))
						{
							creator = (char *) xmlNodeGetContent(creatorChild);
						}
                        creatorChild=creatorChild->next;
                    }
                }
                if (!xmlStrcasecmp(itemChild->name, (xmlChar *) "link"))
                {
                    link = (char *) xmlGetProp(itemChild, (xmlChar *) "href");
                }
                itemChild=itemChild->next;
            }
            printf("%s\n", title.c_str());
            if (argsParser->getTime() && !date.empty()) printf("Aktualizace: %s\n", date.c_str());
            if (argsParser->getAuthor() && !creator.empty()) printf("Autor: %s\n", creator.c_str());
            if (argsParser->getAssociatedUrl() && !link.empty()) printf("URL: %s\n", link.c_str());
            if (argsParser->getAssociatedUrl() || argsParser->getAuthor() || argsParser->getTime()) printf("\n");
        }
        currNode = currNode->next;
    }
    return true;
}

void feedParser::GEFunc(void *ctx, const char *msg, ...)
{
    (void) ctx;
    (void) msg;
}

void feedParser::SEFunc(void *userData, xmlErrorPtr error)
{
    (void) userData;
    (void) error;
}