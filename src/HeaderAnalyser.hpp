#ifndef HEADER_ANALYSER_HPP
#define HEADER_ANALYSER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "JsonParser.hpp"
#include "FileReader.hpp"

#include <boost/property_tree/json_parser.hpp>

typedef bpt::ptree::value_type Node;

enum HeaderSegmentStatus
{
	isNotValid = 0,
	isValid,
	pass
};


class HeaderAnalyser
{
	public:
		HeaderAnalyser( const std::string& filePath );

		bool testExtension( bpt::ptree &pt, const std::string& extension );
		bool analyseFileHeader( bpt::ptree &pt );
		HeaderSegmentStatus testNode( Node &n , char* buffer, unsigned int size);

		void printReport( const bpt::ptree &pt );

	public:
		JsonParser	_jp;	// json parser
		FileReader	_fr;	// raw file reader (bit --> char*)

		std::string _sh;	// display the file header (hexa)		
};

#endif