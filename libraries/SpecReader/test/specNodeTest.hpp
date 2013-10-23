#include <map>
#include <boost/property_tree/json_parser.hpp>

namespace bpt = boost::property_tree;

BOOST_AUTO_TEST_SUITE( spec_reader_test_specNode )

BOOST_AUTO_TEST_CASE( spec_reader_specNode )
{
	LOG_INFO( "\n>>> spec_reader_specNode <<<" );
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

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();

		BOOST_CHECK_EQUAL( node.getId(),          "id"                );
		BOOST_CHECK_EQUAL( node.getLabel(),       "label"             );
		BOOST_CHECK_EQUAL( node.getType(),        eTypeUnknown        );
		BOOST_CHECK_EQUAL( node.getDisplayType(), eDisplayTypeDefault );
		BOOST_CHECK_EQUAL( node.getCount(),       "123"               );
		BOOST_CHECK_EQUAL( node.getRequirement(), "true"              );
		BOOST_CHECK_EQUAL( node.isGroup(),        true                );
		BOOST_CHECK_EQUAL( node.getGroupSize(),   "size"              );
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"key": "value"
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();

		BOOST_CHECK_THROW( node.getId(),    std::runtime_error );
		BOOST_CHECK_THROW( node.getLabel(), std::runtime_error );
		BOOST_CHECK_THROW( node.getType(),  std::runtime_error );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_get_values )
{
	LOG_INFO( "\n>>> spec_reader_specNode_get_values <<<" );
	{

		std::string jsonString = R"*(
				{
					"header": [
						{
							"values": "value"
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getValues().at(0), "value" );
		BOOST_CHECK_EQUAL( node.getValues().size(), 1 );
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{ 
							"values": [
								"value1",
								"value2",
								"value3"
							]
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getValues().at(0), "value1" );
		BOOST_CHECK_EQUAL( node.getValues().at(1), "value2" );
		BOOST_CHECK_EQUAL( node.getValues().at(2), "value3" );
		BOOST_CHECK_EQUAL( node.getValues().size(), 3 );
	}
	
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_get_range )
{
	LOG_INFO( "\n>>> spec_reader_specNode_get_range <<<" );
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"range": [
								{
									"min": "value1",
									"max": "value2"
								}
							]
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getRange().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRange().at(0).second, "value2" );
		BOOST_CHECK_EQUAL( node.getRange().size(), 1 );
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"range": [
								{
									"min": "value1"
								}
							]
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getRange().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRange().at(0).second, "" );
		BOOST_CHECK_EQUAL( node.getRange().size(), 1 );
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"range": [
								{
									"min": "value1",
									"max": "value2"
								},
								{
									"min": "value3",
									"max": "value4"
								}
							]
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getRange().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRange().at(0).second, "value2" );
		BOOST_CHECK_EQUAL( node.getRange().at(1).first,  "value3" );
		BOOST_CHECK_EQUAL( node.getRange().at(1).second, "value4" );
		BOOST_CHECK_EQUAL( node.getRange().size(), 2 );
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"range": [
								{
									"max": "value1"
								},
								{
									"min": "value2"
								}
							]
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getRange().at(0).first,  "" );
		BOOST_CHECK_EQUAL( node.getRange().at(0).second, "value1" );
		BOOST_CHECK_EQUAL( node.getRange().at(1).first,  "value2" );
		BOOST_CHECK_EQUAL( node.getRange().at(1).second, "" );
		BOOST_CHECK_EQUAL( node.getRange().size(), 2 );
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"range": [
								{
									"min": "value1"
								},
								{
									"min": "value2",
									"max": "value3"
								},
								{
									"max": "value4"
								}
							]
						}
					]
				}
			)*";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		
		SpecNode node( nullptr, tree.get_child( "header" ).begin() );
		BOOST_CHECK_EQUAL( node.getRange().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRange().at(0).second, "" );
		BOOST_CHECK_EQUAL( node.getRange().at(1).first,  "value2" );
		BOOST_CHECK_EQUAL( node.getRange().at(1).second, "value3" );
		BOOST_CHECK_EQUAL( node.getRange().at(2).first,  "" );
		BOOST_CHECK_EQUAL( node.getRange().at(2).second, "value4" );
		BOOST_CHECK_EQUAL( node.getRange().size(), 3 );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_get_repetition )
{
	LOG_INFO( "\n>>> spec_reader_specNode_get_repetition <<<" );
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"repeated": 5
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).first,  "5" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).second, "5" );
		BOOST_CHECK_EQUAL( node.getRepetitions().size(), 1 );
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"repeated": "value"
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).first,  "value" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).second, "value" );
		BOOST_CHECK_EQUAL( node.getRepetitions().size(), 1 );
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"repeated": [
								{
									"min": "value1",
									"max": "value2"
								}
							]
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).second, "value2" );
		BOOST_CHECK_EQUAL( node.getRepetitions().size(), 1 );
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"repeated": [
								{
									"min": "value1"
								}
							]
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).second, "" );
		BOOST_CHECK_EQUAL( node.getRepetitions().size(), 1 );
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"repeated": [
								{
									"min": "value1",
									"max": "value2"
								},
								{
									"min": "value3",
									"max": "value4"
								}
							]
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).second, "value2" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(1).first,  "value3" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(1).second, "value4" );
		BOOST_CHECK_EQUAL( node.getRepetitions().size(), 2 );
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"repeated": [
								{
									"max": "value1"
								},
								{
									"min": "value2"
								}
							]
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).first,  "" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).second, "value1" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(1).first,  "value2" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(1).second, "" );
		BOOST_CHECK_EQUAL( node.getRepetitions().size(), 2 );
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"repeated": [
								{
									"min": "value1"
								},
								{
									"min": "value2",
									"max": "value3"
								},
								{
									"max": "value4"
								}
							]
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).second, "" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(1).first,  "value2" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(1).second, "value3" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(2).first,  "" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(2).second, "value4" );
		BOOST_CHECK_EQUAL( node.getRepetitions().size(), 3 );
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"repeated": [
								{
									"min": "value1"
								},
								{
									"min": "value2",
									"max": "value3"
								},
								"value4"
							]
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).second, "" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(1).first,  "value2" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(1).second, "value3" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(2).first,  "value4" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(2).second, "value4" );
		BOOST_CHECK_EQUAL( node.getRepetitions().size(), 3 );
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"repeated": [
								{
									"min": "value1"
								},
								{
									"min": "value2",
									"max": "value3"
								},
								"value4",
								23
							]
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).second, "" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(1).first,  "value2" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(1).second, "value3" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(2).first,  "value4" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(2).second, "value4" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(3).first,  "23" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(3).second, "23" );
		BOOST_CHECK_EQUAL( node.getRepetitions().size(), 4 );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_get_map )
{
	LOG_INFO( "\n>>> spec_reader_specNode_get_map <<<" );
	{
		std::map<std::string, std::string> testMap {
			{ "index1", "value1" },
			{ "index2", "value2" },
			{ "index3", "value3" },
			{ "index4", "value4" }
		};
		
		std::string jsonString = R"*(
			{
				"header": [
					{
						"map": [ )*";

		for( auto mapElem : testMap )
			jsonString += " { \"" + mapElem.first + "\": \"" + mapElem.second + "\" },";
		jsonString.erase( jsonString.end() - 1, jsonString.end() );
		
		jsonString += R"*( ]
					}
				]
			} )*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		
		for( auto mapElem : testMap )
			BOOST_CHECK_EQUAL( node.getMap()[ mapElem.first ],  mapElem.second );

		BOOST_CHECK_EQUAL( node.getMap().size(), 4 );
	}
	
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"map": []
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getMap().size(), 0 );
	}

	{
		std::string jsonString = R"*(
				{
					"header": [
						{
							"unknownKey": "noValue"
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getMap().size(), 0 );
	}
	
	{
		std::string jsonString = R"*(
				{
					"header": [
						{
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getMap().size(), 0 );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_next )
{
	LOG_INFO( "\n>>> spec_reader_specNode_next <<<" );
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

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node.next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node.next()->next()->getId(),         "value3" );
		BOOST_CHECK_EQUAL( node.next()->next()->next()->getId(), "value4" );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_children )
{
	LOG_INFO( "\n>>> spec_reader_specNode_children <<<" );
	{
		std::string jsonString = R"*(
				{
					"header": [
						{ 
							"id": "value1",
							"group": [
								{ "id": "value11" },
								{ "id": "value12" },
								{ "id": "value13" }
							]
						}
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
		BOOST_CHECK_EQUAL( node.getId(),  "value1" );

		std::set< std::string > childrenList = node.getChildrenNodes();
		BOOST_CHECK_EQUAL( childrenList.size(), 3 );
		BOOST_CHECK_EQUAL( childrenList.count( "value11" ), 1 );
		BOOST_CHECK_EQUAL( childrenList.count( "value12" ), 1 );
		BOOST_CHECK_EQUAL( childrenList.count( "value13" ), 1 );
	}
}

BOOST_AUTO_TEST_SUITE_END()

