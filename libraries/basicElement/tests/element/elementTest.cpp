#define BOOST_TEST_MODULE qc_basic_element

#include <boost/test/unit_test.hpp>

#include <Element.hpp>
#include <Data/Data.hpp>
#include <Number/Number.hpp>

using namespace boost::unit_test;
namespace be = basic_element;
namespace dbe = basic_element::data_element;
namespace nbe = basic_element::number_element;

boost::shared_ptr<common::formatters::Formatter> formatter( common::formatters::Formatter::get() );
boost::shared_ptr<common::Color>                 color( common::Color::get() );

std::string id    = "id";
std::string label = "label";

BOOST_AUTO_TEST_SUITE( basic_element_test_suite01 )

BOOST_AUTO_TEST_CASE( basic_element_element )
{
	formatter->init_logging();
	color->enable();
}

BOOST_AUTO_TEST_CASE( basic_element_data )
{
	LOG_INFO( "\n>>> basic_element_data <<<" );
	{
		dbe::Data data;

		data.setId( id );
		BOOST_CHECK_EQUAL( data.getId(), id );

		data.setLabel( label );
		BOOST_CHECK_EQUAL( data.getLabel(), label );
	}
	{
		dbe::Data data;
		char buffer[] = { 0x7f, 0x05, 0x32 };
		
		data.setData( buffer, 3 );
		char* buff = new char [ data.getSize() ];
		data.getData( buff );
		BOOST_CHECK_EQUAL( buff[0], 127 );
		BOOST_CHECK_EQUAL( buff[1], 5 );
		BOOST_CHECK_EQUAL( buff[2], 50 );
		delete[] buff;

		BOOST_CHECK_EQUAL( data.getHexa(), "7f0532" );
	}
	{
		dbe::Data data;
		char buffer[] = { 0x48, 0x65, 0x6c, 0x6c, 0x6f };
		
		data.setData( buffer, 5 );
		BOOST_CHECK_EQUAL( data.getAscii(), "Hello" );
	}
	{
		dbe::Data data;
		char buffer[] = {'d','a','t','a'};

		data.setData( buffer, 4 );
		char* buff = new char [ data.getSize() ];
		data.getData( buff );
		BOOST_CHECK_EQUAL( buff[0], 'd' );
		BOOST_CHECK_EQUAL( buff[1], 'a' );
		BOOST_CHECK_EQUAL( buff[2], 't' );
		BOOST_CHECK_EQUAL( buff[3], 'a' );
		delete[] buff;

		BOOST_CHECK_EQUAL( data.getAscii(), "data" );
		BOOST_CHECK_EQUAL( data.getHexa(), "64617461" );
	}
	{
		dbe::Data data;
		char buffer[] = { 0x00, 0x05, 0x0c, 0x20, 0xff };

		data.setData( buffer, 5 );
		std::vector< unsigned int > vector1 = data.toIntVector();
		std::vector< unsigned int > vector2;
		vector2.push_back(   0 );
		vector2.push_back(   5 );
		vector2.push_back(  12 );
		vector2.push_back(  32 );
		vector2.push_back( 255 );

		for( size_t i = 0; i < 5; ++i )
			BOOST_CHECK_EQUAL( vector1.at(i), vector2.at(i) );
	}
}

BOOST_AUTO_TEST_SUITE_END()

#include "numberTests.hpp"
