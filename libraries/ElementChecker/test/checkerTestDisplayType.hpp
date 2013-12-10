
BOOST_AUTO_TEST_SUITE( element_checker_test_checker_display_type )

BOOST_AUTO_TEST_CASE( element_checker_test_checker_display_type )
{
	std::string jsonString = R"*(
			{
				"content": [
					{ "id": "value1", "label": "Value1", "type": "raw", "size": "5", "display": "ascii" },
					{ "id": "value2", "label": "Value2", "type": "raw", "size": "5", "display": "uint8" },
					{ "id": "value3", "label": "Value3", "type": "raw", "size": "3" },
					{ "id": "value4", "label": "Value4", "type": "ascii", "values": "WAVE4" }
				]
			}
		)*";

	std::vector< char > buff1 { 'W', 'A', 'V', 'E', '1' };
	std::vector< char > buff2 { 'W', 'A', 'V', 'E', '2' };
	std::vector< char > buff3 { 'W', 'A', 'V', 'E', '3' };
	std::vector< char > buff4 { 'W', 'A', 'V', 'E', '4' };

	LOG_INFO( "\n>>> element_checker_test_checker_display_type <<<" );
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1"  );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2"  );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),         "value3"  );
		BOOST_CHECK_EQUAL( node->next()->next()->next()->getId(), "value4"  );
		BOOST_CHECK( node->next()->next()->next()->next() == nullptr );

		Checker checker;
	
		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		BOOST_CHECK_EQUAL( elem1->_displayType, eDisplayTypeAscii );
		elem1->set( buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		BOOST_CHECK_EQUAL( elem1->_dispValue, "WAVE1" );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_displayType, eDisplayTypeNumbers );
		elem2->set( buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusUnknown );
		BOOST_CHECK_EQUAL( elem2->_dispValue, "8765866950" );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem3->_displayType, eDisplayTypeDefault );
		elem3->set( buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusUnknown );
		BOOST_CHECK_EQUAL( elem3->_dispValue, "WAV" );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->next()->next()->next()->getId() );
		BOOST_CHECK_EQUAL( elem4->_displayType, eDisplayTypeDefault );
		elem4->set( buff4, checker.getSize( elem4 ) );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );
		BOOST_CHECK_EQUAL( elem4->_dispValue, "WAVE4" );

		BOOST_CHECK( elem4->next() == nullptr );
	}
}

BOOST_AUTO_TEST_SUITE_END()
