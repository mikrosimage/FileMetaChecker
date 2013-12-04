#include <ElementChecker/Map/Map.hpp>

BOOST_AUTO_TEST_SUITE( element_checker_test_map )

BOOST_AUTO_TEST_CASE( element_checker_test_map )
{
	{
		LOG_INFO( "\n>>> element_checker_test_map int <<<" );
		std::map< std::string, std::string > elemMap {
			{ "0", "zero" },
			{ "1", "one"  },
			{ "2", "two"  }
		};
		Map< int > map( elemMap );
		BOOST_CHECK_EQUAL( map.getSize(), 3 );
		BOOST_CHECK_EQUAL( map.getLabel( 0 ), "zero" );
		BOOST_CHECK_EQUAL( map.getLabel( 1 ), "one"  );
		BOOST_CHECK_EQUAL( map.getLabel( 2 ), "two"  );
		BOOST_CHECK_EQUAL( map.getLabel( 3 ), ""     );
		BOOST_CHECK_EQUAL( map.getSize(), 3 );
	}
	{
		LOG_INFO( "\n>>> element_checker_test_map float <<<" );
		std::map< std::string, std::string > elemMap {
			{ "0", "zero" },
			{ "1", "one"  },
			{ "2", "two"  }
		};
		Map< float > map( elemMap );
		BOOST_CHECK_EQUAL( map.getSize(), 3 );
		BOOST_CHECK_EQUAL( map.getLabel( 0.0 ), "zero" );
		BOOST_CHECK_EQUAL( map.getLabel( 1.0 ), "one"  );
		BOOST_CHECK_EQUAL( map.getLabel( 2.0 ), "two"  );
		BOOST_CHECK_EQUAL( map.getLabel( 3.0 ), ""     );
		BOOST_CHECK_EQUAL( map.getSize(), 3 );
	}
	{
		LOG_INFO( "\n>>> element_checker_test_map char <<<" );
		std::map< std::string, std::string > elemMap {
			{ "0", "zero" },
			{ "1", "one"  },
			{ "2", "two"  }
		};
		Map< char > map( elemMap );
		BOOST_CHECK_EQUAL( map.getSize(), 3 );
		BOOST_CHECK_EQUAL( map.getLabel( 0 ), "zero" );
		BOOST_CHECK_EQUAL( map.getLabel( 1 ), "one"  );
		BOOST_CHECK_EQUAL( map.getLabel( 2 ), "two"  );
		BOOST_CHECK_EQUAL( map.getLabel( 3 ), ""     );
		BOOST_CHECK_EQUAL( map.getSize(), 3 );
	}
	{
		LOG_INFO( "\n>>> element_checker_test_map unsigned char <<<" );
		std::map< std::string, std::string > elemMap {
			{ "0", "zero" },
			{ "1", "one"  },
			{ "2", "two"  }
		};
		Map< unsigned char > map( elemMap );
		BOOST_CHECK_EQUAL( map.getSize(), 3 );
		BOOST_CHECK_EQUAL( map.getLabel( 0 ), "zero" );
		BOOST_CHECK_EQUAL( map.getLabel( 1 ), "one"  );
		BOOST_CHECK_EQUAL( map.getLabel( 2 ), "two"  );
		BOOST_CHECK_EQUAL( map.getLabel( 3 ), ""     );
		BOOST_CHECK_EQUAL( map.getSize(), 3 );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_test_map_errors )
{
	{
		LOG_INFO( "\n>>> element_checker_test_map_errors int <<<" );
		std::map< std::string, std::string > elemMap {
			{ "2147483647", "zero" },
			{ "2147483648", "one"  },
			{ "2147483649", "two"  }
		};
		BOOST_CHECK_THROW( Map< int > map( elemMap ), std::runtime_error );
	}
	{
		LOG_INFO( "\n>>> element_checker_test_map_errors char <<<" );
		std::map< std::string, std::string > elemMap {
			{  "127", "zero" },
			{  "128", "one"  },
			{ "-129", "two"  }
		};
		BOOST_CHECK_THROW( Map< char > map( elemMap ), std::range_error );
	}
	{
		LOG_INFO( "\n>>> element_checker_test_map_errors unsigned char <<<" );
		std::map< std::string, std::string > elemMap {
			{ "255", "zero" },
			{ "256", "one"  },
			{ "257", "two"  }
		};
		BOOST_CHECK_THROW( Map< unsigned char > map( elemMap ), std::range_error );
	}
}

BOOST_AUTO_TEST_SUITE_END()
