
BOOST_AUTO_TEST_SUITE( element_checker_test_checker )

BOOST_AUTO_TEST_CASE( element_checker_checker_data )
{
	LOG_INFO( ">>> element_checker_checker_data <<<" );
	
	std::string jsonStringBegin = R"*(
				{
					"content": [
						{ 
							"id": "id",
							"label": "label",
							"type": "ascii"
			)*";

	std::string jsonStringEnd = R"*(
						}
					]
				}
			)*";

	Checker checker;
	// {
	// 	spec_reader::Specification spec;
	// 	spec.setFromString( jsonStringBegin + jsonStringEnd );
	// 	std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

	// 	std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
	// 	std::vector< char > buff { 'W', 'A', 'V', 'E' };
	// 	elem->set( buff );

	// 	checker.check( elem );
	// 	BOOST_CHECK_EQUAL( elem->_status, eStatusPassOver );	// @todo: support WORD types (end char = null)
	// 	BOOST_CHECK( node->next() == nullptr );
	// 	BOOST_CHECK( elem->next() == nullptr );
	// }
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"values": "WAVE" )*" + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		std::vector< char > buff { 'W', 'A', 'V', 'E' };
		elem->set( buff );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_status, eStatusValid );
		BOOST_CHECK( node->next() == nullptr );
		BOOST_CHECK( elem->next() == nullptr );
	}
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"values": "wave" )*" + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		std::vector< char > buff { 'W', 'A', 'V', 'E' };
		elem->set( buff );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_status, eStatusValid );
		BOOST_CHECK( node->next() == nullptr );
		BOOST_CHECK( elem->next() == nullptr );
	}
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"values": "evaw" )*"  + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		std::vector< char > buff { 'W', 'A', 'V', 'E' };
		elem->set( buff );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_status, eStatusInvalid );
		BOOST_CHECK( ! elem->_error.empty() );
		BOOST_CHECK( node->next() == nullptr );
		BOOST_CHECK( elem->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_number )
{
	LOG_INFO( ">>> element_checker_checker_number <<<" );
	
	std::string jsonStringBegin = R"*(
				{
					"content": [
						{ 
							"id": "id",
							"label": "label",
							"type": "uint32"
			)*";

	std::string jsonStringEnd = R"*(
						}
					]
				}
			)*";

	Checker checker;
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		std::vector< char > buff { 0x00, 0x00, 0x00, 0x01 };
		elem->set( buff );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_type, eTypeUInt32 );
		BOOST_CHECK_EQUAL( elem->_status, eStatusValid );
	}
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"range": [ { "min": "1", "max": "10" } ] )*"  + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		std::vector< char > buff { 0x00, 0x00, 0x00, 0x05 };
		elem->set( buff );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_type, eTypeUInt32 );
		BOOST_CHECK_EQUAL( elem->_status, eStatusValid );
	}

	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"endian":"little","range": [ { "min": "1", "max": "10" } ] )*"  + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		std::vector< char > buff { 0x05, 0x00, 0x00, 0x00 };
		elem->set( buff );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_type, eTypeUInt32 );
		BOOST_CHECK_EQUAL( elem->_status, eStatusValid );
	}

	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"range": [ { "max": "5" }, { "min": "10" } ] )*"  + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		std::vector< char > buff { 0x00, 0x00, 0x00, 0x08 };
		elem->set( buff );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_type, eTypeUInt32 );
		BOOST_CHECK_EQUAL( elem->_status, eStatusInvalid );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_group )
{
	std::string jsonString = R"*(
			{
				"content": [
					{ "id": "value1", "label": "Value1", "type": "ascii", "values": "WAVE1", "group": [
						{ "id": "value11", "label": "Value11", "type": "ascii", "values": "WAVE2" },
						{ "id": "value12", "label": "Value12", "type": "ascii", "values": "WAVE3" }
					] },
					{ "id": "value2", "label": "Value2", "type": "ascii", "values": "WAVE4" }
				]
			}
		)*";

	std::vector< char > buff1 { 'W', 'A', 'V', 'E', '1' };
	std::vector< char > buff2 { 'W', 'A', 'V', 'E', '2' };
	std::vector< char > buff3 { 'W', 'A', 'V', 'E', '3' };
	std::vector< char > buff4 { 'W', 'A', 'V', 'E', '4' };
	std::vector< char > buff5 { 'W', 'A', 'V', 'E', '5' };

	LOG_INFO( "\n>>> element_checker_checker_group <<<" );
	{
		
		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),         "value1"  );
		BOOST_CHECK_EQUAL( node->next()->getId(), "value2"  );
		BOOST_CHECK( node->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		elem1->set( buff1 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->firstChild()->getId() );
		elem2->set( buff2 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem1 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->firstChild()->next()->getId() );
		elem3->set( buff3 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->next()->getId() );
		elem4->set( buff4 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		BOOST_CHECK( elem4->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_checker_group suite <<<" );
	{
		
		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),         "value1"  );
		BOOST_CHECK_EQUAL( node->next()->getId(), "value2"  );
		BOOST_CHECK( node->next()->next() == nullptr );

		Checker checker;

		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		elem1->set( buff1 );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1, elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->firstChild()->getId() );
		elem2->set( buff2 );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2, elem1 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->firstChild()->next()->getId() );
		elem3->set( buff5 );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusInvalid );

		std::shared_ptr< basic_element::Element > elem4( new basic_element::Element( elem3->next(), elem3 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->next()->getId() );
		elem4->set( buff4 );
		checker.check( elem4 );
		BOOST_CHECK_EQUAL( elem4->_status, eStatusValid );

		BOOST_CHECK_EQUAL( elem1->_status, eStatusInvalid );

		BOOST_CHECK( elem4->next() == nullptr );
	}
}

BOOST_AUTO_TEST_SUITE_END()
