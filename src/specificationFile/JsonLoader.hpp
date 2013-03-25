#ifndef _JSON_LOADER_HPP
#define _JSON_LOADER_HPP

#include <iostream>
#include <string>
#include <vector>

#include <boost/property_tree/json_parser.hpp>

namespace bpt = boost::property_tree;

class JsonParser
{
public:
	JsonParser();

	std::vector<bpt::ptree> load( const std::string& folderPath );
};

#endif
