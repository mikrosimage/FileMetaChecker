#define BOOST_TEST_MODULE qc_basic_element_translator

#include <BasicElement/SubElements/Number.hpp>
#include <BasicElement/SubElements/Data.hpp>
#include <BasicElement/Translator/Translator.hpp>

#include <SpecReader/SpecNode.hpp>

#include <Common/log.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::unit_test;
using namespace basic_element;
namespace bpt = boost::property_tree;

BOOST_AUTO_TEST_SUITE( basic_element_test_translator )

BOOST_AUTO_TEST_CASE( basic_element_translator_init )
{
	common::level = common::eLogTrace;
	// LOG_INFO( common::level );
}

BOOST_AUTO_TEST_CASE( basic_element_translator_1 )
{
	std::string jsonString = R"*(
			{
				"header": [
					{ 
						"id": "id",
						"label": "label",
						"type": "type",
						"displayType": "display",
						"count": "123",
						"required": true,
						"groupSize": "size",
						"group": [
							"123"
						]
					}
				]
			}
		)*";

	std::istringstream isstream( jsonString );
	bpt::ptree tree;

	bpt::read_json( isstream, tree );

	spec_reader::SpecNode node( tree.get_child( "header" ).begin() );
	Number elem( &node );

	{
		Translator tr = Translator( &elem );
		BOOST_CHECK_THROW( tr.get< std::string >(), std::runtime_error );
	}
	{
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem.set( (const char*)&buff, 4 );

		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::string >(), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::vector< int > >().at(0), 1163280727 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(0), 87 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(1), 65 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(2), 86 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(3), 69 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get<   int >(), 1163280727 );
		BOOST_CHECK_THROW( Translator( &elem ).get< short >(), std::runtime_error );
	}
	{
		const char buff[2] { 'a', 'b' };
		elem.set( (const char*)&buff, 2 );

		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::string >(), "ab" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::vector< short > >().at(0), 25185 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(0), 97 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(1), 98 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< short >(), 25185 );
		BOOST_CHECK_THROW( Translator( &elem ).get<   int >(), std::runtime_error );
		BOOST_CHECK_THROW( Translator( &elem ).get< std::vector< int > >(), std::runtime_error );
	}
	{
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem.set( (const char*)&buff, 4 );

		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeDefault ), "" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeNumbers ), "87658669" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeAscii   ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeHexa    ), "57415645" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeRaw     ), "WAVE" );
		
	}
	{
		Data data( &node );
		const char buff[4] { 'W', 'A', 'V', 'E' };
		data.set( (const char*)&buff, 4 );

		BOOST_CHECK_EQUAL( Translator( &data ).display( eDisplayTypeDefault ), "" );
		BOOST_CHECK_EQUAL( Translator( &data ).display( eDisplayTypeNumbers ), "87658669" );
		BOOST_CHECK_EQUAL( Translator( &data ).display( eDisplayTypeAscii   ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &data ).display( eDisplayTypeHexa    ), "57415645" );
		BOOST_CHECK_EQUAL( Translator( &data ).display( eDisplayTypeRaw     ), "WAVE" );
		
	}
}

BOOST_AUTO_TEST_CASE( basic_element_translator_2 )
{
	{
		std::string jsonString = R"*(
				{
					"header": [
						{ 
							"id": "id",
							"label": "label",
							"type": "ascii"
						}
					]
				}
			)*";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );

		spec_reader::SpecNode node( tree.get_child( "header" ).begin() );
		Data elem( &node );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem.set( (const char*)&buff, 4 );


		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::string >(), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::vector< int > >().at(0), 1163280727 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(0), 87 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(1), 65 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(2), 86 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(3), 69 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get<   int >(), 1163280727 );
		BOOST_CHECK_THROW( Translator( &elem ).get< short >(), std::runtime_error );
	
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeDefault ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeNumbers ), "87658669" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeAscii   ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeHexa    ), "57415645" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeRaw     ), "WAVE" );
		
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{ 
							"id": "id",
							"label": "label",
							"type": "uint32"
						}
					]
				}
			)*";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );

		spec_reader::SpecNode node( tree.get_child( "header" ).begin() );
		Number elem( &node );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem.set( (const char*)&buff, 4 );


		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::string >(), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::vector< int > >().at(0), 1163280727 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(0), 87 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(1), 65 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(2), 86 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(3), 69 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get<   int >(), 1163280727 );
		BOOST_CHECK_THROW( Translator( &elem ).get< short >(), std::runtime_error );
	
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeDefault ), "1163280727" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeNumbers ), "87658669" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeAscii   ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeHexa    ), "57415645" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeRaw     ), "WAVE" );
		
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{ 
							"id": "id",
							"label": "label",
							"type": "int8"
						}
					]
				}
			)*";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );

		spec_reader::SpecNode node( tree.get_child( "header" ).begin() );
		Number elem( &node );
	
		const char buff[1] { 'W' };
		elem.set( (const char*)&buff, 1 );


		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::string >(), "W" );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(0), 87 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get<  char >(), 'W' );
		BOOST_CHECK_THROW( Translator( &elem ).get< std::vector< int > >(), std::runtime_error );
		BOOST_CHECK_THROW( Translator( &elem ).get<   int >(), std::runtime_error );
		BOOST_CHECK_THROW( Translator( &elem ).get< short >(), std::runtime_error );
	
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeDefault ), "87" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeNumbers ), "87" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeAscii   ), "W" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeHexa    ), "57" );
		BOOST_CHECK_EQUAL( Translator( &elem ).display( eDisplayTypeRaw     ), "W" );
	}
}

BOOST_AUTO_TEST_SUITE_END()
