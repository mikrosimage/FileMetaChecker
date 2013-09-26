
BOOST_AUTO_TEST_SUITE( basic_element_tests_expression_parser )

BOOST_AUTO_TEST_CASE( basic_element_expression_parser_set_variables )
{
	LOG_INFO( "\n >>> basic_element_expression_parser <<< " );
	{
		bep::ExpressionParser expParser = bep::ExpressionParser();
		std::map < std::string, short > map;
		map["abcd"]= 0;

		expParser.setVariables< short >( map );

		BOOST_CHECK_EQUAL( expParser.getContextString(), "abcd = 0\n" );
	}
	{
		bep::ExpressionParser expParser = bep::ExpressionParser();
		std::map < std::string, unsigned int > map;
		map["abcd"]= 0;
		map["efgh"]= 1;
		map["ijkl"]= 2;
		map["mnop"]= 3;

		expParser.setVariables< unsigned int >( map );

		BOOST_CHECK_EQUAL( expParser.getContextString(), "abcd = 0\nefgh = 1\nijkl = 2\nmnop = 3\n" );
	}
	{
		bep::ExpressionParser expParser = bep::ExpressionParser();
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
		bep::ExpressionParser expParser = bep::ExpressionParser();
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
		bep::ExpressionParser expParser = bep::ExpressionParser( map );

		BOOST_CHECK_EQUAL( expParser.getContextString(), "long1 = -600\nlong2 = 1000\nlong3 = 1\n" );
	}
	{
		std::map < std::string, unsigned long long > map;
		map["longlong1"]= std::numeric_limits< unsigned long long >::min();
		map["longlong2"]= std::numeric_limits< unsigned long long >::max();
		bep::ExpressionParser expParser = bep::ExpressionParser( map );

		BOOST_CHECK_EQUAL( expParser.getContextString(), "longlong1 = 0\nlonglong2 = 18446744073709551615\n" );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_expression_parser_addPythonHeader )
{
	{
		bep::ExpressionParser expParser = bep::ExpressionParser();
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

BOOST_AUTO_TEST_CASE( basic_element_expression_parser_get_expression_result )
{
	{
		bep::ExpressionParser expParser = bep::ExpressionParser();
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
		bep::ExpressionParser expParser = bep::ExpressionParser();
		BOOST_CHECK_THROW( expParser.getExpressionResult< int >( "log(1)" ), boost::python::error_already_set );
	}
}


BOOST_AUTO_TEST_CASE( basic_element_expression_parser_shared_ptr )
{
	LOG_WARNING( ">>> basic_element_expression_parser_shared_ptr <<<" );
	{
		std::shared_ptr< be::number_element::Number< int   > > num1( new be::number_element::Number< int   >( id ) );
		std::shared_ptr< be::number_element::Number< short > > num2( new be::number_element::Number< short >( id ) );
		std::shared_ptr< be::number_element::Number< float > > num3( new be::number_element::Number< float >( id ) );

		char buffer1[] = { 0x00, 0x00, 0x00, 0x01 };
		char buffer2[] = { 0x00, 0x02 };
		char buffer3[] = { 0x3f, 0x7f, 0x00, 0x00 };
		
		num1->setData( buffer1, 4 );
		num2->setData( buffer2, 2 );
		num3->setData( buffer3, 4 );

		std::string res1 = num1->get< eDisplayTypeAscii, std::string >();
		std::string res2 = num2->get< eDisplayTypeAscii, std::string >();
		std::string res3 = num3->get< eDisplayTypeAscii, std::string >();
		
		BOOST_CHECK_EQUAL( res1, "1"        );
		BOOST_CHECK_EQUAL( res2, "2"        );
		BOOST_CHECK_EQUAL( res3, "0.996094" );

		std::map< std::string, std::shared_ptr< be::Element > > elementList;
		elementList.insert( std::make_pair( "num1", num1 ) );
		elementList.insert( std::make_pair( "num2", num2 ) );
		elementList.insert( std::make_pair( "num3", num3 ) );

		bep::ExpressionParser expParser = bep::ExpressionParser( elementList );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult< int   >( "num1" ), 1 );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult< int   >( "num2" ), 2 );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult< short >( "num1 + num2" ),  3 );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult< int   >( "num2 - num1" ),  1 );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult< float >( "num1 - num2" ), -1 );
		BOOST_CHECK_EQUAL( expParser.getExpressionResult< int   >( "num1 * num2" ),  2 );
		BOOST_CHECK_CLOSE( expParser.getExpressionResult< float >( "num1 * num3" ),  0.996094, 0.001 );
		BOOST_CHECK_CLOSE( expParser.getExpressionResult< float >( "num3 / num2" ),  0.498047, 0.001 );
	}
}

BOOST_AUTO_TEST_SUITE_END()
