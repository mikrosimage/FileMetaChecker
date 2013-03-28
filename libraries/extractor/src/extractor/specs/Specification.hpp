#ifndef _EXTRACTOR_SPECS_SPECIFICATION_HPP_
#define _EXTRACTOR_SPECS_SPECIFICATION_HPP_

#include <boost/property_tree/ptree.hpp>

#include <vector>

namespace bpt = boost::property_tree;

typedef bpt::ptree Spec;
typedef std::vector< Spec > Specs;

class Specification
{
public:
	Specification();
	
	void init();

	void updateList();
	
	void getSpec( Spec& spec, const size_t index ) const
	{
		spec = specList.at( index );
	}
	
	void getSpecList( Specs& specs ) const
	{
		specs = specList;
	}
	
private:
	Specs specList;
	std::vector< std::string > paths;
};

#endif
