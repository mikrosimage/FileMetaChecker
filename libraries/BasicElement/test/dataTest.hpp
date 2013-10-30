#include <BasicElement/SubElements/Data.hpp>
#include <boost/property_tree/json_parser.hpp>

BOOST_AUTO_TEST_SUITE( basic_element_test_data )

BOOST_AUTO_TEST_CASE( basic_element_data )
{
	LOG_INFO( "\n>>> basic_element_data <<<" );
	std::string jsonString = R"*(
		{ "header": [
			{
				"id": "id",
				"label": "label",
				"type": "ascii",
				"displayType": "raw"			
			}
		  ]
		}
	)*";

	spec_reader::Specification spec;
	spec.setFromString( jsonString );
	std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

	BOOST_CHECK_EQUAL( node->getId(),          id    );
	BOOST_CHECK_EQUAL( node->getLabel(),       label );
	BOOST_CHECK_EQUAL( node->getType(),        eTypeData       );
	BOOST_CHECK_EQUAL( node->getSubType(),     eSubTypeAscii   );
	BOOST_CHECK_EQUAL( node->getDisplayType(), eDisplayTypeRaw );
	{
		Data data( node );
		BOOST_CHECK_EQUAL( data.getId(),          id    );
		BOOST_CHECK_EQUAL( data.getLabel(),       label );
		BOOST_CHECK_EQUAL( data.getType(),        eTypeData       );
		BOOST_CHECK_EQUAL( data.getSubType(),     eSubTypeAscii   );
		BOOST_CHECK_EQUAL( data.getDisplayType(), eDisplayTypeRaw );

		BOOST_CHECK_EQUAL( data.getUniqueId(),    node->getUId()         );
		BOOST_CHECK_EQUAL( data.getCount(),       node->getCount()       );
		BOOST_CHECK_EQUAL( data.getRequirement(), node->getRequirement() );
		BOOST_CHECK_EQUAL( data.getGroupSize(),   node->getGroupSize()   );

		BOOST_CHECK_EQUAL( data.getValues().size(),      node->getValues().size()      );
		BOOST_CHECK_EQUAL( data.getRange().size(),       node->getRange().size()       );
		BOOST_CHECK_EQUAL( data.getRepetitions().size(), node->getRepetitions().size() );
		BOOST_CHECK_EQUAL( data.getMap().size(),         node->getMap().size()         );

		BOOST_CHECK_EQUAL( data.isGroup(),     node->isGroup()     );
		BOOST_CHECK_EQUAL( data.isOrdered(),   node->isOrdered()   );
		BOOST_CHECK_EQUAL( data.isOptional(),  node->isOptional()  );
		BOOST_CHECK_EQUAL( data.isBigEndian(), node->isBigEndian() );

		BOOST_CHECK( data.getData() == nullptr );
	
		const size_t size = 5;
		const char buffer[] = { 0x7d, 0x21, 0x32, 'c', '!' };
		data.set( buffer, size );
		BOOST_CHECK_EQUAL( data.getSize(), size );
		for( size_t i = 0; i < size; ++i )
			BOOST_CHECK_EQUAL( data.getData()[i], buffer[i] );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_subType )
{
	LOG_INFO( "\n>>> basic_element_data_subType <<<" );
	{

		std::string jsonString = R"*(
			{ "header": [
				{
					"id": "id",
					"label": "label",
					"type": "unknown"
				}
			  ]
			}
		)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		
		Data data( node );
		BOOST_CHECK_EQUAL( data.getSubType(), eSubTypeUnknown );
	}
	{
		std::string jsonString = R"*(
			{ "header": [
				{
					"id": "id",
					"label": "label",
					"type": "hexa"
				}
			  ]
			}
		)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		Data data( node );
		BOOST_CHECK_EQUAL( data.getSubType(), eSubTypeHexa );
	}
	{
		std::string jsonString = R"*(
			{ "header": [
				{
					"id": "id",
					"label": "label",
					"type": "ascii"
				}
			  ]
			}
		)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		Data data( node );
		BOOST_CHECK_EQUAL( data.getSubType(), eSubTypeAscii );
	}
	{
		std::string jsonString = R"*(
			{ "header": [
				{
					"id": "id",
					"label": "label",
					"type": "raw"
				}
			  ]
			}
		)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		Data data( node );
		BOOST_CHECK_EQUAL( data.getSubType(), eSubTypeRaw );
	}
	{
		std::string jsonString = R"*(
			{ "header": [
				{
					"id": "id",
					"label": "label",
					"type": "uint8"
				}
			  ]
			}
		)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		Data data( node );
		BOOST_CHECK_EQUAL( data.getSubType(), eSubTypeUInt8 );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_error_warning )
{
	LOG_INFO( "\n>>> basic_element_data_error_warning <<<" );
	
	std::string jsonString = R"*(
		{ "header": [
			{
				"id": "id",
				"label": "label",
				"type": "unknown"
			}
		  ]
		}
	)*";

	spec_reader::Specification spec;
	spec.setFromString( jsonString );
	std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

	{
		Data data( node );
		data.addErrorLabel( kError );
		BOOST_CHECK_EQUAL( data.getErrorLabel(), kError );
	}
	{
		Data data( node );
		data.addErrorLabel( kError );
		data.addErrorLabel( kError );
		BOOST_CHECK_EQUAL( data.getErrorLabel(), kError + " / " + kError );
	}
	{
		Data data( node );
		data.addWarningLabel( kWarning );
		BOOST_CHECK_EQUAL( data.getWarningLabel(), kWarning );
	}
	{
		Data data( node );
		data.addWarningLabel( kWarning );
		data.addWarningLabel( kWarning );
		BOOST_CHECK_EQUAL( data.getWarningLabel(), kWarning + " / " + kWarning );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_next )
{
	LOG_INFO( "\n>>> basic_element_data_next <<<" );
	{
		std::string jsonString = R"*(
				{
					"header": [
						{ "id": "value1" },
						{ "id": "value2" },
						{ "id": "value3" },
						{ "id": "value4" }
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),         "value3" );
		BOOST_CHECK_EQUAL( node->next()->next()->next()->getId(), "value4" );
		BOOST_CHECK( node->next()->next()->next()->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_next_firstchild )
{
	LOG_INFO( "\n>>> basic_element_data_next_firstchild <<<" );
	{
		std::string jsonString = R"*(
				{
					"header": [
						{ "id": "value1", "label": "Value1", "type": "ascii" },
						{ "id": "value2", "label": "Value2", "type": "ascii" },
						{ "id": "value3", "label": "Value3", "type": "ascii" },
						{ "id": "value4", "label": "Value4", "type": "ascii", "group": [
							{ "id": "firstchild" },
							{ "id": "secondchild" },
							{ "id": "thirdchild" }
						] }
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node1 = spec.getFirstNode();

		BOOST_CHECK_EQUAL( node1->getId(), "value1" );
		
		std::shared_ptr< spec_reader::SpecNode > node2 = node1->next();
		BOOST_CHECK_EQUAL( node2->getId(), "value2" );
		
		std::shared_ptr< spec_reader::SpecNode > node3 = node2->next();
		BOOST_CHECK_EQUAL( node3->getId(), "value3" );
		
		std::shared_ptr< spec_reader::SpecNode > node4 = node3->next();
		BOOST_CHECK_EQUAL( node4->getId(), "value4" );
		
		BOOST_CHECK( node1->next()->next()->next()->next() == nullptr );

		LOG_ERROR( "------" );
		std::shared_ptr< spec_reader::SpecNode > child1 = node4->firstChild();
		BOOST_CHECK_EQUAL( child1->getId(), "firstchild" );

		LOG_ERROR( "------" );
		std::shared_ptr< spec_reader::SpecNode > child2 = child1->next();
		BOOST_CHECK_EQUAL( child2->getId(), "secondchild" );

		LOG_ERROR( "------" );
		std::shared_ptr< spec_reader::SpecNode > child3 = child2->next();
		BOOST_CHECK_EQUAL( child3->getId(), "thirdchild" );

		LOG_ERROR( "------" );
		BOOST_CHECK( child3->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_next_first_child_recursivity )
{
	LOG_INFO( "\n>>> basic_element_data_next_first_child_recursivity <<<" );
	{
		std::string jsonString = R"*(
				{
					"header": [
						{ "id": "value1",
						  "label": "Value1",
						  "type": "ascii",
						  "group": [
								{ "id": "value11",
								  "label": "Value11",
								  "type": "ascii" },
								{ "id": "value12",
								  "label": "Value12",
								  "type": "ascii",
								  "group": [
										{ "id": "value121",
										  "label": "Value121",
										  "type": "ascii",
										  "group": [
											{ "id": "value1211", "label": "Value1211", "type": "ascii" },
											{ "id": "value1212", "label": "Value1212", "type": "ascii" }
										  ] }
							      ] },
								{ "id": "value13",
								  "label": "Value13",
								  "type": "ascii" }
						  ] }
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		BOOST_CHECK_EQUAL( node->getId(),  "value1" );
		
		node = node->firstChild();
		BOOST_CHECK_EQUAL( node->getId(), "value11" );
		BOOST_CHECK_EQUAL( node->getParent()->getId(), "value1" );
		
		node = node->next();
		BOOST_CHECK_EQUAL( node->getId(), "value12" );
		BOOST_CHECK_EQUAL( node->getParent()->getId(), "value1" );

		std::shared_ptr< spec_reader::SpecNode > childNode = node->firstChild();
		
		BOOST_CHECK_EQUAL( childNode->getId(), "value121" );
		BOOST_CHECK_EQUAL( childNode->getParent()->getId(), "value12" );

		std::shared_ptr< spec_reader::SpecNode > nullNode = childNode->next();
		
		BOOST_CHECK( nullNode == nullptr );

		childNode = childNode->firstChild();
		BOOST_CHECK_EQUAL( childNode->getId(), "value1211" );

		childNode = childNode->next();
		BOOST_CHECK_EQUAL( childNode->getId(), "value1212" );
		BOOST_CHECK_THROW( childNode->firstChild(), std::runtime_error );
		
		nullNode = childNode->next();
		BOOST_CHECK( nullNode == nullptr );

		node = node->next();
		BOOST_CHECK_EQUAL( node->getId(), "value13" );
		BOOST_CHECK_THROW( node->firstChild(), std::runtime_error );

		node = node->next();
		BOOST_CHECK( node == nullptr );

		LOG_INFO( "//////// ELEMENT /////////" );
		std::shared_ptr< spec_reader::SpecNode > specnode = spec.getFirstNode();

		std::shared_ptr< Data > data1( new Data( specnode ) );
		BOOST_CHECK_EQUAL( data1->getId(), "value1" );
		BOOST_CHECK_EQUAL( data1->getStatus(), eStatusNotChecked );

		std::shared_ptr< Data > data1bis( new Data( data1->next() ) );
		BOOST_CHECK_EQUAL( data1bis->getId(), "value1" );
		data1bis->setStatus( eStatusSkip );

		std::shared_ptr< Data > data2( new Data( data1bis->next(), data1, data1 ) );
		// Data data2( data1bis.next() );
		// data2->setStatus( eStatusSkip );
		// BOOST_CHECK_EQUAL( data2->getId(), "value11" );
		
		// Data data3( data2.next() );
		// data3.setStatus( eStatusSkip );
		// BOOST_CHECK_EQUAL( data3.getId(), "value12" );
		
		// Data data4( data3.next() );
		// data4.setStatus( eStatusSkip );
		// BOOST_CHECK_EQUAL( data4.getId(), "value121" );
		
		// Data data5( data4.next() );
		// data5.setStatus( eStatusSkip );
		// BOOST_CHECK_EQUAL( data5.getId(), "value1211" );
		
		// Data data6( data5.next() );
		// data6.setStatus( eStatusSkip );
		// BOOST_CHECK_EQUAL( data6.getId(), "value1212" );
		
		// Data data7( data6.next() );
		// data7.setStatus( eStatusSkip );
		// BOOST_CHECK_EQUAL( data7.getId(), "value13" );

		// BOOST_CHECK( data7.next() == nullptr );
	}
}

// BOOST_AUTO_TEST_CASE( basic_element_data_unordered_group )
// {
// 	LOG_INFO( "\n>>> basic_element_data_unordered_group <<<" );
// 	{
// 		std::string jsonString = R"*(
// 				{
// 					"header": [
// 						{ "id": "value1",
// 						  "label": "Value1",
// 						  "type": "ascii",
// 						  "ordered": false,
// 						  "group": [
// 								{ "id": "value11",
// 								  "label": "Value11",
// 								  "type": "ascii" },
// 								{ "id": "value12",
// 								  "label": "Value12",
// 								  "type": "ascii" },
// 								{ "id": "value13",
// 								  "label": "Value13",
// 								  "type": "ascii" }
// 						  ] }
// 					]
// 				}
// 			)*";

// 		spec_reader::Specification spec;
// 		spec.setFromString( jsonString );
// 		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

// 		Data data1( node );
// 		BOOST_CHECK_EQUAL( data1.getId(),      "value1"      );
// 		BOOST_CHECK_EQUAL( data1.getLabel(),   "Value1"      );
// 		BOOST_CHECK_EQUAL( data1.getType(),    eTypeData     );
// 		BOOST_CHECK_EQUAL( data1.getSubType(), eSubTypeAscii );
// 		BOOST_CHECK_EQUAL( data1.isGroup(),    true  );
// 		BOOST_CHECK_EQUAL( data1.isOrdered(),  false );
// 		BOOST_CHECK_EQUAL( data1.getStatus(),  eStatusNotChecked );
// 		data1.setStatus( eStatusSkip );

// 		Data data2( data1.next(), std::make_shared< Data >( data1 ) );
// 		BOOST_CHECK_EQUAL( data2.getId(), "value11" );
// 		data2.setStatus( eStatusInvalidButSkip );

// 		Data data3( data2.next(), std::make_shared< Data >( data2 ) );
// 		BOOST_CHECK_EQUAL( data3.getId(), "value12" );
// 		data3.setStatus( eStatusValid );

// 		Data data4( data3.next(), std::make_shared< Data >( data3 ) );
// 		BOOST_CHECK_EQUAL( data4.getId(), "value11" );
// 		data4.setStatus( eStatusValid );

// 		Data data5( data4.next(), std::make_shared< Data >( data4 ) );
// 		BOOST_CHECK_EQUAL( data5.getId(), "value11" );
// 		data5.setStatus( eStatusInvalidButSkip );

// 		Data data6( data5.next(), std::make_shared< Data >( data5 ) );
// 		BOOST_CHECK_EQUAL( data6.getId(), "value12" );
// 		data6.setStatus( eStatusInvalidButSkip );

// 		Data data7( data6.next(), std::make_shared< Data >( data6 ) );
// 		BOOST_CHECK_EQUAL( data7.getId(), "value13" );
// 		data7.setStatus( eStatusValid );

// 		Data data8( data7.next(), std::make_shared< Data >( data7 ) );
// 		BOOST_CHECK_EQUAL( data8.getId(), "value11" );
// 		data8.setStatus( eStatusInvalidButSkip );

// 		Data data9( data8.next(), std::make_shared< Data >( data8 ) );
// 		BOOST_CHECK_EQUAL( data9.getId(), "value12" );
// 		data9.setStatus( eStatusInvalidButSkip );

// 		Data data0( data9.next(), std::make_shared< Data >( data9 ) );
// 		BOOST_CHECK_EQUAL( data0.getId(), "value13" );
// 		data0.setStatus( eStatusInvalidButSkip );

// 		BOOST_CHECK( data0.next() == nullptr );
// 		BOOST_CHECK_EQUAL( data1.getStatus(),  eStatusSkip );
// 	}
// 	LOG_INFO( "\n>>> basic_element_data_unordered_group suite <<<" );
// 	{
// 		std::string jsonString = R"*(
// 				{
// 					"header": [
// 						{ "id": "value1",
// 						  "label": "Value1",
// 						  "type": "ascii",
// 						  "ordered": false,
// 						  "group": [
// 								{ "id": "value11",
// 								  "label": "Value11",
// 								  "type": "ascii" },
// 								{ "id": "value12",
// 								  "label": "Value12",
// 								  "type": "ascii" },
// 								{ "id": "value13",
// 								  "label": "Value13",
// 								  "type": "ascii" }
// 						  ] }
// 					]
// 				}
// 			)*";

// 		spec_reader::Specification spec;
// 		spec.setFromString( jsonString );
// 		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

// 		std::shared_ptr< Data > data1 = std::make_shared< Data >( node );
// 		LOG_ERROR( &*data1 );
// 		BOOST_CHECK_EQUAL( data1->getId(),      "value1"      );
// 		BOOST_CHECK_EQUAL( data1->getLabel(),   "Value1"      );
// 		BOOST_CHECK_EQUAL( data1->getType(),    eTypeData     );
// 		BOOST_CHECK_EQUAL( data1->getSubType(), eSubTypeAscii );
// 		BOOST_CHECK_EQUAL( data1->isGroup(),    true  );
// 		BOOST_CHECK_EQUAL( data1->isOrdered(),  false );
// 		BOOST_CHECK_EQUAL( data1->getStatus(),  eStatusNotChecked );
// 		data1->setStatus( eStatusValid );

// 		std::shared_ptr< Data > data2 = std::make_shared< Data >( data1->next(), data1 );
// 		BOOST_CHECK_EQUAL( data2->getId(), "value11" );
// 		data2->setStatus( eStatusInvalidButSkip );

// 		std::shared_ptr< Data > data3 = std::make_shared< Data >( data2->next(), data2 );
// 		BOOST_CHECK_EQUAL( data3->getId(), "value12" );
// 		data3->setStatus( eStatusInvalidButSkip );

// 		std::shared_ptr< Data > data4 = std::make_shared< Data >( data3->next(), data3 );
// 		BOOST_CHECK_EQUAL( data4->getId(), "value13" );
// 		data4->setStatus( eStatusInvalidButSkip );

// 		BOOST_CHECK( data4->next() == nullptr );
// 		BOOST_CHECK_EQUAL( data1->getStatus(),  eStatusInvalidForUnordered );
// 	}
// }



BOOST_AUTO_TEST_SUITE_END()
