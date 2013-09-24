
BOOST_AUTO_TEST_SUITE( basic_element_test_translator )

BOOST_AUTO_TEST_CASE( basic_element_memory_copy )
{
	LOG_INFO( "\n>>> basic_element_memory_copy <<<" );
	{
		LOG_INFO( " === memory copy ( 1 byte ) ===" );
		char data[] { 0x10 };
		size_t size { sizeof( data ) };
		char buffer1[ size ], buffer2[ size ];
		std::memcpy( buffer1, data, size );
		std::reverse_copy( data, data + size, buffer2 );

		std::stringstream sstr, sstr2, sstr3;
		for( size_t i = 0; i < size; ++i )
		{
			sstr  << (int)(unsigned char) data[i]   << " ";
			sstr2 << (int)(unsigned char) buffer1[i] << " ";
			sstr3 << (int)(unsigned char) buffer2[i] << " ";
		}
		LOG_INFO( " //>    size: "  << size );
		LOG_INFO( " //>    data: "  << sstr.str() );
		LOG_INFO( " //> buffer1: " << sstr2.str() );
		LOG_INFO( " //> buffer2: " << sstr3.str() );
	}
	{
		LOG_INFO( " === memory copy ( 4 bytes ) ===" );
		char data[] { 0x01, 0x02, 0x03, 0x04 };
		size_t size { sizeof( data ) };
		char buffer1[ size ], buffer2[ size ];
		std::memcpy( buffer1, data, size );
		std::reverse_copy( data, data + size, buffer2 );

		std::stringstream sstr, sstr2, sstr3;
		for( size_t i = 0; i < size; ++i )
		{
			sstr  << (int)(unsigned char) data[i]   << " ";
			sstr2 << (int)(unsigned char) buffer1[i] << " ";
			sstr3 << (int)(unsigned char) buffer2[i] << " ";
		}
		LOG_INFO( " //>    size: "  << size );
		LOG_INFO( " //>    data: "  << sstr.str() );
		LOG_INFO( " //> buffer1: " << sstr2.str() );
		LOG_INFO( " //> buffer2: " << sstr3.str() );
	}
	{
		LOG_INFO( " === memory copy ( 2 bytes, value ) ===" );
		char data[] { 0x00, 0x01 };
		size_t size = sizeof( data );
		union NumberData
		{
			int value;
			unsigned char data[0];
		} buffer1, buffer2;
		std::memcpy( buffer1.data, data, size );
		std::reverse_copy( data, data + size, buffer2.data );
		int val1, val2;

		std::stringstream sstr, sstr2, sstr3;
		std::stringstream sstr4, sstr5;
		for( size_t i = 0; i < size; ++i )
		{
			sstr  << (int)(unsigned char) data[i]   << " ";
			sstr2 << std::hex << std::setfill('0') << std::setw(2) << (int)(unsigned char) buffer1.data[i] << " ";
			sstr3 << std::hex << std::setfill('0') << std::setw(2) << (int)(unsigned char) buffer2.data[i] << " ";
			sstr4 << std::hex << std::setfill('0') << std::setw(2) << (int)(unsigned char) buffer1.data[i];
			sstr5 << std::hex << std::setfill('0') << std::setw(2) << (int)(unsigned char) buffer2.data[i];
		}
		sstr4 >> val1;
		sstr5 >> val2;

		LOG_INFO( " //>    size: "  << size );
		LOG_INFO( " //>    data: "  << sstr.str() );
		LOG_INFO( " //> buffer1: " << sstr2.str() );
		LOG_INFO( " //> buffer2: " << sstr3.str() );
		LOG_INFO( " //>   sstr4: " << sstr4.str() );
		LOG_INFO( " //>   sstr5: " << sstr5.str() );
		LOG_INFO( " //>    val1: " << val1 );
		LOG_INFO( " //>    val2: " << val2 );
		BOOST_CHECK_EQUAL( static_cast< int >( buffer1.value ), 256 );	// should be 1
		BOOST_CHECK_EQUAL( static_cast< int >( buffer2.value ), 1 );	// should be 256
		BOOST_CHECK_EQUAL( static_cast< char >( val1 ), 1 );
		BOOST_CHECK_EQUAL( val2, 256 );
		// ==> This System writes in little endian order !
	}
}

BOOST_AUTO_TEST_CASE( basic_element_translate_to_number )
{
	LOG_INFO( "\n>>> basic_element_translate_to_number <<<" );
	{
		LOG_INFO( " === to number: uint8 ===" );
		dbe::Data data( id );
		nbe::Number< nbe::uint8 > num( id );
		char buffer[] { 0x00 };
		trt::ElementTranslator< dbe::Data, nbe::Number< nbe::uint8 > > tr;

		data.setData( buffer, 1 );
		num = tr.translate( data );
		BOOST_CHECK_EQUAL( num.getValue(), 0 );
	}
	{
		LOG_INFO( " === to number: uint8 ===" );
		dbe::Data data( id );
		nbe::Number< nbe::uint8 > num( id );
		char buffer[] { static_cast< char >( 0x80 ) };
		trt::ElementTranslator< dbe::Data, nbe::Number< nbe::uint8 > > tr;

		data.setData( buffer, 1 );
		num = tr.translate( data );
		BOOST_CHECK_EQUAL( num.getValue(), 128 );
	}
	{
		LOG_INFO( " === to number: uint16 ===" );
		dbe::Data data( id );
		nbe::Number< nbe::uint16 > num( id );
		char buffer[] { 0x00, static_cast< char >( 0xff ) };
		trt::ElementTranslator< dbe::Data, nbe::Number< nbe::uint16 > > tr;

		data.setData( buffer, 2 );
		num = tr.translate( data );
		BOOST_CHECK_EQUAL( num.getValue(), 255 );
	}
	{
		LOG_INFO( " === to number: uint32 ===" );
		dbe::Data data( id );
		nbe::Number< nbe::uint32 > num( id );
		char buffer[] { 0x00, 0x00, 0x00, static_cast< char >( 0xff ) };
		trt::ElementTranslator< dbe::Data, nbe::Number< nbe::uint32 > > tr;

		data.setData( buffer, 4 );
		num = tr.translate( data );
		BOOST_CHECK_EQUAL( num.getValue(), 255 );
	}
	{
		LOG_INFO( " === to number: float ===" );
		dbe::Data data( id );
		nbe::Number< float > num( id );
		char buffer[] { static_cast< char >( 0xff ),
			            static_cast< char >( 0xff ),
			            static_cast< char >( 0xff ),
			            static_cast< char >( 0xff ) };
		trt::ElementTranslator< dbe::Data, nbe::Number< float > > tr;

		data.setData( buffer, 4 );
		num = tr.translate( data);
		BOOST_CHECK_EQUAL( isnan( num.getValue() ), true );
	}
	{
		LOG_INFO( " === to number: ieeeExtended ===" );
		dbe::Data data( id );
		nbe::Number< nbe::ieeeExtended > num( id );
		char buffer[] { 0x3f, 
			            static_cast< char >( 0xff ), 
			            static_cast< char >( 0x80 ), 
			            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		trt::ElementTranslator< dbe::Data, nbe::Number< nbe::ieeeExtended > > tr;

		data.setData( buffer, 10 );
		num = tr.translate( data );
		BOOST_CHECK_EQUAL( num.getValue(), 1 );
	}
	{
		LOG_INFO( " === to number: ieeeExtended ===" );
		dbe::Data data( id );
		nbe::Number< nbe::ieeeExtended > num( id );
		char buffer[] { 0x3f, 
			            static_cast< char >( 0xff ), 
			            static_cast< char >( 0x80 ), 
			            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		trt::ElementTranslator< dbe::Data, nbe::Number< nbe::ieeeExtended > > tr;

		data.setData( buffer, 10 );
		num = tr.translate( data );
		BOOST_CHECK_EQUAL( num.getValue(), 1 );
	}
	{
		LOG_INFO( " === to number: ieeeExtended ===" );
		dbe::Data data( id );
		nbe::Number< nbe::ieeeExtended > num( id );
		char buffer[] { static_cast< char >( 0xff ),
			            static_cast< char >( 0xff ),
			            static_cast< char >( 0xff ),
			            static_cast< char >( 0xff ),
			            static_cast< char >( 0xff ),
			            static_cast< char >( 0xff ),
			            static_cast< char >( 0xff ),
			            static_cast< char >( 0xff ),
			            static_cast< char >( 0xff ),
			            static_cast< char >( 0xff ) };
		trt::ElementTranslator< dbe::Data, nbe::Number< nbe::ieeeExtended > > tr;

		data.setData( buffer, 10 );
		num = tr.translate( data );
		BOOST_CHECK_EQUAL( isnan( num.getValue() ), true );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_translate_to_data )
{
	LOG_INFO( "\n>>> basic_element_translate_to_data <<<" );
	{
		LOG_INFO( " === to data ( uint8 ) ===" );
		dbe::Data data( id );
		nbe::Number< nbe::uint8 > num( id );
		char buffer[] { 0x00 };
		trt::ElementTranslator< nbe::Number< nbe::uint8 >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "00" );
	}
	{
		LOG_INFO( " === to data ( int16 ) ===" );
		dbe::Data data( id );
		nbe::Number< nbe::int16 > num( id );
		char buffer[] { 0x00, static_cast< char >( 0xff ) };
		trt::ElementTranslator< nbe::Number< nbe::int16 >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "00ff" );
	}
	{
		LOG_INFO( " === to data ( uint16 ) ===" );
		dbe::Data data( id );
		nbe::Number< nbe::uint16 > num( id );
		char buffer[] { 0x00, static_cast< char >( 0xff ) };
		trt::ElementTranslator< nbe::Number< nbe::uint16 >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "00ff" );
	}
	{
		LOG_INFO( " === to data ( int16 ) ===" );
		dbe::Data data( id );
		nbe::Number< nbe::int16 > num( id );
		num.setBigEndianness( false );
		char buffer[] { 0x00, static_cast< char >( 0xff ) };
		trt::ElementTranslator< nbe::Number< nbe::int16 >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "ff00" );
	}
	{
		LOG_INFO( " === to data ( uint16 ) ===" );
		dbe::Data data( id );
		nbe::Number< nbe::uint16 > num( id );
		num.setBigEndianness( false );
		char buffer[] { 0x00, static_cast< char >( 0xff ) };
		trt::ElementTranslator< nbe::Number< nbe::uint16 >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "ff00" );
	}

	{
		LOG_INFO( " === to data ( uint32 ) ===" );
		dbe::Data data( id );
		nbe::Number< nbe::uint32 > num( id );
		char buffer[] { 0x01, 0x02, 0x03, 0x04 };
		trt::ElementTranslator< nbe::Number< nbe::uint32 >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "01020304" );
	}
	{
		LOG_INFO( " === to data ( uint32 ) ===" );
		dbe::Data data( id );
		nbe::Number< nbe::uint32 > num( id );
		num.setBigEndianness( false );
		char buffer[] { 0x01, 0x02, 0x03, 0x04 };
		trt::ElementTranslator< nbe::Number< nbe::uint32 >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "04030201" );
	}

	{
		LOG_INFO( " === to data ( uint64 ) ===" );
		dbe::Data data( id );
		nbe::Number< nbe::uint64 > num( id );
		char buffer[] { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
		trt::ElementTranslator< nbe::Number< nbe::uint64 >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "0102030405060708" );
	}
	{
		LOG_INFO( " === to data ( uint64 ) ===" );
		dbe::Data data( id );
		nbe::Number< nbe::uint64 > num( id );
		num.setBigEndianness( false );
		char buffer[] { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
		trt::ElementTranslator< nbe::Number< nbe::uint64 >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "0807060504030201" );
	}

	{
		LOG_INFO( " === to data ( float ) ===" );
		dbe::Data data( id );
		nbe::Number< float > num( id );
		char buffer[] { 0x01, 0x02, 0x03, 0x04 };
		trt::ElementTranslator< nbe::Number< float >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "01020304" );
	}
	{
		LOG_INFO( " === to data ( float ) ===" );
		dbe::Data data( id );
		nbe::Number< float > num( id );
		num.setBigEndianness( false );
		char buffer[] { 0x01, 0x02, 0x03, 0x04 };
		trt::ElementTranslator< nbe::Number< float >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "04030201" );
	}

	{
		LOG_INFO( " === to data ( double ) ===" );
		dbe::Data data( id );
		nbe::Number< double > num( id );
		char buffer[] { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
		trt::ElementTranslator< nbe::Number< double >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "0102030405060708" );
	}
	{
		LOG_INFO( " === to data ( double ) ===" );
		dbe::Data data( id );
		nbe::Number< double > num( id );
		num.setBigEndianness( false );
		char buffer[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
		trt::ElementTranslator< nbe::Number< double >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "0807060504030201" );
	}

	{
		LOG_INFO( " === to data ( ieeeExtended ) ===" );
		dbe::Data data( id );
		nbe::Number< nbe::ieeeExtended > num( id );
		char buffer[] { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10 };
		trt::ElementTranslator< nbe::Number< nbe::ieeeExtended >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "01020304050607080910" );
	}
	{
		LOG_INFO( " === to data ( ieeeExtended ) ===" );
		dbe::Data data( id );
		nbe::Number< nbe::ieeeExtended > num( id );
		num.setBigEndianness( false );
		char buffer[] { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10 };
		trt::ElementTranslator< nbe::Number< nbe::ieeeExtended >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "10090807060504030201" );
	}


	{
		LOG_INFO( " === to data ( uint32 ) ===" );
		dbe::Data data( id );
		nbe::Number< nbe::uint32 > num( id );
		char buffer[] { 0x46, 0x6f, 0x75, 0x72 };
		trt::ElementTranslator< nbe::Number< nbe::uint32 >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "466f7572" );
		BOOST_CHECK_EQUAL( data.getAscii(), "Four" );
	}
	{
		LOG_INFO( " === to data ( uint32 ) ===" );
		dbe::Data data( id );
		nbe::Number< nbe::uint32 > num( id );
		num.setBigEndianness( false );
		char buffer[] { 0x46, 0x6f, 0x75, 0x72 };
		trt::ElementTranslator< nbe::Number< nbe::uint32 >, dbe::Data > tr;

		num.setData( buffer, sizeof( buffer ) );
		data = tr.translate( num );
		BOOST_CHECK_EQUAL( data.getHexa(), "72756f46" );
		BOOST_CHECK_EQUAL( data.getAscii(), "ruoF" );
	}
}

BOOST_AUTO_TEST_SUITE_END()
