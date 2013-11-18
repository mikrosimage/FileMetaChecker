#define BOOST_TEST_MODULE qc_comparator

#include <Comparator/Comparator.hpp>
#include <Common/log.hpp>
#include <SpecReader/Specification.hpp>
#include <FileReader/FileReader.hpp>
#include <ReportGenerator/Report.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::unit_test;
namespace bpt = boost::property_tree;

using namespace comparator;

BOOST_AUTO_TEST_SUITE( comparator_test )

BOOST_AUTO_TEST_CASE( comparator_test_init )
{
	common::level = common::eLogTrace;
}

BOOST_AUTO_TEST_CASE( comparator_test_comparator_1 )
{
	LOG_INFO( "\n>>> comparator_test_comparator 1 <<<" );
	std::string jsonString = R"*(
			{
				"header": [
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
								"repeated": "10"
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
								"values": "I9",
								"ordered": false,
								"group": [
									{ "id": "value131", "label": "Value131", "type": "ascii", "values": "I91" },
									{ "id": "value132", "label": "Value132", "type": "ascii", "values": "I92" },
									{ "id": "value133", "label": "Value133", "type": "ascii", "values": "I93" }
								]
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

	Comparator comp;

	std::stringbuf buffer;
	file_reader::FileReader file( &buffer );

	std::string str = "A1";
	str += "B2B2B2B2B2B2B2B2B2B2";
	str += "C3D4EXF600H8";
	str += "I9I92I93I91";
	str += "END";

	buffer.sputn( str.c_str(), str.size() );
	BOOST_CHECK_EQUAL( file.getLength(), str.size() );

	report_generator::Report report;

	comp.check( spec, file, report );

	BOOST_CHECK_EQUAL( file.isEndOfFile(), true );

	report.print();
}

BOOST_AUTO_TEST_CASE( comparator_test_comparator_2 )
{
	LOG_INFO( "\n>>> comparator_test_comparator 2 <<<" );
	std::string jsonString = R"*(
			{
				"header": [
					{
						"id": "value1",
						"label": "Value1",
						"type": "ascii",
						"values": "A1",
						"group": [
							{
								"id": "value14",
								"label": "Value14",
								"type": "ascii",
								"values": "J10",
								"group": [
									{
										"id": "value141",
										"label": "Value141",
										"type": "ascii",
										"values": "K11",
										"group": [
											{ "id": "value1411", "label": "Value1411", "type": "ascii", "values": "L12", "optional": true, "group": [
													{ "id": "value14131", "label": "Value14131", "type": "ascii", "values": "lTwelve1" },
													{ "id": "value14132", "label": "Value14132", "type": "ascii", "values": "lTwelve2" }
												]
											},
											{ "id": "value1412", "label": "Value1412", "type": "ascii", "values": "M13" },
											{ "id": "value1413", "label": "Value1413", "type": "uint16", "group": [
													{ "id": "value14131", "label": "Value14131", "type": "ascii", "values": "O15" },
													{ "id": "value14132", "label": "Value14132", "type": "ascii", "values": "P16" }
												]
											}
										]
									},
									{
										"id": "value142",
										"label": "Value142",
										"type": "ascii",
										"values": "Q17",
										"ordered": false,
										"group": [
											{ "id": "value1421", "label": "Value1421", "type": "ascii", "values": "Q171" },
											{ "id": "value1422", "label": "Value1422", "type": "ascii", "values": "Q172", "group": [
													{ "id": "value14221", "label": "Value14221", "type": "ascii", "values": "Q1721" }
												]
											},
											{ "id": "value1423", "label": "Value1423", "type": "ascii", "values": "Q173" }
										]
									}
								]
							},
							{
								"id": "value15",
								"label": "Value15",
								"type": "ascii",
								"values": "R18",
								"required": "value1413 == 12596",
								"ordered": false,
								"repeated": [
									{ "min":"2" }
								],
								"group": [
									{ "id": "value151", "label": "Value151", "type": "ascii", "values": "R181" },
									{ "id": "value152", "label": "Value152", "type": "ascii", "values": "R182", "optional": true },
									{ "id": "value153", "label": "Value153", "type": "ascii", "values": "R183" }
								]
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
	BOOST_CHECK_EQUAL( node->getId(),                                                                   "value1"    );
	BOOST_CHECK_EQUAL( node->firstChild()->getId(),                                                     "value14"   );
	BOOST_CHECK_EQUAL( node->firstChild()->firstChild()->getId(),                                       "value141"  );
	BOOST_CHECK_EQUAL( node->firstChild()->firstChild()->firstChild()->getId(),                         "value1411" );
	BOOST_CHECK_EQUAL( node->firstChild()->firstChild()->firstChild()->next()->getId(),                 "value1412" );
	BOOST_CHECK_EQUAL( node->firstChild()->firstChild()->firstChild()->next()->next()->getId(),         "value1413" );
	BOOST_CHECK_EQUAL( node->firstChild()->firstChild()->next()->getId(),                               "value142"  );
	BOOST_CHECK_EQUAL( node->firstChild()->firstChild()->next()->firstChild()->getId(),                 "value1421" );
	BOOST_CHECK_EQUAL( node->firstChild()->firstChild()->next()->firstChild()->next()->getId(),         "value1422" );
	BOOST_CHECK_EQUAL( node->firstChild()->firstChild()->next()->firstChild()->next()->next()->getId(), "value1423" );
	BOOST_CHECK_EQUAL( node->firstChild()->next()->getId(),                                             "value15"   );
	BOOST_CHECK_EQUAL( node->firstChild()->next()->firstChild()->getId(),                               "value151"  );
	BOOST_CHECK_EQUAL( node->firstChild()->next()->firstChild()->next()->getId(),                       "value152"  );
	BOOST_CHECK_EQUAL( node->firstChild()->next()->firstChild()->next()->next()->getId(),               "value153"  );
	BOOST_CHECK_EQUAL( node->firstChild()->next()->next()->getId(),                                     "valueEnd"  );

	Comparator comp;

	std::stringbuf buffer;
	file_reader::FileReader file( &buffer );

	std::string str = "A1";
	str += "J10K11M1314O15P16Q17Q173Q172Q1721";
	str += "R18R182R183R181R18R183R181";
	str += "END";

	buffer.sputn( str.c_str(), str.size() );
	BOOST_CHECK_EQUAL( file.getLength(), str.size() );

	report_generator::Report report;

	comp.check( spec, file, report );

	BOOST_CHECK_EQUAL( file.isEndOfFile(), true );

	report.print();
	report.writeXml( "test.xml" );
}

BOOST_AUTO_TEST_CASE( comparator_test_comparator_3 )
{
	LOG_INFO( "\n>>> comparator_test_comparator 3 <<<" );
	std::string jsonString = R"*(
			{
				"header": [
					{
						"id": "value1",
						"label": "Value1",
						"type": "ascii",
						"values": "A1",
						"groupSize": "12",
						"group": [
							{ "id": "value11", "label": "Value11", "type": "ascii", "values": "S19" },
							{ "id": "value12", "label": "Value12", "type": "ascii", "values": "T20" },
							{ "id": "value13", "label": "Value13", "type": "ascii", "values": "U21" }
						]
					},
					{
						"id": "valueEnd",
						"label": "ValueEnd",
						"type": "ascii",
						"values": "END"
					}
				]
			}
		)*";

	spec_reader::Specification spec;
	spec.setFromString( jsonString );
	std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
	BOOST_CHECK_EQUAL( node->getId(),                               "value1"   );
	BOOST_CHECK_EQUAL( node->firstChild()->getId(),                 "value11"  );
	BOOST_CHECK_EQUAL( node->firstChild()->next()->getId(),         "value12"  );
	BOOST_CHECK_EQUAL( node->firstChild()->next()->next()->getId(), "value13"  );
	BOOST_CHECK_EQUAL( node->next()->getId(),                       "valueEnd" );

	Comparator comp;

	std::stringbuf buffer;
	file_reader::FileReader file( &buffer );

	std::string str = "A1";
	str += "S19T20U21";
	str += "END";

	buffer.sputn( str.c_str(), str.size() );
	BOOST_CHECK_EQUAL( file.getLength(), str.size() );

	report_generator::Report report;

	BOOST_CHECK_THROW( comp.check( spec, file, report ), std::runtime_error );
}


BOOST_AUTO_TEST_SUITE_END()
