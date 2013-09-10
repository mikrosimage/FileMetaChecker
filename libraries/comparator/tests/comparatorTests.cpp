#define BOOST_TEST_MODULE qc_report

#include <boost/test/unit_test.hpp>

#include <Comparator.hpp>
#include <FileReader.hpp>
#include <Export/Export.hpp>
#include <Transform/Transform.hpp>

#include <boost/property_tree/xml_parser.hpp>

using namespace boost::unit_test;
using namespace comparator;

namespace sr = spec_reader;
namespace be = basic_element;
namespace rg = report_generator;
namespace fr = filereader;

boost::shared_ptr<common::formatters::Formatter> formatter( common::formatters::Formatter::get() );
boost::shared_ptr<common::Color>                 color( common::Color::get() );

void fillVectorXml( bpt::ptree& tree, std::vector< std::string >& vect, const std::string& key )
{
	for( bpt::ptree::value_type& child : tree )
	{
		if( child.first == "<xmlattr>" )
			continue;
		vect.push_back( child.second.get< std::string >( key ) );
		// LOG_INFO( child.first << " " <<  child.second.get< std::string >( key ) << "\tsize: "<< child.second.size() );
		fillVectorXml( child.second, vect, key );
	}
}

void fillVectorJson( bpt::ptree& tree, std::vector< std::string >& vect, const std::string& key )
{
	for( bpt::ptree::value_type& child : tree )
	{
		vect.push_back( child.second.get< std::string >( key ) );
		// LOG_INFO( child.second.get< std::string >( key ) << "\tsize: "<< child.second.size() );
		if( boost::optional< bpt::ptree& > groupNode = child.second.get_child_optional( "group" ) )
			fillVectorJson( groupNode.get(), vect, key );
	}
}

BOOST_AUTO_TEST_SUITE( comparator_test_suite01 )

BOOST_AUTO_TEST_CASE( report_init )
{
	formatter->init_logging();
	color->enable();
}

BOOST_AUTO_TEST_SUITE_END()

#include "comparatorTestsFeatures.hpp"
#include "comparatorTestsRepetitions.hpp"
// #include "comparatorTestsOptional.hpp"
