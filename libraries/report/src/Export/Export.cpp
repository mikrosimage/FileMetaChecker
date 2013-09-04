#include "Export.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace report_generator
{

Export::Export( const bpt::ptree& report )
	: _report( report )
{
}

void Export::writeJsonFile( const std::string& filename, bool compact )
{
	write_json( filename, _report, std::locale(), compact ? false : true );
}


void Export::writeXmlFile( const std::string& filename, bool compact )
{
	bpt::xml_writer_settings< char > settings( '\t', compact ? 0 : 1 );
	write_xml( filename, _report, std::locale(), settings );
}

std::string Export::getJsonString( bool compact )
{
	std::ostringstream jsonStream;
	write_json( jsonStream, _report, compact ? false : true );
	return jsonStream.str();
}

std::string Export::getXmlString( bool compact )
{
	std::ostringstream xmlStream;
	bpt::xml_writer_settings< char > settings( '\t', compact ? 0 : 1 );
	write_xml( xmlStream, _report, settings );
	return xmlStream.str();
}

}
