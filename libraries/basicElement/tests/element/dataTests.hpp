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
		dbe::Data data( be::Element::eTypeHexa );
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
		dbe::Data data( be::Element::eTypeAscii );
		char buffer[] = { 0x48, 0x65, 0x6c, 0x6c, 0x6f };
		
		data.setData( buffer, 5 );
		BOOST_CHECK_EQUAL( data.getAscii(), "Hello" );
	}
	{
		dbe::Data data( be::Element::eTypeAscii );
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
		dbe::Data data( be::Element::eTypeData );
		char buffer[] = { 0x00, 0x05, 0x0c, 0x20, 0xff };

		data.setData( buffer, 5 );
		std::vector< unsigned int > vector1 = data.toIntVector();
		std::vector< unsigned int > vector2;
		vector2.push_back(   0 );
		vector2.push_back(   5 );
		vector2.push_back(  12 );
		vector2.push_back(  32 );
		vector2.push_back( 255 );

		for( size_t i = 0; i < 5; ++i )
			BOOST_CHECK_EQUAL( vector1.at(i), vector2.at(i) );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_hexa )
{
	LOG_INFO( "\n>>> basic_element_data_hexa <<<" );
	{
		dbe::Data data( be::Element::eTypeHexa );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusPassOver );
	}
	{
		dbe::Data data( be::Element::eTypeHexa );
		data.setSpecData( "ff00" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( be::Element::eTypeHexa );
		data.setSpecData( "ff00" );
		char buffer[] = { 0xff, 0x00 };
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( be::Element::eTypeHexa );
		data.setSpecData( "ff00" );
		char buffer[] = { 0x00, 0xff };
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( be::Element::eTypeHexa );
		data.setSpecData( "ff00" );
		data.setBigEndianness( false );
		char buffer[] = { 0x00, 0xff };
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_ascii )
{
	LOG_INFO( "\n>>> basic_element_data_ascii <<<" );
	{
		dbe::Data data( be::Element::eTypeAscii );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusPassOver );
	}
	{
		dbe::Data data( be::Element::eTypeAscii );
		data.setSpecData( "data" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( be::Element::eTypeAscii );
		data.setSpecData( "data" );
		char buffer[] = {'d','a','t','a'};
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( be::Element::eTypeAscii );
		data.setSpecData( "atad" );
		char buffer[] = {'d','a','t','a'};
		data.setData( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( be::Element::eTypeAscii );
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
		dbe::Data data( be::Element::eTypeNumber );
		data.setSpecData( "123" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusPassOver );
	}
	{
		dbe::Data data( be::Element::eTypeExif );
		data.setSpecData( "anything" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusPassOver );
	}
	{
		dbe::Data data( be::Element::eTypeData );
		data.setSpecData( "lotOfThings" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusPassOver );
	}
	{
		dbe::Data data( be::Element::eTypeKlv );
		data.setSpecData( "anything" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusPassOver );
	}
}

BOOST_AUTO_TEST_SUITE_END()
