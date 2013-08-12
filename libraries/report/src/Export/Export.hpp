#ifndef _REPORT_EXPORT_HPP_
#define _REPORT_EXPORT_HPP_

#include <boost/property_tree/ptree.hpp>

namespace bpt = boost::property_tree;

namespace report_generator
{

class Export
{
public:
	Export( const bpt::ptree& report );
	
	void writeJsonFile( const std::string& filename );
	void writeXmlFile ( const std::string& filename, bool compact = false );
	std::string getJsonString();
	std::string getXmlString();
	
private:
	bpt::ptree _report;
};

}

#endif
