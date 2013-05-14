BOOST_AUTO_TEST_SUITE( nodeSpecification_tests_ascii )

BOOST_AUTO_TEST_CASE( nodeSpecification_asciiTestFile )
{
	const std::string wave = "WAVE";
	const std::string riff = "RIFF";
	bpt::ptree node;
	
	node.put( "id", "asciiTest" );
	node.put( "label", "Ascii Test" );
	node.put( "ascii", wave );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << wave;
		stream.close();

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID ( "asciiTest" );
		CHECK_VALUE_EQUAL( "asciiTest", wave );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << "EVAW";
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID( "asciiTest" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << wave;
		stream.close();

		node.clear();
		node.put( "id", "asciiTest" );
		node.put( "label", "Ascii Test" );

		BOOST_CHECK_THROW( generateReportTree( node ), std::runtime_error );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << wave;
		stream.close();

		node.clear();
		node.put( "id", "asciiMultipleTest" );
		node.put( "label", "Ascii Multiple Test" );
		node.put( "ascii.", wave );
		node.add( "ascii.", riff );

		bpt::ptree report = generateReportTree( node );

		CHECK_NODE_VALID ( "asciiMultipleTest" );
		CHECK_VALUE_EQUAL( "asciiMultipleTest", wave );
	}
}

BOOST_AUTO_TEST_SUITE_END()
