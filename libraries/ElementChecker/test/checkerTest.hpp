
BOOST_AUTO_TEST_SUITE( element_checker_test_checker )

BOOST_AUTO_TEST_CASE( element_checker_checker_data )
{
	LOG_INFO( ">>> element_checker_checker_data <<<" );
	
	std::string jsonStringBegin = R"*(
				{
					"header": [
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
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem->set( (const char*)&buff, 4 );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_status, eStatusPassOver );
		BOOST_CHECK_EQUAL( checker.getElementList().size(), 1 );
	}
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"values": "WAVE" )*" + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem->set( (const char*)&buff, 4 );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_status, eStatusValid );
		BOOST_CHECK_EQUAL( checker.getElementList().size(), 2 );
	}
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"values": "wave" )*" + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem->set( (const char*)&buff, 4 );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_status, eStatusValid );
		BOOST_CHECK_EQUAL( checker.getElementList().size(), 3 );
	}
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"values": "evaw" )*"  + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem->set( (const char*)&buff, 4 );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_status, eStatusInvalid );
		BOOST_CHECK_EQUAL( elem->_error, "Invalid value " );
		BOOST_CHECK_EQUAL( checker.getElementList().size(), 4 );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_number )
{
	LOG_INFO( ">>> element_checker_checker_number <<<" );
	
	std::string jsonStringBegin = R"*(
				{
					"header": [
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
	
		const char buff[4] { 0x00, 0x00, 0x00, 0x01 };
		elem->set( (const char*)&buff, 4 );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_type, eTypeUInt32 );
		BOOST_CHECK_EQUAL( elem->_status, eStatusPassOver );
		BOOST_CHECK_EQUAL( checker.getElementList().size(), 1 );
	}

	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"range": [ { "min": "1", "max": "10" } ] )*"  + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		const char buff[4] { 0x00, 0x00, 0x00, 0x05 };
		elem->set( (const char*)&buff, 4 );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_type, eTypeUInt32 );
		BOOST_CHECK_EQUAL( elem->_status, eStatusValid );
		BOOST_CHECK_EQUAL( checker.getElementList().size(), 2 );
	}

	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"endian":"little","range": [ { "min": "1", "max": "10" } ] )*"  + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		const char buff[4] { 0x05, 0x00, 0x00, 0x00 };
		elem->set( (const char*)&buff, 4 );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_type, eTypeUInt32 );
		BOOST_CHECK_EQUAL( elem->_status, eStatusValid );
		BOOST_CHECK_EQUAL( checker.getElementList().size(), 3 );
	}

	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"range": [ { "max": "5" }, { "min": "10" } ] )*"  + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		const char buff[4] { 0x00, 0x00, 0x00, 0x08 };
		elem->set( (const char*)&buff, 4 );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_type, eTypeUInt32 );
		BOOST_CHECK_EQUAL( elem->_status, eStatusInvalid );
		BOOST_CHECK_EQUAL( checker.getElementList().size(), 4 );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_repetition )
{
	LOG_INFO( "\n>>> element_checker_checker_repetition <<<" );
	{
		std::string jsonString = R"*(
				{
					"header": [
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
							"repeated": "3"
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

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),         "value3" );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;
		const char buff1[5] { 'W', 'A', 'V', 'E', '1' };
		const char buff2[5] { 'W', 'A', 'V', 'E', '2' };
		const char buff3[5] { 'W', 'A', 'V', 'E', '3' };

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
		BOOST_CHECK_EQUAL( elem5->_status, eStatusInvalid );

		std::shared_ptr< basic_element::Element > elem6( new basic_element::Element( elem5->next(), elem5 ) );
		elem6->set( (const char*)&buff3, 5 );
		checker.check( elem6 );
		BOOST_CHECK_EQUAL( elem6->_status, eStatusValid );

		BOOST_CHECK( elem6->next() == nullptr );
	}
}

BOOST_AUTO_TEST_SUITE_END()
