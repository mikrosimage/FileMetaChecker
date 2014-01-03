#include <ElementChecker/Translator/Translator.hpp>

BOOST_AUTO_TEST_SUITE( element_checker_test_translator )

BOOST_AUTO_TEST_CASE( element_checker_translator_1 )
{
	LOG_INFO( ">>> element_checker_translator_1 <<<" );
	std::string jsonString = R"*(
			{
				"content": [
					{ 
						"id": "id",
						"label": "label",
						"type": "type",
						"display": "display",
						"size": "123",
						"requirement": "True",
						"groupSize": "size",
						"group": [
							"123"
						]
					}
				]
			}
		)*";

	spec_reader::Specification spec;
	spec.setFromString( jsonString );
	std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

	std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );

	{
		Translator tr = Translator( elem );
		BOOST_CHECK_THROW( tr.get< std::string >(), std::runtime_error );
	}
	{
		std::vector< char > buff { 'W', 'A', 'V', 'E' };
		elem->set( buff );

		BOOST_CHECK_EQUAL( Translator( elem ).get< std::string >(), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( elem ).get< std::vector< int > >().at(0), 1163280727 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(0), 87 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(1), 65 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(2), 86 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(3), 69 );
		BOOST_CHECK_EQUAL( Translator( elem ).get<   int >(), 1163280727 );
		BOOST_CHECK_THROW( Translator( elem ).get< short >(), std::runtime_error );
	}
	{
		std::vector< char > buff { 'a', 'b' };
		elem->set( buff );

		BOOST_CHECK_EQUAL( Translator( elem ).get< std::string >(), "ab" );
		BOOST_CHECK_EQUAL( Translator( elem ).get< std::vector< short > >().at(0), 25185 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(0), 97 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(1), 98 );
		BOOST_CHECK_EQUAL( Translator( elem ).get< short >(), 25185 );
		BOOST_CHECK_THROW( Translator( elem ).get<   int >(), std::runtime_error );
		BOOST_CHECK_THROW( Translator( elem ).get< std::vector< int > >(), std::runtime_error );
	}
	{
		std::vector< char > buff { 'W', 'A', 'V', 'E' };
		elem->set( buff );

		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeDefault ), "" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeNumbers ), "87658669" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeAscii   ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeHexa    ), "57415645" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeRaw     ), "WAVE" );
		
	}
	{
		std::shared_ptr< basic_element::Element > data( new basic_element::Element( node ) );
		std::vector< char > buff { 'W', 'A', 'V', 'E' };
		data->set( buff );

		BOOST_CHECK_EQUAL( Translator( data ).get( eDisplayTypeDefault ), "" );
		BOOST_CHECK_EQUAL( Translator( data ).get( eDisplayTypeNumbers ), "87658669" );
		BOOST_CHECK_EQUAL( Translator( data ).get( eDisplayTypeAscii   ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( data ).get( eDisplayTypeHexa    ), "57415645" );
		BOOST_CHECK_EQUAL( Translator( data ).get( eDisplayTypeRaw     ), "WAVE" );
		
	}
}

BOOST_AUTO_TEST_CASE( element_checker_translator_2 )
{
	LOG_INFO( ">>> element_checker_translator_2 <<<" );
	{
		LOG_INFO( "\t>>> translator_2_1 <<<" );
		std::string jsonString = R"*(
				{
					"content": [
						{ 
							"id": "id",
							"label": "label",
							"type": "ascii"
						}
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		std::vector< char > buff { 'W', 'A', 'V', 'E' };
		elem->set( buff );


		BOOST_CHECK_EQUAL( Translator( elem ).get< std::string >(), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( elem ).get< std::vector< int > >().at(0), 1163280727 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(0), 87 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(1), 65 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(2), 86 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(3), 69 );
		BOOST_CHECK_EQUAL( Translator( elem ).get<   int >(), 1163280727 );
		BOOST_CHECK_THROW( Translator( elem ).get< short >(), std::runtime_error );
	
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeDefault ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeNumbers ), "87658669" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeAscii   ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeHexa    ), "57415645" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeRaw     ), "WAVE" );
		
	}
	{
		LOG_INFO( "\t>>> translator_2_2 <<<" );
		std::string jsonString = R"*(
				{
					"content": [
						{ 
							"id": "id",
							"label": "label",
							"type": "uint32"
						}
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		std::vector< char > buff { 'W', 'A', 'V', 'E' };
		elem->set( buff );


		BOOST_CHECK_EQUAL( Translator( elem ).get< std::string >(), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( elem ).get< std::vector< int > >().at(0), 1463899717 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(0), 87 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(1), 65 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(2), 86 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(3), 69 );
		BOOST_CHECK_EQUAL( Translator( elem ).get<   int >(), 1463899717 );
		BOOST_CHECK_THROW( Translator( elem ).get< short >(), std::runtime_error );
	
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeDefault ), "1463899717" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeNumbers ), "87658669" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeAscii   ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeHexa    ), "57415645" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeRaw     ), "WAVE" );
		
	}
	{
		LOG_INFO( "\t>>> translator_2_3 <<<" );
		std::string jsonString = R"*(
				{
					"content": [
						{ 
							"id": "id",
							"label": "label",
							"type": "int8"
						}
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		std::vector< char > buff { 'W' };
		elem->set( buff );


		BOOST_CHECK_EQUAL( Translator( elem ).get< std::string >(), "W" );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(0), 87 );
		BOOST_CHECK_EQUAL( Translator( elem ).get<  char >(), 'W' );
		BOOST_CHECK_THROW( Translator( elem ).get< std::vector< int > >(), std::runtime_error );
		BOOST_CHECK_THROW( Translator( elem ).get<   int >(), std::runtime_error );
		BOOST_CHECK_THROW( Translator( elem ).get< short >(), std::runtime_error );
	
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeDefault ), "87" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeNumbers ), "87" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeAscii   ), "W" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeHexa    ), "57" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeRaw     ), "W" );
	}

	{
		LOG_INFO( "\t>>> translator_2_4 <<<" );
		std::string jsonString = R"*(
				{
					"content": [
						{ 
							"id": "id",
							"label": "label",
							"type": "uint32",
							"endian": "little"
						}
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		std::vector< char > buff { 0x01, 0x00, 0x00, 0x00 };
		elem->set( buff );

		BOOST_CHECK_EQUAL( elem->_isBigEndian, false );
		BOOST_CHECK_EQUAL( Translator( elem ).get<   int >(), 1 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().size(), 4 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(0),  1 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(1),  0 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(2),  0 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(3),  0 );

		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< short > >().size(), 2 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< short > >().at(0),  1 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< short > >().at(1),  0 );
		
		BOOST_CHECK_THROW( Translator( elem ).get< short >(), std::runtime_error );
	}
	{
		LOG_INFO( "\t>>> translator_2_5 <<<" );
		std::string jsonString = R"*(
				{
					"content": [
						{ 
							"id": "id",
							"label": "label",
							"type": "uint32"
						}
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		std::vector< char > buff { 0x62, 0x61, 0x62, 0x61 };
		elem->set( buff );

		BOOST_CHECK_EQUAL( Translator( elem ).get<   int >(), 1650549345 );
		BOOST_CHECK_EQUAL( Translator( elem ).get< std::string >(), "baba" );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().size(), 4 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(0), 98 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(1), 97 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(2), 98 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< char > >().at(3), 97 );

		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< short > >().size(),    2 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< short > >().at(0), 24930 );
		BOOST_CHECK_EQUAL( (short) Translator( elem ).get< std::vector< short > >().at(1), 24930 );
		
		BOOST_CHECK_THROW( Translator( elem ).get< short >(), std::runtime_error );
	}
	{
		LOG_INFO( "\t>>> translator_2_6 <<<" );
		std::string jsonString = R"*(
				{
					"content": [
						{ 
							"id": "id",
							"label": "label",
							"type": "half"
						}
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
	
		std::vector< char > buff { 0x3C, 0x00 };
		elem1->set( buff );

		BOOST_CHECK_EQUAL( elem1->_data.size(), 2 );

		BOOST_CHECK_EQUAL( Translator( elem1 ).get< unsigned short >(), 15360 );
		BOOST_CHECK_CLOSE( Translator( elem1 ).get< float >(), 1, 0.001 );
		BOOST_CHECK_EQUAL( Translator( elem1 ).get< std::vector< char > >().size(), 2 );
		BOOST_CHECK_EQUAL( Translator( elem1 ).get< std::vector< char > >().at(0), 0x3C );
		BOOST_CHECK_EQUAL( Translator( elem1 ).get< std::vector< char > >().at(1), 0x00 );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( node ) );
	
		buff.clear();
		buff.push_back( 0xC0 );
		buff.push_back( 0x00 );
		elem2->set( buff );

		BOOST_CHECK_EQUAL( elem2->_data.size(), 2 );

		BOOST_CHECK_EQUAL( Translator( elem2 ).get< unsigned short >(), 49152 );
		BOOST_CHECK_CLOSE( Translator( elem2 ).get< float >(), -2, 0.001 );
		BOOST_CHECK_EQUAL( Translator( elem2 ).get< std::vector< char > >().size(), 2 );
		BOOST_CHECK_EQUAL( Translator( elem2 ).get< std::vector< unsigned char > >().at(0), 0xC0 );
		BOOST_CHECK_EQUAL( Translator( elem2 ).get< std::vector< char > >().at(1), 0x00 );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( node ) );
	
		buff.clear();
		buff.push_back( 0x35 );
		buff.push_back( 0x55 );
		elem3->set( buff );

		BOOST_CHECK_EQUAL( elem3->_data.size(), 2 );

		BOOST_CHECK_EQUAL( Translator( elem3 ).get< unsigned short >(), 13653 );
		BOOST_CHECK_CLOSE( Translator( elem3 ).get< float >(), 0.333251953, 0.001 );
		BOOST_CHECK_EQUAL( Translator( elem3 ).get< std::vector< char > >().size(), 2 );
		BOOST_CHECK_EQUAL( Translator( elem3 ).get< std::vector< char > >().at(0), 0x35 );
		BOOST_CHECK_EQUAL( Translator( elem3 ).get< std::vector< char > >().at(1), 0x55 );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( node ) );
	
		buff.clear();
		buff.push_back( 0x7C );
		buff.push_back( 0x00 );
		elem4->set( buff );

		BOOST_CHECK_EQUAL( elem4->_data.size(), 2 );

		BOOST_CHECK_EQUAL( Translator( elem4 ).get< unsigned short >(), 31744 );
		BOOST_CHECK_EQUAL( Translator( elem4 ).get< float >(), std::numeric_limits< float >::infinity() );
		BOOST_CHECK_EQUAL( Translator( elem4 ).get< std::vector< char > >().size(), 2 );
		BOOST_CHECK_EQUAL( Translator( elem4 ).get< std::vector< char > >().at(0), 0x7C );
		BOOST_CHECK_EQUAL( Translator( elem4 ).get< std::vector< char > >().at(1), 0x00 );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( node ) );
	
		buff.clear();
		buff.push_back( 0xFF );
		buff.push_back( 0xFF );
		elem5->set( buff );

		BOOST_CHECK_EQUAL( elem5->_data.size(), 2 );

		BOOST_CHECK_EQUAL( Translator( elem5 ).get< unsigned short >(), 65535 );
		BOOST_CHECK( std::isnan( Translator( elem5 ).get< float >() ) );
		BOOST_CHECK_EQUAL( Translator( elem5 ).get< std::vector< char > >().size(), 2 );
		BOOST_CHECK_EQUAL( Translator( elem5 ).get< std::vector< unsigned char > >().at(0), 0xFF );
		BOOST_CHECK_EQUAL( Translator( elem5 ).get< std::vector< unsigned char > >().at(1), 0xFF );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( node ) );
	
		buff.clear();
		buff.push_back( 0x00 );
		buff.push_back( 0x01 );
		elem6->set( buff );

		BOOST_CHECK_EQUAL( elem6->_data.size(), 2 );

		BOOST_CHECK_EQUAL( Translator( elem6 ).get< unsigned short >(), 1 );
		BOOST_CHECK_CLOSE( Translator( elem6 ).get< float >(), 5.96046e-08, 0.001 );
		BOOST_CHECK_EQUAL( Translator( elem6 ).get< std::vector< char > >().size(), 2 );
		BOOST_CHECK_EQUAL( Translator( elem6 ).get< std::vector< unsigned char > >().at(0), 0x00 );
		BOOST_CHECK_EQUAL( Translator( elem6 ).get< std::vector< unsigned char > >().at(1), 0x01 );
	}
}

BOOST_AUTO_TEST_SUITE_END()
