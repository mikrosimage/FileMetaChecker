#define BOOST_TEST_MODULE qc_spec_reader

#include <Common/log.hpp>
#include <BasicElement/Element.hpp>
#include <SpecReader/Specification.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::unit_test;
using namespace basic_element;

std::string id    = "id";
std::string label = "label";

BOOST_AUTO_TEST_SUITE( basic_element_test_suite01 )

BOOST_AUTO_TEST_CASE( basic_element_init )
{
	common::level = common::eLogTrace;
	// LOG_INFO( common::level );
}

BOOST_AUTO_TEST_CASE( basic_element_element )
{
	LOG_INFO( "\n>>> basic_element_element <<<" );
	std::string jsonString = R"*(
		{ "content": [
			{
				"id": "id",
				"label": "label",
				"type": "ascii",
				"display": "raw"
			}
		  ]
		}
	)*";

	spec_reader::Specification spec;
	spec.setFromString( jsonString );
	std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

	BOOST_CHECK_EQUAL( node->getId(),          id    );
	BOOST_CHECK_EQUAL( node->getLabel(),       label );
	BOOST_CHECK_EQUAL( node->getType(),        eTypeAscii      );
	BOOST_CHECK_EQUAL( node->getDisplayType(), eDisplayTypeRaw );
	{
		Element elem( node );
		BOOST_CHECK_EQUAL( elem._id,          id    );
		BOOST_CHECK_EQUAL( elem._label,       label );
		BOOST_CHECK_EQUAL( elem._type,        eTypeAscii      );
		BOOST_CHECK_EQUAL( elem._displayType, eDisplayTypeRaw );

		BOOST_CHECK_EQUAL( elem._uId,           node->getUId()         );
		BOOST_CHECK_EQUAL( elem._countExpr,     node->getCount()       );
		BOOST_CHECK_EQUAL( elem._requiredExpr,  node->getRequirement() );
		BOOST_CHECK_EQUAL( elem._groupSizeExpr, node->getGroupSize()   );

		BOOST_CHECK_EQUAL( elem._values.size(),    node->getValues().size()      );
		BOOST_CHECK_EQUAL( elem._rangeExpr.size(), node->getRange().size()       );
		BOOST_CHECK_EQUAL( elem._repetExpr.size(), node->getRepetitions().size() );
		BOOST_CHECK_EQUAL( elem._map.size(),       node->getMap().size()         );

		BOOST_CHECK_EQUAL( elem._isGroup,     node->isGroup()     );
		BOOST_CHECK_EQUAL( elem._isOrdered,   node->isOrdered()   );
		BOOST_CHECK_EQUAL( elem._isOptional,  node->isOptional()  );
		BOOST_CHECK_EQUAL( elem._isBigEndian, node->isBigEndian() );

		BOOST_CHECK( elem._data.empty() );
	
		const size_t size = 5;
		std::vector< char > buffer { 0x7d, 0x21, 0x32, 'c', '!' };
		elem.set( buffer, size );
		BOOST_CHECK_EQUAL( elem._data.size(), size );
		for( size_t i = 0; i < size; ++i )
			BOOST_CHECK_EQUAL( elem._data.at( i ), buffer[i] );

		elem._error.push_back( kError );
		BOOST_CHECK_EQUAL( elem._error.at( 0 ), kError );

		elem._warning.push_back( kWarning );
		BOOST_CHECK_EQUAL( elem._warning.at( 0 ), kWarning );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_type )
{
	LOG_INFO( "\n>>> basic_element_type <<<" );
	{

		std::string jsonString = R"*(
			{ "content": [
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
		
		Element elem( node );
		BOOST_CHECK_EQUAL( elem._type, eTypeUnknown );
	}
	{
		std::string jsonString = R"*(
			{ "content": [
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

		Element elem( node );
		BOOST_CHECK_EQUAL( elem._type, eTypeHexa );
	}
	{
		std::string jsonString = R"*(
			{ "content": [
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

		Element elem( node );
		BOOST_CHECK_EQUAL( elem._type, eTypeAscii );
	}
	{
		std::string jsonString = R"*(
			{ "content": [
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

		Element elem( node );
		BOOST_CHECK_EQUAL( elem._type, eTypeRaw );
	}
	{
		std::string jsonString = R"*(
			{ "content": [
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

		Element elem( node );
		BOOST_CHECK_EQUAL( elem._type, eTypeUInt8 );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_next )
{
	LOG_INFO( "\n>>> basic_element_next <<<" );
	{
		std::string jsonString = R"*(
				{
					"content": [
						{ "id": "value1", "label": "Value1", "type": "ascii" },
						{ "id": "value2", "label": "Value2", "type": "ascii" },
						{ "id": "value3", "label": "Value3", "type": "ascii" },
						{ "id": "value4", "label": "Value4", "type": "ascii" }
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

		Element elem1( node );
		BOOST_CHECK_EQUAL( elem1._id, node->getId() );
		elem1._status = eStatusValid;
		
		Element elem2( elem1.next() );
		BOOST_CHECK_EQUAL( elem2._id, node->next()->getId() );
		elem2._status = eStatusValid;

		Element elem3( elem2.next() );
		BOOST_CHECK_EQUAL( elem3._id, node->next()->next()->getId() );
		elem3._status = eStatusValid;

		Element elem4( elem3.next() );
		BOOST_CHECK_EQUAL( elem4._id, node->next()->next()->next()->getId() );
		elem4._status = eStatusValid;

		BOOST_CHECK( elem4.next() == nullptr );
	}
}

typedef std::shared_ptr< Element > ShPtrElement;

inline ShPtrElement checkElement( ShPtrElement& elem, ShPtrElement& previous, ShPtrElement& parent, const std::string& nodeId, const std::shared_ptr< spec_reader::SpecNode >& node )
{
	ShPtrElement newElem( new Element( elem->next(), previous, parent ) );
	BOOST_CHECK_EQUAL( newElem->_id, node->getId() );
	BOOST_CHECK_EQUAL( newElem->getPrevious()->_uId, elem->_uId );
	BOOST_CHECK_EQUAL( node->getId(), nodeId );
	newElem->_status = eStatusValid;
	return newElem;
}

BOOST_AUTO_TEST_CASE( basic_element_next_first_child_recursivity )
{
	LOG_INFO( "\n>>> basic_element_next_first_child_recursivity <<<" );
	{
		std::string jsonString = R"*(
			{
				"content": [
				{
					"id": "value1",
					"label": "Value1",
					"type": "ascii",
					"group": [
						{
							"id": "value11",
							"label": "Value11",
							"type": "ascii"
						},
						{
							"id": "value12",
							"label": "Value12",
							"type": "ascii",
							"group": [
							{
								"id": "value121",
								"label": "Value121",
								"type": "ascii",
								"group": [
								{ "id": "value1211", "label": "Value1211", "type": "ascii" },
								{ "id": "value1212", "label": "Value1212", "type": "ascii" }
								]
							} ]
						},
						{
							"id": "value13",
							"label": "Value13",
							"type": "ascii",
							"group": [
								{ "id": "value131", "label": "Value131", "type": "ascii" }
							]
						} 
					]
				} 
			]
		} )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
	
		ShPtrElement elem1( new Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		BOOST_CHECK_EQUAL( node->getId(), "value1" );
		elem1->_status = eStatusValid;

		ShPtrElement elem2 = checkElement( elem1, elem1, elem1, "value11",   node->firstChild() );
		ShPtrElement elem3 = checkElement( elem2, elem2, elem1, "value12",   node->firstChild()->next() );
		ShPtrElement elem4 = checkElement( elem3, elem3, elem3, "value121",  node->firstChild()->next()->firstChild() );
		ShPtrElement elem5 = checkElement( elem4, elem4, elem4, "value1211", node->firstChild()->next()->firstChild()->firstChild() );
		ShPtrElement elem6 = checkElement( elem5, elem5, elem4, "value1212", node->firstChild()->next()->firstChild()->firstChild()->next() );
		ShPtrElement elem7 = checkElement( elem6, elem6, elem1, "value13",   node->firstChild()->next()->next() );
		ShPtrElement elem8 = checkElement( elem7, elem7, elem7, "value131",  node->firstChild()->next()->next()->firstChild() );

		BOOST_CHECK( elem8->next() == nullptr );
		BOOST_CHECK( node->firstChild()->next()->next()->firstChild()->next() == nullptr );
		BOOST_CHECK( node->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_data_unordered_group )
{
	LOG_INFO( "\n>>> basic_element_data_unordered_group <<<" );
	{
		std::string jsonString = R"*(
				{
					"content": [
						{ "id": "value1",
						  "label": "Value1",
						  "type": "ascii",
						  "ordered": false,
						  "group": [
								{ "id": "value11",
								  "label": "Value11",
								  "type": "ascii" },
								{ "id": "value12",
								  "label": "Value12",
								  "type": "ascii" },
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

		std::shared_ptr< Element > elem1( new Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id,        "value1"      );
		BOOST_CHECK_EQUAL( elem1->_label,     "Value1"      );
		BOOST_CHECK_EQUAL( elem1->_type,      eTypeAscii );
		BOOST_CHECK_EQUAL( elem1->_isGroup,   true  );
		BOOST_CHECK_EQUAL( elem1->_isOrdered, false );
		BOOST_CHECK_EQUAL( elem1->_status,    eStatusNotChecked );
		elem1->_status = eStatusSkip;

		std::shared_ptr< Element > elem2( new Element( elem1->next(), elem1, elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, "value11" );
		elem2->_status = eStatusInvalidButSkip;

		std::shared_ptr< Element > elem3( new Element( elem2->next(), elem2, elem1 ) );
		BOOST_CHECK_EQUAL( elem3->_id, "value12" );
		elem3->_status = eStatusValid;

		std::shared_ptr< Element > elem4( new Element( elem3->next(), elem3, elem1 ) );
		BOOST_CHECK_EQUAL( elem4->_id, "value11" );
		elem4->_status = eStatusValid;

		std::shared_ptr< Element > elem5( new Element( elem4->next(), elem4, elem1 ) );
		BOOST_CHECK_EQUAL( elem5->_id, "value11" );
		elem5->_status = eStatusInvalidButSkip;

		std::shared_ptr< Element > elem6( new Element( elem5->next(), elem5, elem1 ) );
		BOOST_CHECK_EQUAL( elem6->_id, "value12" );
		elem6->_status = eStatusInvalidButSkip;

		std::shared_ptr< Element > elem7( new Element( elem6->next(), elem6, elem1 ) );
		BOOST_CHECK_EQUAL( elem7->_id, "value13" );
		elem7->_status = eStatusValid;

		std::shared_ptr< Element > elem8( new Element( elem7->next(), elem7, elem1 ) );
		BOOST_CHECK_EQUAL( elem8->_id, "value11" );
		elem8->_status = eStatusInvalidButSkip;

		std::shared_ptr< Element > elem9( new Element( elem8->next(), elem8, elem1 ) );
		BOOST_CHECK_EQUAL( elem9->_id, "value12" );
		elem9->_status = eStatusInvalidButSkip;

		std::shared_ptr< Element > elem0( new Element( elem9->next(), elem9, elem1 ) );
		BOOST_CHECK_EQUAL( elem0->_id, "value13" );
		elem0->_status = eStatusInvalidButSkip;

		BOOST_CHECK( elem0->next() == nullptr );
		BOOST_CHECK_EQUAL( elem1->_status,  eStatusSkip );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_next_repetition_one )
{
	LOG_INFO( "\n>>> basic_element_next_repetition_one <<<" );
	{
		std::string jsonString = R"*(
				{
					"content": [
						{ "id": "value1", "label": "Value1", "type": "ascii" },
						{ "id": "value2", "label": "Value2", "type": "ascii", "repetition": "3" },
						{ "id": "value3", "label": "Value3", "type": "ascii" },
						{ "id": "value4", "label": "Value4", "type": "ascii" }
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

		std::shared_ptr< Element > elem1( new Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id,        "value1"      );
		BOOST_CHECK_EQUAL( elem1->_label,     "Value1"      );
		BOOST_CHECK_EQUAL( elem1->_type,      eTypeAscii );
		BOOST_CHECK_EQUAL( elem1->_status,    eStatusNotChecked );
		elem1->_status = eStatusSkip;

		std::shared_ptr< Element > elem2( new Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		elem2->_status = eStatusValid;

		std::shared_ptr< Element > elem3( new Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->getId() );
		elem3->_status = eStatusValid;

		std::shared_ptr< Element > elem4( new Element( elem3->next(), elem3 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->next()->getId() );
		elem4->_status = eStatusValid;

		std::shared_ptr< Element > elem5( new Element( elem4->next(), elem4 ) );
		BOOST_CHECK_EQUAL( elem5->_id, node->next()->getId() );
		elem5->_status = eStatusInvalid;

		std::shared_ptr< Element > elem6( new Element( elem5->next(), elem5 ) );
		BOOST_CHECK_EQUAL( elem6->_id, node->next()->next()->getId() );
		elem6->_status = eStatusInvalid;

		std::shared_ptr< Element > elem7( new Element( elem6->next(), elem6 ) );
		BOOST_CHECK_EQUAL( elem7->_id, node->next()->next()->next()->getId() );
		elem7->_status = eStatusValid;

		BOOST_CHECK( elem7->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_next_repetition_first )
{
	LOG_INFO( "\n>>> basic_element_next_repetition_first <<<" );
	{
		std::string jsonString = R"*(
				{
					"content": [
						{ "id": "value1", "label": "Value1", "type": "ascii", "repetition": "3" },
						{ "id": "value2", "label": "Value2", "type": "ascii" }
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2" );

		std::shared_ptr< Element > elem1( new Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id,        "value1"      );
		BOOST_CHECK_EQUAL( elem1->_label,     "Value1"      );
		BOOST_CHECK_EQUAL( elem1->_type,      eTypeAscii );
		BOOST_CHECK_EQUAL( elem1->_status,    eStatusNotChecked );
		elem1->_status = eStatusValid;

		std::shared_ptr< Element > elem2( new Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->getId() );
		elem2->_status = eStatusValid;

		std::shared_ptr< Element > elem3( new Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->getId() );
		elem3->_status = eStatusValid;

		std::shared_ptr< Element > elem4( new Element( elem3->next(), elem3 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->getId() );
		elem4->_status = eStatusInvalid;

		std::shared_ptr< Element > elem5( new Element( elem4->next(), elem4 ) );
		BOOST_CHECK_EQUAL( elem5->_id, node->next()->getId() );
		elem5->_status = eStatusValid;

		BOOST_CHECK( elem5->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_next_repetition_last )
{
	LOG_INFO( "\n>>> basic_element_next_repetition_last <<<" );
	{
		std::string jsonString = R"*(
		{
			"content": [
			{ "id": "value1", "label": "Value1", "type": "ascii" },
			{ "id": "value2", "label": "Value2", "type": "ascii", "repetition": "3" } ]
		} )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2" );

		std::shared_ptr< Element > elem1( new Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id,        "value1"      );
		BOOST_CHECK_EQUAL( elem1->_label,     "Value1"      );
		BOOST_CHECK_EQUAL( elem1->_type,      eTypeAscii );
		BOOST_CHECK_EQUAL( elem1->_status,    eStatusNotChecked );
		elem1->_status = eStatusValid;

		std::shared_ptr< Element > elem2( new Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		elem2->_status = eStatusValid;

		std::shared_ptr< Element > elem3( new Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->getId() );
		elem3->_status = eStatusValid;

		std::shared_ptr< Element > elem4( new Element( elem3->next(), elem3 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->next()->getId() );
		elem4->_status = eStatusValid;

		std::shared_ptr< Element > elem5( new Element( elem4->next(), elem4 ) );
		BOOST_CHECK_EQUAL( elem5->_id, node->next()->getId() );
		elem5->_status = eStatusInvalid;

		BOOST_CHECK( elem5->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_next_repetition_group )
{
	LOG_INFO( "\n>>> basic_element_next_repetition_group <<<" );
	{
		std::string jsonString = R"*(
				{
					"content": [
						{ "id": "value1",
						  "label": "Value1",
						  "type": "ascii",
						  "repetition": "2",
						  "group": [
								{ "id": "value11",
								  "label": "Value11",
								  "type": "ascii" },
								{ "id": "value12",
								  "label": "Value12",
								  "type": "ascii" },
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
		BOOST_CHECK_EQUAL( node->getId(),                               "value1" );
		BOOST_CHECK_EQUAL( node->firstChild()->getId(),                 "value11" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->getId(),         "value12" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->next()->getId(), "value13" );

		std::shared_ptr< Element > elem1( new Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id,        "value1"      );
		BOOST_CHECK_EQUAL( elem1->_label,     "Value1"      );
		BOOST_CHECK_EQUAL( elem1->_type,      eTypeAscii );
		BOOST_CHECK_EQUAL( elem1->_status,    eStatusNotChecked );
		elem1->_status = eStatusValid;

		std::shared_ptr< Element > elem2( new Element( elem1->next(), elem1, elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->firstChild()->getId() );
		elem2->_status = eStatusValid;

		std::shared_ptr< Element > elem3( new Element( elem2->next(), elem2, elem1 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->firstChild()->next()->getId() );
		elem3->_status = eStatusValid;

		std::shared_ptr< Element > elem4( new Element( elem3->next(), elem3, elem1 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->firstChild()->next()->next()->getId() );
		elem4->_status = eStatusValid;

		std::shared_ptr< Element > elem5( new Element( elem4->next(), elem4 ) );
		BOOST_CHECK_EQUAL( elem5->_id, node->getId() );
		elem5->_status = eStatusValid;

		std::shared_ptr< Element > elem6( new Element( elem5->next(), elem5, elem4 ) );
		BOOST_CHECK_EQUAL( elem6->_id, node->firstChild()->getId() );
		elem6->_status = eStatusValid;

		std::shared_ptr< Element > elem7( new Element( elem6->next(), elem6, elem4 ) );
		BOOST_CHECK_EQUAL( elem7->_id, node->firstChild()->next()->getId() );
		elem7->_status = eStatusValid;

		std::shared_ptr< Element > elem8( new Element( elem7->next(), elem7, elem4 ) );
		BOOST_CHECK_EQUAL( elem8->_id, node->firstChild()->next()->next()->getId() );
		elem8->_status = eStatusValid;

		std::shared_ptr< Element > elem9( new Element( elem8->next(), elem8 ) );
		BOOST_CHECK_EQUAL( elem9->_id, node->getId() );
		elem9->_status = eStatusInvalid;

		BOOST_CHECK( elem9->next() == nullptr );
	}
	LOG_INFO( "\n>>> basic_element_next_repetition_group suite <<<" );
	{
		std::string jsonString = R"*(
				{
					"content": [
						{ "id": "value1",
						  "label": "Value1",
						  "type": "ascii",
						  "repetition": "2",
						  "group": [
								{ "id": "value11",
								  "label": "Value11",
								  "type": "ascii" },
								{ "id": "value12",
								  "label": "Value12",
								  "type": "ascii" },
								{ "id": "value13",
								  "label": "Value13",
								  "type": "ascii" }
						  ]
						},
						{ "id": "value2",
						  "label": "Value2",
						  "type": "ascii" }
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                               "value1" );
		BOOST_CHECK_EQUAL( node->firstChild()->getId(),                 "value11" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->getId(),         "value12" );
		BOOST_CHECK_EQUAL( node->firstChild()->next()->next()->getId(), "value13" );

		std::shared_ptr< Element > elem1( new Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id,        "value1"      );
		BOOST_CHECK_EQUAL( elem1->_label,     "Value1"      );
		BOOST_CHECK_EQUAL( elem1->_type,      eTypeAscii );
		BOOST_CHECK_EQUAL( elem1->_status,    eStatusNotChecked );
		elem1->_status = eStatusValid;

		std::shared_ptr< Element > elem2( new Element( elem1->next(), elem1, elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->firstChild()->getId() );
		elem2->_status = eStatusValid;

		std::shared_ptr< Element > elem3( new Element( elem2->next(), elem2, elem1 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->firstChild()->next()->getId() );
		elem3->_status = eStatusValid;

		std::shared_ptr< Element > elem4( new Element( elem3->next(), elem3, elem1 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->firstChild()->next()->next()->getId() );
		elem4->_status = eStatusValid;

		std::shared_ptr< Element > elem5( new Element( elem4->next(), elem4 ) );
		BOOST_CHECK_EQUAL( elem5->_id, node->getId() );
		elem5->_status = eStatusValid;

		std::shared_ptr< Element > elem6( new Element( elem5->next(), elem5, elem4 ) );
		BOOST_CHECK_EQUAL( elem6->_id, node->firstChild()->getId() );
		elem6->_status = eStatusValid;

		std::shared_ptr< Element > elem7( new Element( elem6->next(), elem6, elem4 ) );
		BOOST_CHECK_EQUAL( elem7->_id, node->firstChild()->next()->getId() );
		elem7->_status = eStatusValid;

		std::shared_ptr< Element > elem8( new Element( elem7->next(), elem7, elem4 ) );
		BOOST_CHECK_EQUAL( elem8->_id, node->firstChild()->next()->next()->getId() );
		elem8->_status = eStatusValid;

		std::shared_ptr< Element > elem9( new Element( elem8->next(), elem8 ) );
		BOOST_CHECK_EQUAL( elem9->_id, node->getId() );
		elem9->_status = eStatusInvalid;

		std::shared_ptr< Element > elem10( new Element( elem9->next(), elem8 ) );
		BOOST_CHECK_EQUAL( elem10->_id, node->next()->getId() );
		elem10->_status = eStatusValid;

		BOOST_CHECK( elem10->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_next_optional )
{
	LOG_INFO( "\n>>> basic_element_next_optional <<<" );
	{
		std::string jsonString = R"*(
				{
					"content": [
						{ "id": "value1", "label": "Value1", "type": "ascii" },
						{ "id": "value2", "label": "Value2", "type": "ascii", "optional": true },
						{ "id": "value3", "label": "Value3", "type": "ascii" },
						{ "id": "value4", "label": "Value4", "type": "ascii" }
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

		std::shared_ptr< Element > elem1( new Element( node ) );
		elem1->_status = eStatusValid;

		std::shared_ptr< Element > elem2( new Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_isOptional, true );
		elem2->_status = eStatusInvalidButOptional;

		std::shared_ptr< Element > elem3( new Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		elem3->_status = eStatusValid;

		std::shared_ptr< Element > elem4( new Element( elem3->next(), elem3 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->next()->next()->next()->getId() );
		elem4->_status = eStatusValid;

		BOOST_CHECK( elem4->next() == nullptr );
	}
	LOG_INFO( "\n>>> basic_element_next_optional in group <<<" );
	{
		std::string jsonString = R"*(
				{
					"content": [
						{ "id": "value1", "label": "Value1", "type": "ascii" },
						{ "id": "value2", "label": "Value2", "type": "ascii", "group": [
							{ "id": "value21", "label": "Value21", "type": "ascii", "optional": true }
						] },
						{ "id": "value3", "label": "Value3", "type": "ascii" }
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1"  );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2"  );
		BOOST_CHECK_EQUAL( node->next()->firstChild()->getId(),   "value21" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),         "value3"  );

		std::shared_ptr< Element > elem1( new Element( node ) );
		elem1->_status = eStatusValid;

		std::shared_ptr< Element > elem2( new Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		elem2->_status = eStatusValid;

		std::shared_ptr< Element > elem3( new Element( elem2->next(), elem2, elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->firstChild()->getId() );
		BOOST_CHECK_EQUAL( elem3->_isOptional, true );
		elem3->_status = eStatusInvalidButOptional;

		std::shared_ptr< Element > elem4( new Element( elem3->next(), elem3 ) );
		BOOST_CHECK_EQUAL( elem4->_id, node->next()->next()->getId() );
		elem4->_status = eStatusValid;

		BOOST_CHECK( elem4->next() == nullptr );
	}
	LOG_INFO( "\n>>> basic_element_next_optional group <<<" );
	{
		std::string jsonString = R"*(
				{
					"content": [
						{ "id": "value1", "label": "Value1", "type": "ascii" },
						{ "id": "value2", "label": "Value2", "type": "ascii", "optional": true, "group": [
							{ "id": "value21", "label": "Value21", "type": "ascii" }
						] },
						{ "id": "value3", "label": "Value3", "type": "ascii" }
					]
				}
			)*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                         "value1" );
		BOOST_CHECK_EQUAL( node->next()->getId(),                 "value2" );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),         "value3" );

		std::shared_ptr< Element > elem1( new Element( node ) );
		elem1->_status = eStatusValid;

		std::shared_ptr< Element > elem2( new Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK_EQUAL( elem2->_isOptional, true );
		elem2->_status = eStatusInvalidButOptional;

		std::shared_ptr< Element > elem3( new Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		elem3->_status = eStatusValid;

		BOOST_CHECK( elem3->next() == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_get_children )
{
	LOG_INFO( "\n>>> basic_element_get_children <<<" );
	{
		std::string jsonString = R"*(
			{
				"content": [
				{
					"id": "value1",
					"label": "Value1",
					"type": "ascii",
					"group": [
						{
							"id": "value11",
							"label": "Value11",
							"type": "ascii"
						},
						{
							"id": "value12",
							"label": "Value12",
							"type": "ascii",
							"group": [
							{
								"id": "value121",
								"label": "Value121",
								"type": "ascii",
								"group": [
								{ "id": "value1211", "label": "Value1211", "type": "ascii" },
								{ "id": "value1212", "label": "Value1212", "type": "ascii" }
								]
							} ]
						},
						{
							"id": "value13",
							"label": "Value13",
							"type": "ascii",
							"group": [
								{ "id": "value131", "label": "Value131", "type": "ascii" }
							]
						} 
					]
				} 
			]
		} )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonString );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
	
		ShPtrElement elem1( new Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		BOOST_CHECK_EQUAL( node->getId(), "value1" );
		elem1->_status = eStatusValid;

		ShPtrElement elem2 = checkElement( elem1, elem1, elem1, "value11",   node->firstChild() );
		ShPtrElement elem3 = checkElement( elem2, elem2, elem1, "value12",   node->firstChild()->next() );
		ShPtrElement elem4 = checkElement( elem3, elem3, elem3, "value121",  node->firstChild()->next()->firstChild() );
		ShPtrElement elem5 = checkElement( elem4, elem4, elem4, "value1211", node->firstChild()->next()->firstChild()->firstChild() );
		ShPtrElement elem6 = checkElement( elem5, elem5, elem4, "value1212", node->firstChild()->next()->firstChild()->firstChild()->next() );
		ShPtrElement elem7 = checkElement( elem6, elem6, elem1, "value13",   node->firstChild()->next()->next() );
		ShPtrElement elem8 = checkElement( elem7, elem7, elem7, "value131",  node->firstChild()->next()->next()->firstChild() );

		BOOST_CHECK( elem8->next() == nullptr );
		BOOST_CHECK( node->firstChild()->next()->next()->firstChild()->next() == nullptr );
		// BOOST_CHECK_EQUAL( node->getIterator() + 1,  spec.end() );
		BOOST_CHECK( node->next() == nullptr );

		BOOST_CHECK_EQUAL( elem1->getChildren().size(), 3 );
		BOOST_CHECK_EQUAL( elem2->getChildren().size(), 0 );
		BOOST_CHECK_EQUAL( elem3->getChildren().size(), 1 );
		BOOST_CHECK_EQUAL( elem4->getChildren().size(), 2 );
		BOOST_CHECK_EQUAL( elem5->getChildren().size(), 0 );
		BOOST_CHECK_EQUAL( elem6->getChildren().size(), 0 );
		BOOST_CHECK_EQUAL( elem7->getChildren().size(), 1 );
		BOOST_CHECK_EQUAL( elem8->getChildren().size(), 0 );
	}
}

BOOST_AUTO_TEST_SUITE_END()
