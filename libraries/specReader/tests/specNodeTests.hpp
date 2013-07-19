#include <boost/foreach.hpp>

BOOST_AUTO_TEST_SUITE( spec_reader_test_specNode )

BOOST_AUTO_TEST_CASE( spec_reader_specNode )
{
	LOG_INFO( "\n>>> spec_reader_specNode <<<" );
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"id\": \"node\", \"label\": \"Node\", \"type\": \"type\" },";
		jsonString += " { \"id\": \"node\", \"label\": \"Node\", \"type\": \"type\" }";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node;
			node.setNode( n );
			BOOST_CHECK_EQUAL( node.getId(), "node" );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 2 );
		
		i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getLabel(), "Node" );
			BOOST_CHECK_EQUAL( node.getType(),  "type" );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 2 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"displayType\": \"display\", \"count\": \"123\", \"required\": true }";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getDisplayType(), "display" );
			BOOST_CHECK_EQUAL( node.getCount(),  "123" );
			BOOST_CHECK_EQUAL( node.getRequired(),  "true" );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"values\": [ \"value1\", \"value2\", \"value3\" ] }";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getValues().at(0), "value1" );
			BOOST_CHECK_EQUAL( node.getValues().at(1), "value2" );
			BOOST_CHECK_EQUAL( node.getValues().at(2), "value3" );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"values\": \"value\" }";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getValues().at(0), "value" );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
}

BOOST_AUTO_TEST_SUITE_END()
