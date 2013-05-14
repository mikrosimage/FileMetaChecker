BOOST_AUTO_TEST_SUITE( nodeSpecification_tests_numbers )

BOOST_AUTO_TEST_CASE( nodeSpecification_int8TestFile )
{
	const std::string nodename = "int8Test";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Int8 Test" );
	node.put( "type", "int8" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (int8) 0;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (int8) 127;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "127" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (int8) -1;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-1" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (int8) -128;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-128" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "-40" );
	range.put( "max", "40" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (int8) -41;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (int8) 41;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (int8) 4;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "4" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_uint8TestFile )
{
	const std::string nodename = "uint8Test";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Uint8 Test" );
	node.put( "type", "uint8" );

	{
		/// check min
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		/// check max
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 255;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "255" );
	}
	
	/// check range
	bpt::ptree range;
	range.put( "min", "20" );
	range.put( "max", "100" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 101;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 50;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "50" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_int16TestFile )
{
	const std::string nodename = "int16Test";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Int16 Test" );
	node.put( "type", "int16" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x7f << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "32767" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-1" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x80 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-32768" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "-100" );
	range.put( "max", "100" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0x9b;		// = -101
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x65;		// = 101
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xef;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-17" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_uint16TestFile )
{
	const std::string nodename = "uint16Test";
	
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Uint16 Test" );
	node.put( "type", "uint16" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "65535" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "10" );
	range.put( "max", "1000" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x09;		// = 9
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x03 << (uint8) 0xe9;		// = 1001
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x55;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "85" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_int32TestFile )
{
	const std::string nodename = "int32Test";
	
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Int32 Test" );
	node.put( "type", "int32" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x7f << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "2147483647" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-1" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x80 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-2147483648" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "-1000" );
	range.put( "max", "1000" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff << (uint8) 0xfc << (uint8) 0x17;		// = -1001
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x03 << (uint8) 0xe9;		// = 1001
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xf4;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-12" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_uint32TestFile )
{
	const std::string nodename = "uint32Test";
	
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Uint32 Test" );
	node.put( "type", "uint32" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "4294967295" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "100" );
	range.put( "max", "10000" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x63;		// = 99
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x27 << (uint8) 0x11;		// = 10001
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x01 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "256" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_int64TestFile )
{
	const std::string nodename = "int64Test";
	
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Int64 Test" );
	node.put( "type", "int64" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x7f << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff <<
				  (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "9223372036854775807" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff <<
				  (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-1" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x80 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-9223372036854775808" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "-10000" );
	range.put( "max", "10000" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff <<
				  (uint8) 0xff << (uint8) 0xff << (uint8) 0xd8 << (uint8) 0xef;		// = -10001
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x27 << (uint8) 0x11;		// = 10001
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x11;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "17" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_uint64TestFile )
{
	const std::string nodename = "uint64Test";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Uint64 Test" );
	node.put( "type", "uint64" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "0" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff <<
				  (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "18446744073709551615" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "1000" );
	range.put( "max", "100000" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x03 << (uint8) 0xe7;		// = 999
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x01 << (uint8) 0x86 << (uint8) 0xa1;		// = 100001
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x27 << (uint8) 0x10;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "10000" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_floatTestFile )
{
	const std::string nodename = "floatTest";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Float Test" );
	node.put( "type", "float" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x3f << (uint8) 0x80 << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "1" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x7f << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "1.17549e-38" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x7f << (uint8) 0x7f << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "3.40282e+38" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0x7f << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-3.40282e+38" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "-10.5" );
	range.put( "max", "9.3" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xc1 << (uint8) 0x28 << (uint8) 0xcc << (uint8) 0xcd;		// = -10.55
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x41 << (uint8) 0x15 << (uint8) 0x99 << (uint8) 0x9a;		// = 9.35
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x40 << (uint8) 0xa7 << (uint8) 0x8d << (uint8) 0x50; 
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "5.236" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_doubleTestFile )
{
	const std::string nodename = "doubleTest";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Double Test" );
	node.put( "type", "double" );

	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x3f << (uint8) 0xf0 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "1" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x00 << (uint8) 0x10 << (uint8) 0x00 << (uint8) 0x00 <<
				  (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00 << (uint8) 0x00;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "2.22507e-308" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x7f << (uint8) 0xef << (uint8) 0xff << (uint8) 0xff <<
				  (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "1.79769e+308" );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xff << (uint8) 0xef << (uint8) 0xff << (uint8) 0xff <<
				  (uint8) 0xff << (uint8) 0xff << (uint8) 0xff << (uint8) 0xff;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-1.79769e+308" );
	}

	/// check range
	bpt::ptree range;
	range.put( "min", "-99.999" );
	range.put( "max", "99.999" );
	node.add_child( "range.", range );
	
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xc0 << (uint8) 0x58 << (uint8) 0xff << (uint8) 0xfe <<
				  (uint8) 0x5c << (uint8) 0x91 << (uint8) 0xd1 << (uint8) 0x4e;		// = -99.9999
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0x40 << (uint8) 0x58 << (uint8) 0xff << (uint8) 0xfe <<
				  (uint8) 0x5c << (uint8) 0x91 << (uint8) 0xd1 << (uint8) 0x4e;		// = 99.9999
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_INVALID ( nodename );
	}
	{
		std::ofstream stream( testFile.c_str(), std::ofstream::out );
		stream << (uint8) 0xc0 << (uint8) 0x1a << (uint8) 0xaa << (uint8) 0xb3 <<
				  (uint8) 0x67 << (uint8) 0xa0 << (uint8) 0xf9 << (uint8) 0x09;
		stream.close();

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "-6.6667" );
	}
}

BOOST_AUTO_TEST_CASE( nodeSpecification_endiannessTestFile )
{
	const std::string nodename = "endianTest";
	bpt::ptree node;
	node.put( "id", nodename );
	node.put( "label", "Endian Test" );
	node.put( "type", "uint16" );

	std::ofstream stream( testFile.c_str(), std::ofstream::out );
	stream << (uint8) 0x00 << (uint8) 0x01;
	stream.close();

	{
		node.put( "endian", "big" );

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "1" );
	}
	{
		node.put( "endian", "little" );

		bpt::ptree report = generateReportTree( node );
		CHECK_NODE_VALID ( nodename );
		CHECK_VALUE_EQUAL( nodename, "256" );
	}
}

BOOST_AUTO_TEST_SUITE_END()
