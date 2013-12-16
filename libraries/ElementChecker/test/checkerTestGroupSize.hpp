
BOOST_AUTO_TEST_SUITE( element_checker_test_checker_groupSize )

BOOST_AUTO_TEST_CASE( element_checker_checker_groupSize )
{
	LOG_INFO( "\n>>> element_checker_checker_groupSize <<<" );
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
							{ "id": "value23", "label": "Value23", "type": "ascii", "values": "WAVE6" },
							{ "id": "value24", "label": "Value24", "type": "ascii", "values": "WAVE7" }
						] },
						{ "id": "value3", "label": "Value3", "type": "ascii", "values": "WAVE3" }
					]
				}
			)*";

		std::vector< char > buff1 { 0x00, 0x00, 0x00, 0x14 };
		std::vector< char > buff2 { 'W', 'A', 'V', 'E', '2' };
		std::vector< char > buff3 { 'W', 'A', 'V', 'E', '3' };
		std::vector< char > buff4 { 'W', 'A', 'V', 'E', '4' };
		std::vector< char > buff5 { 'W', 'A', 'V', 'E', '5' };
		std::vector< char > buff6 { 'W', 'A', 'V', 'E', '6' };
		std::vector< char > buff7 { 'W', 'A', 'V', 'E', '7' };
		
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
		elem1->set( buff1 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_groupSizeExpr, node->getId() );
		elem2->set( buff2 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->firstChild()->getId() );
		elem3->set( buff4 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem2 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->next()->firstChild()->next()->getId() );
		elem4->set( buff5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem2 ) );
		BOOST_CHECK_EQUAL( elem5->_id, node->next()->firstChild()->next()->next()->getId() );
		elem5->set( buff6 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5, elem2 ) );
		BOOST_CHECK_EQUAL( elem6->_id, node->next()->firstChild()->next()->next()->next()->getId() );
		elem6->set( buff7 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem7( new basic_element::Element( elem6->next(), elem6 ) );
		BOOST_CHECK_EQUAL( elem7->_id, node->next()->next()->getId() );
		elem7->set( buff3 );
		checker.check( elem7 );
		BOOST_CHECK_EQUAL( elem7->_status, eStatusValid );

		BOOST_CHECK( elem7->next() == nullptr );
		BOOST_CHECK_EQUAL( elem2->_specGroupSize, 20 );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_groupSize_2 )
{
	LOG_INFO( "\n>>> element_checker_checker_groupSize_2 <<<" );
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

		std::vector< char > buff1 { 'W', 'A', 'V', 'E', '1' };
		std::vector< char > buff2 { 'W', 'A', 'V', 'E', '2' };
		std::vector< char > buff3 { 'W', 'A', 'V', 'E', '3' };
		std::vector< char > buff4 { 0x00, 0x00, 0x00, 0x0e };
		std::vector< char > buff5 { 'W', 'A', 'V', 'E', '5' };
		std::vector< char > buff6 { 'W', 'A', 'V', 'E', '6' };
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
		elem1->set( buff1 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_groupSizeExpr, node->next()->firstChild()->getId() );
		elem2->set( buff2 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->firstChild()->getId() );
		elem3->set( buff4 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );
		BOOST_CHECK_EQUAL( elem3->_dispValue, "14" );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3, elem2 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->next()->firstChild()->next()->getId() );
		elem4->set( buff5 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem5( new basic_element::Element( elem4->next(), elem4, elem2 ) );
		BOOST_CHECK_EQUAL( elem5->_id, node->next()->firstChild()->next()->next()->getId() );
		elem5->set( buff6 );
		checker.check( elem5 );
		BOOST_CHECK_EQUAL( elem5->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		BOOST_CHECK_EQUAL( elem6->_id, node->next()->next()->getId() );
		elem6->set( buff3 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		BOOST_CHECK( elem6->next() == nullptr );
		BOOST_CHECK_EQUAL( elem2->_specGroupSize, 14 );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_groupSize_3 )
{
	LOG_INFO( "\n>>> element_checker_checker_groupSize_3 <<<" );
	{
		std::string jsonString = R"*(
				{
					"content": [
						{ "id": "value1", "label": "Value1", "type": "ascii", "values": "WAVE1" },
						{ "id": "value2", "label": "Value2", "type": "ascii", "values": "WAVE2",
						  "groupSize": "value21",
						  "group": [
							{ "id": "value21", "label": "size of value2", "type": "uint32" }
						] },
						{ "id": "value3", "label": "Value3", "type": "ascii", "values": "WAVE3" }
					]
				}
			)*";

		std::vector< char > buff1 { 'W', 'A', 'V', 'E', '1' };
		std::vector< char > buff2 { 'W', 'A', 'V', 'E', '2' };
		std::vector< char > buff3 { 'W', 'A', 'V', 'E', '3' };
		std::vector< char > buff4 { 0x00, 0x00, 0x00, 0x04 };

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
		elem1->set( buff1 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_groupSizeExpr, node->next()->firstChild()->getId() );
		elem2->set( buff2 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->firstChild()->getId() );
		elem3->set( buff4 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );
		BOOST_CHECK_EQUAL( elem3->_dispValue, "4" );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->next()->next()->getId() );
		elem4->set( buff3 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		BOOST_CHECK( elem4->next() == nullptr );
		BOOST_CHECK_EQUAL( elem2->_specGroupSize, 4 );
	}
}

BOOST_AUTO_TEST_SUITE_END()
