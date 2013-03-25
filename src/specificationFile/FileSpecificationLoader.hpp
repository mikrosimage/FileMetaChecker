#ifndef _FILE_SPECIFICATION_LOADER_HPP
#define _FILE_SPECIFICATION_LOADER_HPP

#include <vector>
#include <boost/property_tree/ptree.hpp>

namespace bpt = boost::property_tree;

class FileSpecificationLoader
{
public:
	FileSpecificationLoader();

	void load( );

	std::vector<bpt::ptree> getSpecifications()
	{
		return _ptreeList;
	}

private:
	std::vector<bpt::ptree> _ptreeList;
};

#endif
