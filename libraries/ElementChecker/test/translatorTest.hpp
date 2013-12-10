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
		elem->set( buff, 4 );

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
		elem->set( buff, 2 );

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
		elem->set( buff, 4 );

		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeDefault ), "" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeNumbers ), "87658669" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeAscii   ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeHexa    ), "57415645" );
		BOOST_CHECK_EQUAL( Translator( elem ).get( eDisplayTypeRaw     ), "WAVE" );
		
	}
	{
		std::shared_ptr< basic_element::Element > data( new basic_element::Element( node ) );
		std::vector< char > buff { 'W', 'A', 'V', 'E' };
		data->set( buff, 4 );

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
		elem->set( buff, 4 );


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
		elem->set( buff, 4 );


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
		elem->set( buff, 1 );


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
		elem->set( buff, 4 );

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
		elem->set( buff, 4 );

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
}

BOOST_AUTO_TEST_SUITE_END()
