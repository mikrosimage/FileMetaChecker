#include <BasicElement/SubElements/Data.hpp>
#include <boost/property_tree/json_parser.hpp>

BOOST_AUTO_TEST_SUITE( basic_element_test_data )

BOOST_AUTO_TEST_CASE( basic_element_data )
{
	LOG_INFO( "\n>>> basic_element_data <<<" );
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

	boost::property_tree::ptree tree;
	spec_reader::SpecNode node = getSpecNode( tree, jsonString );

	BOOST_CHECK_EQUAL( node.getId(),          id    );
	BOOST_CHECK_EQUAL( node.getLabel(),       label );
	BOOST_CHECK_EQUAL( node.getType(),        eTypeData       );
	BOOST_CHECK_EQUAL( node.getSubType(),     eSubTypeAscii   );
	BOOST_CHECK_EQUAL( node.getDisplayType(), eDisplayTypeRaw );
	{
		Data data( &node );
		BOOST_CHECK_EQUAL( data.getId(),          id    );
		BOOST_CHECK_EQUAL( data.getLabel(),       label );
		BOOST_CHECK_EQUAL( data.getType(),        eTypeData       );
		BOOST_CHECK_EQUAL( data.getSubType(),     eSubTypeAscii   );
		BOOST_CHECK_EQUAL( data.getDisplayType(), eDisplayTypeRaw );

		BOOST_CHECK_EQUAL( data.getUniqueId(),    node.getUId()         );
		BOOST_CHECK_EQUAL( data.getCount(),       node.getCount()       );
		BOOST_CHECK_EQUAL( data.getRequirement(), node.getRequirement() );
		BOOST_CHECK_EQUAL( data.getGroupSize(),   node.getGroupSize()   );

		BOOST_CHECK_EQUAL( data.getValues().size(),      node.getValues().size()      );
		BOOST_CHECK_EQUAL( data.getRange().size(),       node.getRange().size()       );
		BOOST_CHECK_EQUAL( data.getRepetitions().size(), node.getRepetitions().size() );
		BOOST_CHECK_EQUAL( data.getMap().size(),         node.getMap().size()         );

		BOOST_CHECK_EQUAL( data.isGroup(),     node.isGroup()     );
		BOOST_CHECK_EQUAL( data.isOrdered(),   node.isOrdered()   );
		BOOST_CHECK_EQUAL( data.isOptional(),  node.isOptional()  );
		BOOST_CHECK_EQUAL( data.isBigEndian(), node.isBigEndian() );

		BOOST_CHECK( data.getData() == nullptr );
	
		const size_t size = 5;
		const char buffer[] = { 0x7d, 0x21, 0x32, 'c', '!' };
		data.set( buffer, size );
		BOOST_CHECK_EQUAL( data.getSize(), size );
		for( size_t i = 0; i < size; ++i )
			BOOST_CHECK_EQUAL( data.getData()[i], buffer[i] );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_subType )
{
	LOG_INFO( "\n>>> basic_element_data_subType <<<" );
	{

		std::string jsonString = R"*(
			{ "header": [
				{
					"id": "id",
					"label": "label",
					"type": "unknown"
				}
			  ]
			}
		)*";

		boost::property_tree::ptree tree;
		spec_reader::SpecNode node = getSpecNode( tree, jsonString );
		
		Data data( &node );
		BOOST_CHECK_EQUAL( data.getSubType(), eSubTypeUnknown );
	}
	{
		std::string jsonString = R"*(
			{ "header": [
				{
					"id": "id",
					"label": "label",
					"type": "hexa"
				}
			  ]
			}
		)*";

		boost::property_tree::ptree tree;
		spec_reader::SpecNode node = getSpecNode( tree, jsonString );

		Data data( &node );
		BOOST_CHECK_EQUAL( data.getSubType(), eSubTypeHexa );
	}
	{
		std::string jsonString = R"*(
			{ "header": [
				{
					"id": "id",
					"label": "label",
					"type": "ascii"
				}
			  ]
			}
		)*";

		boost::property_tree::ptree tree;
		spec_reader::SpecNode node = getSpecNode( tree, jsonString );

		Data data( &node );
		BOOST_CHECK_EQUAL( data.getSubType(), eSubTypeAscii );
	}
	{
		std::string jsonString = R"*(
			{ "header": [
				{
					"id": "id",
					"label": "label",
					"type": "raw"
				}
			  ]
			}
		)*";

		boost::property_tree::ptree tree;
		spec_reader::SpecNode node = getSpecNode( tree, jsonString );

		Data data( &node );
		BOOST_CHECK_EQUAL( data.getSubType(), eSubTypeRaw );
	}
	{
		std::string jsonString = R"*(
			{ "header": [
				{
					"id": "id",
					"label": "label",
					"type": "uint8"
				}
			  ]
			}
		)*";

		boost::property_tree::ptree tree;
		spec_reader::SpecNode node = getSpecNode( tree, jsonString );

		Data data( &node );
		BOOST_CHECK_EQUAL( data.getSubType(), eSubTypeUInt8 );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_error_warning )
{
	LOG_INFO( "\n>>> basic_element_data_error_warning <<<" );
	
	std::string jsonString = R"*(
		{ "header": [
			{
				"id": "id",
				"label": "label",
				"type": "unknown"
			}
		  ]
		}
	)*";

	boost::property_tree::ptree tree;
	spec_reader::SpecNode node = getSpecNode( tree, jsonString );

	{
		Data data( &node );
		data.addErrorLabel( kError );
		BOOST_CHECK_EQUAL( data.getErrorLabel(), kError );
	}
	{
		Data data( &node );
		data.addErrorLabel( kError );
		data.addErrorLabel( kError );
		BOOST_CHECK_EQUAL( data.getErrorLabel(), kError + kError );
	}
	{
		Data data( &node );
		data.addWarningLabel( kWarning );
		BOOST_CHECK_EQUAL( data.getWarningLabel(), kWarning );
	}
	{
		Data data( &node );
		data.addWarningLabel( kWarning );
		data.addWarningLabel( kWarning );
		BOOST_CHECK_EQUAL( data.getWarningLabel(), kWarning + kWarning );
	}
}

BOOST_AUTO_TEST_SUITE_END()
