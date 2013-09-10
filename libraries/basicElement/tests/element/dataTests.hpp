BOOST_AUTO_TEST_SUITE( basic_element_test_data )

BOOST_AUTO_TEST_CASE( basic_element_data )
{
	LOG_INFO( "\n>>> basic_element_data <<<" );
	{
		dbe::Data data;

		data.setId( id );
		BOOST_CHECK_EQUAL( data.getId(), id );

		data.setLabel( label );
		BOOST_CHECK_EQUAL( data.getLabel(), label );
	}
	{
		dbe::Data data( eDataTypeHexa );
		char buffer[] = { 0x7f, 0x05, 0x32 };
		
		data.setData( buffer, 3 );
		char* buff = new char [ data.getSize() ];
		data.getData( buff );
		BOOST_CHECK_EQUAL( buff[0], 127 );
		BOOST_CHECK_EQUAL( buff[1], 5 );
		BOOST_CHECK_EQUAL( buff[2], 50 );
		delete[] buff;

		BOOST_CHECK_EQUAL( data.getHexa(), "7f0532" );
	}
	{
		dbe::Data data( eDataTypeAscii );
		char buffer[] = { 0x48, 0x65, 0x6c, 0x6c, 0x6f };
		
		data.setData( buffer, 5 );
		BOOST_CHECK_EQUAL( data.getAscii(), "Hello" );
	}
	{
		dbe::Data data( eDataTypeRaw );
		char buffer[] = {'d','a','t','a'};

		data.setData( buffer, 4 );
		char* buff = new char [ data.getSize() ];
		data.getData( buff );
		BOOST_CHECK_EQUAL( buff[0], 'd' );
		BOOST_CHECK_EQUAL( buff[1], 'a' );
		BOOST_CHECK_EQUAL( buff[2], 't' );
		BOOST_CHECK_EQUAL( buff[3], 'a' );
		delete[] buff;

		BOOST_CHECK_EQUAL( data.getAscii(), "data" );
		BOOST_CHECK_EQUAL( data.getHexa(), "64617461" );
	}
	{
		std::vector< unsigned int > ref;
		ref.push_back(   0 );
		ref.push_back(   5 );
		ref.push_back(  12 );
		ref.push_back(  32 );
		ref.push_back( 255 );

		dbe::Data data( eDataTypeRaw );
		char buffer[ ref.size() ];
		for( size_t i = 0; i < ref.size(); i++ )
			buffer[i] = ref.at(i);

		data.setData( buffer, 5 );
		std::vector< unsigned int > vector1 = data.toIntVector();

		for( size_t i = 0; i < 5; ++i )
			BOOST_CHECK_EQUAL( vector1.at(i), ref.at(i) );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_hexa )
{
	LOG_INFO( "\n>>> basic_element_data_hexa <<<" );
	{
		dbe::Data data( eDataTypeHexa );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusPassOver );
	}
	{
		dbe::Data data( eDataTypeHexa );
		data.setSpecData( "ff00" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( eDataTypeHexa );
		data.setSpecData( "ff00" );
		char buffer[] = { static_cast< char >( 0xff ), 0x00 };
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( eDataTypeHexa );
		data.setSpecData( "ff00" );
		char buffer[] = { 0x00, static_cast< char >( 0xff ) };
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( eDataTypeHexa );
		data.setSpecData( "ff00" );
		data.setBigEndianness( false );
		char buffer[] = { 0x00, static_cast< char >( 0xff ) };
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( eDataTypeHexa );
		std::vector< std::string > values;
		values.push_back( "00ff" );
		data.setSpecData( values );
		char buffer[] = { 0x00, static_cast< char >( 0xff ) };
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( eDataTypeHexa );
		std::vector< std::string > values;
		values.push_back( "f00f" );
		values.push_back( "ff00" );
		data.setSpecData( values );
		data.setBigEndianness( false );
		char buffer[] = { 0x00, static_cast< char >( 0xff ) };
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( eDataTypeHexa );
		std::vector< std::string > values;
		values.push_back( "f00f" );
		values.push_back( "0ff0" );
		data.setSpecData( values );
		char buffer[] = { 0x00, static_cast< char >( 0xff ) };
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( eDataTypeHexa );
		std::vector< std::string > values;
		values.push_back( "000000" );
		values.push_back( "ffffffff" );
		BOOST_CHECK_THROW( data.setSpecData( values ), std::runtime_error );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_ascii )
{
	LOG_INFO( "\n>>> basic_element_data_ascii <<<" );
	{
		dbe::Data data( eDataTypeAscii );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusPassOver );
	}
	{
		dbe::Data data( eDataTypeAscii );
		data.setSpecData( "data" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( eDataTypeAscii );
		data.setSpecData( "data" );
		char buffer[] = {'d','a','t','a'};
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( eDataTypeAscii );
		data.setSpecData( "atad" );
		char buffer[] = {'d','a','t','a'};
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( eDataTypeAscii );
		data.setSpecData( "atad" );
		data.setBigEndianness( false );
		char buffer[] = {'d','a','t','a'};
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( eDataTypeAscii );
		std::vector< std::string > values;
		values.push_back( "atad" );
		data.setSpecData( values );
		data.setBigEndianness( false );
		char buffer[] = {'d','a','t','a'};
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( eDataTypeAscii );
		std::vector< std::string > values;
		values.push_back( "atad" );
		values.push_back( "taad" );
		data.setSpecData( values );
		data.setBigEndianness( false );
		char buffer[] = {'d','a','t','a'};
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( eDataTypeAscii );
		std::vector< std::string > values;
		values.push_back( "atad" );
		values.push_back( "taad" );
		data.setSpecData( values );
		char buffer[] = {'d','a','t','a'};
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( eDataTypeAscii );
		std::vector< std::string > values;
		values.push_back( "long" );
		values.push_back( "toolong" );
		BOOST_CHECK_THROW( data.setSpecData( values ), std::runtime_error );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_other )
{
	LOG_INFO( "\n>>> basic_element_data_other <<<" );
	{
		dbe::Data data( eDataTypeUnknown );
		data.setSpecData( "anything" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusUnknown );
	}
	{
		dbe::Data data( eDataTypeRaw );
		data.setSpecData( "lotOfThings" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusPassOver );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_subType )
{
	LOG_INFO( "\n>>> basic_element_data_subType <<<" );
	{
		dbe::Data data( eDataTypeUnknown );
		BOOST_CHECK_EQUAL( data.getDataSubType(), eDataTypeUnknown );
	}
	{
		dbe::Data data( eDataTypeHexa );
		BOOST_CHECK_EQUAL( data.getDataSubType(), eDataTypeHexa );
	}
	{
		dbe::Data data( eDataTypeAscii );
		BOOST_CHECK_EQUAL( data.getDataSubType(), eDataTypeAscii );
	}
	{
		dbe::Data data( eDataTypeRaw );
		BOOST_CHECK_EQUAL( data.getDataSubType(), eDataTypeRaw );
	}

	{
		dbe::Data data( eDataTypeRaw );
		BOOST_CHECK_EQUAL( data.getNumberSubType(), eNumberTypeUnknown );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_error_warning )
{
	LOG_INFO( "\n>>> basic_element_data_error_warning <<<" );
	{
		dbe::Data data( eDataTypeUnknown );
		std::string error = "error";
		data.addErrorLabel( error );
		BOOST_CHECK_EQUAL( data.getErrorLabel(), error );
		BOOST_CHECK_EQUAL( data.getStatus(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( eDataTypeUnknown );
		std::string error = "error";
		data.addErrorLabel( error );
		data.addErrorLabel( error );
		BOOST_CHECK_EQUAL( data.getErrorLabel(), "errorerror" );
		BOOST_CHECK_EQUAL( data.getStatus(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( eDataTypeUnknown );
		std::string warning = "warning";
		data.addWarningLabel( warning );
		BOOST_CHECK_EQUAL( data.getWarningLabel(), warning );
	}
	{
		dbe::Data data( eDataTypeUnknown );
		std::string warning = "warning";
		data.addWarningLabel( warning );
		data.addWarningLabel( warning );
		BOOST_CHECK_EQUAL( data.getWarningLabel(), "warningwarning" );
	}
}

BOOST_AUTO_TEST_SUITE_END()
