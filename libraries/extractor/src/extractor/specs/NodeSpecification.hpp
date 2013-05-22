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

	bool isValid(SubSpec& subSpec, GroupProperties& parentProperties, bpt::ptree& nodeReport );

	template< typename DataType >
	void exportValidData( const bool isValid, const std::string& id, const DataType& data, GroupProperties& groupProperties, bpt::ptree& nodeReport );

private:
	bool isValidSubGroup( SubSpec& subSpec, GroupProperties& groupProp, bpt::ptree& nodeReport );
	
	File*       _file;
	ElementsMap _headerElements;
};

#include "NodeSpecification.tcc"

#endif
