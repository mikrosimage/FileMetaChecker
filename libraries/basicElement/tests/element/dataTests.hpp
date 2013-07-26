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
		dbe::Data data( be::Element::eDataTypeHexa );
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
		dbe::Data data( be::Element::eDataTypeAscii );
		char buffer[] = { 0x48, 0x65, 0x6c, 0x6c, 0x6f };
		
		data.setData( buffer, 5 );
		BOOST_CHECK_EQUAL( data.getAscii(), "Hello" );
	}
	{
		dbe::Data data( be::Element::eDataTypeRaw );
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

		dbe::Data data( be::Element::eDataTypeRaw );
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
		dbe::Data data( be::Element::eDataTypeHexa );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusPassOver );
	}
	{
		dbe::Data data( be::Element::eDataTypeHexa );
		data.setSpecData( "ff00" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( be::Element::eDataTypeHexa );
		data.setSpecData( "ff00" );
		char buffer[] = { static_cast< char >( 0xff ), 0x00 };
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( be::Element::eDataTypeHexa );
		data.setSpecData( "ff00" );
		char buffer[] = { 0x00, static_cast< char >( 0xff ) };
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( be::Element::eDataTypeHexa );
		data.setSpecData( "ff00" );
		data.setBigEndianness( false );
		char buffer[] = { 0x00, static_cast< char >( 0xff ) };
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_ascii )
{
	LOG_INFO( "\n>>> basic_element_data_ascii <<<" );
	{
		dbe::Data data( be::Element::eDataTypeAscii );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusPassOver );
	}
	{
		dbe::Data data( be::Element::eDataTypeAscii );
		data.setSpecData( "data" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( be::Element::eDataTypeAscii );
		data.setSpecData( "data" );
		char buffer[] = {'d','a','t','a'};
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( be::Element::eDataTypeAscii );
		data.setSpecData( "atad" );
		char buffer[] = {'d','a','t','a'};
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( be::Element::eDataTypeAscii );
		data.setSpecData( "atad" );
		data.setBigEndianness( false );
		char buffer[] = {'d','a','t','a'};
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_other )
{
	LOG_INFO( "\n>>> basic_element_data_other <<<" );
	{
		dbe::Data data( be::Element::eDataTypeUnknown );
		data.setSpecData( "anything" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusUnknown );
	}
	{
		dbe::Data data( be::Element::eDataTypeRaw );
		data.setSpecData( "lotOfThings" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusPassOver );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_subType )
{
	LOG_INFO( "\n>>> basic_element_data_subType <<<" );
	{
		dbe::Data data( be::Element::eDataTypeUnknown );
		BOOST_CHECK_EQUAL( data.getDataSubType(), be::Element::eDataTypeUnknown );
	}
	{
		dbe::Data data( be::Element::eDataTypeHexa );
		BOOST_CHECK_EQUAL( data.getDataSubType(), be::Element::eDataTypeHexa );
	}
	{
		dbe::Data data( be::Element::eDataTypeAscii );
		BOOST_CHECK_EQUAL( data.getDataSubType(), be::Element::eDataTypeAscii );
	}
	{
		dbe::Data data( be::Element::eDataTypeRaw );
		BOOST_CHECK_EQUAL( data.getDataSubType(), be::Element::eDataTypeRaw );
	}

	{
		dbe::Data data( be::Element::eDataTypeRaw );
		BOOST_CHECK_EQUAL( data.getNumberSubType(), be::Element::eNumberTypeUnknown );
	}
}

BOOST_AUTO_TEST_SUITE_END()
