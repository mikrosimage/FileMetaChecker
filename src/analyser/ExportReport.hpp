#ifndef _EXPORT_REPORT_HPP
#define _EXPORT_REPORT_HPP

#include <boost/property_tree/ptree.hpp>

namespace bpt = boost::property_tree;

class ExportReport
{
public:
	ExportReport( );
	
	void writeJson( bpt::ptree& specTree );
	void writeXml ( bpt::ptree& specTree );
	
	//void writeJson( FileSystemInfo& sysInfo, Header& header );
	
};

#endif

