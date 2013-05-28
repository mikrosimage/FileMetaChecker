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

BOOST_AUTO_TEST_CASE( nodeSpecification_orderedGroupTestFile )
{
	{
		const std::string nodename = "validOrderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Valid Ordered Group Test" );
		node.put( "ordered", true );

		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub1" << "sub2";
		stream.close();

		const std::string nodename1 = "validSubGroupNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Valid SubGroupNode 1" );
		subnode1.put( "ascii", "sub1" );

		const std::string nodename2 = "validSubGroupNode2";
		bpt::ptree subnode2;
		subnode2.put( "id",  nodename2 );
		subnode2.put( "label", "Valid SubGroupNode 2" );
		subnode2.put( "ascii", "sub2" );

		node.add_child( "group.", subnode1 );
		node.add_child( "group.", subnode2 );
		
		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename2, "sub2" );
		CHECK_NODE_VALID ( nodename );
	}
	{
		const std::string nodename = "validOrderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Valid Ordered Group with Value Test" );
		node.put( "ascii", "group" );
		node.put( "ordered", true );

		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "group" << "sub1" << "sub2";
		stream.close();

		const std::string nodename1 = "validSubGroupNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Valid SubGroupNode 1" );
		subnode1.put( "ascii", "sub1" );

		const std::string nodename2 = "validSubGroupNode2";
		bpt::ptree subnode2;
		subnode2.put( "id",  nodename2 );
		subnode2.put( "label", "Valid SubGroupNode 2" );
		subnode2.put( "ascii", "sub2" );

		node.add_child( "group.", subnode1 );
		node.add_child( "group.", subnode2 );
		
		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename2, "sub2" );
		CHECK_VALUE_EQUAL( nodename, "group" );
		CHECK_NODE_VALID ( nodename );
	}
	{
		const std::string nodename = "invalidOrderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Invalid Ordered Group Test" );
		node.put( "ordered", true );

		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub2" << "sub1";
		stream.close();

		const std::string nodename1 = "invalidSubGroupNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Invalid SubGroupNode 1" );
		subnode1.put( "ascii", "sub1" );

		const std::string nodename2 = "invalidSubGroupNode2";
		bpt::ptree subnode2;
		subnode2.put( "id",  nodename2 );
		subnode2.put( "label", "Invalid SubGroupNode 2" );
		subnode2.put( "ascii", "sub2" );

		node.add_child( "group.", subnode1 );
		node.add_child( "group.", subnode2 );
		
		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_repetitionOrderedGroupTestFile )
{
	{
		const std::string nodename = "validRepetitionOrderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Repetition Ordered Group Test" );
		node.put( "ordered", true );

		const std::string nodename1 = "repeatedSubGroupNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Repeated SubGroupNode 1" );
		subnode1.put( "ascii", "sub1" );
		subnode1.put( "repeated", "2" );
		node.add_child( "group.", subnode1 );

		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub1" << "sub1";
		stream.close();
		
		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID ( nodename );
	}
	{
		const std::string nodename = "invalidRepetitionOrderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Repetition Ordered Group Test" );
		node.put( "ordered", true );

		const std::string nodename1 = "repeatedSubGroupNode2";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Repeated SubGroupNode 2" );
		subnode1.put( "ascii", "sub1" );
		subnode1.put( "repeated", "2" );
		node.add_child( "group.", subnode1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub1" << "sub2";
		stream.close();
		
		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		const std::string nodename = "error1_RepetitionOrderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Error1 Repetition Ordered Group Test" );
		node.put( "ordered", true );

		const std::string nodename1 = "error1_repeatedSubGroupNode3";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Error1 Repeated SubGroupNode 3" );
		subnode1.put( "ascii", "sub1" );

		bpt::ptree range;
		range.put( "min",  "2" );

		subnode1.add_child( "repeated.", range );
		node.add_child( "group.", subnode1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub1" << "sub1";
		stream.close();
		
		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}
	{
		const std::string nodename = "error2_RepetitionOrderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Error2 Repetition Ordered Group Test" );
		node.put( "ordered", true );

		const std::string nodename1 = "error2_repeatedSubGroupNode4";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Error2 Repeated SubGroupNode 4" );
		subnode1.put( "ascii", "sub1" );
		subnode1.put( "repeated", "0" );

		node.add_child( "group.", subnode1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub1" << "sub1";
		stream.close();
		
		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}
	{
		const std::string nodename = "error3_RepetitionOrderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Error3 Repetition Ordered Group Test" );
		node.put( "ordered", true );

		const std::string nodename1 = "error3_repeatedSubGroupNode5";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Error3 Repeated SubGroupNode 5" );
		subnode1.put( "ascii", "sub1" );
		subnode1.put( "repeated", -2 );

		node.add_child( "group.", subnode1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub1" << "sub1";
		stream.close();
		
		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}
	{
		const std::string nodename = "error3_RepetitionOrderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Error3 Repetition Ordered Group Test" );
		node.put( "ordered", true );

		const std::string nodename1 = "error3_repeatedSubGroupNode5";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Error3 Repeated SubGroupNode 5" );
		subnode1.put( "ascii", "sub1" );
		subnode1.put( "repeated", 1.5 );

		node.add_child( "group.", subnode1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub1" << "sub1";
		stream.close();
		
		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_repetition0rderedGroupTestFile2 )
{
	Hexa hexa1, hexa2, hexa3;
	
	hexa1.originalCaseValue = "ffd8";
	hexa2.originalCaseValue = "abcd";
	hexa3.originalCaseValue = "1234";

	const std::string nodename = "hexaTest";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Hexa Test" );
	node.put( "hexa", "ffd8" );
	// node.put( "ordered", false );

	const std::string nodename1 = "hexaNode1";
	bpt::ptree node1;
	node1.put( "id", nodename1 );
	node1.put( "label", "Hexa Node 1" );
	node1.put( "hexa", "abcd" );
	node1.put( "repeated", "2" );

	const std::string nodename2 = "hexaNode2";
	bpt::ptree node2;
	node2.put( "id", nodename2 );
	node2.put( "label", "Hexa Node 2" );
	node2.put( "hexa", "1234" );
	node2.put( "repeated", "4" );

	node.add_child( "group.", node1 );
	node.add_child( "group.", node2 );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << hexa1 << hexa2 << hexa2 << hexa3 << hexa3 << hexa3 << hexa3;
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1, "abcd" );
		CHECK_NODE_VALID ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename2, "1234" );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "ffd8" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << hexa1 << hexa2 << hexa2 << hexa3 << hexa3 << hexa3;
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID  ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL ( nodename + "." + nodename1, "abcd" );
		CHECK_NODE_VALID  ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL ( nodename + "." + nodename2, "1234" );
		CHECK_VALUE_EQUAL ( nodename, "ffd8" );
		CHECK_NODE_INVALID( nodename );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_unorderedGroupTestFile )
{
	const std::string nodename = "validUnorderedGroupTest";
	bpt::ptree node;
	node.put( "id",  nodename );
	node.put( "label", "Valid Unordered Group Test" );
	node.put( "ordered", false );

	const std::string nodename1 = "validSubGroupNode1";
	bpt::ptree subnode1;
	subnode1.put( "id",  nodename1 );
	subnode1.put( "label", "Valid SubGroupNode 1" );
	subnode1.put( "ascii", "sub1" );

	const std::string nodename2 = "validSubGroupNode2";
	bpt::ptree subnode2;
	subnode2.put( "id",  nodename2 );
	subnode2.put( "label", "Valid SubGroupNode 2" );
	subnode2.put( "ascii", "sub2" );

	const std::string nodename3 = "validSubGroupNode3";
	bpt::ptree subnode3;
	subnode3.put( "id",  nodename3 );
	subnode3.put( "label", "Valid SubGroupNode 3" );
	subnode3.put( "ascii", "sub3" );

	node.add_child( "group.", subnode1 );
	node.add_child( "group.", subnode2 );
	node.add_child( "group.", subnode3 );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub2" << "sub1" << "sub3" << "hello";
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename2, "sub2" );
		CHECK_NODE_VALID ( nodename + "." + nodename3 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename3, "sub3" );
		CHECK_NODE_VALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub2" << "sub1" << "sub3";
		stream.close();
		
		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename2, "sub2" );
		CHECK_NODE_VALID ( nodename + "." + nodename3 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename3, "sub3" );
		CHECK_NODE_VALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub1" << "sub3";
		stream.close();
		
		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID ( nodename + "." + nodename3 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename3, "sub3" );

		CHECK_NODE_INVALID ( nodename );		 // NEEDS SUB2 TO BE OPTIONAL !
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_repetitionUnorderedGroupTestFile )
{
	const std::string nodename = "repetitionUnorderedGroupTest";
	bpt::ptree node;
	node.put( "id",  nodename );
	node.put( "label", "Repetition Unordered Group Test" );
	node.put( "ordered", false );

	const std::string nodename1 = "repeatedSubGroupNode1";
	bpt::ptree subnode1;
	subnode1.put( "id",  nodename1 );
	subnode1.put( "label", "Repeated SubGroupNode 1" );
	subnode1.put( "ascii", "sub1" );
	subnode1.put( "repeated", "2" );

	bpt::ptree max;
	max.put( "max", "3" );
	
	const std::string nodename2 = "repeatedSubGroupNode2";
	bpt::ptree subnode2;
	subnode2.put( "id",  nodename2 );
	subnode2.put( "label", "Repeated SubGroupNode 2" );
	subnode2.put( "ascii", "sub2" );
	subnode2.add_child( "repeated.", max );

	node.add_child( "group.", subnode1 );
	node.add_child( "group.", subnode2 );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub1" << "sub2" << "sub2" << "sub1" << "sub2";
		stream.close();
		
		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename2, "sub2" );
		CHECK_NODE_VALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub1" << "sub2" << "sub2" << "sub1" << "sub2" << "sub2";
		stream.close();
		
		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID   ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID   ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename2, "sub2" );

		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub1" << "sub2" << "sub3" << "sub1" << "sub2" << "sub2";
		stream.close();
		
		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID   ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID   ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename2, "sub2" );

		CHECK_NODE_INVALID ( nodename );
	}

	bpt::ptree min;
	min.put( "min", "2" );

	const std::string nodename3 = "repeatedSubGroupNode3";
	bpt::ptree subnode3;
	subnode3.put( "id",  nodename3 );
	subnode3.put( "label", "Repeated SubGroupNode 3" );
	subnode3.put( "ascii", "sub3" );
	subnode3.add_child( "repeated.", min );

	node.add_child( "group.", subnode3 );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub1" << "sub2" << "sub3" << "sub1" << "sub2" << "sub3" << "sub2";
		stream.close();
		
		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename2, "sub2" );
		CHECK_NODE_VALID ( nodename + "." + nodename3 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename3, "sub3" );
		CHECK_NODE_VALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub1" << "sub2" << "sub1" << "sub2" << "sub3" << "sub2";
		stream.close();
		
		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID   ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID   ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename2, "sub2" );
		CHECK_NODE_VALID   ( nodename + "." + nodename3 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename3, "sub3" );
		
		CHECK_NODE_INVALID ( nodename );
	}

	bpt::ptree range;
	range.put( "min", "2" );
	range.put( "max", "4" );

	const std::string nodename4 = "repeatedSubGroupNode4";
	bpt::ptree subnode4;
	subnode4.put( "id",  nodename4 );
	subnode4.put( "label", "Repeated SubGroupNode 4" );
	subnode4.put( "ascii", "sub4" );
	subnode4.add_child( "repeated.", range );

	node.add_child( "group.", subnode4 );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub4" << "sub2" << "sub3" << "sub1" << "sub4" << "sub3" << "sub2";
		stream << "sub1" << "sub2" << "sub3" << "sub4";
		stream.close();
		
		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename2, "sub2" );
		CHECK_NODE_VALID ( nodename + "." + nodename3 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename3, "sub3" );
		CHECK_NODE_VALID ( nodename + "." + nodename4 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename4, "sub4" );
		CHECK_NODE_VALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub4" << "sub2" << "sub3" << "sub1" << "sub4" << "sub3" << "sub4";
		stream << "sub1" << "sub2" << "sub3" << "sub4" << "sub4" << "sub2";
		stream.close();
		
		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID   ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID   ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename2, "sub2" );
		CHECK_NODE_VALID   ( nodename + "." + nodename3 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename3, "sub3" );
		CHECK_NODE_VALID   ( nodename + "." + nodename4 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename4, "sub4" );

		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub2" << "sub3" << "sub1" << "sub4" << "sub3";
		stream << "sub1" << "sub2" << "sub3" << "sub2";
		stream.close();
		
		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID   ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID   ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename2, "sub2" );
		CHECK_NODE_VALID   ( nodename + "." + nodename3 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename3, "sub3" );
		CHECK_NODE_VALID   ( nodename + "." + nodename4 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename4, "sub4" );

		CHECK_NODE_INVALID ( nodename );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_repetitionUnorderedGroupTestFile2 )
{
	Hexa hexa1, hexa2, hexa3;
	
	hexa1.originalCaseValue = "ffd8";
	hexa2.originalCaseValue = "abcd";
	hexa3.originalCaseValue = "1234";

	const std::string nodename = "hexaTest";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Hexa Test" );
	node.put( "hexa", "ffd8" );
	node.put( "ordered", false );

	const std::string nodename1 = "hexaNode1";
	bpt::ptree node1;
	node1.put( "id", nodename1 );
	node1.put( "label", "Hexa Node 1" );
	node1.put( "hexa", "abcd" );
	node1.put( "repeated", "2" );

	const std::string nodename2 = "hexaNode2";
	bpt::ptree node2;
	node2.put( "id", nodename2 );
	node2.put( "label", "Hexa Node 2" );
	node2.put( "hexa", "1234" );
	node2.put( "repeated", "4" );

	node.add_child( "group.", node1 );
	node.add_child( "group.", node2 );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << hexa1 << hexa2 << hexa3 << hexa3 << hexa3 << hexa2 << hexa3;
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1, "abcd" );
		CHECK_NODE_VALID ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename2, "1234" );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "ffd8" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << hexa1 << hexa2 << hexa3 << hexa3 << hexa2 << hexa3;
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID  ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL ( nodename + "." + nodename1, "abcd" );
		CHECK_NODE_VALID  ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL ( nodename + "." + nodename2, "1234" );
		CHECK_VALUE_EQUAL ( nodename, "ffd8" );
		CHECK_NODE_INVALID( nodename );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_errorsRepetitionUnorderedGroupTestFile )
{
	{
		const std::string nodename = "error1_repetitionUnorderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Error1 Repetition Unordered Group Test" );
		node.put( "ordered", false );

		const std::string nodename1 = "error1_repeatedSubGroupNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Error1 Unordered SubGroupNode 1" );
		subnode1.put( "ascii", "sub1" );
		subnode1.put( "repeated", "-2" );

		node.add_child( "group.", subnode1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub2" << "sub1" << "sub1" << "sub2";
		stream.close();
		
		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}
	{
		const std::string nodename = "error2_repetitionUnorderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Error2 Repetition Unordered Group Test" );
		node.put( "ordered", false );

		const std::string nodename1 = "error2_repeatedSubGroupNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Error2 Unordered SubGroupNode 1" );
		subnode1.put( "ascii", "sub1" );
		subnode1.put( "repeated", "0" );

		node.add_child( "group.", subnode1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub2" << "sub1" << "sub1" << "sub2";
		stream.close();
		
		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}
	{
		const std::string nodename = "error3_repetitionUnorderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Error3 Repetition Unordered Group Test" );
		node.put( "ordered", false );

		const std::string nodename1 = "error3_repeatedSubGroupNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Error3 Unordered SubGroupNode 1" );
		subnode1.put( "ascii", "sub1" );
		subnode1.put( "repeated", "1.1" );

		node.add_child( "group.", subnode1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub2" << "sub1" << "sub1" << "sub2";
		stream.close();
		
		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}

	{
		const std::string nodename = "error4_repetitionUnorderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Error4 Repetition Unordered Group Test" );
		node.put( "ordered", false );

		const std::string nodename1 = "error4_repeatedSubGroupNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Error4 Unordered SubGroupNode 1" );
		subnode1.put( "ascii", "sub1" );

		bpt::ptree min;
		min.put( "min", "0" );

		subnode1.add_child( "repeated.", min );
		node.add_child( "group.", subnode1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub2" << "sub1" << "sub1" << "sub2";
		stream.close();
		
		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}
	{
		const std::string nodename = "error5_repetitionUnorderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Error5 Repetition Unordered Group Test" );
		node.put( "ordered", false );

		const std::string nodename1 = "error5_repeatedSubGroupNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Error5 Unordered SubGroupNode 1" );
		subnode1.put( "ascii", "sub1" );

		bpt::ptree max;
		max.put( "max", "0" );

		subnode1.add_child( "repeated.", max );
		node.add_child( "group.", subnode1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub2" << "sub1" << "sub1" << "sub2";
		stream.close();
		
		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}

	{
		const std::string nodename = "error6_repetitionUnorderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Error6 Repetition Unordered Group Test" );
		node.put( "ordered", false );

		const std::string nodename1 = "error6_repeatedSubGroupNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Error6 Unordered SubGroupNode 1" );
		subnode1.put( "ascii", "sub1" );

		bpt::ptree min;
		min.put( "min", "-1" );

		subnode1.add_child( "repeated.", min );
		node.add_child( "group.", subnode1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub2" << "sub1" << "sub1" << "sub2";
		stream.close();
		
		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}
	{
		const std::string nodename = "error7_repetitionUnorderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Error7 Repetition Unordered Group Test" );
		node.put( "ordered", false );

		const std::string nodename1 = "error7_repeatedSubGroupNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Error7 Unordered SubGroupNode 1" );
		subnode1.put( "ascii", "sub1" );

		bpt::ptree min;
		min.put( "max", "-1" );

		subnode1.add_child( "repeated.", min );
		node.add_child( "group.", subnode1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub2" << "sub1" << "sub1" << "sub2";
		stream.close();
		
		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}

	{
		const std::string nodename = "error8_repetitionUnorderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Error8 Repetition Unordered Group Test" );
		node.put( "ordered", false );

		const std::string nodename1 = "error8_repeatedSubGroupNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Error8 Unordered SubGroupNode 1" );
		subnode1.put( "ascii", "sub1" );

		bpt::ptree min;
		min.put( "min", "1.05" );

		subnode1.add_child( "repeated.", min );
		node.add_child( "group.", subnode1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub2" << "sub1" << "sub1" << "sub2";
		stream.close();
		
		BOOST_CHECK_THROW( generateReportTree( node ), bpt::ptree_bad_data );
	}
	{
		const std::string nodename = "error9_repetitionUnorderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Error9 Repetition Unordered Group Test" );
		node.put( "ordered", false );

		const std::string nodename1 = "error9_repeatedSubGroupNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Error9 Unordered SubGroupNode 1" );
		subnode1.put( "ascii", "sub1" );

		bpt::ptree min;
		min.put( "max", "1.05" );

		subnode1.add_child( "repeated.", min );
		node.add_child( "group.", subnode1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub2" << "sub1" << "sub1" << "sub2";
		stream.close();
		
		BOOST_CHECK_THROW( generateReportTree( node ), bpt::ptree_bad_data );
	}

	{
		const std::string nodename = "error10_repetitionUnorderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Error10 Repetition Unordered Group Test" );
		node.put( "ordered", false );

		const std::string nodename1 = "error10_repeatedSubGroupNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  nodename1 );
		subnode1.put( "label", "Error10 Unordered SubGroupNode 1" );
		subnode1.put( "ascii", "sub1" );

		bpt::ptree max;
		max.put( "min", "10" );
		max.put( "max", "5" );

		subnode1.add_child( "repeated.", max );
		node.add_child( "group.", subnode1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub2" << "sub1" << "sub1" << "sub2";
		stream.close();
		
		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_recursiveRepetitionOrderedGroupTestFile )
{
	{
		const std::string nodename = "recursiveRepetitionOrderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Recursive Repetition Ordered Group Test" );
		node.put( "ordered", true );

		const std::string nodename1 = "recursiveSubGroupNode1";
		bpt::ptree node1;
		node1.put( "id",  nodename1 );
		node1.put( "label", "Ordered SubGroupNode 1" );
		node1.put( "ascii", "sub1" );
		node1.put( "repeated", "2" );

		const std::string subnodename1 = "subNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  subnodename1 );
		subnode1.put( "label", "subNode 1" );
		subnode1.put( "ascii", "subNode1" );

		const std::string subnodename2 = "subNode2";
		bpt::ptree subnode2;
		subnode2.put( "id",  subnodename2 );
		subnode2.put( "label", "subNode 2" );
		subnode2.put( "ascii", "subNode2" );
		subnode2.put( "repeated", "2" );

		node1.add_child( "group.", subnode1 );
		node1.add_child( "group.", subnode2 );

		node.add_child( "group.", node1 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub1" << "subNode1" << "subNode2" << "subNode2";
		stream << "sub1" << "subNode1" << "subNode2" << "subNode2";
		stream.close();
		
		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID   ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID   ( nodename + "." + nodename1 + "." + subnodename1 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename1 + "." + subnodename1, "subNode1" );
		CHECK_NODE_VALID   ( nodename + "." + nodename1 + "." + subnodename2 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename1 + "." + subnodename2, "subNode2" );
		CHECK_NODE_VALID   ( nodename );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_recursiveRepetitionUnorderedGroupTestFile1 )
{
	{
		const std::string nodename = "recursiveRepetitionUnorderedGroupTest";
		bpt::ptree node;
		node.put( "id",  nodename );
		node.put( "label", "Recursive Repetition Unordered Group Test" );
		node.put( "ordered", false );

		const std::string nodename1 = "recursiveSubGroupNode1";
		bpt::ptree node1;
		node1.put( "id",  nodename1 );
		node1.put( "label", "Unordered SubGroupNode 1" );
		node1.put( "ascii", "sub1" );
		node1.put( "repeated", "2" );

		const std::string subnodename1 = "subNode1";
		bpt::ptree subnode1;
		subnode1.put( "id",  subnodename1 );
		subnode1.put( "label", "subNode 1" );
		subnode1.put( "ascii", "subNode1" );

		const std::string subnodename2 = "subNode2";
		bpt::ptree subnode2;
		subnode2.put( "id",  subnodename2 );
		subnode2.put( "label", "subNode 2" );
		subnode2.put( "ascii", "subNode2" );
		subnode2.put( "repeated", "2" );

		node1.add_child( "group.", subnode1 );
		node1.add_child( "group.", subnode2 );

		const std::string nodename2 = "recursiveSubGroupNode2";
		bpt::ptree node2;
		node2.put( "id",  nodename2 );
		node2.put( "label", "Unordered SubGroupNode 2" );
		node2.put( "ascii", "sub2" );
		node2.put( "repeated", "2" );
		node2.put( "ordered", false );

		const std::string subnodename3 = "subNode3";
		bpt::ptree subnode3;
		subnode3.put( "id",  subnodename3 );
		subnode3.put( "label", "subNode 3" );
		subnode3.put( "ascii", "subNode3" );

		const std::string subnodename4 = "subNode4";
		bpt::ptree subnode4;
		subnode4.put( "id",  subnodename4 );
		subnode4.put( "label", "subNode 4" );
		subnode4.put( "ascii", "subNode4" );
		subnode4.put( "repeated", "2" );

		node2.add_child( "group.", subnode3 );
		node2.add_child( "group.", subnode4 );

		node.add_child( "group.", node1 );
		node.add_child( "group.", node2 );
		
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "sub1" << "subNode1" << "subNode2" << "subNode2";	// 28
		stream << "sub2" << "subNode4" << "subNode3" << "subNode4";	// 56
		stream << "sub2" << "subNode4" << "subNode4" << "subNode3";	// 84
		stream << "sub1" << "subNode1" << "subNode2" << "subNode2";	// 104
		stream.close();
		
		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID   ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename1, "sub1" );
		CHECK_NODE_VALID   ( nodename + "." + nodename1 + "." + subnodename1 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename1 + "." + subnodename1, "subNode1" );
		CHECK_NODE_VALID   ( nodename + "." + nodename1 + "." + subnodename2 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename1 + "." + subnodename2, "subNode2" );
		CHECK_NODE_VALID   ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename2, "sub2" );
		CHECK_NODE_VALID   ( nodename + "." + nodename2 + "." + subnodename3 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename2 + "." + subnodename3, "subNode3" );
		CHECK_NODE_VALID   ( nodename + "." + nodename2 + "." + subnodename4 );
		CHECK_VALUE_EQUAL  ( nodename + "." + nodename2 + "." + subnodename4, "subNode4" );
		CHECK_NODE_VALID   ( nodename );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_recursiveRepetitionUnorderedGroupTestFile2 )
{
	Hexa hexa1, hexa2, hexa3, hexa4, hexa5;
	
	hexa1.originalCaseValue = "ffd8";
	hexa2.originalCaseValue = "abcd";
	hexa3.originalCaseValue = "1234";
	hexa4.originalCaseValue = "ef01";
	hexa5.originalCaseValue = "1010";

	const std::string nodename = "hexaTest";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Hexa Test" );
	node.put( "hexa", "ffd8" );
	node.put( "ordered", false );

	const std::string nodename1 = "hexaNode1";
	bpt::ptree node1;
	node1.put( "id", nodename1 );
	node1.put( "label", "Hexa Node 1" );
	node1.put( "hexa", "abcd" );
	node1.put( "ordered", false );

	const std::string subname1 = "hexaSub1";
	bpt::ptree subnode1;
	subnode1.put( "id", subname1 );
	subnode1.put( "hexa", "ef01" );
	subnode1.put( "repeated", "3" );

	const std::string subname2 = "hexaSub2";
	bpt::ptree subnode2;
	subnode2.put( "id", subname2 );
	subnode2.put( "hexa", "1234" );
	subnode2.put( "repeated", "4" );

	const std::string nodename2 = "hexaNode2";
	bpt::ptree node2;
	node2.put( "id", nodename2 );
	node2.put( "label", "Hexa Node 2" );
	node2.put( "hexa", "1010" );
	node2.put( "repeated", "2" );

	node1.add_child( "group.", subnode1 );
	node1.add_child( "group.", subnode2 );
	node.add_child ( "group.", node1 );
	node.add_child ( "group.", node2 );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << hexa1 << hexa5 << hexa2 << hexa3 << hexa4 << hexa3;
		stream << hexa3 << hexa4 << hexa3 << hexa4 << hexa5;
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1, "abcd" );
		CHECK_NODE_VALID ( nodename + "." + nodename1 + "." + subname1);
		CHECK_VALUE_EQUAL( nodename + "." + nodename1 + "." + subname1, "ef01" );
		CHECK_NODE_VALID ( nodename + "." + nodename1 + "." + subname2);
		CHECK_VALUE_EQUAL( nodename + "." + nodename1 + "." + subname2, "1234" );
		CHECK_NODE_VALID ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename2, "1010" );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "ffd8" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << hexa1 << hexa5 << hexa2 << hexa3 << hexa4 << hexa3;
		stream << hexa3 << hexa4 << hexa4 << hexa5;
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << hexa1 << hexa5 << hexa2 << hexa3 << hexa4 << hexa3;
		stream << hexa3 << hexa4 << hexa5 << hexa4 << hexa3;
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_INVALID ( nodename );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_recursiveRepetitionUnorderedGroupTestFile3 )
{
	Hexa hexa1, hexa2;
	
	hexa1.originalCaseValue = "1234";
	hexa2.originalCaseValue = "abcd";
	

	const std::string nodename = "hexaTest";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Hexa Test" );
	node.put( "ascii", "node1" );
	node.put( "ordered", false );

	const std::string nodename1 = "hexaNode1";
	bpt::ptree node1;
	node1.put( "id", nodename1 );
	node1.put( "label", "Hexa Node 1" );
	node1.put( "hexa", "1234" );
	node1.put( "ordered", false );

	const std::string subname1 = "hexaSub1";
	bpt::ptree subnode1;
	subnode1.put( "id", subname1 );
	subnode1.put( "ascii", "sub1" );

	const std::string subname2 = "hexaSub2";
	bpt::ptree subnode2;
	subnode2.put( "id", subname2 );
	subnode2.put( "ascii", "sub2" );
	subnode2.put( "repeated", "3" );

	const std::string nodename2 = "hexaNode2";
	bpt::ptree node2;
	node2.put( "id", nodename2 );
	node2.put( "label", "Hexa Node 2" );
	node2.put( "hexa", "abcd" );
	node2.put( "ordered", false );
	node2.put( "repeated", "2" );

	const std::string subname3 = "hexaSub3";
	bpt::ptree subnode3;
	subnode3.put( "id", subname3 );
	subnode3.put( "ascii", "sub3" );
	subnode3.put( "repeated", "3" );

	const std::string subname4 = "hexaSub4";
	bpt::ptree subnode4;
	subnode4.put( "id", subname4 );
	subnode4.put( "ascii", "sub4" );

	node1.add_child( "group.", subnode1 );
	node1.add_child( "group.", subnode2 );
	node2.add_child( "group.", subnode3 );
	node2.add_child( "group.", subnode4 );
	node.add_child ( "group.", node1 );
	node.add_child ( "group.", node2 );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "node1";
		stream << hexa2 << "sub3" << "sub3" << "sub4" << "sub3";
		stream << hexa1 << "sub2" << "sub1" << "sub2" << "sub2";
		stream << hexa2 << "sub3" << "sub3" << "sub4" << "sub3";
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID ( nodename + "." + nodename1 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1, "1234" );
		CHECK_NODE_VALID ( nodename + "." + nodename1 + "." + subname1 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1 + "." + subname1, "sub1" );
		CHECK_NODE_VALID ( nodename + "." + nodename1 + "." + subname2 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename1 + "." + subname2, "sub2" );
		CHECK_NODE_VALID ( nodename + "." + nodename2 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename2, "abcd" );
		CHECK_NODE_VALID ( nodename + "." + nodename2 + "." + subname3 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename2 + "." + subname3, "sub3" );
		CHECK_NODE_VALID ( nodename + "." + nodename2 + "." + subname4 );
		CHECK_VALUE_EQUAL( nodename + "." + nodename2 + "." + subname4, "sub4" );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "node1" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_error_GroupTestFile )
{
	Hexa hexa1, hexa2;
	
	hexa1.originalCaseValue = "1234";
	hexa2.originalCaseValue = "abcd";
	

	const std::string nodename = "hexaTest";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Hexa Test" );
	node.put( "ascii", "node1" );
	node.put( "ordered", false );

	const std::string subname1 = "hexaSub1";
	bpt::ptree subnode1;
	subnode1.put( "id", subname1 );
	subnode1.put( "ascii", "sub1" );

	const std::string subname2 = "hexaSub2";
	bpt::ptree subnode2;
	subnode2.put( "id", subname2 );
	subnode2.put( "ascii", "sub2" );
	subnode2.put( "repeated", "3" );

	node.add_child ( "group.", subnode1 );
	node.add_child ( "group.", subnode2 );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "node1" << "sub2" << "sub1" << " " << "sub2" << "sub2";
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID ( nodename + "." + subname1 );
		CHECK_VALUE_EQUAL( nodename + "." + subname1, "sub1" );
		CHECK_NODE_VALID ( nodename + "." + subname2 );
		CHECK_VALUE_EQUAL( nodename + "." + subname2, "sub2" );

		CHECK_NODE_INVALID ( nodename );
	}
}


BOOST_AUTO_TEST_SUITE_END()
