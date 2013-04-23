#ifndef _EXTRACTOR_SPECS_NODE_SPECIFICATION_HPP_
#define _EXTRACTOR_SPECS_NODE_SPECIFICATION_HPP_

#include "GroupProperties.hpp"
#include "common.hpp"

#include <boost/property_tree/ptree.hpp>

#include <vector>
#include <map>

class File;

class NodeSpecification
{
public:
	NodeSpecification( File* file );

	bool isValid( SubSpec& subSpec, GroupProperties& groupProperties, bpt::ptree& nodeReport );

private:
	File*       _file;
	ElementsMap _headerElements;
	
	std::string message;
	std::string id;
	std::string label;
	std::string typeValue;
	std::string count;
	std::string groupSize;
	
	std::vector< std::string > asciiValues;
	std::vector< std::string > hexaValues;

	bool isValidNode;
	bool endianValue;
	bool optional;
	bool group;
};

#endif
