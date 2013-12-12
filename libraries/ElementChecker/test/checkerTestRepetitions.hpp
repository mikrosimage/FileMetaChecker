
BOOST_AUTO_TEST_SUITE( element_checker_test_checker_repetition )

BOOST_AUTO_TEST_CASE( element_checker_checker_repetition )
{
	std::string jsonStringBegin = R"*(
				{
					"content": [
						{
							"id": "value1",
							"label": "Value1",
							"type": "ascii",
							"values": "WAVE1"
						},
						{
							"id": "value2",
							"label": "Value2",
							"type": "ascii",
							"values": "WAVE2",
							"repetition": )*";
						

	std::string jsonStringEnd = R"*(
						},
						{
							"id": "value3",
							"label": "Value3",
							"type": "ascii",
							"values": "WAVE3"
						}
					]
				}
			)*";

	std::vector< char > buff1 { 'W', 'A', 'V', 'E', '1' };
	std::vector< char > buff2 { 'W', 'A', 'V', 'E', '2' };
	std::vector< char > buff3 { 'W', 'A', 'V', 'E', '3' };

	LOG_INFO( "\n>>> element_checker_checker_repetition <<<" );
	{
		std::string jsonStringRepetition = R"*("3")*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringRepetition + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),         "value3" );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( buff2, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( buff2, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( buff3, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusSkip );
		BOOST_CHECK( ! elem5->_error.empty() );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		elem6->set( buff3, 5 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		BOOST_CHECK( elem6->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_checker_repetition suite <<<" );
	{
		std::string jsonStringRepetition = R"*( [
									{ "min": "2" }
								] )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringRepetition + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),         "value3" );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( buff2, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( buff2, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( buff3, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusSkip );
		BOOST_CHECK( ! elem5->_error.empty() );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		elem6->set( buff3, 5 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		BOOST_CHECK( elem6->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_checker_repetition suite 2 <<<" );
	{
		std::string jsonStringRepetition = R"*( [
									{ "min": "2", "max": "4" }
								] )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringRepetition + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),         "value3" );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( buff2, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( buff2, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( buff3, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusSkip );
		BOOST_CHECK( ! elem5->_error.empty() );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		elem6->set( buff3, 5 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		BOOST_CHECK( elem6->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_checker_repetition suite 3 <<<" );
	{
		std::string jsonStringRepetition = R"*( [
									{ "max": "4" }
								] )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringRepetition + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),         "value3" );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( buff2, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( buff2, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( buff3, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusSkip );
		BOOST_CHECK( ! elem5->_error.empty() );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		elem6->set( buff3, 5 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		BOOST_CHECK( elem6->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_repetition_limited )
{
	std::string jsonStringBegin = R"*(
				{
					"content": [
						{
							"id": "value1",
							"label": "Value1",
							"type": "uint16",
							"repetition": )*";
						

	std::string jsonStringEnd = R"*(
						},
						{
							"id": "valueEnd",
							"label": "Value End",
							"type": "ascii",
							"values": "END"
						}
					]
				}
			)*";

	std::vector< char > buff1 { 0x00, 0x01 };
	std::vector< char > buff2 { 0x00, 0x02 };
	std::vector< char > buff3 { 0x00, 0x03 };
	std::vector< char > buff4 { 'E', 'N', 'D' };

	LOG_INFO( "\n>>> element_checker_checker_repetition_limited <<<" );
	{
		std::string jsonStringRepetition = R"*("3")*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringRepetition + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "valueEnd" );
		BOOST_CHECK( node->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff1, 2 );
		checker.check( elem1 );
		LOG_FATAL( elem1->_id << " | " << typeStringMap.at( elem1->_type ) << " | " << statusMap.at( elem1->_status ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( buff2, 2 );
		checker.check( elem2 );
		LOG_FATAL( elem2->_id << " | " << typeStringMap.at( elem2->_type ) << " | " << statusMap.at( elem2->_status ) );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( buff3, 2 );
		checker.check( elem3 );
		LOG_FATAL( elem3->_id << " | " << typeStringMap.at( elem3->_type ) << " | " << statusMap.at( elem3->_status ) );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( buff4, 2 );
		checker.check( elem4 );
		LOG_FATAL( elem4->_id << " | " << typeStringMap.at( elem4->_type ) << " | " << statusMap.at( elem4->_status ) );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusSkip );
		BOOST_CHECK( elem4->_error.empty() );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( buff4, 3 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );
		LOG_FATAL( elem5->_id << " | " << typeStringMap.at( elem5->_type ) << " | " << statusMap.at( elem5->_status ) );

		BOOST_CHECK( elem5->next() == nullptr );
	}
		LOG_INFO( "\n>>> element_checker_checker_repetition_limited suite <<<" );
	{
		std::string jsonStringRepetition = R"*( [
									1, 2, 3
								] )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringRepetition + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "valueEnd" );
		BOOST_CHECK( node->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff1, 2 );
		checker.check( elem1 );
		LOG_FATAL( elem1->_id << " | " << typeStringMap.at( elem1->_type ) << " | " << statusMap.at( elem1->_status ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( buff2, 2 );
		checker.check( elem2 );
		LOG_FATAL( elem2->_id << " | " << typeStringMap.at( elem2->_type ) << " | " << statusMap.at( elem2->_status ) );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( buff3, 2 );
		checker.check( elem3 );
		LOG_FATAL( elem3->_id << " | " << typeStringMap.at( elem3->_type ) << " | " << statusMap.at( elem3->_status ) );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( buff4, 2 );
		checker.check( elem4 );
		LOG_FATAL( elem4->_id << " | " << typeStringMap.at( elem4->_type ) << " | " << statusMap.at( elem4->_status ) );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusSkip );
		BOOST_CHECK( elem4->_error.empty() );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( buff4, 3 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );
		LOG_FATAL( elem5->_id << " | " << typeStringMap.at( elem5->_type ) << " | " << statusMap.at( elem5->_status ) );

		BOOST_CHECK( elem5->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_repetition_invalid )
{
	std::string jsonStringBegin = R"*(
				{
					"content": [
						{
							"id": "value1",
							"label": "Value1",
							"type": "ascii",
							"values": "WAVE1"
						},
						{
							"id": "value2",
							"label": "Value2",
							"type": "ascii",
							"values": "WAVE2",
							"repetition": )*";
						

	std::string jsonStringEnd = R"*(
						},
						{
							"id": "value3",
							"label": "Value3",
							"type": "ascii",
							"values": "WAVE3"
						}
					]
				}
			)*";

	std::vector< char > buff1 { 'W', 'A', 'V', 'E', '1' };
	std::vector< char > buff2 { 'W', 'A', 'V', 'E', '2' };
	std::vector< char > buff3 { 'W', 'A', 'V', 'E', '3' };

	LOG_INFO( "\n>>> element_checker_checker_repetition_invalid <<<" );
	{
		std::string jsonStringRepetition = R"*("3")*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringRepetition + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),         "value3" );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;
		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( buff2, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( buff3, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusInvalid );
		BOOST_CHECK( ! elem4->_error.empty() );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( buff3, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		BOOST_CHECK( elem5->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_checker_repetition_invalid suite <<<" );
	{
		std::string jsonStringRepetition = R"*( [
									{ "min": "2" }
								] )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringRepetition + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),         "value3" );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( buff3, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusInvalid );
		BOOST_CHECK( ! elem3->_error.empty() );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( buff3, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		BOOST_CHECK( elem4->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_checker_repetition_invalid suite 2 <<<" );
	{
		std::string jsonStringRepetition = R"*( [
									{ "min": "2", "max": "4" }
								] )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringRepetition + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),         "value3" );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( buff2, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( buff2, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( buff2, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		elem6->set( buff2, 5 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6 ) );
		elem7->set( buff3, 5 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusInvalid );
		BOOST_CHECK( ! elem7->_error.empty() );

		std::shared_ptr< basic_element::Element > elem8( new basic_element::Element( elem7->next(), elem7 ) );
		elem8->set( buff3, 5 );
		checker.check( elem8 );
		BOOST_CHECK_EQUAL( elem8->_status, eStatusValid );

		BOOST_CHECK( elem8->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_checker_repetition_invalid suite 3 <<<" );
	{
		std::string jsonStringRepetition = R"*( [
									{ "max": "4" }
								] )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringRepetition + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),         "value3" );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		elem1->set( buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( buff2, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( buff2, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( buff2, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		elem6->set( buff2, 5 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6 ) );
		elem7->set( buff3, 5 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusInvalid );
		BOOST_CHECK( ! elem7->_error.empty() );

		std::shared_ptr< basic_element::Element > elem8( new basic_element::Element( elem7->next(), elem7 ) );
		elem8->set( buff3, 5 );
		checker.check( elem8 );
		BOOST_CHECK_EQUAL( elem8->_status, eStatusValid );

		BOOST_CHECK( elem8->next() == nullptr );
	}
}

BOOST_AUTO_TEST_SUITE_END()
