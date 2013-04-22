#ifndef _EXTRACTOR_SPECS_NODE_SPECIFICATION_HPP_
#define _EXTRACTOR_SPECS_NODE_SPECIFICATION_HPP_

#include "GroupProperties.hpp"

#include <boost/property_tree/ptree.hpp>

#include <vector>
#include <map>

namespace bpt = boost::property_tree;

typedef bpt::ptree Spec;
typedef bpt::ptree::value_type Node;
typedef bpt::ptree::const_assoc_iterator SpecIt;
typedef const bpt::ptree::value_type SubSpec;

typedef std::map<std::string, unsigned int> ElementsMap;
typedef std::pair<std::string, unsigned int> ElementsPair;

class File;



class NodeSpecification
{
public:
	NodeSpecification( File* file );

	bool isValid( SubSpec& subSpec, GroupProperties& groupProp );

private:
	File*       _file;
	ElementsMap _headerElements;
};

#endif
