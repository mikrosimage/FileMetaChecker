
BOOST_AUTO_TEST_SUITE( element_checker_test_checker_map )

BOOST_AUTO_TEST_CASE( element_checker_test_checker_map )
{
	std::string jsonString = R"*(
			{
				"header": [
					{ "id": "value1", "label": "Value1", "type": "ascii", "values": "WAVE1" },
					{ "id": "value2", "label": "Value2", "type": "uint32", "map": [
						{ "1": "First Label"  },
						{ "2": "Second Label" },
						{ "3": "Third Label"  }
					] },
					{ "id": "value3", "label": "Value3", "type": "ascii", "values": "WAVE3" }
				]
			}
		)*";

	const char buff1[5] { 'W', 'A', 'V', 'E', '1' };
	const char buff3[5] { 'W', 'A', 'V', 'E', '3' };

	spec_reader::Specification spec;
	spec.setFromString( jsonString );
	std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
	BOOST_CHECK_EQUAL( node->getId(),                                       "value1"  );
	BOOST_CHECK_EQUAL( node->next()->getId(),                               "value2"  );
	BOOST_CHECK_EQUAL( node->next()->next()->getId(),                       "value3"  );
	BOOST_CHECK( node->next()->next()->next() == nullptr );

	Checker checker;

	LOG_INFO( "\n>>> element_checker_test_checker_map 1 <<<" );
	{
		const char buff2[4] { 0x00, 0x00, 0x00, 0x01 };
	
		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		BOOST_CHECK( elem1->_map.empty() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK( ! elem2->_map.empty() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusPassOver );
		BOOST_CHECK_EQUAL( elem2->_mapValue, "First Label" );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		BOOST_CHECK( elem3->_map.empty() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_test_checker_map 2 <<<" );
	{
		const char buff2[4] { 0x00, 0x00, 0x00, 0x02 };
			
		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		BOOST_CHECK( elem1->_map.empty() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK( ! elem2->_map.empty() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusPassOver );
		BOOST_CHECK_EQUAL( elem2->_mapValue, "Second Label" );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		BOOST_CHECK( elem3->_map.empty() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_test_checker_map 3 <<<" );
	{
		const char buff2[4] { 0x00, 0x00, 0x00, 0x03 };

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		BOOST_CHECK( elem1->_map.empty() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK( ! elem2->_map.empty() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusPassOver );
		BOOST_CHECK_EQUAL( elem2->_mapValue, "Third Label" );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		BOOST_CHECK( elem3->_map.empty() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_test_checker_map invalid <<<" );
	{
		const char buff2[4] { 0x00, 0x00, 0x00, 0x04 };

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		BOOST_CHECK( elem1->_map.empty() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK( ! elem2->_map.empty() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusPassOver );
		BOOST_CHECK( elem2->_mapValue.empty() );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		BOOST_CHECK( elem3->_map.empty() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_test_checker_map_hexa )
{
	std::string jsonString = R"*(
			{
				"header": [
					{ "id": "value1", "label": "Value1", "type": "ascii", "values": "WAVE1" },
					{ "id": "value2", "label": "Value2", "type": "hexa", "count": "2", "map": [
						{ "0123": "First Label"  },
						{ "89ab": "Second Label" },
						{ "cdef": "Third Label"  }
					] },
					{ "id": "value3", "label": "Value3", "type": "ascii", "values": "WAVE3" }
				]
			}
		)*";

	const char buff1[5] { 'W', 'A', 'V', 'E', '1' };
	const char buff3[5] { 'W', 'A', 'V', 'E', '3' };

	spec_reader::Specification spec;
	spec.setFromString( jsonString );
	std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
	BOOST_CHECK_EQUAL( node->getId(),                                       "value1"  );
	BOOST_CHECK_EQUAL( node->next()->getId(),                               "value2"  );
	BOOST_CHECK_EQUAL( node->next()->next()->getId(),                       "value3"  );
	BOOST_CHECK( node->next()->next()->next() == nullptr );

	Checker checker;

	LOG_INFO( "\n>>> element_checker_test_checker_map_hexa 1 <<<" );
	{
		const char buff2[2] { 0x01, 0x23 };
	
		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		BOOST_CHECK( elem1->_map.empty() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK( ! elem2->_map.empty() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusPassOver );
		BOOST_CHECK_EQUAL( elem2->_mapValue, "First Label" );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		BOOST_CHECK( elem3->_map.empty() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_test_checker_map_hexa 2 <<<" );
	{
		const char buff2[2] { 0x89, 0xAB };
			
		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		BOOST_CHECK( elem1->_map.empty() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK( ! elem2->_map.empty() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusPassOver );
		BOOST_CHECK_EQUAL( elem2->_mapValue, "Second Label" );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		BOOST_CHECK( elem3->_map.empty() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_test_checker_map_hexa 3 <<<" );
	{
		const char buff2[2] { 0xcd, 0xEF };

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		BOOST_CHECK( elem1->_map.empty() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK( ! elem2->_map.empty() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusPassOver );
		BOOST_CHECK_EQUAL( elem2->_mapValue, "Third Label" );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		BOOST_CHECK( elem3->_map.empty() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_test_checker_map_hexa invalid <<<" );
	{
		const char buff2[2] { 0x00, 0x00 };

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		BOOST_CHECK( elem1->_map.empty() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK( ! elem2->_map.empty() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusInvalid );
		BOOST_CHECK( elem2->_mapValue.empty() );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		BOOST_CHECK( elem3->_map.empty() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
}

BOOST_AUTO_TEST_SUITE_END()
