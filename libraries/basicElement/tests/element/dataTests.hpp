BOOST_AUTO_TEST_SUITE( basic_element_test_data )

BOOST_AUTO_TEST_CASE( basic_element_data )
{
	LOG_INFO( "\n>>> basic_element_data <<<" );
	{
		dbe::Data data( id );

		BOOST_CHECK_EQUAL( data.getId(), id );

		data.setLabel( label );
		BOOST_CHECK_EQUAL( data.getLabel(), label );
	}
	{
		dbe::Data data( id, eSubTypeHexa );
		char buffer[] = { 0x7f, 0x05, 0x32 };
		
		data.set( buffer, 3 );
		char* buff = data.get< char*, eDisplayTypeRaw >( );
		BOOST_CHECK_EQUAL( buff[0], 127 );
		BOOST_CHECK_EQUAL( buff[1], 5 );
		BOOST_CHECK_EQUAL( buff[2], 50 );

		std::string res = data.get< std::string, eDisplayTypeHexa >();
		BOOST_CHECK_EQUAL( res, "7f0532" );
	}
	{
		dbe::Data data( id, eSubTypeAscii );
		char buffer[] = { 0x48, 0x65, 0x6c, 0x6c, 0x6f };
		
		data.set( buffer, 5 );
		std::string res = data.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "Hello" );
	}
	{
		dbe::Data data( id, eSubTypeRaw );
		char buffer[] = {'d','a','t','a'};

		data.set( buffer, 4 );
		char* buff = data.get< char*, eDisplayTypeRaw >( );
		BOOST_CHECK_EQUAL( buff[0], 'd' );
		BOOST_CHECK_EQUAL( buff[1], 'a' );
		BOOST_CHECK_EQUAL( buff[2], 't' );
		BOOST_CHECK_EQUAL( buff[3], 'a' );

		std::string res = data.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "data" );
		res = data.get< std::string, eDisplayTypeHexa >();
		BOOST_CHECK_EQUAL( res, "64617461" );
	}
	{
		std::vector< nbe::uint8 > ref;
		ref.push_back(   0 );
		ref.push_back(   5 );
		ref.push_back(  12 );
		ref.push_back(  32 );
		ref.push_back( 255 );

		dbe::Data data( id, eSubTypeRaw );
		char buffer[ ref.size() ];
		for( size_t i = 0; i < ref.size(); i++ )
			buffer[i] = ref.at(i);

		data.set( buffer, 5 );
		std::vector< nbe::uint8 > vector1 = data.get< std::vector< nbe::uint8 >, eDisplayTypeNumbers >();

		for( size_t i = 0; i < 5; ++i )
			BOOST_CHECK_EQUAL( vector1.at(i), ref.at(i) );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_hexa )
{
	LOG_INFO( "\n>>> basic_element_data_hexa <<<" );
	{
		dbe::Data data( id, eSubTypeHexa );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusPassOver );
	}
	{
		dbe::Data data( id, eSubTypeHexa );
		data.setSpecData( "ff00" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( id, eSubTypeHexa );
		data.setSpecData( "ff00" );
		char buffer[] = { static_cast< char >( 0xff ), 0x00 };
		data.set( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( id, eSubTypeHexa );
		data.setSpecData( "ff00" );
		char buffer[] = { 0x00, static_cast< char >( 0xff ) };
		data.set( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( id, eSubTypeHexa );
		data.setSpecData( "ff00" );
		data.setBigEndianness( false );
		char buffer[] = { 0x00, static_cast< char >( 0xff ) };
		data.set( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( id, eSubTypeHexa );
		std::vector< std::string > values;
		values.push_back( "00ff" );
		data.setSpecData( values );
		char buffer[] = { 0x00, static_cast< char >( 0xff ) };
		data.set( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( id, eSubTypeHexa );
		std::vector< std::string > values;
		values.push_back( "f00f" );
		values.push_back( "ff00" );
		data.setSpecData( values );
		data.setBigEndianness( false );
		char buffer[] = { 0x00, static_cast< char >( 0xff ) };
		data.set( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( id, eSubTypeHexa );
		std::vector< std::string > values;
		values.push_back( "f00f" );
		values.push_back( "0ff0" );
		data.setSpecData( values );
		char buffer[] = { 0x00, static_cast< char >( 0xff ) };
		data.set( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( id, eSubTypeHexa );
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
		dbe::Data data( id, eSubTypeAscii );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusPassOver );
	}
	{
		dbe::Data data( id, eSubTypeAscii );
		data.setSpecData( "data" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( id, eSubTypeAscii );
		data.setSpecData( "data" );
		char buffer[] = {'d','a','t','a'};
		data.set( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( id, eSubTypeAscii );
		data.setSpecData( "atad" );
		char buffer[] = {'d','a','t','a'};
		data.set( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( id, eSubTypeAscii );
		data.setSpecData( "atad" );
		data.setBigEndianness( false );
		char buffer[] = {'d','a','t','a'};
		data.set( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( id, eSubTypeAscii );
		std::vector< std::string > values;
		values.push_back( "atad" );
		data.setSpecData( values );
		data.setBigEndianness( false );
		char buffer[] = {'d','a','t','a'};
		data.set( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( id, eSubTypeAscii );
		std::vector< std::string > values;
		values.push_back( "atad" );
		values.push_back( "taad" );
		data.setSpecData( values );
		data.setBigEndianness( false );
		char buffer[] = {'d','a','t','a'};
		data.set( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusValid );
	}
	{
		dbe::Data data( id, eSubTypeAscii );
		std::vector< std::string > values;
		values.push_back( "atad" );
		values.push_back( "taad" );
		data.setSpecData( values );
		char buffer[] = {'d','a','t','a'};
		data.set( buffer, sizeof( buffer ) );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( id, eSubTypeAscii );
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
		dbe::Data data( id, eSubTypeUnknown );
		data.setSpecData( "anything" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusUnknown );
	}
	{
		dbe::Data data( id, eSubTypeRaw );
		data.setSpecData( "lotOfThings" );
		BOOST_CHECK_EQUAL( data.checkData(), be::Element::eStatusPassOver );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_subType )
{
	LOG_INFO( "\n>>> basic_element_data_subType <<<" );
	{
		dbe::Data data( id, eSubTypeUnknown );
		BOOST_CHECK_EQUAL( data.getSubType(), eSubTypeUnknown );
	}
	{
		dbe::Data data( id, eSubTypeHexa );
		BOOST_CHECK_EQUAL( data.getSubType(), eSubTypeHexa );
	}
	{
		dbe::Data data( id, eSubTypeAscii );
		BOOST_CHECK_EQUAL( data.getSubType(), eSubTypeAscii );
	}
	{
		dbe::Data data( id, eSubTypeRaw );
		BOOST_CHECK_EQUAL( data.getSubType(), eSubTypeRaw );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_error_warning )
{
	LOG_INFO( "\n>>> basic_element_data_error_warning <<<" );
	{
		dbe::Data data( id, eSubTypeUnknown );
		data.addErrorLabel( kError );
		BOOST_CHECK_EQUAL( data.getErrorLabel(), kError );
		BOOST_CHECK_EQUAL( data.getStatus(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( id, eSubTypeUnknown );
		data.addErrorLabel( kError );
		data.addErrorLabel( kError );
		BOOST_CHECK_EQUAL( data.getErrorLabel(), kError + kError );
		BOOST_CHECK_EQUAL( data.getStatus(), be::Element::eStatusInvalid );
	}
	{
		dbe::Data data( id, eSubTypeUnknown );
		data.addWarningLabel( kWarning );
		BOOST_CHECK_EQUAL( data.getWarningLabel(), kWarning );
	}
	{
		dbe::Data data( id, eSubTypeUnknown );
		data.addWarningLabel( kWarning );
		data.addWarningLabel( kWarning );
		BOOST_CHECK_EQUAL( data.getWarningLabel(), kWarning + kWarning );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_set_from_node )
{
LOG_INFO( "\n>>> basic_element_data_set_from_node <<<" );
	{
		std::string jsonString = R"*(
			{ "header": [
				{
					"id": "id",
					"label": "label",
					"type": "ascii",
					"displayType": "raw"			
				}
			  ]
			}
		)*";

		std::istringstream isstream( jsonString );
		boost::property_tree::ptree tree;
		boost::property_tree::json_parser::read_json( isstream, tree );

		spec_reader::SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getId(),          "id"    );
		BOOST_CHECK_EQUAL( node.getLabel(),       "label" );
		BOOST_CHECK_EQUAL( node.getType(),        "ascii" );
		BOOST_CHECK_EQUAL( node.getDisplayType(), "raw"   );
		BOOST_CHECK_EQUAL( node.getCount(),       ""      );
		BOOST_CHECK_EQUAL( node.getGroupSize(),   ""      );
		BOOST_CHECK_EQUAL( node.getRequirement(), ""      );

		BOOST_CHECK_EQUAL( node.getRepetitions().size(), 0 );
		BOOST_CHECK_EQUAL( node.isBigEndian(), true  );
		BOOST_CHECK_EQUAL( node.isOptional(),  false );
		BOOST_CHECK_EQUAL( node.isOrdered(),   true  );
		BOOST_CHECK_EQUAL( node.hasGroup(),    false );
	
		dbe::Data data( node );
		BOOST_CHECK_EQUAL( data.getId(),    "id"    );
		BOOST_CHECK_EQUAL( data.getLabel(), "label" );
		BOOST_CHECK_EQUAL( data.getType(),  eTypeData );
	}
	{
		std::string jsonString = R"*(
			{ "header": [
				{
					"id": "id",
					"label": "label",
					"type": "ascii",
					"displayType": "raw",
					"count": "23",
					"groupSize": "number",
					"required": "hello",
					"endian": "little",
					"optional": "true",
					"ordered": "false",
					"repeated": [
						{ "min": "0", "max": "Max" }
					],
					"group": [
					]					
				}
			  ]
			}
		)*";

		std::istringstream isstream( jsonString );
		boost::property_tree::ptree tree;
		boost::property_tree::json_parser::read_json( isstream, tree );

		spec_reader::SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getId(),          "id"    );
		BOOST_CHECK_EQUAL( node.getLabel(),       "label" );
		BOOST_CHECK_EQUAL( node.getType(),        "ascii" );
		BOOST_CHECK_EQUAL( node.getDisplayType(), "raw"   );
		BOOST_CHECK_EQUAL( node.getCount(),       "23"    );
		BOOST_CHECK_EQUAL( node.getGroupSize(),   "number" );
		BOOST_CHECK_EQUAL( node.getRequirement(), "hello"  );
		BOOST_CHECK_EQUAL( node.getRepetitions().size(), 1  );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).first,  "0"  );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).second, "Max"  );
		BOOST_CHECK_EQUAL( node.isBigEndian(), false );
		BOOST_CHECK_EQUAL( node.isOptional(),  true  );
		BOOST_CHECK_EQUAL( node.isOrdered(),   false );
		BOOST_CHECK_EQUAL( node.hasGroup(),    true  );

		dbe::Data data( node );
		BOOST_CHECK_EQUAL( data.getId(),    "id"    );
		BOOST_CHECK_EQUAL( data.getLabel(), "label" );
		BOOST_CHECK_EQUAL( data.getType(),  eTypeData );
		BOOST_CHECK_EQUAL( data.getCount(),       "23"    );
		BOOST_CHECK_EQUAL( data.getGroupSize(),   "number" );
		BOOST_CHECK_EQUAL( data.getRequirement(), "hello"  );
		BOOST_CHECK_EQUAL( data.getRepetitions().size(), 1  );
		BOOST_CHECK_EQUAL( data.getRepetitions().at(0).first,  "0"  );
		BOOST_CHECK_EQUAL( data.getRepetitions().at(0).second, "Max"  );
		BOOST_CHECK_EQUAL( data.isBigEndian(), false );
		BOOST_CHECK_EQUAL( data.isOptional(),  true  );
		BOOST_CHECK_EQUAL( data.isOrdered(),   false );
		BOOST_CHECK_EQUAL( data.hasGroup(),    true  );
	}
}

BOOST_AUTO_TEST_SUITE_END()
