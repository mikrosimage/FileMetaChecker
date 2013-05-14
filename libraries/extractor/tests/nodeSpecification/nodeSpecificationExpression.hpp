BOOST_AUTO_TEST_SUITE( nodeSpecification_tests_expression )

BOOST_AUTO_TEST_CASE( nodeSpecification_expressionTestFile )
{
	{
		const std::string nodename = "expressionTest";
		bpt::ptree tree;
		tree.put( "id",  nodename );
		tree.put( "label", "Expression Test" );

		const std::string subnodename1 = "value1";
		bpt::ptree subnode1;
		subnode1.put( "id",  subnodename1 );
		subnode1.put( "label", "Value 1" );
		subnode1.put( "type", "uint8" );

		const std::string subnodename2 = "value2";
		bpt::ptree subnode2;
		subnode2.put( "id",  subnodename2 );
		subnode2.put( "label", "Value 2" );
		subnode2.put( "type", "uint8" );

		const std::string subnodename3 = "value3";
		bpt::ptree subnode3;
		subnode3.put( "id",  subnodename3 );
		subnode3.put( "label", "Value 3" );
		subnode3.put( "type", "data" );
		subnode3.put( "count", "2*value1 + value2" );

		tree.add_child( "group.", subnode1 );
		tree.add_child( "group.", subnode2 );
		tree.add_child( "group.", subnode3 );

		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 1 << (uint8) 2 << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( tree );
		CHECK_NODE_VALID ( nodename );
		CHECK_NODE_VALID ( nodename + "." + subnodename1 );
		CHECK_NODE_VALID ( nodename + "." + subnodename2 );
		CHECK_NODE_VALID ( nodename + "." + subnodename3 );
		CHECK_NODE( nodename + "." + subnodename3, "<xmlattr>.size", "4" );
	}
}

BOOST_AUTO_TEST_SUITE_END()
