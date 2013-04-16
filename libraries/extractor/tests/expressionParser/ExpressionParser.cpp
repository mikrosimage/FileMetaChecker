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
		int result;
		result = expParser.parseExpression( "ijkl + mnop + efgh" );
		BOOST_CHECK_EQUAL( result, 6 );
		result = expParser.parseExpression( "3 * abcd" );
		BOOST_CHECK_EQUAL( result, 0 );
		result = expParser.parseExpression( "mnop / 3" );
		BOOST_CHECK_EQUAL( result, 1 );
		BOOST_CHECK_THROW( expParser.parseExpression( " mnop / 3" ), boost::python::error_already_set );
		BOOST_CHECK_THROW( expParser.parseExpression( "" ), boost::python::error_already_set );
		BOOST_CHECK_THROW( expParser.parseExpression( " " ), boost::python::error_already_set );
	}
}

BOOST_AUTO_TEST_SUITE_END()
