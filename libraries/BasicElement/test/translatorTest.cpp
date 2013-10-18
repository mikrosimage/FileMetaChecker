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
namespace bpt = boost::property_tree;

BOOST_AUTO_TEST_SUITE( basic_element_test_translator )

BOOST_AUTO_TEST_CASE( basic_element_translator_init )
{
	common::level = common::eLogTrace;
	// LOG_INFO( common::level );
}

BOOST_AUTO_TEST_CASE( basic_element_translator )
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
	basic_element::Number elem( &node );

	{
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem.set( (const char*)&buff, 4 );

		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::string >(), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::vector< int > >().at(0), 87 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::vector< int > >().at(1), 65 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::vector< int > >().at(2), 86 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::vector< int > >().at(3), 69 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< int >(), 1163280727 );
		BOOST_CHECK_THROW( Translator( &elem ).get< short >(), std::runtime_error );
	}
	{
		const char buff[2] { 'a', 'b' };
		elem.set( (const char*)&buff, 2 );

		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::string >(), "ab" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::vector< int > >().at(0), 97 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::vector< int > >().at(1), 98 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< short >(), 25185 );
		BOOST_CHECK_THROW( Translator( &elem ).get< int >(), std::runtime_error );
	}
}

BOOST_AUTO_TEST_SUITE_END()
