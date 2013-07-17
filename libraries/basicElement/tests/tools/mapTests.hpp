
BOOST_AUTO_TEST_SUITE( basic_element_test_map )

BOOST_AUTO_TEST_CASE( basic_element_map )
{
	LOG_INFO( "\n>>> basic_element_map <<<" );
	{
		std::map< int, std::string > map;
		map.insert( std::pair< int, std::string >( 1, "First number"  ) );
		map.insert( std::pair< int, std::string >( 2, "Second number" ) );
		map.insert( std::pair< int, std::string >( 3, "Third number"  ) );

		be::Map< int > mapObj( map );

		BOOST_CHECK_EQUAL( mapObj.getLabel( 0 ), ""              );
		BOOST_CHECK_EQUAL( mapObj.getLabel( 1 ), "First number"  );
		BOOST_CHECK_EQUAL( mapObj.getLabel( 2 ), "Second number" );
		BOOST_CHECK_EQUAL( mapObj.getLabel( 3 ), "Third number"  );

		BOOST_CHECK_EQUAL( mapObj.getSize(), 3 );
	}
	{
		std::map< char, std::string > map;
		map.insert( std::pair< char, std::string >( '1', "First number"  ) );
		map.insert( std::pair< char, std::string >( '2', "Second number" ) );
		map.insert( std::pair< char, std::string >( '3', "Third number"  ) );

		be::Map< char > mapObj;
		mapObj.setMap( map );

		BOOST_CHECK_EQUAL( mapObj.getLabel( '0' ), ""              );
		BOOST_CHECK_EQUAL( mapObj.getLabel( '1' ), "First number"  );
		BOOST_CHECK_EQUAL( mapObj.getLabel( '2' ), "Second number" );
		BOOST_CHECK_EQUAL( mapObj.getLabel( '3' ), "Third number"  );
		
		BOOST_CHECK_EQUAL( mapObj.getSize(), 3 );
	}
	{
		std::map< int, std::string > map;
		std::pair< int, std::string > pair1( 1, "First number"  );
		std::pair< int, std::string > pair2( 2, "Second number" );
		std::pair< int, std::string > pair3( 3, "Third number"  );

		be::Map< int > mapObj;
		mapObj.setMap( map );

		mapObj.insertPair( pair1 );
		BOOST_CHECK_EQUAL( mapObj.getLabel( 1 ), "First number"  );
		BOOST_CHECK_EQUAL( mapObj.getSize(), 1 );

		mapObj.insertPair( pair3 );
		BOOST_CHECK_EQUAL( mapObj.getLabel( 3 ), "Third number"  );
		BOOST_CHECK_EQUAL( mapObj.getSize(), 2 );

		mapObj.insertPair( pair2 );
		BOOST_CHECK_EQUAL( mapObj.getLabel( 2 ), "Second number"  );
		BOOST_CHECK_EQUAL( mapObj.getSize(), 3 );

		mapObj.addPair( 10, "Tenth number" );
		BOOST_CHECK_EQUAL( mapObj.getLabel( 10 ), "Tenth number"  );
		BOOST_CHECK_EQUAL( mapObj.getSize(), 4 );
	}
}

BOOST_AUTO_TEST_SUITE_END()
