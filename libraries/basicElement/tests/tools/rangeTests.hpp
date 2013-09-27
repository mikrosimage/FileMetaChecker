
BOOST_AUTO_TEST_SUITE( basic_element_test_range )

BOOST_AUTO_TEST_CASE( basic_element_range )
{
	LOG_INFO( "\n>>> basic_element_range <<<" );
	{
		be::Range< int > rangeObj;
		rangeObj.setRange( 0, 10 );
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
		BOOST_CHECK_EQUAL( rangeObj.isInRange( 10 ), true );
	}
	{
		be::Range< int > rangeObj;
		rangeObj.setMinimum( 0 );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(   -1 ), false );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(    0 ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(    5 ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(   10 ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( 1110 ), true );
	}
	{
		be::Range< int > rangeObj;
		rangeObj.setMaximum( 0 );
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
		be::Range< float > rangeObj;
		rangeObj.setMaximum( 0 );
		BOOST_CHECK_EQUAL( rangeObj.isInRange(  0.01 ), false );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( -0.01 ), true  );
		BOOST_CHECK_EQUAL( rangeObj.isInRange( -1000 ), true  );
	}

	{
		be::Range< long double > range1;
		BOOST_CHECK_EQUAL( range1.isInRange( -1.1896e+4932L ), true  );
		BOOST_CHECK_EQUAL( range1.isInRange( -2.0000e+4932L ), false );
	}
}


BOOST_AUTO_TEST_CASE( basic_element_ranges )
{
	LOG_INFO( "\n>>> basic_element_ranges <<<" );
	{
		be::Range< int > range1;
		be::Range< int > range2;
		be::Range< int > range3;
		be::Range< int > range4;

		range1.setRange( -5,  5 );
		range2.setRange( -2,  8 );
		range3.setRange(  0, 10 );
		range4.setRange(  2, 12 );

		be::Ranges< int > ranges;
		ranges.addRange( range1 );
		ranges.addRange( range2 );
		ranges.addRange( range3 );
		ranges.addRange( range4 );

		BOOST_CHECK_EQUAL( ranges.isInRanges( -6 ), false );
		BOOST_CHECK_EQUAL( ranges.isInRanges( -2 ), true  );
		BOOST_CHECK_EQUAL( ranges.isInRanges(  3 ), true  );
		BOOST_CHECK_EQUAL( ranges.isInRanges(  9 ), true  );
		BOOST_CHECK_EQUAL( ranges.isInRanges( 11 ), true  );
		BOOST_CHECK_EQUAL( ranges.isInRanges( 13 ), false );
	}
	{
		be::Range< int > range1;
		be::Range< int > range2;
		be::Range< int > range3;
		
		range1.setMaximum( -50 );
		range2.setMinimum(  50 );
		range3.setRange( -1, 1 );

		be::Ranges< int > ranges;
		ranges.addRange( range1 );
		ranges.addRange( range2 );
		ranges.addRange( range3 );

		BOOST_CHECK_EQUAL( ranges.isInRanges( -50 ), true  );
		BOOST_CHECK_EQUAL( ranges.isInRanges( -49 ), false );
		BOOST_CHECK_EQUAL( ranges.isInRanges(  -2 ), false );
		BOOST_CHECK_EQUAL( ranges.isInRanges(   0 ), true  );
		BOOST_CHECK_EQUAL( ranges.isInRanges(   2 ), false );
		BOOST_CHECK_EQUAL( ranges.isInRanges(  49 ), false );
		BOOST_CHECK_EQUAL( ranges.isInRanges(  50 ), true  );
	}

	{
		be::Range< float > range1;
		be::Range< float > range2;
		be::Range< float > range3;
		be::Range< float > range4;

		range1.setRange( -1,    0.5  );
		range2.setRange( -1.5,  0    );
		range3.setRange( -1.25, 0.25 );
		range4.setRange( -0.5,  1    );

		be::Ranges< float > ranges;
		ranges.addRange( range1 );
		ranges.addRange( range2 );
		ranges.addRange( range3 );
		ranges.addRange( range4 );

		BOOST_CHECK_EQUAL( ranges.isInRanges( -1.51 ), false );
		BOOST_CHECK_EQUAL( ranges.isInRanges( -0.56 ), true  );
		BOOST_CHECK_EQUAL( ranges.isInRanges(  0.26 ), true  );
		BOOST_CHECK_EQUAL( ranges.isInRanges(  1.01 ), false );
	}
	{
		be::Range< float > range1;
		be::Range< float > range2;
		be::Range< float > range3;
		
		range1.setMaximum( -1 );
		range2.setMinimum(  1 );
		range3.setRange( -0.01, 0.01 );

		be::Ranges< float > ranges;
		ranges.addRange( range1 );
		ranges.addRange( range2 );
		ranges.addRange( range3 );

		BOOST_CHECK_EQUAL( ranges.isInRanges( -1.01 ), true  );
		BOOST_CHECK_EQUAL( ranges.isInRanges( -0.99 ), false );
		BOOST_CHECK_EQUAL( ranges.isInRanges( -0.02 ), false );
		BOOST_CHECK_EQUAL( ranges.isInRanges(  0    ), true  );
		BOOST_CHECK_EQUAL( ranges.isInRanges(  0.02 ), false );
		BOOST_CHECK_EQUAL( ranges.isInRanges(  0.99 ), false );
		BOOST_CHECK_EQUAL( ranges.isInRanges(  1.01 ), true  );
	}
}

BOOST_AUTO_TEST_SUITE_END()
