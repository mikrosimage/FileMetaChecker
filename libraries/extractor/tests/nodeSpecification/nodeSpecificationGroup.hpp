BOOST_AUTO_TEST_SUITE( nodeSpecification_tests_group )

BOOST_AUTO_TEST_CASE( nodeSpecification_genericGroupTestFile )
{
	{
		const std::string nodename = "genericGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Generic Group Test" );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "ASCII" << (uint8) 0x00 << (uint8) 0x10;
		stream.close();

		const std::string nodename1 = "groupNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Group Node 1" );
		subnode1.put( "ascii", "ASCII" );

		const std::string nodename2 = "groupNode2";
		bpt::ptree subnode2;
		subnode2.put( "id",  nodename2 );
		subnode2.put( "label", "Group Node 2" );
		subnode2.put( "type", "uint16" );

		node.add_child( "group.", subnode1 );
		node.add_child( "group.", subnode2 );

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1, "ASCII" );
		CHECK_NODE_VALID ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename2, "16" );
		CHECK_NODE_VALID ( nodename );
	}
}

BOOST_AUTO_TEST_SUITE_END()
