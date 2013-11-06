
BOOST_AUTO_TEST_SUITE( element_checker_test_checker_unordered_group )

BOOST_AUTO_TEST_CASE( element_checker_checker_unordered_group )
{
	std::string jsonString = R"*(
			{
				"header": [
					{ "id": "value1",
					  "label": "Value1",
					  "type": "ascii",
					  "ordered": false,
					  "values": "WAVE4",
					  "group": [
							{ "id": "value11",
							  "label": "Value11",
							  "type": "ascii",
							  "values": "WAVE1" },
							{ "id": "value12",
							  "label": "Value12",
							  "type": "ascii",
							  "values": "WAVE2" },
							{ "id": "value13",
							  "label": "Value13",
							  "type": "ascii",
							  "values": "WAVE3" }
					  ] }
				]
			}
		)*";

	const char buff1[5] { 'W', 'A', 'V', 'E', '1' };
	const char buff2[5] { 'W', 'A', 'V', 'E', '2' };
	const char buff3[5] { 'W', 'A', 'V', 'E', '3' };
	const char buff4[5] { 'W', 'A', 'V', 'E', '4' };

	LOG_INFO( "\n>>> element_checker_checker_unordered_group <<<" );
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                               "value1" );
		BOOST_CHECK_EQUAL( node->firstChild()->getId(),                 "value11" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->getId(),         "value12" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->next()->getId(), "value13" );
		BOOST_CHECK( node->firstChild()->next()->next()->next() == nullptr );
		BOOST_CHECK( node->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem0( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusNotChecked );
		elem0->set( (const char*)&buff4, 5 );
		checker.check( elem0 );
		BOOST_CHECK_EQUAL( elem0->_id,     node->getId() );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( elem0->next(), elem0, elem0 ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		elem1->set( (const char*)&buff2, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem0 ) );
		elem2->set( (const char*)&buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem0 ) );
		elem3->set( (const char*)&buff3, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem0 ) );
		elem4->set( (const char*)&buff3, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem0 ) );
		elem5->set( (const char*)&buff3, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5, elem0 ) );
		elem6->set( (const char*)&buff1, 5 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6, elem0 ) );
		elem7->set( (const char*)&buff4, 5 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem8( new basic_element::Element( elem7->next(), elem7, elem0 ) );
		elem8->set( (const char*)&buff4, 5 );
		checker.check( elem8 );
		BOOST_CHECK_EQUAL( elem8->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem8->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem9( new basic_element::Element( elem8->next(), elem8, elem0 ) );
		elem9->set( (const char*)&buff4, 5 );
		checker.check( elem9 );
		BOOST_CHECK_EQUAL( elem9->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem9->_status, eStatusInvalidButSkip );

		BOOST_CHECK( elem9->next() == nullptr );
		BOOST_CHECK_EQUAL( elem0->_status,  eStatusValid );
	}
	LOG_INFO( "\n>>> element_checker_checker_unordered_group suite <<<" );
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                               "value1" );
		BOOST_CHECK_EQUAL( node->firstChild()->getId(),                 "value11" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->getId(),         "value12" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->next()->getId(), "value13" );
		BOOST_CHECK( node->firstChild()->next()->next()->next() == nullptr );
		BOOST_CHECK( node->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem0( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusNotChecked );
		elem0->set( (const char*)&buff4, 5 );
		checker.check( elem0 );
		BOOST_CHECK_EQUAL( elem0->_id,     node->getId() );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( elem0->next(), elem0, elem0 ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		elem1->set( (const char*)&buff4, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem0 ) );
		elem2->set( (const char*)&buff4, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem0 ) );
		elem3->set( (const char*)&buff4, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusInvalidButSkip );

		BOOST_CHECK( elem3->next() == nullptr );
		BOOST_CHECK_EQUAL( elem0->_status,  eStatusInvalidForUnordered );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_unordered_group_and_repetitions )
{
	std::string jsonString = R"*(
			{
				"header": [
					{ "id": "value1",
					  "label": "Value1",
					  "type": "ascii",
					  "ordered": false,
					  "values": "WAVE4",
					  "group": [
							{ "id": "value11",
							  "label": "Value11",
							  "type": "ascii",
							  "values": "WAVE1" },
							{ "id": "value12",
							  "label": "Value12",
							  "type": "ascii",
							  "values": "WAVE2",
							  "repeated": "2" },
							{ "id": "value13",
							  "label": "Value13",
							  "type": "ascii",
							  "values": "WAVE3" }
					  ] }
				]
			}
		)*";

	const char buff1[5] { 'W', 'A', 'V', 'E', '1' };
	const char buff2[5] { 'W', 'A', 'V', 'E', '2' };
	const char buff3[5] { 'W', 'A', 'V', 'E', '3' };
	const char buff4[5] { 'W', 'A', 'V', 'E', '4' };

	LOG_INFO( "\n>>> element_checker_checker_unordered_group_and_repetitions <<<" );
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                               "value1" );
		BOOST_CHECK_EQUAL( node->firstChild()->getId(),                 "value11" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->getId(),         "value12" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->next()->getId(), "value13" );
		BOOST_CHECK( node->firstChild()->next()->next()->next() == nullptr );
		BOOST_CHECK( node->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem0( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusNotChecked );
		elem0->set( (const char*)&buff4, 5 );
		checker.check( elem0 );
		BOOST_CHECK_EQUAL( elem0->_id,     node->getId() );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( elem0->next(), elem0, elem0 ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		elem1->set( (const char*)&buff2, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem0 ) );
		elem2->set( (const char*)&buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem0 ) );
		elem3->set( (const char*)&buff3, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem0 ) );
		elem4->set( (const char*)&buff3, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem0 ) );
		elem5->set( (const char*)&buff3, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5, elem0 ) );
		elem6->set( (const char*)&buff1, 5 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6, elem0 ) );
		elem7->set( (const char*)&buff2, 5 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem8( new basic_element::Element( elem7->next(), elem7, elem0 ) );
		elem8->set( (const char*)&buff2, 5 );
		checker.check( elem8 );
		BOOST_CHECK_EQUAL( elem8->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem8->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem9( new basic_element::Element( elem8->next(), elem8, elem0 ) );
		elem9->set( (const char*)&buff4, 5 );
		checker.check( elem9 );
		BOOST_CHECK_EQUAL( elem9->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem9->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem10( new basic_element::Element( elem9->next(), elem9, elem0 ) );
		elem10->set( (const char*)&buff4, 5 );
		checker.check( elem10 );
		BOOST_CHECK_EQUAL( elem10->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem10->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem11( new basic_element::Element( elem10->next(), elem10, elem0 ) );
		elem11->set( (const char*)&buff4, 5 );
		checker.check( elem11 );
		BOOST_CHECK_EQUAL( elem11->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem11->_status, eStatusInvalidButSkip );

		BOOST_CHECK( elem11->next() == nullptr );
		BOOST_CHECK_EQUAL( elem0->_status,  eStatusValid );
	}
	LOG_INFO( "\n>>> element_checker_checker_unordered_group_and_repetitions suite <<<" );
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                               "value1" );
		BOOST_CHECK_EQUAL( node->firstChild()->getId(),                 "value11" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->getId(),         "value12" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->next()->getId(), "value13" );
		BOOST_CHECK( node->firstChild()->next()->next()->next() == nullptr );
		BOOST_CHECK( node->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem0( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusNotChecked );
		elem0->set( (const char*)&buff4, 5 );
		checker.check( elem0 );
		BOOST_CHECK_EQUAL( elem0->_id,     node->getId() );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( elem0->next(), elem0, elem0 ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		elem1->set( (const char*)&buff2, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem0 ) );
		elem2->set( (const char*)&buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem0 ) );
		elem3->set( (const char*)&buff3, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem0 ) );
		elem4->set( (const char*)&buff3, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem0 ) );
		elem5->set( (const char*)&buff3, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5, elem0 ) );
		elem6->set( (const char*)&buff1, 5 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6, elem0 ) );
		elem7->set( (const char*)&buff4, 5 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem8( new basic_element::Element( elem7->next(), elem7, elem0 ) );
		elem8->set( (const char*)&buff4, 5 );
		checker.check( elem8 );
		BOOST_CHECK_EQUAL( elem8->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem8->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem9( new basic_element::Element( elem8->next(), elem8, elem0 ) );
		elem9->set( (const char*)&buff4, 5 );
		checker.check( elem9 );
		BOOST_CHECK_EQUAL( elem9->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem9->_status, eStatusInvalidButSkip );

		BOOST_CHECK( elem9->next() == nullptr );
		BOOST_CHECK_EQUAL( elem0->_status,  eStatusInvalidGroupForIteration );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_unordered_group_and_repetitions_2 )
{
	std::string jsonStringBegin = R"*(
				{
					"header": [
						{
							"id": "value1",
							"label": "Value1",
							"type": "ascii",
							"ordered": false,
							"values": "WAVE4",
							"group": [
								{
									"id": "value11",
									"label": "Value11",
									"type": "ascii",
									"values": "WAVE1"
								},
								{
									"id": "value12",
									"label": "Value12",
									"type": "ascii",
									"values": "WAVE2",
									"repeated": )*";

	std::string jsonStringEnd = R"*(
								},
								{
									"id": "value13",
									"label": "Value13",
									"type": "ascii",
									"values": "WAVE3"
								}
							]
						}
					]
				}
			)*";

	const char buff1[5] { 'W', 'A', 'V', 'E', '1' };
	const char buff2[5] { 'W', 'A', 'V', 'E', '2' };
	const char buff3[5] { 'W', 'A', 'V', 'E', '3' };
	const char buff4[5] { 'W', 'A', 'V', 'E', '4' };

	LOG_INFO( "\n>>> element_checker_checker_unordered_group_and_repetitions_2 <<<" );
	{
		std::string jsonStringRepetition = R"*( [
									{ "min": "2", "max": "4" }
								] )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringRepetition + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                               "value1" );
		BOOST_CHECK_EQUAL( node->firstChild()->getId(),                 "value11" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->getId(),         "value12" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->next()->getId(), "value13" );
		BOOST_CHECK( node->firstChild()->next()->next()->next() == nullptr );
		BOOST_CHECK( node->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem0( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusNotChecked );
		elem0->set( (const char*)&buff4, 5 );
		checker.check( elem0 );
		BOOST_CHECK_EQUAL( elem0->_id,     node->getId() );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( elem0->next(), elem0, elem0 ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		elem1->set( (const char*)&buff2, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem0 ) );
		elem2->set( (const char*)&buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem0 ) );
		elem3->set( (const char*)&buff3, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem0 ) );
		elem4->set( (const char*)&buff3, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem0 ) );
		elem5->set( (const char*)&buff3, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5, elem0 ) );
		elem6->set( (const char*)&buff1, 5 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6, elem0 ) );
		elem7->set( (const char*)&buff2, 5 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem8( new basic_element::Element( elem7->next(), elem7, elem0 ) );
		elem8->set( (const char*)&buff2, 5 );
		checker.check( elem8 );
		BOOST_CHECK_EQUAL( elem8->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem8->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem9( new basic_element::Element( elem8->next(), elem8, elem0 ) );
		elem9->set( (const char*)&buff4, 5 );
		checker.check( elem9 );
		BOOST_CHECK_EQUAL( elem9->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem9->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem10( new basic_element::Element( elem9->next(), elem9, elem0 ) );
		elem10->set( (const char*)&buff4, 5 );
		checker.check( elem10 );
		BOOST_CHECK_EQUAL( elem10->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem10->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem11( new basic_element::Element( elem10->next(), elem10, elem0 ) );
		elem11->set( (const char*)&buff4, 5 );
		checker.check( elem11 );
		BOOST_CHECK_EQUAL( elem11->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem11->_status, eStatusInvalidButSkip );

		BOOST_CHECK( elem11->next() == nullptr );
		BOOST_CHECK_EQUAL( elem0->_status,  eStatusValid );
	}
	LOG_INFO( "\n>>> element_checker_checker_unordered_group_and_repetitions_2 suite <<<" );
	{
		std::string jsonStringRepetition = R"*( [
									{ "max": "2" }
								] )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringRepetition + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                               "value1" );
		BOOST_CHECK_EQUAL( node->firstChild()->getId(),                 "value11" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->getId(),         "value12" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->next()->getId(), "value13" );
		BOOST_CHECK( node->firstChild()->next()->next()->next() == nullptr );
		BOOST_CHECK( node->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem0( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusNotChecked );
		elem0->set( (const char*)&buff4, 5 );
		checker.check( elem0 );
		BOOST_CHECK_EQUAL( elem0->_id,     node->getId() );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( elem0->next(), elem0, elem0 ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		elem1->set( (const char*)&buff2, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem0 ) );
		elem2->set( (const char*)&buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem0 ) );
		elem3->set( (const char*)&buff3, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem0 ) );
		elem4->set( (const char*)&buff3, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem0 ) );
		elem5->set( (const char*)&buff3, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5, elem0 ) );
		elem6->set( (const char*)&buff1, 5 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6, elem0 ) );
		elem7->set( (const char*)&buff2, 5 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem8( new basic_element::Element( elem7->next(), elem7, elem0 ) );
		elem8->set( (const char*)&buff2, 5 );
		checker.check( elem8 );
		BOOST_CHECK_EQUAL( elem8->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem8->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem9( new basic_element::Element( elem8->next(), elem8, elem0 ) );
		elem9->set( (const char*)&buff2, 5 );
		checker.check( elem9 );
		BOOST_CHECK_EQUAL( elem9->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem9->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem10( new basic_element::Element( elem9->next(), elem9, elem0 ) );
		elem10->set( (const char*)&buff2, 5 );
		checker.check( elem10 );
		BOOST_CHECK_EQUAL( elem10->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem10->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem11( new basic_element::Element( elem10->next(), elem10, elem0 ) );
		elem11->set( (const char*)&buff4, 5 );
		checker.check( elem11 );
		BOOST_CHECK_EQUAL( elem11->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem11->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem12( new basic_element::Element( elem11->next(), elem11, elem0 ) );
		elem12->set( (const char*)&buff4, 5 );
		checker.check( elem12 );
		BOOST_CHECK_EQUAL( elem12->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem12->_status, eStatusInvalidButSkip );

		std::shared_ptr< basic_element::Element > elem13( new basic_element::Element( elem12->next(), elem12, elem0 ) );
		elem13->set( (const char*)&buff4, 5 );
		checker.check( elem13 );
		BOOST_CHECK_EQUAL( elem13->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem13->_status, eStatusInvalidButSkip );

		BOOST_CHECK( elem13->next() == nullptr );
		BOOST_CHECK_EQUAL( elem0->_status,  eStatusInvalidGroupForIteration );
	}
}

BOOST_AUTO_TEST_SUITE_END()
