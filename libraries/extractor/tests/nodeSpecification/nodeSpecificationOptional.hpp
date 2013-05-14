BOOST_AUTO_TEST_SUITE( nodeSpecification_tests_optional )

BOOST_AUTO_TEST_CASE( nodeSpecification_optionalTestFile )
{
	{
		const std::string nodename = "optionalTest1";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Optional Test 1" );
		node.put( "optional", "true" );
		node.put( "ascii", "OPTIONAL_NODE" );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "OPTIONAL_NODE";
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "OPTIONAL_NODE" );
		CHECK_NODE( nodename, "<xmlattr>.optional", "true" );
	}
	{
		const std::string nodename = "optionalTest2";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Optional Test 2" );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "GOOD_VALUE";
		stream.close();

		const std::string nodename1 = "optionalSubNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Optional SubNode 1" );
		subnode1.put( "optional", "true" );
		subnode1.put( "ascii", "BAD_VALUE" );

		const std::string nodename2 = "optionalSubNode2";
		bpt::ptree subnode2;
		subnode2.put( "id",  nodename2 );
		subnode2.put( "label", "Optional SubNode 2" );
		subnode2.put( "optional", "true" );
		subnode2.put( "ascii", "GOOD_VALUE" );

		node.add_child( "group.", subnode1 );
		node.add_child( "group.", subnode2 );

		bpt::ptree report = generateReportTree( node );
		BOOST_CHECK_THROW( CHECK_NODE_VALID ( nodename + "." + nodename1 ), std::runtime_error );
		CHECK_VALUE_EQUAL( nodename + "." + nodename2, "GOOD_VALUE" );
		CHECK_NODE_VALID ( nodename + "." + nodename2 );
		CHECK_NODE_VALID ( nodename );
	}
}

BOOST_AUTO_TEST_SUITE_END()
