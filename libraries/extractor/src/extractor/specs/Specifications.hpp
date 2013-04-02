#ifndef _EXTRACTOR_SPECS_SPECIFICATIONS_HPP_
#define _EXTRACTOR_SPECS_SPECIFICATIONS_HPP_

#include "Specification.hpp"

#include <boost/property_tree/ptree.hpp>

#include <vector>

namespace bpt = boost::property_tree;

typedef std::vector< Specification > Specs;

class Specifications
{
public:
	Specifications();

	void init();

	void updateList();

	void getSpec( Specification& spec, const size_t index ) const
	{
		spec = specList.at( index );
	}

	void getSpecList( Specs& specs ) const
	{
		specs = specList;
	}

	size_t getNumSpec( ) const
	{
		return specList.size();
	}

private:
	Specs specList;
	std::vector< std::string > paths;
};

#endif
