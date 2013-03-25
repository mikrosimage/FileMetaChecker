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

std::ostream& operator<<( std::ostream& out, HeaderSegmentStatus status );


class HeaderAnalyser
{
	public:
		HeaderAnalyser( const std::string& filePath );

		bool testExtension( bpt::ptree &pt, const std::string& extension );
		bool analyseFileHeader( bpt::ptree &pt );
		HeaderSegmentStatus testNode( Node &n , char* buffer, unsigned int size);

		void writeReport( const bpt::ptree &pt );

	public:
		FileReader  _fileReader;

		std::string _hexaHeader;
};

#endif
