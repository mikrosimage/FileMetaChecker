#ifndef _EXTRACTOR_SPECS_NODE_SPECIFICATION_HPP_
#define _EXTRACTOR_SPECS_NODE_SPECIFICATION_HPP_

#include <boost/property_tree/ptree.hpp>

#include <vector>

namespace bpt = boost::property_tree;

typedef bpt::ptree Spec;
typedef bpt::ptree::value_type Node;
typedef bpt::ptree::const_assoc_iterator SpecIt;
typedef const bpt::ptree::value_type SubSpec;

class File;

class NodeSpecification
{
public:
	NodeSpecification( File* file, SubSpec& subSpec );

	bool isValid();

private:
	File*    _file;
	SubSpec& _subSpec;
};

#endif
