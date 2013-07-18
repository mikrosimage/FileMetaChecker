
BOOST_AUTO_TEST_SUITE( basic_element_test_range )

BOOST_AUTO_TEST_CASE( basic_element_range )
{
	LOG_INFO( "\n>>> basic_element_range <<<" );
	{
		be::Range< short > rangeObj( 0, 10 );
		BOOST_CHECK_EQUAL( rangeObj.isSet(), true );
	}
	{
		be::Range< int > rangeObj;
		rangeObj.setRange( 0, 10 );
		BOOST_CHECK_EQUAL( rangeObj.isSet(), true );
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
		be::Range< int > rangeObj;
		BOOST_CHECK_THROW( rangeObj.isInRange( 10 ), std::range_error );
	}
	{
		be::Range< int > rangeObj;
		rangeObj.setMinimum( 0 );
		BOOST_CHECK_EQUAL( rangeObj.isSet(), true );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(   -1 ), false );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(    0 ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(    5 ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(   10 ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( 1110 ), true );
	}
	{
		be::Range< int > rangeObj;
		rangeObj.setMaximum( 0 );
		BOOST_CHECK_EQUAL( rangeObj.isSet(), true );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(   -1 ), true );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(    0 ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(    5 ), false  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(   10 ), false  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( 1110 ), false );
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
		BOOST_CHECK_THROW( rangeObj.isInRange( "10" ), std::range_error );
	}
	{
		be::Range< std::string > rangeObj;
		rangeObj.setRange( "min", "max" );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( "0" ), true );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( "1" ), false );
	}
	{
		be::Range< std::string > rangeObj;
		rangeObj.setMinimum( "10" );
		BOOST_CHECK_EQUAL( rangeObj.isSet(), true );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(    "0" ), false  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(    "5" ), false  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(   "10" ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( "1110" ), true );
	}
	{
		be::Range< std::string > rangeObj;
		rangeObj.setMaximum( "10" );
		BOOST_CHECK_EQUAL( rangeObj.isSet(), true );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(    "0" ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(    "5" ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(   "10" ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( "1110" ), false );
	}
}

BOOST_AUTO_TEST_SUITE_END()
