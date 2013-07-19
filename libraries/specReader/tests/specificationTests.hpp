
BOOST_AUTO_TEST_SUITE( spec_reader_test_specification )

BOOST_AUTO_TEST_CASE( spec_reader_specification )
{
	LOG_INFO( "\n>>> spec_reader_specification <<<" );
	{
		bpt::ptree pt, node1, node2, node3;

		node3.put( "key1", "value1" );
		node3.put( "key2", "value2" );
		node3.put( "key3", "value3" );

		node2.push_back( std::make_pair( "", "ext1" ) );
		node2.push_back( std::make_pair( "", "ext2" ) );
		node2.push_back( std::make_pair( "", "ext3" ) );

		node1.put( "id", "test" );
		node1.put( "label", "Test" );
		node1.put( "type", "tester" );
		node1.add_child( "extension", node2 );

		pt.add_child( "standard", node1 );
		pt.add_child( "header", node3 );
		write_json( "test.json", pt );

		Specification spec;
		spec.setFromTree( pt );
		BOOST_CHECK_EQUAL( spec.getId(), "test" );
		BOOST_CHECK_EQUAL( spec.getLabel(), "Test" );
		BOOST_CHECK_EQUAL( spec.getType(), "tester" );

		std::vector< std::string > ext = spec.getSupportedExtensions();
		BOOST_CHECK_EQUAL( ext.at(0), "ext1" );
		BOOST_CHECK_EQUAL( ext.at(1), "ext2" );
		BOOST_CHECK_EQUAL( ext.at(2), "ext3" );

		bpt::ptree bodyTree = spec.getBody();
		BOOST_CHECK_EQUAL( bodyTree.get_child( "key1" ).data(), "value1" );
		BOOST_CHECK_EQUAL( bodyTree.get_child( "key2" ).data(), "value2" );
		BOOST_CHECK_EQUAL( bodyTree.get_child( "key3" ).data(), "value3" );
		// write_json( "test.json", bodyTree );

	}
	{
		std::string jsonString = " { \"standard\": { \"id\": \"test\", \"label\": \"Test\", \"type\": \"tester\" } } ";

		Specification spec;
		spec.setFromString( jsonString );
		BOOST_CHECK_EQUAL( spec.getId(), "test" );
		BOOST_CHECK_EQUAL( spec.getLabel(), "Test" );
		BOOST_CHECK_EQUAL( spec.getType(), "tester" );
	}
	{
		Specification spec;
		bool isSet = spec.setFromFile( "test.json" );
		BOOST_CHECK_EQUAL( isSet, true );
		BOOST_CHECK_EQUAL( spec.getId(), "test" );
		BOOST_CHECK_EQUAL( spec.getLabel(), "Test" );
		BOOST_CHECK_EQUAL( spec.getType(), "tester" );
	}
	{
		Specification spec;
		bool isSet = spec.setFromFile( "test.txt" );
		BOOST_CHECK_EQUAL( isSet, false );
	}
	{
		Specification spec;
		bool isSet = spec.setFromFile( "tests" );
		BOOST_CHECK_EQUAL( isSet, false );
	}
	{
		Specification spec;
		bool isSet = spec.setFromFile( "test" );
		BOOST_CHECK_EQUAL( isSet, false );
	}
}

BOOST_AUTO_TEST_SUITE_END()
