
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
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		std::vector< char > buff { 'W', 'A', 'V', 'E' };
		elem->set( buff, 4 );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_status, eStatusPassOver );
		BOOST_CHECK( node->next() == nullptr );
		BOOST_CHECK( elem->next() == nullptr );
	}
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"values": "WAVE" )*" + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		std::vector< char > buff { 'W', 'A', 'V', 'E' };
		elem->set( buff, 4 );

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
		elem->set( buff, 4 );

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
		elem->set( buff, 4 );

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
		elem->set( buff, 4 );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_type, eTypeUInt32 );
		BOOST_CHECK_EQUAL( elem->_status, eStatusPassOver );
	}

	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"range": [ { "min": "1", "max": "10" } ] )*"  + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		std::vector< char > buff { 0x00, 0x00, 0x00, 0x05 };
		elem->set( buff, 4 );

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
		elem->set( buff, 4 );

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
		elem->set( buff, 4 );

		checker.check( elem );
		BOOST_CHECK_EQUAL( elem->_type, eTypeUInt32 );
		BOOST_CHECK_EQUAL( elem->_status, eStatusInvalid );
	}
}

BOOST_AUTO_TEST_SUITE_END()
