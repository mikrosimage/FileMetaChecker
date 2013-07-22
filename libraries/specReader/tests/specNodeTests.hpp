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
		jsonString += " { \"groupSize\": \"size\", \"group\": [ \"123\" ] }";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.hasGroup(),  true );
			BOOST_CHECK_EQUAL( node.getGroupSize(), "size" );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_get_values )
{
	LOG_INFO( "\n>>> spec_reader_specNode_get_values <<<" );
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
			BOOST_CHECK_EQUAL( node.getValues().size(), 1 );
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
			BOOST_CHECK_EQUAL( node.getValues().size(), 3 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
	
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_get_range )
{
	LOG_INFO( "\n>>> spec_reader_specNode_get_range <<<" );
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"range\": [ ";
		jsonString += " { \"min\": \"value1\", \"max\": \"value2\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getRange().at(0).first,  "value1" );
			BOOST_CHECK_EQUAL( node.getRange().at(0).second, "value2" );
			BOOST_CHECK_EQUAL( node.getRange().size(), 1 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"range\": [ ";
		jsonString += " { \"min\": \"value1\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getRange().at(0).first,  "value1" );
			BOOST_CHECK_EQUAL( node.getRange().at(0).second, "" );
			BOOST_CHECK_EQUAL( node.getRange().size(), 1 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"range\": [ ";
		jsonString += " { \"min\": \"value1\", \"max\": \"value2\" },";
		jsonString += " { \"min\": \"value3\", \"max\": \"value4\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getRange().at(0).first,  "value1" );
			BOOST_CHECK_EQUAL( node.getRange().at(0).second, "value2" );
			BOOST_CHECK_EQUAL( node.getRange().at(1).first,  "value3" );
			BOOST_CHECK_EQUAL( node.getRange().at(1).second, "value4" );
			BOOST_CHECK_EQUAL( node.getRange().size(), 2 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"range\": [ ";
		jsonString += " { \"max\": \"value1\" },";
		jsonString += " { \"min\": \"value2\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getRange().at(0).first,  "" );
			BOOST_CHECK_EQUAL( node.getRange().at(0).second, "value1" );
			BOOST_CHECK_EQUAL( node.getRange().at(1).first,  "value2" );
			BOOST_CHECK_EQUAL( node.getRange().at(1).second, "" );
			BOOST_CHECK_EQUAL( node.getRange().size(), 2 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"range\": [ ";
		jsonString += " { \"min\": \"value1\" },";
		jsonString += " { \"min\": \"value2\", \"max\": \"value3\" },";
		jsonString += " { \"max\": \"value4\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getRange().at(0).first,  "value1" );
			BOOST_CHECK_EQUAL( node.getRange().at(0).second, "" );
			BOOST_CHECK_EQUAL( node.getRange().at(1).first,  "value2" );
			BOOST_CHECK_EQUAL( node.getRange().at(1).second, "value3" );
			BOOST_CHECK_EQUAL( node.getRange().at(2).first,  "" );
			BOOST_CHECK_EQUAL( node.getRange().at(2).second, "value4" );
			BOOST_CHECK_EQUAL( node.getRange().size(), 3 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_get_repetition )
{
	LOG_INFO( "\n>>> spec_reader_specNode_get_repetition <<<" );
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": 5 } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "5" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "5" );
			BOOST_CHECK_EQUAL( node.getRepetition().size(), 1 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": \"value\" } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "value" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "value" );
			BOOST_CHECK_EQUAL( node.getRepetition().size(), 1 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": [ ";
		jsonString += " { \"min\": \"value1\", \"max\": \"value2\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "value1" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "value2" );
			BOOST_CHECK_EQUAL( node.getRepetition().size(), 1 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": [ ";
		jsonString += " { \"min\": \"value1\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "value1" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "" );
			BOOST_CHECK_EQUAL( node.getRepetition().size(), 1 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": [ ";
		jsonString += " { \"min\": \"value1\", \"max\": \"value2\" },";
		jsonString += " { \"min\": \"value3\", \"max\": \"value4\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "value1" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "value2" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(1).first,  "value3" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(1).second, "value4" );
			BOOST_CHECK_EQUAL( node.getRepetition().size(), 2 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": [ ";
		jsonString += " { \"max\": \"value1\" },";
		jsonString += " { \"min\": \"value2\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "value1" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(1).first,  "value2" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(1).second, "" );
			BOOST_CHECK_EQUAL( node.getRepetition().size(), 2 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": [ ";
		jsonString += " { \"min\": \"value1\" },";
		jsonString += " { \"min\": \"value2\", \"max\": \"value3\" },";
		jsonString += " { \"max\": \"value4\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "value1" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(1).first,  "value2" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(1).second, "value3" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(2).first,  "" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(2).second, "value4" );
			BOOST_CHECK_EQUAL( node.getRepetition().size(), 3 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": [ ";
		jsonString += " { \"min\": \"value1\" },";
		jsonString += " { \"min\": \"value2\", \"max\": \"value3\" },";
		jsonString += " \"value4\"";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "value1" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(1).first,  "value2" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(1).second, "value3" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(2).first,  "value4" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(2).second, "value4" );
			BOOST_CHECK_EQUAL( node.getRepetition().size(), 3 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": [ ";
		jsonString += " { \"min\": \"value1\" },";
		jsonString += " { \"min\": \"value2\", \"max\": \"value3\" },";
		jsonString += " \"value4\",";
		jsonString += " 23";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "value1" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(1).first,  "value2" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(1).second, "value3" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(2).first,  "value4" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(2).second, "value4" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(3).first,  "23" );
			BOOST_CHECK_EQUAL( node.getRepetition().at(3).second, "23" );
			BOOST_CHECK_EQUAL( node.getRepetition().size(), 4 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_get_map )
{
	LOG_INFO( "\n>>> spec_reader_specNode_get_map <<<" );
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"map\": [ ";
		jsonString += " { \"index1\": \"value1\" },";
		jsonString += " { \"index2\": \"value2\" },";
		jsonString += " { \"index3\": \"value3\" },";
		jsonString += " { \"index4\": \"value4\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		size_t i = 0;
		BOOST_FOREACH( bpt::ptree::value_type n, tree.get_child( "header" ) )
		{
			SpecNode node( n );
			BOOST_CHECK_EQUAL( node.getMap()[ "index1" ],  "value1" );
			BOOST_CHECK_EQUAL( node.getMap()[ "index2" ],  "value2" );
			BOOST_CHECK_EQUAL( node.getMap()[ "index3" ],  "value3" );
			BOOST_CHECK_EQUAL( node.getMap()[ "index4" ],  "value4" );
			BOOST_CHECK_EQUAL( node.getMap().size(), 4 );
			i++;
		}
		BOOST_CHECK_EQUAL( i, 1 );
	}
}

BOOST_AUTO_TEST_SUITE_END()
