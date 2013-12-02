
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
							"repeated": )*";
						

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

	const char buff1[5] { 'W', 'A', 'V', 'E', '1' };
	const char buff2[5] { 'W', 'A', 'V', 'E', '2' };
	const char buff3[5] { 'W', 'A', 'V', 'E', '3' };

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
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		elem1->set( (const char*)&buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( (const char*)&buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( (const char*)&buff2, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( (const char*)&buff2, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( (const char*)&buff3, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusInvalidButSkip );
		BOOST_CHECK( elem5->_error.find( "repetition" ) == std::string::npos );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		elem6->set( (const char*)&buff3, 5 );
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
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		elem1->set( (const char*)&buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( (const char*)&buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( (const char*)&buff2, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( (const char*)&buff2, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( (const char*)&buff3, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusInvalidButSkip );
		BOOST_CHECK( elem5->_error.find( "repetition" ) == std::string::npos );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		elem6->set( (const char*)&buff3, 5 );
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
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		elem1->set( (const char*)&buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( (const char*)&buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( (const char*)&buff2, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( (const char*)&buff2, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( (const char*)&buff3, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusInvalidButSkip );
		BOOST_CHECK( elem5->_error.find( "repetition" ) == std::string::npos );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		elem6->set( (const char*)&buff3, 5 );
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
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		elem1->set( (const char*)&buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( (const char*)&buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( (const char*)&buff2, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( (const char*)&buff2, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( (const char*)&buff3, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusInvalidButSkip );
		BOOST_CHECK( elem5->_error.find( "repetition" ) == std::string::npos );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		elem6->set( (const char*)&buff3, 5 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		BOOST_CHECK( elem6->next() == nullptr );
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
							"repeated": )*";
						

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

	const char buff1[5] { 'W', 'A', 'V', 'E', '1' };
	const char buff2[5] { 'W', 'A', 'V', 'E', '2' };
	const char buff3[5] { 'W', 'A', 'V', 'E', '3' };

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
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		elem1->set( (const char*)&buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( (const char*)&buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( (const char*)&buff2, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( (const char*)&buff3, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusInvalidForIteration );
		BOOST_CHECK( elem4->_error.find( "repetition" ) != std::string::npos );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( (const char*)&buff3, 5 );
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
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		elem1->set( (const char*)&buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( (const char*)&buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( (const char*)&buff3, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusInvalidForIteration );
		BOOST_CHECK( elem3->_error.find( "repetition" ) != std::string::npos );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( (const char*)&buff3, 5 );
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
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		elem1->set( (const char*)&buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( (const char*)&buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( (const char*)&buff2, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( (const char*)&buff2, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( (const char*)&buff2, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		elem6->set( (const char*)&buff2, 5 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6 ) );
		elem7->set( (const char*)&buff3, 5 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusInvalidForIteration );
		BOOST_CHECK( elem7->_error.find( "repetition" ) != std::string::npos );

		std::shared_ptr< basic_element::Element > elem8( new basic_element::Element( elem7->next(), elem7 ) );
		elem8->set( (const char*)&buff3, 5 );
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
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		elem1->set( (const char*)&buff1, 5 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		elem2->set( (const char*)&buff2, 5 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		elem3->set( (const char*)&buff2, 5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		elem4->set( (const char*)&buff2, 5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		elem5->set( (const char*)&buff2, 5 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		elem6->set( (const char*)&buff2, 5 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6 ) );
		elem7->set( (const char*)&buff3, 5 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusInvalidForIteration );
		BOOST_CHECK( elem7->_error.find( "repetition" ) != std::string::npos );

		std::shared_ptr< basic_element::Element > elem8( new basic_element::Element( elem7->next(), elem7 ) );
		elem8->set( (const char*)&buff3, 5 );
		checker.check( elem8 );
		BOOST_CHECK_EQUAL( elem8->_status, eStatusValid );

		BOOST_CHECK( elem8->next() == nullptr );
	}
}

BOOST_AUTO_TEST_SUITE_END()
