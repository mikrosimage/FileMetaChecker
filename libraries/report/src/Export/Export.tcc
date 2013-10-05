#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace report_generator
{

Export::Export( const bpt::ptree& report )
	: _report( report )
{
}

template< >
void Export::write< Export::eExportTypeJson >( const std::string& filename, bool compact )
{
	write_json( filename, _report, std::locale(), ! compact );
}

template< >
void Export::write< Export::eExportTypeXml >( const std::string& filename, bool compact )
{
	bpt::xml_writer_settings< char > settings( '\t', ! compact );
	write_xml( filename, _report, std::locale(), settings );
}

template< >
std::string Export::get< Export::eExportTypeJson >( bool compact )
{
	std::ostringstream jsonStream;
	write_json( jsonStream, _report, ! compact );
	return jsonStream.str();
}

template< >
std::string Export::get< Export::eExportTypeXml >( bool compact )
{
	std::ostringstream xmlStream;
	bpt::xml_writer_settings< char > settings( '\t', ! compact );
	write_xml( xmlStream, _report, settings );
	return xmlStream.str();
}

}
