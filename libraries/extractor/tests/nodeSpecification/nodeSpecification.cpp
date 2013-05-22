#include <extractor/specs/NodeSpecification.hpp>
#include <extractor/specs/GroupProperties.hpp>
#include <extractor/specs/common.hpp>
#include <extractor/inputFile/File.hpp>
#include <extractor/inputFile/types.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

// #include "testStreamTools.hpp"
#include <common/global.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>

#define BOOST_TEST_MODULE qc_extractor_nodeSpecification
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;
namespace bpt = boost::property_tree;

const std::string testFile   = "./tests/jpegTest.jpg";
const std::string reportFile = "./tests/jpegTestReport.xml";
const std::string specFile   = "./tests/jpegTest.json";

boost::shared_ptr<common::formatters::Formatter> formatter( common::formatters::Formatter::get() );
boost::shared_ptr<common::Color>                 color( common::Color::get() );

std::ostream& operator<<( std::ostream& ofs, const Hexa hexaValue )
{
	for( size_t index = 0; index < hexaValue.value.size(); index += 2 )
	{
		std::stringstream str;
		int value;
		str << hexaValue.value.substr( index, 2 );
		str >> std::hex >> value;
		ofs << (unsigned char)value;
	}
	return ofs;
}

bpt::ptree generateReportTree( const bpt::ptree& node )
{
	bpt::ptree testTree;
	bpt::ptree report, nodeReport;
	GroupProperties rootProperties;

	File file;
	file.open( testFile );
	NodeSpecification ns( &file );

	testTree.add_child( "header.", node );
	bpt::write_json( specFile, testTree );

	BOOST_FOREACH( SubSpec v, testTree.get_child( "header" ) )
	{
		ns.isValid( v, rootProperties, nodeReport );
		if( v.second.get_child_optional( "id" ) )
		{
			// LOG_INFO( v.second.get< std::string >( "id" ) );
			report.push_back( bpt::ptree::value_type( v.second.get< std::string >( "id" ), nodeReport ) );
		}
	}
	bpt::write_xml( reportFile, report );

	file.close();

	return report;
}

#define CHECK_NODE( NODE_NAME, STATUS, RESULT ) BOOST_CHECK_EQUAL( report.get_child( NODE_NAME ).get< std::string >( STATUS ), RESULT )

#define CHECK_NODE_VALID( NODE_NAME ) CHECK_NODE( NODE_NAME, "<xmlattr>.status", "valid" )
#define CHECK_NODE_INVALID( NODE_NAME ) CHECK_NODE( NODE_NAME, "<xmlattr>.status", "invalid" )

#define CHECK_VALUE_EQUAL( NODE_NAME, VALUE ) BOOST_CHECK_EQUAL( report.get_child( NODE_NAME ).get_value< std::string >(), VALUE );


BOOST_AUTO_TEST_SUITE( nodeSpecification_tests_suite01 )

BOOST_AUTO_TEST_CASE( nodeSpecification_genericFailedTest )
{
	formatter->init_logging();
	color->enable();
	
	Hexa hexa;
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		hexa.value = "ffd8";
		stream << hexa;
		stream.close();
		
		bpt::ptree node;
		node.put( "label", "No Id Test" );
		node.put( "hexa", hexa.value );
		// COMMON_COUT( ">>> No id test (hexa value)" );

		BOOST_CHECK_THROW( generateReportTree( node ), bpt::ptree_bad_path );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		hexa.value = "00";
		stream.close();

		bpt::ptree node;
		node.put( "id", "noTypeTest" );
		node.put( "label", "No Type Test" );

		// COMMON_COUT( ">>> No type test" );
		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}
}

BOOST_AUTO_TEST_SUITE_END()

#include "nodeSpecificationNumber.hpp"
#include "nodeSpecificationHexa.hpp"
#include "nodeSpecificationAscii.hpp"
#include "nodeSpecificationGroup.hpp"
#include "nodeSpecificationOptional.hpp"
#include "nodeSpecificationExpression.hpp"