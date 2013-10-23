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

	spec_reader::Specification spec;
	spec.setFromString( jsonString );
	spec_reader::SpecNode node = *spec.getFirstNode();

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

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		spec_reader::SpecNode node = *spec.getFirstNode();
		
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

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		spec_reader::SpecNode node = *spec.getFirstNode();

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

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		spec_reader::SpecNode node = *spec.getFirstNode();

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

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		spec_reader::SpecNode node = *spec.getFirstNode();

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

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		spec_reader::SpecNode node = *spec.getFirstNode();

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

	spec_reader::Specification spec;
	spec.setFromString( jsonString );
	spec_reader::SpecNode node = *spec.getFirstNode();

	{
		Data data( &node );
		data.addErrorLabel( kError );
		BOOST_CHECK_EQUAL( data.getErrorLabel(), kError );
	}
	{
		Data data( &node );
		data.addErrorLabel( kError );
		data.addErrorLabel( kError );
		BOOST_CHECK_EQUAL( data.getErrorLabel(), kError + " / " + kError );
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
		BOOST_CHECK_EQUAL( data.getWarningLabel(), kWarning + " / " + kWarning );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_next )
{
	LOG_INFO( "\n>>> basic_element_data_next <<<" );
	{
		std::string jsonString = R"*(
				{
					"header": [
						{ "id": "value1" },
						{ "id": "value2" },
						{ "id": "value3" },
						{ "id": "value4" }
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		spec_reader::SpecNode node = *spec.getFirstNode();

		BOOST_CHECK_EQUAL( node.getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node.next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node.next()->next()->getId(),         "value3" );
		BOOST_CHECK_EQUAL( node.next()->next()->next()->getId(), "value4" );
		BOOST_CHECK( node.next()->next()->next()->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_next_firstchild )
{
	LOG_INFO( "\n>>> basic_element_data_next_firstchild <<<" );
	{
		std::string jsonString = R"*(
				{
					"header": [
						{ "id": "value1", "label": "Value1", "type": "ascii" },
						{ "id": "value2", "label": "Value2", "type": "ascii" },
						{ "id": "value3", "label": "Value3", "type": "ascii" },
						{ "id": "value4", "label": "Value4", "type": "ascii", "group": [
							{ "id": "firstchild" },
							{ "id": "secondchild" },
							{ "id": "thirdchild" }
						] }
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		spec_reader::SpecNode node = *spec.getFirstNode();

		BOOST_CHECK_EQUAL( node.getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node.next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node.next()->next()->getId(),         "value3" );
		BOOST_CHECK_EQUAL( node.next()->next()->next()->getId(), "value4" );

		std::shared_ptr< Element > parent = std::make_shared< Element >( node.next()->next()->next() );
		
		BOOST_CHECK_EQUAL( node.next()->next()->next()->firstChild( parent )->getId(),                 "firstchild" );
		BOOST_CHECK_EQUAL( node.next()->next()->next()->firstChild( parent )->next()->getId(),        "secondchild" );
		BOOST_CHECK_EQUAL( node.next()->next()->next()->firstChild( parent )->next()->next()->getId(), "thirdchild" );

		BOOST_CHECK( node.next()->next()->next()->firstChild( parent )->next()->next()->next() == nullptr );
	}
}

// BOOST_AUTO_TEST_CASE( spec_reader_spec_reader::SpecNode_first_child )
// {
// 	LOG_INFO( "\n>>> spec_reader_spec_reader::SpecNode_first_child <<<" );	// @todo : finish it after basic_element !
// 	{
// 		std::string jsonString = R"*(
// 				{
// 					"header": [
// 						{ 
// 							"id": "value1",
// 							"group": [
// 								{ "id": "value11" },
// 								{ "id": "value12" },
// 								{ "id": "value13" }
// 							]
// 						}
// 					]
// 				}
// 			)*";

// 		std::istringstream isstream( jsonString );
// 		bpt::ptree tree;

// 		bpt::read_json( isstream, tree );

// 		spec_reader::SpecNode node( tree.get_child( "header" ).begin() );
// 		BOOST_CHECK_EQUAL( node.getId(),  "value1" );

// 		std::set< std::string > childrenList = node.getChildrenNodes();
// 		BOOST_CHECK_EQUAL( childrenList.size(), 3 );
// 		BOOST_CHECK_EQUAL( childrenList.count( "value11" ), 1 );
// 		BOOST_CHECK_EQUAL( childrenList.count( "value12" ), 1 );
// 		BOOST_CHECK_EQUAL( childrenList.count( "value13" ), 1 );

// 		// std::shared_ptr< basic_element::Element > element( new basic_element::Element( &node, NULL ) );
// 		// node = *node.firstChild( element );
// 		// BOOST_CHECK_EQUAL( node.getId(), "value11" );
// 		// BOOST_CHECK_EQUAL( node.getParent(), element );

// 		// node = *node.next();
// 		// BOOST_CHECK_EQUAL( node.getId(), "value12" );
// 		// BOOST_CHECK_EQUAL( node.getParent(), element );

// 		// node = *node.next();
// 		// BOOST_CHECK_EQUAL( node.getId(), "value13" );
// 		// BOOST_CHECK_EQUAL( node.getParent(), element );

// 		// node = *node.next();
// 		// LOG_INFO( node.getId() );
// 	}
// 	// {
// 	// 	std::string jsonString = " { \"header\": [ ";
// 	// 	jsonString += " { \"id\": \"value1\" },";
// 	// 	jsonString += " { \"id\": \"value2\" },";
// 	// 	jsonString += " { \"id\": \"value3\" },";
// 	// 	jsonString += " { \"id\": \"value4\" }";
// 	// 	jsonString += " ] } ";

// 	// 	std::istringstream isstream( jsonString );
// 	// 	bpt::ptree tree;

// 	// 	bpt::read_json( isstream, tree );
// 	// 	spec_reader::SpecNode node( tree.get_child( "header" ).begin(), 0, 4 );
// 	// 	BOOST_CHECK_EQUAL( node.getId(), "value1"  );
// 	// 	BOOST_CHECK_THROW( node.firstChild(), std::runtime_error );

// 	// 	node = node.next();
// 	// 	BOOST_CHECK_EQUAL( node.getId(), "value2" );

// 	// 	node = node.next();
// 	// 	BOOST_CHECK_EQUAL( node.getId(), "value3" );

// 	// 	node = node.next();
// 	// 	BOOST_CHECK_EQUAL( node.getId(), "value4" );

// 	// 	node = node.next();
// 	// 	BOOST_CHECK_EQUAL( node.getIndex(),      4 );
// 	// 	BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );
// 	// }
// }

// // BOOST_AUTO_TEST_CASE( spec_reader_spec_reader::SpecNode_first_child_recursivity )
// // {
// // 	LOG_INFO( "\n>>> spec_reader_spec_reader::SpecNode_first_child_recursivity <<<" );
// // 	{
// // 		std::string jsonString = " { \"header\": [ ";
// // 		jsonString += " { \"id\": \"value1\",";
// // 		jsonString += "   \"group\": [ ";
// // 		jsonString += "     { \"id\": \"value11\" },";
// // 		jsonString += "     { \"id\": \"value12\",";
// // 		jsonString += "       \"group\": [ ";
// // 		jsonString += "          { \"id\": \"value121\", ";
// // 		jsonString += "            \"group\": [ ";
// // 		jsonString += "               { \"id\": \"value1211\" },";
// // 		jsonString += "               { \"id\": \"value1212\" }";
// // 		jsonString += "            ] } ";
// // 		jsonString += "       ] }, ";
// // 		jsonString += "     { \"id\": \"value13\" }";
// // 		jsonString += "   ] } ";
// // 		jsonString += " ] } ";

// // 		std::istringstream isstream( jsonString );
// // 		bpt::ptree tree;

// // 		bpt::read_json( isstream, tree );
// // 		spec_reader::SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
// // 		BOOST_CHECK_EQUAL( node.getId(),  "value1" );
// // 		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
// // 		BOOST_CHECK_EQUAL( node.getIndexTotal(), 1 );

// // 		node = node.firstChild();
// // 		BOOST_CHECK_EQUAL( node.getId(), "value11" );
// // 		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
// // 		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );
		
// // 		node = node.next();
// // 		BOOST_CHECK_EQUAL( node.getId(), "value12" );
// // 		BOOST_CHECK_EQUAL( node.getIndex(),      1 );
// // 		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );

// // 		spec_reader::SpecNode childNode = node.firstChild();
// // 		BOOST_CHECK_EQUAL( childNode.getId(), "value121" );
// // 		BOOST_CHECK_EQUAL( childNode.getIndex(),       0 );
// // 		BOOST_CHECK_EQUAL( childNode.getIndexTotal(),  1 );

// // 		childNode = childNode.next();
// // 		BOOST_CHECK_EQUAL( childNode.getIndex(),      1 );
// // 		BOOST_CHECK_EQUAL( childNode.getIndexTotal(), 1 );

// // 		childNode = childNode.firstChild();
// // 		BOOST_CHECK_EQUAL( childNode.getId(), "value1211" );
// // 		BOOST_CHECK_EQUAL( childNode.getIndex(),        0 );
// // 		BOOST_CHECK_EQUAL( childNode.getIndexTotal(),   2 );

// // 		childNode = childNode.next();
// // 		BOOST_CHECK_EQUAL( childNode.getId(), "value1212" );
// // 		BOOST_CHECK_EQUAL( childNode.getIndex(),        1 );
// // 		BOOST_CHECK_EQUAL( childNode.getIndexTotal(),   2 );

// // 		BOOST_CHECK_THROW( childNode.firstChild(), std::runtime_error );
		
// // 		childNode = childNode.next();
// // 		BOOST_CHECK_EQUAL( childNode.getIndex(),      2 );
// // 		BOOST_CHECK_EQUAL( childNode.getIndexTotal(), 2 );

// // 		node = node.next();
// // 		BOOST_CHECK_EQUAL( node.getId(), "value13" );
// // 		BOOST_CHECK_EQUAL( node.getIndex(),      2 );
// // 		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );

// // 		BOOST_CHECK_THROW( node.firstChild(), std::runtime_error );

// // 		node = node.next();
// // 		BOOST_CHECK_EQUAL( node.getIndex(),      3 );
// // 		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );

// // 		node = node.next();
// // 		BOOST_CHECK_EQUAL( node.getIndex(),      3 );
// // 		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );
// // 	}
// // }


BOOST_AUTO_TEST_SUITE_END()
