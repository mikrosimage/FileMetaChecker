#ifndef _REPORT_EXPORT_HPP_
#define _REPORT_EXPORT_HPP_

#include <boost/property_tree/ptree.hpp>
namespace bpt = boost::property_tree;

class Export
{
public:
	Export();
	
	void writeJson( const bpt::ptree& specTree, const std::string& filename );
	void writeXml ( const bpt::ptree& specTree, const std::string& filename );
	
private:
};

#endif
