
BOOST_AUTO_TEST_SUITE( spec_reader_test_specification )

BOOST_AUTO_TEST_CASE( spec_reader_specification )
{
	LOG_INFO( "\n>>> spec_reader_specification <<<" );
	{
		std::string jsonString = R"*(
				{
					"standard": {
						"label": "Specification",
						"id": "specification",
						"type": "file",
						"extension": [
							".spc",
							".spec"
						]
					},
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
		BOOST_CHECK_EQUAL( spec.getId(),    "specification" );
		BOOST_CHECK_EQUAL( spec.getLabel(), "Specification" );
		BOOST_CHECK_EQUAL( spec.getType(),  "file" );
		BOOST_CHECK_EQUAL( spec.getSupportedExtensions().size(), 2 );
		BOOST_CHECK_EQUAL( spec.getSupportedExtensions().at(0),  ".spc" );
		BOOST_CHECK_EQUAL( spec.getSupportedExtensions().at(1),  ".spec" );
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

