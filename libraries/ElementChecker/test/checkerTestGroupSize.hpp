
BOOST_AUTO_TEST_SUITE( element_checker_test_checker_groupSize )

BOOST_AUTO_TEST_CASE( element_checker_checker_groupSize )
{
	std::string jsonString = R"*(
			{
				"content": [
					{ "id": "value1", "label": "size of value2", "type": "uint32" },
					{ "id": "value2", "label": "Value2", "type": "ascii", "values": "WAVE2",
					  "groupSize": "value1",
					  "group": [
						{ "id": "value21", "label": "Value21", "type": "ascii", "values": "WAVE4" },
						{ "id": "value22", "label": "Value22", "type": "ascii", "values": "WAVE5" },
						{ "id": "value23", "label": "Value23", "type": "ascii", "values": "WAVE6" }
					] },
					{ "id": "value3", "label": "Value3", "type": "ascii", "values": "WAVE3" }
				]
			}
		)*";

	const char buff2[5] { 'W', 'A', 'V', 'E', '2' };
	const char buff3[5] { 'W', 'A', 'V', 'E', '3' };
	const char buff4[5] { 'W', 'A', 'V', 'E', '4' };
	const char buff5[5] { 'W', 'A', 'V', 'E', '5' };
	const char buff6[5] { 'W', 'A', 'V', 'E', '6' };


	LOG_INFO( "\n>>> element_checker_checker_groupSize <<<" );
	{
		const char buff1[4] { 0x00, 0x00, 0x00, 0x14 };
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
		BOOST_CHECK_EQUAL( elem1->_status, eStatusPassOver );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_groupSizeExpr, node->getId() );
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
		elem4->set( (const char*)&buff5, checker.getSize( elem4 ) );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

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
		BOOST_CHECK_EQUAL( elem2->_groupSize, 20 );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_groupSize_2 )
{
	std::string jsonString = R"*(
			{
				"content": [
					{ "id": "value1", "label": "Value1", "type": "ascii", "values": "WAVE1" },
					{ "id": "value2", "label": "Value2", "type": "ascii", "values": "WAVE2",
					  "groupSize": "value21",
					  "group": [
						{ "id": "value21", "label": "size of value2", "type": "uint32" },
						{ "id": "value22", "label": "Value22", "type": "ascii", "values": "WAVE5" },
						{ "id": "value23", "label": "Value23", "type": "ascii", "values": "WAVE6" }
					] },
					{ "id": "value3", "label": "Value3", "type": "ascii", "values": "WAVE3" }
				]
			}
		)*";

	const char buff1[5] { 'W', 'A', 'V', 'E', '1' };
	const char buff2[5] { 'W', 'A', 'V', 'E', '2' };
	const char buff3[5] { 'W', 'A', 'V', 'E', '3' };
	const char buff5[5] { 'W', 'A', 'V', 'E', '5' };
	const char buff6[5] { 'W', 'A', 'V', 'E', '6' };


	LOG_INFO( "\n>>> element_checker_checker_groupSize_2 <<<" );
	{
		const char buff4[4] { 0x00, 0x00, 0x00, 0x14 };
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
		BOOST_CHECK_EQUAL( elem2->_groupSizeExpr, node->next()->firstChild()->getId() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->firstChild()->getId() );
		elem3->set( (const char*)&buff4, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusPassOver );
		BOOST_CHECK_EQUAL( elem3->_dispValue, "20" );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem2 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->next()->firstChild()->next()->getId() );
		elem4->set( (const char*)&buff5, checker.getSize( elem4 ) );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

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
		BOOST_CHECK_EQUAL( elem2->_groupSize, 20 );
	}
}

// @todo : test with the group size value in one child !!!!!

BOOST_AUTO_TEST_SUITE_END()
