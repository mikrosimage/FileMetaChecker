#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include <iostream>
#include <string>
#include <vector>

#include <boost/property_tree/json_parser.hpp>

namespace bpt = boost::property_tree;

class JsonParser
{
	public:
		JsonParser();
		// JsonParser( const std::string& filePath );

		void parseJsonFile( const std::string& filePath );
		void readJsonFolder( const std::string& folderPath );

	public:
		std::vector<bpt::ptree> _ptreeList;
};

#endif