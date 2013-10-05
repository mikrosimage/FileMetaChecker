#ifndef _REPORT_EXPORT_HPP_
#define _REPORT_EXPORT_HPP_

#include <boost/property_tree/ptree.hpp>

namespace bpt = boost::property_tree;

namespace report_generator
{

class Export
{
public:
	enum EEXportType
	{
		eExportTypeJson = 0,
		eExportTypeXml
	};

	Export( const bpt::ptree& report );
	
	template< EEXportType >
	void write( const std::string& filename, bool compact = false );

	template< EEXportType >
	std::string get( bool compact = false );
	
private:
	bpt::ptree _report;
};

}

#include "Export.tcc"

#endif
