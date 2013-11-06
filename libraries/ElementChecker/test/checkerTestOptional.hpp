
BOOST_AUTO_TEST_SUITE( element_checker_test_checker_optional )

BOOST_AUTO_TEST_CASE( element_checker_checker_optional )
{
	std::string jsonString = R"*(
			{
				"header": [
					{ "id": "value1", "label": "Value1", "type": "ascii", "values": "WAVE1" },
					{ "id": "value2", "label": "Value2", "type": "ascii", "values": "WAVE2", "optional": true },
					{ "id": "value3", "label": "Value3", "type": "ascii", "values": "WAVE3" }
				]
			}
		)*";

	const char buff1[5] { 'W', 'A', 'V', 'E', '1' };
	const char buff3[5] { 'W', 'A', 'V', 'E', '3' };

	LOG_INFO( "\n>>> element_checker_checker_optional <<<" );
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                                       "value1"  );
		BOOST_CHECK_EQUAL( node->next()->getId(),                               "value2"  );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),                       "value3"  );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		elem2->set( (const char*)&buff3, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_isOptional, true );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusInvalid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_optional_repeated )
{
	std::string jsonString = R"*(
			{
				"header": [
					{ "id": "value1", "label": "Value1", "type": "ascii", "values": "WAVE1" },
					{ "id": "value2", "label": "Value2", "type": "ascii", "values": "WAVE2", "optional": true, "repeated": "2" },
					{ "id": "value3", "label": "Value3", "type": "ascii", "values": "WAVE3" }
				]
			}
		)*";

	const char buff1[5] { 'W', 'A', 'V', 'E', '1' };
	const char buff2[5] { 'W', 'A', 'V', 'E', '2' };
	const char buff3[5] { 'W', 'A', 'V', 'E', '3' };

	LOG_INFO( "\n>>> element_checker_checker_optional_repeated <<<" );
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                                       "value1"  );
		BOOST_CHECK_EQUAL( node->next()->getId(),                               "value2"  );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),                       "value3"  );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		elem2->set( (const char*)&buff3, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_isOptional, true );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusInvalid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_checker_optional_repeated valid <<<" );
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                                       "value1"  );
		BOOST_CHECK_EQUAL( node->next()->getId(),                               "value2"  );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),                       "value3"  );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_isOptional, true );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->getId() );
		elem3->set( (const char*)&buff2, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_isOptional, true );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->next()->getId() );
		elem4->set( (const char*)&buff3, checker.getSize( elem4 ) );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_isOptional, true );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusInvalid );
		BOOST_CHECK( elem3->_error.find( "repetition" ) == std::string::npos );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4 ) );
		BOOST_CHECK_EQUAL( elem5->_id, node->next()->next()->getId() );
		elem5->set( (const char*)&buff3, checker.getSize( elem5 ) );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		BOOST_CHECK( elem5->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_checker_optional_repeated invalid <<<" );
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                                       "value1"  );
		BOOST_CHECK_EQUAL( node->next()->getId(),                               "value2"  );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),                       "value3"  );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_isOptional, true );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->getId() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_isOptional, true );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusInvalid );
		BOOST_CHECK( elem3->_error.find( "repetition" ) != std::string::npos );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->next()->next()->getId() );
		elem4->set( (const char*)&buff3, checker.getSize( elem4 ) );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		BOOST_CHECK( elem4->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_optional_group )
{
	std::string jsonStringBegin = R"*(
			{
				"header": [
					{ "id": "value1", "label": "Value1", "type": "ascii", "values": "WAVE1" },
					{ "id": "value2", "label": "Value2", "type": "ascii", "values": "WAVE2",
					)*";

	std::string jsonStringEnd = R"*(
					 "group": [
						{ "id": "value21", "label": "Value21", "type": "ascii", "values": "WAVE4" },
						{ "id": "value22", "label": "Value22", "type": "ascii", "values": "WAVE5", "optional": true },
						{ "id": "value23", "label": "Value23", "type": "ascii", "values": "WAVE6" }
					] },
					{ "id": "value3", "label": "Value3", "type": "ascii", "values": "WAVE3" }
				]
			}
		)*";

	const char buff1[5] { 'W', 'A', 'V', 'E', '1' };
	const char buff2[5] { 'W', 'A', 'V', 'E', '2' };
	const char buff3[5] { 'W', 'A', 'V', 'E', '3' };
	const char buff4[5] { 'W', 'A', 'V', 'E', '4' };
	const char buff5[5] { 'W', 'A', 'V', 'E', '5' };
	const char buff6[5] { 'W', 'A', 'V', 'E', '6' };

	LOG_INFO( "\n>>> element_checker_checker_optional_group <<<" );
	{
		std::string jsonStringOption = R"*( "optional": true, )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringOption + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                                       "value1"  );
		BOOST_CHECK_EQUAL( node->next()->getId(),                               "value2"  );
		BOOST_CHECK_EQUAL( node->next()->firstChild()->getId(),                 "value21" );
		BOOST_CHECK_EQUAL( node->next()->firstChild()->next()->getId(),         "value22" );
		BOOST_CHECK_EQUAL( node->next()->firstChild()->next()->next()->getId(), "value23" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),                       "value3"  );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		elem2->set( (const char*)&buff3, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_isOptional, true );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusInvalid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_checker_optional_group child <<<" );
	{
		std::string jsonStringOption = "";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringOption + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                                       "value1"  );
		BOOST_CHECK_EQUAL( node->next()->getId(),                               "value2"  );
		BOOST_CHECK_EQUAL( node->next()->firstChild()->getId(),                 "value21" );
		BOOST_CHECK_EQUAL( node->next()->firstChild()->next()->getId(),         "value22" );
		BOOST_CHECK_EQUAL( node->next()->firstChild()->next()->next()->getId(), "value23" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),                       "value3"  );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->firstChild()->getId() );
		elem3->set( (const char*)&buff4, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem2 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->next()->firstChild()->next()->getId() );
		elem4->set( (const char*)&buff6, checker.getSize( elem4 ) );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_isOptional, true );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusInvalid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem2 ) );
		BOOST_CHECK_EQUAL( elem5->_id, node->next()->firstChild()->next()->next()->getId() );
		elem5->set( (const char*)&buff6, checker.getSize( elem5 ) );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		BOOST_CHECK_EQUAL( elem6->_id, node->next()->next()->getId() );
		elem6->set( (const char*)&buff3, checker.getSize( elem6 ) );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		BOOST_CHECK( elem6->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_checker_optional_group repeated group <<<" );
	{
		std::string jsonStringOption = R"*( "repeated": "2", )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringOption + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                                       "value1"  );
		BOOST_CHECK_EQUAL( node->next()->getId(),                               "value2"  );
		BOOST_CHECK_EQUAL( node->next()->firstChild()->getId(),                 "value21" );
		BOOST_CHECK_EQUAL( node->next()->firstChild()->next()->getId(),         "value22" );
		BOOST_CHECK_EQUAL( node->next()->firstChild()->next()->next()->getId(), "value23" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),                       "value3"  );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->firstChild()->getId() );
		elem3->set( (const char*)&buff4, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem2 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->next()->firstChild()->next()->getId() );
		elem4->set( (const char*)&buff6, checker.getSize( elem4 ) );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_isOptional, true );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusInvalid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem2 ) );
		BOOST_CHECK_EQUAL( elem5->_id, node->next()->firstChild()->next()->next()->getId() );
		elem5->set( (const char*)&buff6, checker.getSize( elem5 ) );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		BOOST_CHECK_EQUAL( elem6->_id, node->next()->getId() );
		elem6->set( (const char*)&buff2, checker.getSize( elem6 ) );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6, elem6 ) );
		BOOST_CHECK_EQUAL( elem7->_id, node->next()->firstChild()->getId() );
		elem7->set( (const char*)&buff4, checker.getSize( elem7 ) );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem8( new basic_element::Element( elem7->next(), elem7, elem6 ) );
		BOOST_CHECK_EQUAL( elem8->_id, node->next()->firstChild()->next()->getId() );
		elem8->set( (const char*)&buff5, checker.getSize( elem8 ) );
		checker.check( elem8 );
		BOOST_CHECK_EQUAL( elem8->_isOptional, true );
		BOOST_CHECK_EQUAL( elem8->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem9( new basic_element::Element( elem8->next(), elem8, elem6 ) );
		BOOST_CHECK_EQUAL( elem9->_id, node->next()->firstChild()->next()->next()->getId() );
		elem9->set( (const char*)&buff6, checker.getSize( elem9 ) );
		checker.check( elem9 );
		BOOST_CHECK_EQUAL( elem9->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem10( new basic_element::Element( elem9->next(), elem9 ) );
		BOOST_CHECK_EQUAL( elem10->_id, node->next()->getId() );
		elem10->set( (const char*)&buff3, checker.getSize( elem10 ) );
		checker.check( elem10 );
		BOOST_CHECK_EQUAL( elem10->_status, eStatusInvalid );

		std::shared_ptr< basic_element::Element > elem11( new basic_element::Element( elem10->next(), elem10 ) );
		BOOST_CHECK_EQUAL( elem11->_id, node->next()->next()->getId() );
		elem11->set( (const char*)&buff3, checker.getSize( elem11 ) );
		checker.check( elem11 );
		BOOST_CHECK_EQUAL( elem11->_status, eStatusValid );

		BOOST_CHECK( elem11->next() == nullptr );
	}
}

BOOST_AUTO_TEST_SUITE_END()
