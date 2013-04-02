#ifndef _EXTRACTOR_SPECS_SPECIFICATION_HPP_
#define _EXTRACTOR_SPECS_SPECIFICATION_HPP_

#include <boost/property_tree/ptree.hpp>

#include <vector>

namespace bpt = boost::property_tree;

typedef bpt::ptree Spec;
typedef bpt::ptree::value_type Node;
typedef bpt::ptree::const_assoc_iterator SpecIt;
class Specification
{
public:
	Specification();

	void setSpecTree( const Spec& spec );

	std::string getId( );
	std::string getLabel( );
	std::string getType( );

	std::vector< std::string > getsupportedExtensions( );

	SpecIt getHeader( );

private:
	Spec _spec;
};

#endif
