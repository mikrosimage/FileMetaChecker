#include "Export.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace report_generator
{

Export::Export( const bpt::ptree& report )
	: _report( report )
{
}

void Export::writeJson( const std::string& filename )
{
	write_json( filename, _report );
}

void Export::writeXml( const std::string& filename, bool compact )
{
	bpt::xml_writer_settings< char > settings( '\t', compact ? 0 : 1 );
	write_xml( filename, _report, std::locale(), settings );
}

}
