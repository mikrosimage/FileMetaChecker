#define BOOST_TEST_MODULE qc_spec_reader_expression_parser

#include <boost/test/unit_test.hpp>
#include <common/global.hpp>
#include <ExpressionParser/ExpressionParser.hpp>

using namespace boost::unit_test;
using namespace spec_reader::expression_parser;

boost::shared_ptr<common::formatters::Formatter> formatter( common::formatters::Formatter::get() );
boost::shared_ptr<common::Color>                 color( common::Color::get() );

BOOST_AUTO_TEST_SUITE( spec_reader_tests_expression_parser )

BOOST_AUTO_TEST_CASE( spec_reader_expression_parser_set_variables )
{
	formatter->init_logging();
	color->enable();
	LOG_INFO( " >>> spec_reader_expression_parser_set_variables <<< " );
	{
		ExpressionParser expParser = ExpressionParser();
		std::map < std::string, short > map;
		map["abcd"]= 0;

		expParser.setVariables< short >( map );

		BOOST_CHECK_EQUAL( expParser.getContextString(), "abcd = 0\n" );
	}
	{
		ExpressionParser expParser = ExpressionParser();
		std::map < std::string, unsigned int > map;
		map["abcd"]= 0;
		map["efgh"]= 1;
		map["ijkl"]= 2;
		map["mnop"]= 3;

		expParser.setVariables< unsigned int >( map );

		BOOST_CHECK_EQUAL( expParser.getContextString(), "abcd = 0\nefgh = 1\nijkl = 2\nmnop = 3\n" );
	}
	{
		ExpressionParser expParser = ExpressionParser();
		std::map < std::string, short > mapShort;
		std::map < std::string, int > mapInt;
		mapShort["abcd"]= 0;
		mapShort["efgh"]= 1;
		mapInt["ijkl"]= -20;
		mapInt["mnop"]= 30;

		expParser.setVariables< short >( mapShort );
		expParser.setVariables< int >( mapInt );

		BOOST_CHECK_EQUAL( expParser.getContextString(), "abcd = 0\nefgh = 1\nijkl = -20\nmnop = 30\n" );
	}
	{
		ExpressionParser expParser = ExpressionParser();
		std::map < std::string, float > map;
		map["float1"]= 0.001;
		map["float2"]= 1.23;
		map["float3"]= true;
		expParser.setVariables< float >( map );

		BOOST_CHECK_EQUAL( expParser.getContextString(), "float1 = 0.001\nfloat2 = 1.23\nfloat3 = 1\n" );
	}
	{
		
		std::map < std::string, long > map;
		map["long1"]= -600;
		map["long2"]= 1000;
		map["long3"]= true;
		ExpressionParser expParser = ExpressionParser( map );

		BOOST_CHECK_EQUAL( expParser.getContextString(), "long1 = -600\nlong2 = 1000\nlong3 = 1\n" );
	}
	{
		std::map < std::string, unsigned long long > map;
		map["longlong1"]= std::numeric_limits< unsigned long long >::min();
		map["longlong2"]= std::numeric_limits< unsigned long long >::max();
		ExpressionParser expParser = ExpressionParser( map );

		BOOST_CHECK_EQUAL( expParser.getContextString(), "longlong1 = 0\nlonglong2 = 18446744073709551615\n" );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_expression_parser_addPythonHeader )
{
	LOG_INFO( " >>> spec_reader_expression_parser_addPythonHeader <<< " );
	{
		ExpressionParser expParser = ExpressionParser();
		std::map < std::string, short > map;
		map["abcd"]= 0;
		map["efgh"]= 1;
		map["ijkl"]= 2;
		map["mnop"]= 3;

		expParser.setVariables< short >( map );
		expParser.addPythonHeader( "from math import *");

		BOOST_CHECK_EQUAL( expParser.getContextString(), "from math import *\nabcd = 0\nefgh = 1\nijkl = 2\nmnop = 3\n" );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_expression_parser_get_expression_result )
{
	LOG_INFO( " >>> spec_reader_expression_parser_get_expression_result <<< " );
	{
		ExpressionParser expParser = ExpressionParser();
		std::map < std::string, int > map;
		map["abcd"]= 0;
		map["efgh"]= 1;
		map["ijkl"]= 2;
		map["mnop"]= 3;

		expParser.setVariables< int >( map );
		expParser.addPythonHeader( "from math import *");
		
		BOOST_CHECK_EQUAL( expParser.getExpressionResult<size_t>( "8" ), 8 );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult<size_t>( "3 * abcd" ), 0 );
		
		BOOST_CHECK_EQUAL( expParser.getExpressionResult<int>( "3 * abcd" ), 0 );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult<int>( "mnop / 3" ), 1 );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult<int>(  "8" ),  8 );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult<int>( "-8" ), -8 );

		BOOST_REQUIRE_CLOSE( expParser.getExpressionResult<float>( "2.2*2"   ), 4.40,  0.01 );
		BOOST_REQUIRE_CLOSE( expParser.getExpressionResult<float>( "2.002*3" ), 6.006, 0.01 );

		BOOST_CHECK_EQUAL( expParser.getExpressionResult<std::string>( "'2.002*3'" ), "2.002*3" );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult<std::string>( "str(2*3)"  ), "6"       );		 

		BOOST_CHECK_THROW( expParser.getExpressionResult<int>        ( "3 * other" ), boost::python::error_already_set );
		BOOST_CHECK_THROW( expParser.getExpressionResult<float>      ( " mnop / 3" ), boost::python::error_already_set );
		BOOST_CHECK_THROW( expParser.getExpressionResult<int>        ( "" ),          boost::python::error_already_set );
		BOOST_CHECK_THROW( expParser.getExpressionResult<size_t>     ( " " ),         boost::python::error_already_set );
		BOOST_CHECK_THROW( expParser.getExpressionResult<std::string>( "2*3" ),       boost::python::error_already_set );

		BOOST_CHECK_THROW( expParser.getExpressionResult<std::string>( "quotation marks are missing" ), boost::python::error_already_set );
	}
	{
		ExpressionParser expParser = ExpressionParser();
		BOOST_CHECK_THROW( expParser.getExpressionResult< int >( "log(1)" ), boost::python::error_already_set );
	}
}

BOOST_AUTO_TEST_SUITE_END()
