#include <map>

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
							"required": "true",
							"optional": false,
							"endian": "little",
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
		BOOST_CHECK_EQUAL( node.isOptional(),     false               );
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

		Specification spec;
		spec.setFromString( jsonString );
		SpecNode node = *spec.getFirstNode();
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
		std::vector< std::pair< std::string, std::string > > repet = node.getRepetitions();
		BOOST_CHECK_EQUAL( node.getRepetitions().size(), 3 );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).second, "" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(1).first,  "value2" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(1).second, "value3" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(2).first,  "value4" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(2).second, "value4" );
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
		BOOST_CHECK_EQUAL( node.getRepetitions().size(), 4 );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(0).second, "" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(1).first,  "value2" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(1).second, "value3" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(2).first,  "value4" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(2).second, "value4" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(3).first,  "23" );
		BOOST_CHECK_EQUAL( node.getRepetitions().at(3).second, "23" );
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
		
		std::map< std::string, std::string > map = node.getMap();

		BOOST_CHECK_EQUAL( node.getMap().size(), 4 );
		for( auto mapElem : testMap )
			BOOST_CHECK_EQUAL( node.getMap().at( mapElem.first ),  mapElem.second );

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
		BOOST_CHECK( node.next()->next()->next()->next() == nullptr );
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

BOOST_AUTO_TEST_CASE( spec_reader_specNode_firstchild )
{
	LOG_INFO( "\n>>> spec_reader_specNode_firstchild <<<" );
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

		Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< SpecNode > node1 = spec.getFirstNode();

		BOOST_CHECK_EQUAL( node1->getId(), "value1" );
		
		std::shared_ptr< SpecNode > node2 = node1->next();
		BOOST_CHECK_EQUAL( node2->getId(), "value2" );
		
		std::shared_ptr< SpecNode > node3 = node2->next();
		BOOST_CHECK_EQUAL( node3->getId(), "value3" );
		
		std::shared_ptr< SpecNode > node4 = node3->next();
		BOOST_CHECK_EQUAL( node4->getId(), "value4" );
		
		BOOST_CHECK( node1->next()->next()->next()->next() == nullptr );

		std::shared_ptr< SpecNode > child1 = node4->firstChild();
		BOOST_CHECK_EQUAL( child1->getId(), "firstchild" );

		std::shared_ptr< SpecNode > child2 = child1->next();
		BOOST_CHECK_EQUAL( child2->getId(), "secondchild" );

		std::shared_ptr< SpecNode > child3 = child2->next();
		BOOST_CHECK_EQUAL( child3->getId(), "thirdchild" );

		BOOST_CHECK( child3->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_next_first_child_recursivity )
{
	LOG_INFO( "\n>>> spec_reader_specNode_next_first_child_recursivity <<<" );
	{
		std::string jsonString = R"*(
				{
					"header": [
						{ "id": "value1",
						  "label": "Value1",
						  "type": "ascii",
						  "group": [
								{ "id": "value11",
								  "label": "Value11",
								  "type": "ascii" },
								{ "id": "value12",
								  "label": "Value12",
								  "type": "ascii",
								  "group": [
										{ "id": "value121",
										  "label": "Value121",
										  "type": "ascii",
										  "group": [
											{ "id": "value1211", "label": "Value1211", "type": "ascii" },
											{ "id": "value1212", "label": "Value1212", "type": "ascii" }
										  ] }
							      ] },
								{ "id": "value13",
								  "label": "Value13",
								  "type": "ascii" }
						  ] }
					]
				}
			)*";

		Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< SpecNode > node1 = spec.getFirstNode();

		BOOST_CHECK_EQUAL( node1->getId(),  "value1" );
		BOOST_CHECK( node1->getParent() == nullptr );
		
		std::shared_ptr< SpecNode > node2 = node1->firstChild();
		BOOST_CHECK_EQUAL( node2->getId(), "value11" );
		BOOST_CHECK_EQUAL( node2->getParent()->getId(), "value1" );
		BOOST_CHECK_EQUAL( node2->getParent()->getUId(), node1->getUId() );
		
		std::shared_ptr< SpecNode > node3 = node2->next();
		BOOST_CHECK_EQUAL( node3->getId(), "value12" );
		BOOST_CHECK_EQUAL( node3->getParent()->getId(), "value1" );
		BOOST_CHECK_EQUAL( node2->getParent()->getUId(), node1->getUId() );

		std::shared_ptr< SpecNode > childNode1 = node3->firstChild();
		
		BOOST_CHECK_EQUAL( childNode1->getId(), "value121" );
		BOOST_CHECK_EQUAL( childNode1->getParent()->getId(), "value12" );
		BOOST_CHECK_EQUAL( childNode1->getParent()->getUId(), node3->getUId() );

		std::shared_ptr< SpecNode > nullNode = childNode1->next();
		
		BOOST_CHECK( nullNode == nullptr );

		std::shared_ptr< SpecNode > childNode2 = childNode1->firstChild();
		BOOST_CHECK_EQUAL( childNode2->getId(), "value1211" );
		BOOST_CHECK_EQUAL( childNode2->getParent()->getId(), "value121" );
		BOOST_CHECK_EQUAL( childNode2->getParent()->getUId(), childNode1->getUId() );

		std::shared_ptr< SpecNode > childNode3 = childNode2->next();
		BOOST_CHECK_EQUAL( childNode3->getId(), "value1212" );
		BOOST_CHECK_EQUAL( childNode3->getParent()->getId(), "value121" );
		BOOST_CHECK_EQUAL( childNode3->getParent()->getUId(), childNode1->getUId() );
		BOOST_CHECK_THROW( childNode3->firstChild(), std::runtime_error );
		
		nullNode = childNode3->next();
		BOOST_CHECK( nullNode == nullptr );

		std::shared_ptr< SpecNode > node4 = node3->next();
		BOOST_CHECK_EQUAL( node4->getId(), "value13" );
		BOOST_CHECK_THROW( node4->firstChild(), std::runtime_error );

		std::shared_ptr< SpecNode > node5 = node4->next();
		BOOST_CHECK( node5 == nullptr );
	}
}

BOOST_AUTO_TEST_SUITE_END()

