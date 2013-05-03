#include <extractor/specs/NodeSpecification.hpp>
#include <extractor/specs/GroupProperties.hpp>
#include <extractor/specs/common.hpp>
#include <extractor/inputFile/File.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

// #include "testStreamTools.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#define BOOST_TEST_MODULE qc_extractor_nodeSpecification
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;
namespace bpt = boost::property_tree;

std::string testFile   = "jpegTest.jpg";
std::string reportFile = "jpegTestReport.xml";
std::string specFile   = "jpegTest.json";


void addHexaStream( std::string hexaWord, std::ofstream &output )
{
	for( size_t i=0; i < hexaWord.size(); i+=2 )
	{
		std::stringstream str;
		int value;
		str << hexaWord.substr( i, 2 );
		str >> std::hex >> value;
		output << (unsigned char)value ;
	}
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
	bpt::write_xml( reportFile, report );

	file.close();
	
	return report;
}



BOOST_AUTO_TEST_SUITE( nodeSpecification_tests_suite01 )

BOOST_AUTO_TEST_CASE( nodeSpecification_genericFailedTest )
{
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "ffd8", stream );
		stream.close();
		
		bpt::ptree node;
		node.put( "label", "No Id Test" );
		node.put( "hexa", "ffd8" );
		bpt::ptree report = generateReportTree( node );

		BOOST_CHECK_THROW( report.get_child( "noIdTest" ).get_value< std::string >(), bpt::ptree_bad_path );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "00", stream );
		stream.close();

		bpt::ptree node;
		node.put( "id", "noTypeTest" );
		node.put( "label", "No Type Test" );

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "noTypeTest" ).get_value< std::string >(), "" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_hexaTestFile )
{
	bpt::ptree node;
	node.put( "id", "hexaTest" );
	node.put( "label", "Hexa Test" );
	node.put( "hexa", "ffd8" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "ffd8", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );

		BOOST_CHECK_EQUAL( report.get_child( "hexaTest" ).get_value< std::string >(), "ffd8" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "FFD8", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );

		BOOST_CHECK_EQUAL( report.get_child( "hexaTest" ).get_value< std::string >(), "ffd8" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "abcd", stream );
		stream.close();	

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "hexaTest" ).get_value< std::string >(), "" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "ffd8", stream );
		stream.close();

		node.clear();
		node.put( "id", "hexaTest" );
		node.put( "label", "Hexa Test" );
		bpt::ptree report = generateReportTree( node );

		BOOST_CHECK_EQUAL( report.get_child( "hexaTest" ).get_value< std::string >(), "" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_asciiTestFile )
{
	bpt::ptree node;
	node.put( "id", "asciiTest" );
	node.put( "label", "Ascii Test" );
	node.put( "ascii", "WAVE" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "57415645", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );

		BOOST_CHECK_EQUAL( report.get_child( "asciiTest" ).get_value< std::string >(), "WAVE" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "abcdef00", stream );
		stream.close();	

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "asciiTest" ).get_value< std::string >(), "" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "57415645", stream );
		stream.close();

		node.clear();
		node.put( "id", "asciiTest" );
		node.put( "label", "Ascii Test" );
		bpt::ptree report = generateReportTree( node );

		BOOST_CHECK_EQUAL( report.get_child( "asciiTest" ).get_value< std::string >(), "" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_int8TestFile )
{
	bpt::ptree node;
	node.put( "id", "int8Test" );
	node.put( "label", "Int8 Test" );
	node.put( "type", "int8" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "00", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int8Test" ).get_value< std::string >(), "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "7F", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int8Test" ).get_value< std::string >(), "127" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "FF", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int8Test" ).get_value< std::string >(), "-1" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "80", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int8Test" ).get_value< std::string >(), "-128" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_uint8TestFile )
{
	bpt::ptree node;
	node.put( "id", "uint8Test" );
	node.put( "label", "Uint8 Test" );
	node.put( "type", "uint8" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "00", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "uint8Test" ).get_value< std::string >(), "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "FF", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "uint8Test" ).get_value< std::string >(), "255" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_int16TestFile )
{
	bpt::ptree node;
	node.put( "id", "int16Test" );
	node.put( "label", "Int16 Test" );
	node.put( "type", "int16" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "0000", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int16Test" ).get_value< std::string >(), "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "7FFF", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int16Test" ).get_value< std::string >(), "32767" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "FFFF", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int16Test" ).get_value< std::string >(), "-1" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "8000", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int16Test" ).get_value< std::string >(), "-32768" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_uint16TestFile )
{
	bpt::ptree node;
	node.put( "id", "uint16Test" );
	node.put( "label", "Uint16 Test" );
	node.put( "type", "uint16" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "0000", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "uint16Test" ).get_value< std::string >(), "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "FFFF", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "uint16Test" ).get_value< std::string >(), "65535" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_int32TestFile )
{
	bpt::ptree node;
	node.put( "id", "int32Test" );
	node.put( "label", "Int32 Test" );
	node.put( "type", "int32" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "00000000", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int32Test" ).get_value< std::string >(), "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "7FFFFFFF", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int32Test" ).get_value< std::string >(), "2147483647" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "FFFFFFFF", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int32Test" ).get_value< std::string >(), "-1" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "80000000", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int32Test" ).get_value< std::string >(), "-2147483648" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_uint32TestFile )
{
	bpt::ptree node;
	node.put( "id", "uint32Test" );
	node.put( "label", "Uint32 Test" );
	node.put( "type", "uint32" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "00000000", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "uint32Test" ).get_value< std::string >(), "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "FFFFFFFF", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "uint32Test" ).get_value< std::string >(), "4294967295" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_int64TestFile )
{
	bpt::ptree node;
	node.put( "id", "int64Test" );
	node.put( "label", "Int64 Test" );
	node.put( "type", "int64" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "0000000000000000", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int64Test" ).get_value< std::string >(), "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "7FFFFFFFFFFFFFFF", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int64Test" ).get_value< std::string >(), "9223372036854775807" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "FFFFFFFFFFFFFFFF", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int64Test" ).get_value< std::string >(), "-1" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "8000000000000000", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "int64Test" ).get_value< std::string >(), "-9223372036854775808" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_uint64TestFile )
{
	bpt::ptree node;
	node.put( "id", "uint64Test" );
	node.put( "label", "Uint64 Test" );
	node.put( "type", "uint64" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "0000000000000000", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "uint64Test" ).get_value< std::string >(), "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out | std::ofstream::binary);
		addHexaStream( "FFFFFFFFFFFFFFFF", stream );
		stream.close();

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_EQUAL( report.get_child( "uint64Test" ).get_value< std::string >(), "18446744073709551615" );
	}
}

BOOST_AUTO_TEST_SUITE_END()


