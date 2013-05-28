BOOST_AUTO_TEST_SUITE( nodeSpecification_tests_hexa )

BOOST_AUTO_TEST_CASE( nodeSpecification_hexaTestFile )
{
	Hexa hexa;
	bpt::ptree node;
	
	hexa.originalCaseValue = "ffd8";
	node.put( "id", "hexaTest" );
	node.put( "label", "Hexa Test" );
	node.put( "hexa", hexa.originalCaseValue );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << hexa;
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID ( "hexaTest" );
		CHECK_VALUE_EQUAL( "hexaTest", hexa.originalCaseValue );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		hexa.originalCaseValue = "FFD8";
		stream << hexa;
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID ( "hexaTest" );
		CHECK_VALUE_EQUAL( "hexaTest", "ffd8" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		hexa.originalCaseValue = "abcd";
		stream << hexa;
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_INVALID( "hexaTest" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		hexa.originalCaseValue = "ffd8";
		stream << hexa;
		stream.close();

		node.clear();
		node.put( "id", "hexaTest" );
		node.put( "label", "Hexa Test" );

		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		hexa.originalCaseValue = "ffd8";
		stream << hexa;
		stream.close();

		node.clear();
		node.put( "id", "hexaMultipleTest" );
		node.put( "label", "Hexa Multiple Test" );
		node.put( "hexa.", hexa.originalCaseValue );
		
		node.add( "hexa.", "ffe0" );

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID ( "hexaMultipleTest" );
		CHECK_VALUE_EQUAL( "hexaMultipleTest", hexa.originalCaseValue );
	}
}

BOOST_AUTO_TEST_SUITE_END()
