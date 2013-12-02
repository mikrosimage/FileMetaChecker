
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
							"count": )*";

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

	const char buff1[4] { 0x00, 0x00, 0x00, 0x05 };
	const char buff2[5] { 'W', 'A', 'V', 'E', '2' };
	const char buff3[5] { 'W', 'A', 'V', 'E', '3' };

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
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		BOOST_CHECK_EQUAL( elem1->_size,             4 );
		BOOST_CHECK_EQUAL( checker.getSize( elem1 ), 4 );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusPassOver );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_size,             0 );
		BOOST_CHECK_EQUAL( checker.getSize( elem2 ), 5 );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_size,   5 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusPassOver );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_size,             5 );
		BOOST_CHECK_EQUAL( checker.getSize( elem3 ), 5 );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
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
		BOOST_CHECK_EQUAL( elem1->_status, eStatusNotChecked );
		BOOST_CHECK_EQUAL( elem1->_size,             4 );
		BOOST_CHECK_EQUAL( checker.getSize( elem1 ), 4 );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusPassOver );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_size,             0 );
		BOOST_CHECK_EQUAL( checker.getSize( elem2 ), 3 );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_size,   3 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusPassOver );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_size,             5 );
		BOOST_CHECK_EQUAL( checker.getSize( elem3 ), 5 );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
}

BOOST_AUTO_TEST_SUITE_END()
