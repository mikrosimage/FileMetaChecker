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
	GroupProperties groupProperties;

	File file;
	file.open( testFile );
	NodeSpecification ns( &file );

	testTree.add_child( "header.", node );
	bpt::write_json( specFile, testTree );

	BOOST_FOREACH( SubSpec v, testTree.get_child( "header" ) )
	{
		ns.isValid( v, groupProperties, nodeReport );
		if( v.second.get_child_optional( "id" ) )
		{
			report.push_back( bpt::ptree::value_type( v.second.get< std::string >( "id" ), nodeReport ) );
		}
	}
	// bpt::write_xml( reportFile, report );

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

BOOST_AUTO_TEST_CASE( nodeSpecification_hexaTestFile )
{
	Hexa hexa;
	bpt::ptree node;
	
	hexa.value = "ffd8";
	node.put( "id", "hexaTest" );
	node.put( "label", "Hexa Test" );
	node.put( "hexa", hexa.value );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << hexa;
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID ( "hexaTest" );
		CHECK_VALUE_EQUAL( "hexaTest", hexa.value );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		hexa.value = "FFD8";
		stream << hexa;
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID ( "hexaTest" );
		CHECK_VALUE_EQUAL( "hexaTest", "ffd8" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		hexa.value = "abcd";
		stream << hexa;
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_INVALID( "hexaTest" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		hexa.value = "ffd8";
		stream << hexa;
		stream.close();

		node.clear();
		node.put( "id", "hexaTest" );
		node.put( "label", "Hexa Test" );

		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		hexa.value = "ffd8";
		stream << hexa;
		stream.close();

		node.clear();
		node.put( "id", "hexaMultipleTest" );
		node.put( "label", "Hexa Multiple Test" );
		node.put( "hexa.", hexa.value );
		
		node.add( "hexa.", "ffe0" );

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID ( "hexaMultipleTest" );
		CHECK_VALUE_EQUAL( "hexaMultipleTest", hexa.value );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_asciiTestFile )
{
	const std::string wave = "WAVE";
	const std::string riff = "RIFF";
	bpt::ptree node;
	
	node.put( "id", "asciiTest" );
	node.put( "label", "Ascii Test" );
	node.put( "ascii", wave );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << wave;
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID ( "asciiTest" );
		CHECK_VALUE_EQUAL( "asciiTest", wave );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "EVAW";
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID( "asciiTest" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << wave;
		stream.close();

		node.clear();
		node.put( "id", "asciiTest" );
		node.put( "label", "Ascii Test" );

		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << wave;
		stream.close();

		node.clear();
		node.put( "id", "asciiMultipleTest" );
		node.put( "label", "Ascii Multiple Test" );
		node.put( "ascii.", wave );
		node.add( "ascii.", riff );

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID ( "asciiMultipleTest" );
		CHECK_VALUE_EQUAL( "asciiMultipleTest", wave );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_int8TestFile )
{
	const std::string nodename = "int8Test";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Int8 Test" );
	node.put( "type", "int8" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (int8) 0;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (int8) 127;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "127" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (int8) -1;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-1" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (int8) -128;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-128" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "-40" );
	range.put( "max", "40" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (int8) -41;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (int8) 41;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (int8) 4;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "4" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_uint8TestFile )
{
	const std::string nodename = "uint8Test";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Uint8 Test" );
	node.put( "type", "uint8" );

	{
		/// check min
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		/// check max
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 255;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "255" );
	}
	
	/// check range
	bpt::ptree range;
	range.put( "min", "20" );
	range.put( "max", "100" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 101;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 50;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "50" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_int16TestFile )
{
	const std::string nodename = "int16Test";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Int16 Test" );
	node.put( "type", "int16" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x7f << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "32767" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-1" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x80 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-32768" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "-100" );
	range.put( "max", "100" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0x9b;		// = -101
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x65;		// = 101
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xef;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-17" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_uint16TestFile )
{
	const std::string nodename = "uint16Test";
	
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Uint16 Test" );
	node.put( "type", "uint16" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "65535" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "10" );
	range.put( "max", "1000" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x09;		// = 9
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x03 << (uint8) 0xe9;		// = 1001
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x55;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "85" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_int32TestFile )
{
	const std::string nodename = "int32Test";
	
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Int32 Test" );
	node.put( "type", "int32" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x7f << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "2147483647" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-1" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x80 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-2147483648" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "-1000" );
	range.put( "max", "1000" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff << (uint8) 0xfc << (uint8) 0x17;		// = -1001
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x03 << (uint8) 0xe9;		// = 1001
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xf4;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-12" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_uint32TestFile )
{
	const std::string nodename = "uint32Test";
	
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Uint32 Test" );
	node.put( "type", "uint32" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "4294967295" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "100" );
	range.put( "max", "10000" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x63;		// = 99
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x27 << (uint8) 0x11;		// = 10001
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x01 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "256" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_int64TestFile )
{
	const std::string nodename = "int64Test";
	
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Int64 Test" );
	node.put( "type", "int64" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x7f << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff <<
				  (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "9223372036854775807" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff <<
				  (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-1" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x80 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-9223372036854775808" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "-10000" );
	range.put( "max", "10000" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff <<
				  (uint8) 0xff << (uint8) 0xff << (uint8) 0xd8 << (uint8) 0xef;		// = -10001
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x27 << (uint8) 0x11;		// = 10001
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x11;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "17" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_uint64TestFile )
{
	const std::string nodename = "uint64Test";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Uint64 Test" );
	node.put( "type", "uint64" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff <<
				  (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "18446744073709551615" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "1000" );
	range.put( "max", "100000" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x03 << (uint8) 0xe7;		// = 999
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x01 << (uint8) 0x86 << (uint8) 0xa1;		// = 100001
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x27 << (uint8) 0x10;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "10000" );
	}
}

BOOST_AUTO_TEST_SUITE_END()
