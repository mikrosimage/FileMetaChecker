
BOOST_AUTO_TEST_SUITE( basic_element_test_range )

BOOST_AUTO_TEST_CASE( basic_element_range )
{
	LOG_INFO( "\n>>> basic_element_range <<<" );
	{
		std::vector< short > vectObj;
		vectObj.push_back(  0 );
		vectObj.push_back( 10 );

		be::Range< short > rangeObj(vectObj );

		BOOST_CHECK_EQUAL( rangeObj.at( 0 ),  0 );
		BOOST_CHECK_EQUAL( rangeObj.at( 1 ), 10 );
		
		BOOST_CHECK_EQUAL( rangeObj.getSize(), 2 );
	}
	{
		be::Range< int > rangeObj;

		rangeObj.setRange( 0, 10 );
		BOOST_CHECK_EQUAL( rangeObj.at( 0 ),  0 );
		BOOST_CHECK_EQUAL( rangeObj.at( 1 ), 10 );

		BOOST_CHECK_EQUAL( rangeObj.getSize(), 2 );

		BOOST_CHECK_EQUAL( rangeObj.isInRange( -1 ), false );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(  0 ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(  5 ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( 10 ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( 11 ), false );
	}
	{
		be::Range< int > rangeObj;
		BOOST_CHECK_THROW( rangeObj.setRange( 10, 0 ), std::range_error );
	}

	{
		be::Range< float > rangeObj;

		rangeObj.setRange( 0, 0.99 );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( -0.01 ), false );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(  0    ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(  0.45 ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( 	0.99 ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(  1    ), false );
	}

	{
		be::Range< std::string > rangeObj;

		rangeObj.setRange( "0", "ff" );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(   "0" ), true );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(   "f" ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(  "ff" ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( "100" ), false );
	}
	{
		be::Range< std::string > rangeObj;
		BOOST_CHECK_THROW( rangeObj.setRange( "10", "0" ), std::range_error );
	}
	{
		be::Range< std::string > rangeObj;

		rangeObj.setRange( "min", "max" );
		BOOST_CHECK_EQUAL( rangeObj.at( 0 ), "min" );
		BOOST_CHECK_EQUAL( rangeObj.at( 1 ), "max" );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( "0" ), true );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( "1" ), false );
	}
}

BOOST_AUTO_TEST_SUITE_END()
