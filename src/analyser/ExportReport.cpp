#include "ExportReport.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

ExportReport::ExportReport( )
{
	
}

void ExportReport::writeJson( bpt::ptree& specTree )
{
	write_json( "output.json", specTree );
}

void ExportReport::writeXml( bpt::ptree& specTree )
{
	write_xml( "output.xml", specTree );
}

