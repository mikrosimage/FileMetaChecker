#include <boost/property_tree/json_parser.hpp>

BOOST_AUTO_TEST_SUITE( report_test_report_node )

BOOST_AUTO_TEST_CASE( report_report_node )
{
	LOG_INFO( ">>> report_report_node <<<" );
	{
		std::string jsonString;
		jsonString  = " { \"header\": [ ";
		jsonString += " { \"id\": \"id\", \"label\": \"label\", \"type\": \"type\",";
		jsonString += "   \"displayType\": \"display\", \"count\": \"123\", \"required\": true,";
		jsonString += "   \"groupSize\": \"size\", \"group\": [ \"123\" ] }";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;
		bpt::read_json( isstream, tree );

		ReportNode node( tree.get_child( "header" ).begin(), 0, 1 );
		
		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 1 );
	}
}

BOOST_AUTO_TEST_CASE( report_report_node_next )
{
	LOG_INFO( ">>> report_report_node_next <<<" );
	{
		std::string jsonString;
		jsonString  = " { \"header\": [ ";
		jsonString += " { \"id\": \"value1\" },";
		jsonString += " { \"id\": \"value2\" },";
		jsonString += " { \"id\": \"value3\" },";
		jsonString += " { \"id\": \"value4\" }";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;
		bpt::read_json( isstream, tree );

		ReportNode node( tree.get_child( "header" ).begin(), 0, 4 );
		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );
		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      1 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );
		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      2 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );
		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      3 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );
	}
}

BOOST_AUTO_TEST_SUITE_END()
