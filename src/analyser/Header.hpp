#ifndef _HEADER_HPP
#define _HEADER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "fileSystemInfo/FileSystemInfo.hpp"

#include <boost/property_tree/ptree.hpp>

namespace bpt = boost::property_tree;

typedef bpt::ptree::value_type Node;

enum HeaderSegmentStatus
{
	eStatusNotValid = 0,
	eStatusValid,
	eStatusPass
};

std::ostream& operator<<( std::ostream& out, HeaderSegmentStatus status );


class Header
{
public:
	Header( );
	bool analyseFileHeader( const std::string& filePath );
	
	bool checkInformations( bpt::ptree& pt );
	
	bool foundExtensionInSpec( bpt::ptree &pt, const std::string& extension );
	
	HeaderSegmentStatus testNode( Node &n , char* buffer, size_t size);
	HeaderSegmentStatus analyseChunkNode( Node &n );
	
	void displayResults( std::string id, HeaderSegmentStatus status );
	
	void printReport();
	
public:
	FileSystemInfo  _fileReader;
	std::string _hexaHeader;
	bpt::ptree  _specTree;
	std::vector<bpt::ptree> _specs;
	bool reportStatus;
	std::string specName;
};

#endif
