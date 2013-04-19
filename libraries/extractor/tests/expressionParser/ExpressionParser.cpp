#include <extractor/expressionParser/ExpressionParser.hpp>

#include <iostream>

#define BOOST_TEST_MODULE qc_extractor_expressionParser
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;
using namespace std;

BOOST_AUTO_TEST_SUITE( expressionParser_tests_suite01 )

BOOST_AUTO_TEST_CASE( expressionParser_setVariables )
{
	{
		ExpressionParser expParser = ExpressionParser();
		ElementsMap map;
		map["abcd"]= 0;

		expParser.setVariables( map );

		BOOST_CHECK_EQUAL( expParser.getPythonHeader(), "abcd = 0\n" );
	}
	{
		ExpressionParser expParser = ExpressionParser();
		ElementsMap map;
		map["abcd"]= 0;
		map["efgh"]= 1;
		map["ijkl"]= 2;
		map["mnop"]= 3;

		expParser.setVariables( map );

		BOOST_CHECK_EQUAL( expParser.getPythonHeader(), "abcd = 0\nefgh = 1\nijkl = 2\nmnop = 3\n" );
	}
}

BOOST_AUTO_TEST_CASE( expressionParser_addPythonHeader )
{
	{
		ExpressionParser expParser = ExpressionParser();
		ElementsMap map;
		map["abcd"]= 0;
		map["efgh"]= 1;
		map["ijkl"]= 2;
		map["mnop"]= 3;

		expParser.setVariables( map );
		expParser.addPythonHeader( "from math import *");

		BOOST_CHECK_EQUAL( expParser.getPythonHeader(), "from math import *\nabcd = 0\nefgh = 1\nijkl = 2\nmnop = 3\n" );
	}
}

BOOST_AUTO_TEST_CASE( expressionParser_parseExpression )
{
	{
		ExpressionParser expParser = ExpressionParser();
		ElementsMap map;
		map["abcd"]= 0;
		map["efgh"]= 1;
		map["ijkl"]= 2;
		map["mnop"]= 3;

		expParser.setVariables( map );
		expParser.addPythonHeader( "from math import *");
		
		BOOST_CHECK_EQUAL( expParser.parseExpression<size_t>( "8" ), 8 );
		BOOST_CHECK_EQUAL( expParser.parseExpression<size_t>( "3 * abcd" ), 0 );
		
		BOOST_CHECK_EQUAL( expParser.parseExpression<int>( "3 * abcd" ), 0 );
		BOOST_CHECK_EQUAL( expParser.parseExpression<int>( "mnop / 3" ), 1 );
		BOOST_CHECK_EQUAL( expParser.parseExpression<int>( "8" ), 8 );
		BOOST_CHECK_EQUAL( expParser.parseExpression<int>( "-8" ), -8 );

		BOOST_REQUIRE_CLOSE( expParser.parseExpression<float>( "2.2*2" ), 4.40, 0.01 );
		BOOST_REQUIRE_CLOSE( expParser.parseExpression<float>( "2.002*3" ), 6.006, 0.01 );

		BOOST_CHECK_EQUAL( expParser.parseExpression<std::string>( "'2.002*3'" ), "2.002*3" );
		BOOST_CHECK_EQUAL( expParser.parseExpression<std::string>( "str(2*3)" ), "6" );		 

		BOOST_CHECK_THROW( expParser.parseExpression<int>        ( "3 * other" ), boost::python::error_already_set );
		BOOST_CHECK_THROW( expParser.parseExpression<float>      ( " mnop / 3" ), boost::python::error_already_set );
		BOOST_CHECK_THROW( expParser.parseExpression<int>        ( "" ),          boost::python::error_already_set );
		BOOST_CHECK_THROW( expParser.parseExpression<size_t>     ( " " ),         boost::python::error_already_set );
		BOOST_CHECK_THROW( expParser.parseExpression<std::string>( "2*3" ),       boost::python::error_already_set );

		BOOST_CHECK_THROW( expParser.parseExpression<std::string>( "quotation marks are missing" ), boost::python::error_already_set );

	}
}

BOOST_AUTO_TEST_SUITE_END()
