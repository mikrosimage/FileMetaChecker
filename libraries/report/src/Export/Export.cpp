#include "Export.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace report_generator
{

Export::Export( const bpt::ptree& report )
	: _report( report )
{
}

void Export::writeJsonFile( const std::string& filename )
{
	write_json( filename, _report );
}


void Export::writeXmlFile( const std::string& filename, bool compact )
{
	bpt::xml_writer_settings< char > settings( '\t', compact ? 0 : 1 );
	write_xml( filename, _report, std::locale(), settings );
}

std::string Export::getJsonString()		// @ todo : add compacity option
{
	std::ostringstream jsonStream;
	write_json( jsonStream, _report );
	return jsonStream.str();
}

std::string Export::getXmlString()		// @ todo : add compacity option
{
	std::ostringstream xmlStream;
	write_xml( xmlStream, _report );
	return xmlStream.str();
}

}
