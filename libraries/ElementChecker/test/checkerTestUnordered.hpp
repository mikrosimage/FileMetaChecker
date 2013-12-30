
BOOST_AUTO_TEST_SUITE( element_checker_test_checker_unordered_group )

BOOST_AUTO_TEST_CASE( element_checker_checker_unordered_group )
{
	std::string jsonString = R"*(
			{
				"content": [
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

	std::vector< char > buff1 { 'W', 'A', 'V', 'E', '1' };
	std::vector< char > buff2 { 'W', 'A', 'V', 'E', '2' };
	std::vector< char > buff3 { 'W', 'A', 'V', 'E', '3' };
	std::vector< char > buff4 { 'W', 'A', 'V', 'E', '4' };

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
		BOOST_CHECK_EQUAL( elem0->_status, eStatusUnknown );
		elem0->set( buff4 );
		checker.check( elem0 );
		BOOST_CHECK_EQUAL( elem0->_id,     node->getId() );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( elem0->next(), elem0, elem0 ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff2 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem0 ) );
		elem2->set( buff2 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem0 ) );
		elem3->set( buff3 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem0 ) );
		elem4->set( buff3 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem0 ) );
		elem5->set( buff3 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5, elem0 ) );
		elem6->set( buff1 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6, elem0 ) );
		elem7->set( buff4 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem8( new basic_element::Element( elem7->next(), elem7, elem0 ) );
		elem8->set( buff4 );
		checker.check( elem8 );
		BOOST_CHECK_EQUAL( elem8->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem8->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem9( new basic_element::Element( elem8->next(), elem8, elem0 ) );
		elem9->set( buff4 );
		checker.check( elem9 );
		BOOST_CHECK_EQUAL( elem9->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem9->_status, eStatusSkip );

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
		BOOST_CHECK_EQUAL( elem0->_status, eStatusUnknown );
		elem0->set( buff4 );
		checker.check( elem0 );
		BOOST_CHECK_EQUAL( elem0->_id,     node->getId() );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( elem0->next(), elem0, elem0 ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff4 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem0 ) );
		elem2->set( buff4 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem0 ) );
		elem3->set( buff4 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusSkip );

		BOOST_CHECK( elem3->next() == nullptr );
		BOOST_CHECK_EQUAL( elem0->_status,  eStatusInvalid );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_unordered_group_2 )
{
	std::string jsonString = R"*(
			{
				"content": [
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
							  "group": [
								{ "id": "value121",
								  "label": "Value121",
								  "type": "ascii",
								  "values": "WAVE3" }
							] }
					  ] }
				]
			}
		)*";

	std::vector< char > buff1 { 'W', 'A', 'V', 'E', '1' };
	std::vector< char > buff2 { 'W', 'A', 'V', 'E', '2' };
	std::vector< char > buff3 { 'W', 'A', 'V', 'E', '3' };
	std::vector< char > buff4 { 'W', 'A', 'V', 'E', '4' };

	LOG_INFO( "\n>>> element_checker_checker_unordered_group_2 <<<" );
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                                     "value1" );
		BOOST_CHECK_EQUAL( node->firstChild()->getId(),                       "value11" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->getId(),               "value12" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->firstChild()->getId(), "value121" );
		BOOST_CHECK( node->firstChild()->next()->firstChild()->next() == nullptr );
		BOOST_CHECK( node->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem0( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusUnknown );
		elem0->set( buff4 );
		checker.check( elem0 );
		BOOST_CHECK_EQUAL( elem0->_id,     node->getId() );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( elem0->next(), elem0, elem0 ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff2 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem0 ) );
		elem2->set( buff2 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem2 ) );
		elem3->set( buff3 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_id,     node->firstChild()->next()->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem0 ) );
		elem4->set( buff1 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem0 ) );
		elem5->set( buff4 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5, elem0 ) );
		elem6->set( buff4 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusSkip );

		BOOST_CHECK( elem6->next() == nullptr );
		BOOST_CHECK_EQUAL( elem0->_status,  eStatusValid );
	}
	LOG_INFO( "\n>>> element_checker_checker_unordered_group suite <<<" );
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                                     "value1" );
		BOOST_CHECK_EQUAL( node->firstChild()->getId(),                       "value11" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->getId(),               "value12" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->firstChild()->getId(), "value121" );
		BOOST_CHECK( node->firstChild()->next()->firstChild()->next() == nullptr );
		BOOST_CHECK( node->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem0( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusUnknown );
		elem0->set( buff4 );
		checker.check( elem0 );
		BOOST_CHECK_EQUAL( elem0->_id,     node->getId() );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( elem0->next(), elem0, elem0 ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff4 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem0 ) );
		elem2->set( buff4 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusSkip );

		BOOST_CHECK( elem2->next() == nullptr );
		BOOST_CHECK_EQUAL( elem0->_status,  eStatusInvalid );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_unordered_group_and_repetitions )
{
	std::string jsonString = R"*(
			{
				"content": [
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
							  "repetition": "2" },
							{ "id": "value13",
							  "label": "Value13",
							  "type": "ascii",
							  "values": "WAVE3" }
					  ] }
				]
			}
		)*";

	std::vector< char > buff1 { 'W', 'A', 'V', 'E', '1' };
	std::vector< char > buff2 { 'W', 'A', 'V', 'E', '2' };
	std::vector< char > buff3 { 'W', 'A', 'V', 'E', '3' };
	std::vector< char > buff4 { 'W', 'A', 'V', 'E', '4' };

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
		BOOST_CHECK_EQUAL( elem0->_status, eStatusUnknown );
		elem0->set( buff4 );
		checker.check( elem0 );
		BOOST_CHECK_EQUAL( elem0->_id,     node->getId() );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( elem0->next(), elem0, elem0 ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff2 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem0 ) );
		elem2->set( buff2 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem0 ) );
		elem3->set( buff3 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem0 ) );
		elem4->set( buff3 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem0 ) );
		elem5->set( buff3 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5, elem0 ) );
		elem6->set( buff1 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6, elem0 ) );
		elem7->set( buff2 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem8( new basic_element::Element( elem7->next(), elem7, elem0 ) );
		elem8->set( buff2 );
		checker.check( elem8 );
		BOOST_CHECK_EQUAL( elem8->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem8->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem9( new basic_element::Element( elem8->next(), elem8, elem0 ) );
		elem9->set( buff4 );
		checker.check( elem9 );
		BOOST_CHECK_EQUAL( elem9->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem9->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem10( new basic_element::Element( elem9->next(), elem9, elem0 ) );
		elem10->set( buff4 );
		checker.check( elem10 );
		BOOST_CHECK_EQUAL( elem10->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem10->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem11( new basic_element::Element( elem10->next(), elem10, elem0 ) );
		elem11->set( buff4 );
		checker.check( elem11 );
		BOOST_CHECK_EQUAL( elem11->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem11->_status, eStatusSkip );

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
		BOOST_CHECK_EQUAL( elem0->_status, eStatusUnknown );
		elem0->set( buff4 );
		checker.check( elem0 );
		BOOST_CHECK_EQUAL( elem0->_id,     node->getId() );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( elem0->next(), elem0, elem0 ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff2 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem0 ) );
		elem2->set( buff2 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem0 ) );
		elem3->set( buff3 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem0 ) );
		elem4->set( buff3 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem0 ) );
		elem5->set( buff3 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5, elem0 ) );
		elem6->set( buff1 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6, elem0 ) );
		elem7->set( buff4 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem8( new basic_element::Element( elem7->next(), elem7, elem0 ) );
		elem8->set( buff4 );
		checker.check( elem8 );
		BOOST_CHECK_EQUAL( elem8->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem8->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem9( new basic_element::Element( elem8->next(), elem8, elem0 ) );
		elem9->set( buff4 );
		checker.check( elem9 );
		BOOST_CHECK_EQUAL( elem9->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem9->_status, eStatusSkip );

		BOOST_CHECK( elem9->next() == nullptr );
		BOOST_CHECK_EQUAL( elem0->_status,  eStatusInvalid );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_unordered_group_and_repetitions_2 )
{
	std::string jsonStringBegin = R"*(
				{
					"content": [
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
									"repetition": )*";

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

	std::vector< char > buff1 { 'W', 'A', 'V', 'E', '1' };
	std::vector< char > buff2 { 'W', 'A', 'V', 'E', '2' };
	std::vector< char > buff3 { 'W', 'A', 'V', 'E', '3' };
	std::vector< char > buff4 { 'W', 'A', 'V', 'E', '4' };

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
		BOOST_CHECK_EQUAL( elem0->_status, eStatusUnknown );
		elem0->set( buff4 );
		checker.check( elem0 );
		BOOST_CHECK_EQUAL( elem0->_id,     node->getId() );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( elem0->next(), elem0, elem0 ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff2 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem0 ) );
		elem2->set( buff2 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem0 ) );
		elem3->set( buff3 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem0 ) );
		elem4->set( buff3 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem0 ) );
		elem5->set( buff3 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5, elem0 ) );
		elem6->set( buff1 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6, elem0 ) );
		elem7->set( buff2 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem8( new basic_element::Element( elem7->next(), elem7, elem0 ) );
		elem8->set( buff2 );
		checker.check( elem8 );
		BOOST_CHECK_EQUAL( elem8->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem8->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem9( new basic_element::Element( elem8->next(), elem8, elem0 ) );
		elem9->set( buff4 );
		checker.check( elem9 );
		BOOST_CHECK_EQUAL( elem9->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem9->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem10( new basic_element::Element( elem9->next(), elem9, elem0 ) );
		elem10->set( buff4 );
		checker.check( elem10 );
		BOOST_CHECK_EQUAL( elem10->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem10->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem11( new basic_element::Element( elem10->next(), elem10, elem0 ) );
		elem11->set( buff4 );
		checker.check( elem11 );
		BOOST_CHECK_EQUAL( elem11->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem11->_status, eStatusSkip );

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
		BOOST_CHECK_EQUAL( elem0->_status, eStatusUnknown );
		elem0->set( buff4 );
		checker.check( elem0 );
		BOOST_CHECK_EQUAL( elem0->_id,     node->getId() );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( elem0->next(), elem0, elem0 ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff2 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem0 ) );
		elem2->set( buff2 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem0 ) );
		elem3->set( buff3 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem0 ) );
		elem4->set( buff3 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem0 ) );
		elem5->set( buff3 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5, elem0 ) );
		elem6->set( buff1 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6, elem0 ) );
		elem7->set( buff2 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem8( new basic_element::Element( elem7->next(), elem7, elem0 ) );
		elem8->set( buff2 );
		checker.check( elem8 );
		BOOST_CHECK_EQUAL( elem8->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem8->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem9( new basic_element::Element( elem8->next(), elem8, elem0 ) );
		elem9->set( buff2 );
		checker.check( elem9 );
		BOOST_CHECK_EQUAL( elem9->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem9->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem10( new basic_element::Element( elem9->next(), elem9, elem0 ) );
		elem10->set( buff2 );
		checker.check( elem10 );
		BOOST_CHECK_EQUAL( elem10->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem10->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem11( new basic_element::Element( elem10->next(), elem10, elem0 ) );
		elem11->set( buff4 );
		checker.check( elem11 );
		BOOST_CHECK_EQUAL( elem11->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem11->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem12( new basic_element::Element( elem11->next(), elem11, elem0 ) );
		elem12->set( buff4 );
		checker.check( elem12 );
		BOOST_CHECK_EQUAL( elem12->_id,     node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem12->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem13( new basic_element::Element( elem12->next(), elem12, elem0 ) );
		elem13->set( buff4 );
		checker.check( elem13 );
		BOOST_CHECK_EQUAL( elem13->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem13->_status, eStatusSkip );

		BOOST_CHECK( elem13->next() == nullptr );
		BOOST_CHECK_EQUAL( elem0->_status,  eStatusInvalid );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_unordered_group_and_optional )
{
	std::string jsonStringBegin = R"*(
				{
					"content": [
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
									"values": "WAVE2" )*";

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

	std::vector< char > buff1 { 'W', 'A', 'V', 'E', '1' };
	std::vector< char > buff3 { 'W', 'A', 'V', 'E', '3' };
	std::vector< char > buff4 { 'W', 'A', 'V', 'E', '4' };

	LOG_INFO( "\n>>> element_checker_checker_unordered_group_and_optional <<<" );
	{
		std::string jsonStringRepetition = R"*( ,"optional": true )*";

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
		BOOST_CHECK_EQUAL( elem0->_status, eStatusUnknown );
		elem0->set( buff4 );
		checker.check( elem0 );
		BOOST_CHECK_EQUAL( elem0->_id,     node->getId() );
		BOOST_CHECK_EQUAL( elem0->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( elem0->next(), elem0, elem0 ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff3 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem0 ) );
		elem2->set( buff3 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_id,         node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_isOptional, true );
		BOOST_CHECK_EQUAL( elem2->_status,     eStatusSkip );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem0 ) );
		elem3->set( buff3 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem0 ) );
		elem4->set( buff1 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem0 ) );
		elem5->set( buff4 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_id,     node->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5, elem0 ) );
		elem6->set( buff4 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_id,         node->firstChild()->next()->getId() );
		BOOST_CHECK_EQUAL( elem6->_isOptional, true );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6, elem0 ) );
		elem7->set( buff4 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_id,     node->firstChild()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusSkip );

		BOOST_CHECK( elem7->next() == nullptr );
		BOOST_CHECK_EQUAL( elem0->_status,  eStatusValid );
	}
}

BOOST_AUTO_TEST_SUITE_END()
