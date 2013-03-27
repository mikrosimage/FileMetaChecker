#include "Export.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

Export::Export( )
{
	
}

void Export::writeJson( const bpt::ptree& specTree, const std::string& filename )
{
	write_json( filename, specTree );
}

void Export::writeXml( const bpt::ptree& specTree, const std::string& filename )
{
	write_xml( filename, specTree );
}
