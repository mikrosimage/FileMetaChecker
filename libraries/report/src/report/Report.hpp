#ifndef _REPORT_REPORT_HPP_
#define _REPORT_REPORT_HPP_

#include <boost/property_tree/ptree.hpp>
namespace bpt = boost::property_tree;

#include <vector>

class Report
{
public:
	Report();
	
	void add( const std::string& entry, const std::string& value );
	
	void add(const bpt::ptree &tree, const std::string& rootpath );

	void add( const std::vector< bpt::ptree >& tree, const std::string& rootpath );
	
	void exportReport( const std::string& filename );
	
private:
	bpt::ptree reportTree;
};

#endif
