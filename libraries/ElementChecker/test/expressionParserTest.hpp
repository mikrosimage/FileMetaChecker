
#include <ElementChecker/Translator/Translator.hpp>
#include <ElementChecker/ExpressionParser/ExpressionParser.hpp>

BOOST_AUTO_TEST_SUITE( element_checker_tests_expression_parser )

// BOOST_AUTO_TEST_CASE( element_checker_expression_parser_set_variables )
// {
// 	LOG_INFO( "\n >>> element_checker_expression_parser_set_variables <<< " );
// 	{
// 		bep::ExpressionParser expParser = bep::ExpressionParser();
// 		std::map < std::string, short > map;
// 		map["abcd"]= 0;

// 		expParser.setVariables< short >( map );

// 		BOOST_CHECK_EQUAL( expParser.getContextString(), "abcd = 0\n" );
// 	}
// 	{
// 		bep::ExpressionParser expParser = bep::ExpressionParser();
// 		std::map < std::string, unsigned int > map;
// 		map["abcd"]= 0;
// 		map["efgh"]= 1;
// 		map["ijkl"]= 2;
// 		map["mnop"]= 3;

// 		expParser.setVariables< unsigned int >( map );

// 		BOOST_CHECK_EQUAL( expParser.getContextString(), "abcd = 0\nefgh = 1\nijkl = 2\nmnop = 3\n" );
// 	}
// 	{
// 		bep::ExpressionParser expParser = bep::ExpressionParser();
// 		std::map < std::string, short > mapShort;
// 		std::map < std::string, int > mapInt;
// 		mapShort["abcd"]= 0;
// 		mapShort["efgh"]= 1;
// 		mapInt["ijkl"]= -20;
// 		mapInt["mnop"]= 30;

// 		expParser.setVariables< short >( mapShort );
// 		expParser.setVariables< int >( mapInt );

// 		BOOST_CHECK_EQUAL( expParser.getContextString(), "abcd = 0\nefgh = 1\nijkl = -20\nmnop = 30\n" );
// 	}
// 	{
// 		bep::ExpressionParser expParser = bep::ExpressionParser();
// 		std::map < std::string, float > map;
// 		map["float1"]= 0.001;
// 		map["float2"]= 1.23;
// 		map["float3"]= true;
// 		expParser.setVariables< float >( map );

// 		BOOST_CHECK_EQUAL( expParser.getContextString(), "float1 = 0.001\nfloat2 = 1.23\nfloat3 = 1\n" );
// 	}
// 	{
// 		std::map < std::string, long > map;
// 		map["long1"]= -600;
// 		map["long2"]= 1000;
// 		map["long3"]= true;
// 		bep::ExpressionParser expParser = bep::ExpressionParser( map );

// 		BOOST_CHECK_EQUAL( expParser.getContextString(), "long1 = -600\nlong2 = 1000\nlong3 = 1\n" );
// 	}
// 	{
// 		std::map < std::string, unsigned long long > map;
// 		map["longlong1"]= std::numeric_limits< unsigned long long >::min();
// 		map["longlong2"]= std::numeric_limits< unsigned long long >::max();
// 		bep::ExpressionParser expParser = bep::ExpressionParser( map );

// 		BOOST_CHECK_EQUAL( expParser.getContextString(), "longlong1 = 0\nlonglong2 = 18446744073709551615\n" );
// 	}
// }

// BOOST_AUTO_TEST_CASE( element_checker_expression_parser_addPythonHeader )
// {
// 	{
// 		bep::ExpressionParser expParser = bep::ExpressionParser();
// 		std::map < std::string, short > map;
// 		map["abcd"]= 0;
// 		map["efgh"]= 1;
// 		map["ijkl"]= 2;
// 		map["mnop"]= 3;

// 		expParser.setVariables< short >( map );
// 		expParser.addPythonHeader( "from math import *");

// 		BOOST_CHECK_EQUAL( expParser.getContextString(), "from math import *\nabcd = 0\nefgh = 1\nijkl = 2\nmnop = 3\n" );
// 	}
// }

// BOOST_AUTO_TEST_CASE( element_checker_expression_parser_get_expression_result )
// {
// 	{
// 		bep::ExpressionParser expParser = bep::ExpressionParser();
// 		std::map < std::string, int > map;
// 		map["abcd"]= 0;
// 		map["efgh"]= 1;
// 		map["ijkl"]= 2;
// 		map["mnop"]= 3;

// 		expParser.setVariables< int >( map );
// 		expParser.addPythonHeader( "from math import *");
		
// 		BOOST_CHECK_EQUAL( expParser.getExpressionResult<size_t>( "8" ), 8 );
// 		BOOST_CHECK_EQUAL( expParser.getExpressionResult<size_t>( "3 * abcd" ), 0 );
		
// 		BOOST_CHECK_EQUAL( expParser.getExpressionResult<int>( "3 * abcd" ), 0 );
// 		BOOST_CHECK_EQUAL( expParser.getExpressionResult<int>( "mnop / 3" ), 1 );
// 		BOOST_CHECK_EQUAL( expParser.getExpressionResult<int>(  "8" ),  8 );
// 		BOOST_CHECK_EQUAL( expParser.getExpressionResult<int>( "-8" ), -8 );

// 		BOOST_REQUIRE_CLOSE( expParser.getExpressionResult<float>( "2.2*2"   ), 4.40,  0.01 );
// 		BOOST_REQUIRE_CLOSE( expParser.getExpressionResult<float>( "2.002*3" ), 6.006, 0.01 );

// 		BOOST_CHECK_EQUAL( expParser.getExpressionResult<std::string>( "'2.002*3'" ), "2.002*3" );
// 		BOOST_CHECK_EQUAL( expParser.getExpressionResult<std::string>( "str(2*3)"  ), "6"       );		 

// 		BOOST_CHECK_THROW( expParser.getExpressionResult<int>        ( "3 * other" ), boost::python::error_already_set );
// 		BOOST_CHECK_THROW( expParser.getExpressionResult<float>      ( " mnop / 3" ), boost::python::error_already_set );
// 		BOOST_CHECK_THROW( expParser.getExpressionResult<int>        ( "" ),          boost::python::error_already_set );
// 		BOOST_CHECK_THROW( expParser.getExpressionResult<size_t>     ( " " ),         boost::python::error_already_set );
// 		BOOST_CHECK_THROW( expParser.getExpressionResult<std::string>( "2*3" ),       boost::python::error_already_set );

// 		BOOST_CHECK_THROW( expParser.getExpressionResult<std::string>( "quotation marks are missing" ), boost::python::error_already_set );
// 	}
// 	{
// 		bep::ExpressionParser expParser = bep::ExpressionParser();
// 		BOOST_CHECK_THROW( expParser.getExpressionResult< int >( "log(1)" ), boost::python::error_already_set );
// 	}
// }


BOOST_AUTO_TEST_CASE( element_checker_expression_parser_shared_ptr )
{
	LOG_INFO( ">>> element_checker_expression_parser_shared_ptr <<<" );
	{
		std::string jsonString = R"*(
				{
					"content": [
						{ "id": "value1", "label": "Value1", "type": "uint32" },
						{ "id": "value2", "label": "Value2", "type": "uint16" },
						{ "id": "value3", "label": "Value3", "type": "float" },
						{ "id": "value4", "label": "Value4", "type": "ascii" },
						{ "id": "value0", "label": "Value0", "type": "uint32" },
						{ "id": "value5", "label": "Value5", "type": "ascii" }
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > num1( new basic_element::Element( node ) );
		std::shared_ptr< basic_element::Element > num2( new basic_element::Element( node->next() ) );
		std::shared_ptr< basic_element::Element > num3( new basic_element::Element( node->next()->next() ) );
		std::shared_ptr< basic_element::Element > num4( new basic_element::Element( node->next()->next()->next() ) );
		std::shared_ptr< basic_element::Element > num0( new basic_element::Element( node->next()->next()->next()->next() ) );
		std::shared_ptr< basic_element::Element > num5( new basic_element::Element( node->next()->next()->next()->next()->next() ) );

		char buffer1[] = { 0x00, 0x00, 0x00, 0x01 };
		char buffer2[] = { 0x00, 0x02 };
		char buffer3[] = { 0x3f, 0x7f, 0x00, 0x00 };
		char buffer0[] = { 0x00, 0x00, 0x00, 0x00 };
		char buffer5[] = { 'w', 'o', 'r', 'd' };
		
		num1->set( buffer1, 4 );
		num2->set( buffer2, 2 );
		num3->set( buffer3, 4 );
		num0->set( buffer0, 4 );
		num5->set( buffer5, 4 );

		std::ostringstream res1;
		std::ostringstream res2;
		std::ostringstream res3;
		std::ostringstream res0;
		std::ostringstream res5;

		res1 << Translator( num1 ).get< basic_element::uint32 >();
		res2 << Translator( num2 ).get< basic_element::uint16 >();
		res3 << Translator( num3 ).get< float >();
		res0 << Translator( num0 ).get< basic_element::uint32 >();
		res5 << Translator( num5 ).get();

		BOOST_CHECK_EQUAL( res1.str(), "1"        );
		BOOST_CHECK_EQUAL( res2.str(), "2"        );
		BOOST_CHECK_EQUAL( res3.str(), "0.996094" );
		BOOST_CHECK_EQUAL( res0.str(), "0" );
		BOOST_CHECK_EQUAL( res5.str(), "word" );

		std::vector< std::shared_ptr< basic_element::Element > > elementList;
		elementList.push_back( num1 );
		elementList.push_back( num2 );
		elementList.push_back( num3 );
		elementList.push_back( num4 );
		elementList.push_back( num0 );
		elementList.push_back( num5 );

		ExpressionParser expParser( elementList );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult< int   >( "value1" ), 1 );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult< int   >( "value2" ), 2 );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult< short >( "value1 + value2" ),  3 );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult< int   >( "value2 - value1" ),  1 );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult< float >( "value1 - value2" ), -1 );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult< int   >( "value1 * value2" ),  2 );
		BOOST_CHECK_CLOSE( expParser.getExpressionResult< float >( "value1 * value3" ),  0.996094, 0.001 );
		BOOST_CHECK_CLOSE( expParser.getExpressionResult< float >( "value3 / value2" ),  0.498047, 0.001 );
		
		BOOST_CHECK_EQUAL( expParser.getExpressionResult< bool  >( "value4 == ''" ), true );
		
		BOOST_CHECK_EQUAL( expParser.getExpressionResult< int   >( "value0" ), 0 );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult< bool  >( "value0" ), false );

		BOOST_CHECK_EQUAL( expParser.getExpressionResult< bool  >( "value5 is 'word'" ), true );
	}
}

BOOST_AUTO_TEST_SUITE_END()
