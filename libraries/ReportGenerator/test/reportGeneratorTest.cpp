#define BOOST_TEST_MODULE qc_report_generator

#include <Common/log.hpp>
#include <BasicElement/Element.hpp>
#include <SpecReader/Specification.hpp>
#include <ReportGenerator/Report.hpp>

#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;
using namespace report_generator;

typedef std::shared_ptr< basic_element::Element > ShPtrElement;
typedef std::shared_ptr< spec_reader::SpecNode  > ShPtrSpecNode;

BOOST_AUTO_TEST_SUITE( report_generator_test )

BOOST_AUTO_TEST_CASE( report_generator_test_init )
{
	common::level = common::eLogTrace;
}

inline ShPtrElement checkElement( const ShPtrSpecNode& node,
                                  report_generator::Report& report,
                                  ShPtrElement& previous,
                                  ShPtrElement& parent,
                                  const std::string& nodeId,
                                  const EStatus status )
{
	ShPtrElement newElem( new basic_element::Element( node, previous, parent ) );
	BOOST_CHECK_EQUAL( newElem->_id, node->getId() );
	if( previous != nullptr )
		BOOST_CHECK_EQUAL( newElem->getPrevious()->_uId, previous->_uId );
	BOOST_CHECK_EQUAL( node->getId(), nodeId );
	newElem->_status = status;
	report.add( newElem );
	return newElem;
}

BOOST_AUTO_TEST_CASE( report_generator_test_report )
{
	LOG_INFO( "\n>>> report_generator_report <<<" );
	std::string jsonString = R"*(
			{
				"content": [
					{
						"id": "value1",
						"label": "Value1",
						"type": "ascii",
						"values": "A1",
						"group": [
							{
								"id": "value11",
								"label": "Value11",
								"type": "ascii",
								"values": "B2",
								"repeated": "2"
							},
							{
								"id": "value12",
								"label": "Value12",
								"type": "ascii",
								"values": "C3",
								"group": [
									{
										"id": "value121",
										"label": "Value121",
										"type": "ascii",
										"values": "D4",
										"groupSize": "6",
										"group": [
											{ "id": "value1211", "label": "Value1211", "type": "ascii", "values": "E5" },
											{ "id": "value1212", "label": "Value1212", "type": "uint16" },
											{ "id": "value1213", "label": "Value1213", "type": "ascii", "values": "G7", "optional": true, "group": [
													{ "id": "value14131", "label": "Value14131", "type": "ascii", "values": "gSeven1" },
													{ "id": "value14132", "label": "Value14132", "type": "ascii", "values": "gSeven2" }
												]
											}
										]
									},
									{
										"id": "value122",
										"label": "Value122",
										"type": "ascii",
										"values": "H8",
										"optional": true
									}
								]
							},
							{
								"id": "value13",
								"label": "Value13",
								"type": "ascii",
								"values": "I9"
							},
							{
								"id": "valueEnd",
								"label": "ValueEnd",
								"type": "ascii",
								"values": "END"
							}
						]
					}
				]
			}
		)*";

	spec_reader::Specification spec;
	spec.setFromString( jsonString );
	std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
	BOOST_CHECK_EQUAL( node->getId(),                                                           "value1"    );
	BOOST_CHECK_EQUAL( node->firstChild()->getId(),                                             "value11"   );
	BOOST_CHECK_EQUAL( node->firstChild()->next()->getId(),                                     "value12"   );
	BOOST_CHECK_EQUAL( node->firstChild()->next()->firstChild()->getId(),                       "value121"  );
	BOOST_CHECK_EQUAL( node->firstChild()->next()->firstChild()->firstChild()->getId(),         "value1211" );
	BOOST_CHECK_EQUAL( node->firstChild()->next()->firstChild()->firstChild()->next()->getId(), "value1212" );
	BOOST_CHECK_EQUAL( node->firstChild()->next()->next()->getId(),                             "value13"   );
	BOOST_CHECK_EQUAL( node->firstChild()->next()->next()->next()->getId(),                     "valueEnd"  );

	report_generator::Report report;

	ShPtrElement elem0( new basic_element::Element( node ) );
	BOOST_CHECK_EQUAL( elem0->_id, node->getId() );
	elem0->_status = eStatusValid;
	report.add( elem0 );

	ShPtrElement elem1  = checkElement( elem0->next(),  report, elem0,   elem0,   "value11",   eStatusValid );
	ShPtrElement elem2  = checkElement( elem1->next(),  report, elem1,   elem0,   "value11",   eStatusValid );
	ShPtrElement elem3  = checkElement( elem2->next(),  report, elem2,   elem0,   "value11",   eStatusInvalidButSkip );
	ShPtrElement elem4  = checkElement( elem3->next(),  report, elem3,   elem0,   "value12",   eStatusValid );
	ShPtrElement elem5  = checkElement( elem4->next(),  report, elem4,   elem4,   "value121",  eStatusValid );
	ShPtrElement elem6  = checkElement( elem5->next(),  report, elem5,   elem5,   "value1211", eStatusInvalid );
	ShPtrElement elem7  = checkElement( elem6->next(),  report, elem6,   elem5,   "value1212", eStatusPassOver );
	ShPtrElement elem8  = checkElement( elem7->next(),  report, elem7,   elem5,   "value1213", eStatusInvalidButOptional );
	ShPtrElement elem9  = checkElement( elem8->next(),  report, elem8,   elem4,   "value122",  eStatusValid );
	ShPtrElement elem10 = checkElement( elem9->next(),  report, elem9,   elem0,   "value13",   eStatusValid );
	ShPtrElement elem11 = checkElement( elem10->next(), report, elem10,  elem0,   "valueEnd",  eStatusValid );

	ShPtrElement back = report.get( "value11" );
	BOOST_CHECK_EQUAL( back->_id,                    "value11" );
	BOOST_CHECK_EQUAL( back->_iteration,             1 );
	BOOST_CHECK_EQUAL( back->_status,                eStatusValid );
	BOOST_CHECK_EQUAL( back->getSpecNode()->getId(), node->firstChild()->getId() );

	back = report.get( "value11", 2 );
	BOOST_CHECK_EQUAL( back->_id,                    "value11" );
	BOOST_CHECK_EQUAL( back->_iteration,             2 );
	BOOST_CHECK_EQUAL( back->_status,                eStatusValid );
	BOOST_CHECK_EQUAL( back->getSpecNode()->getId(), node->firstChild()->getId() );

	back = report.get( "value11", 3 );
	BOOST_CHECK_EQUAL( back->_id,                    "value11" );
	BOOST_CHECK_EQUAL( back->_iteration,             3 );
	BOOST_CHECK_EQUAL( back->_status,                eStatusInvalidButSkip );
	BOOST_CHECK_EQUAL( back->getSpecNode()->getId(), node->firstChild()->getId() );

	back = report.get( elem6->_id );
	BOOST_CHECK_EQUAL( back->_id,                    "value1211" );
	BOOST_CHECK_EQUAL( back->_iteration,             1 );
	BOOST_CHECK_EQUAL( back->_status,                eStatusInvalid );
	BOOST_CHECK_EQUAL( back->getSpecNode()->getId(), node->firstChild()->next()->firstChild()->firstChild()->getId() );

	elem6->_status = eStatusValid;
	report.update( elem6 );
	BOOST_CHECK_EQUAL( report.get( elem6->_id )->_status, eStatusValid );

	BOOST_CHECK_EQUAL( elem1->_error, "" );
	elem1->_error = "Error";
	report.update( elem1 );
	BOOST_CHECK_EQUAL( report.get( elem1->_id )->_error, elem1->_error  );
	BOOST_CHECK_EQUAL( report.get( elem1->_id    )->_uId, elem1->_uId  );
	BOOST_CHECK_EQUAL( report.get( elem1->_id, 2 )->_uId, elem2->_uId );
	BOOST_CHECK( report.get( "other" ) == nullptr );

	report.print();
}

BOOST_AUTO_TEST_SUITE_END()
