
BOOST_AUTO_TEST_SUITE( element_checker_test_checker_count )

BOOST_AUTO_TEST_CASE( element_checker_checker_count )
{
	std::string jsonStringBegin = R"*(
				{
					"content": [
						{
							"id": "value1",
							"label": "Value1",
							"type": "uint32"
						},
						{
							"id": "value2",
							"label": "Value2",
							"type": "raw",
							"size": )*";

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

	std::vector< char > buff1 { 0x00, 0x00, 0x00, 0x05 };
	std::vector< char > buff2 { 'W', 'A', 'V', 'E', '2' };
	std::vector< char > buff3 { 'W', 'A', 'V', 'E', '3' };

	LOG_INFO( "\n>>> element_checker_checker_count <<<" );
	{
		std::string jsonStringCount = R"*( "5" )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringCount + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),         "value3" );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		BOOST_CHECK_EQUAL( checker.getSize( elem1 ), 4 );
		elem1->set( buff1, checker.getSize( elem1 ) );
		BOOST_CHECK_EQUAL( elem1->_data.size(),      4 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_data.size(),      0 );
		BOOST_CHECK_EQUAL( checker.getSize( elem2 ), 5 );
		elem2->set( buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_data.size(),      5 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( checker.getSize( elem3 ), 5 );
		elem3->set( buff3, checker.getSize( elem3 ) );
		BOOST_CHECK_EQUAL( elem3->_data.size(),      5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_checker_count suite <<<" );
	{
		std::string jsonStringCount = R"*( "value1 - 2" )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringCount + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),         "value3" );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusUnknown );
		BOOST_CHECK_EQUAL( checker.getSize( elem1 ), 4 );
		elem1->set( buff1, checker.getSize( elem1 ) );
		BOOST_CHECK_EQUAL( elem1->_data.size(),      4 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_data.size(),      0 );
		BOOST_CHECK_EQUAL( checker.getSize( elem2 ), 3 );
		elem2->set( buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_data.size(),      3 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( checker.getSize( elem3 ), 5 );
		elem3->set( buff3, checker.getSize( elem3 ) );
		BOOST_CHECK_EQUAL( elem3->_data.size(),      5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
}

BOOST_AUTO_TEST_SUITE_END()
