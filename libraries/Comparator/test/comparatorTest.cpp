#define BOOST_TEST_MODULE qc_comparator

#include <Comparator/Comparator.hpp>
#include <Common/log.hpp>
#include <BasicElement/Element.hpp>
#include <SpecReader/Specification.hpp>
#include <FileReader/FileReader.hpp>
#include <Report/Report.hpp>

#include <boost/test/unit_test.hpp>
// #include <boost/property_tree/ptree.hpp>
// #include <boost/property_tree/json_parser.hpp>

using namespace boost::unit_test;
namespace bpt = boost::property_tree;

using namespace comparator;

BOOST_AUTO_TEST_SUITE( comparator_test )

BOOST_AUTO_TEST_CASE( comparator_test_init )
{
	common::level = common::eLogTrace;
}

inline bool checkElementStatus( const report_generator::Report& report, const std::string id, const EStatus status )
{
	bool elementIsOk = false;
	for( std::shared_ptr< basic_element::Element > elem : report.getElementList() )
	{
		if( elem->_id == id && elem->_status == status )
			elementIsOk = true;
	}
	return elementIsOk;
}

BOOST_AUTO_TEST_CASE( comparator_test_comparator_1 )
{
	LOG_INFO( "\n>>> comparator_test_comparator 1 <<<" );
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
								"repetition": "10"
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
										"groupSize": "4",
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
										"values": "H8"
									},
									{
										"id": "value123",
										"label": "Value123",
										"type": "ascii",
										"values": "H85",
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
	str += "C3D4EXF6H8";
	str += "I9I92I93I91";
	str += "END";

	buffer.sputn( str.c_str(), str.size() );
	BOOST_CHECK_EQUAL( file.getLength(), str.size() );

	report_generator::Report report;

	comp.check( spec, file, report );

	BOOST_CHECK_EQUAL( report.get( "value1"      )->_status, eStatusInvalid  );
	BOOST_CHECK_EQUAL( report.get( "value11",  1 )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value11",  2 )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value11",  3 )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value11",  4 )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value11",  5 )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value11",  6 )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value11",  7 )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value11",  8 )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value11",  9 )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value11", 10 )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value12"     )->_status, eStatusInvalid  );
	BOOST_CHECK_EQUAL( report.get( "value121"    )->_status, eStatusInvalid  );
	BOOST_CHECK_EQUAL( report.get( "value1211"   )->_status, eStatusInvalid  );
	BOOST_CHECK_EQUAL( report.get( "value1212"   )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value122"    )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value13"     )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "valueEnd"    )->_status, eStatusValid    );

	BOOST_CHECK_EQUAL( file.isEndOfFile(), true );

}

BOOST_AUTO_TEST_CASE( comparator_test_comparator_2 )
{
	LOG_INFO( "\n>>> comparator_test_comparator 2 <<<" );
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
										"groupSize": "14",
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
								"requirement": "value1413 == 12596",
								"ordered": false,
								"repetition": [
									{ "min":"2" }
								],
								"group": [
									{ "id": "value151", "label": "Value151", "type": "ascii", "values": "R181" },
									{ "id": "value152", "label": "Value152", "type": "ascii", "values": "R182", "optional": true },
									{ "id": "value153", "label": "Value153", "type": "ascii", "values": "R183" }
								]
							},
							{
								"id": "value16",
								"label": "Value16",
								"type": "hexa",
								"values": "533230"
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
	BOOST_CHECK_EQUAL( node->firstChild()->next()->next()->getId(),                                     "value16"   );
	BOOST_CHECK_EQUAL( node->firstChild()->next()->next()->next()->getId(),                             "valueEnd"  );

	Comparator comp;

	std::stringbuf buffer;
	file_reader::FileReader file( &buffer );

	std::string str = "A1";
	str += "J10";
	str += "K11M1314O15P16000";
	str += "Q17Q173Q172Q1721";
	str += "R18R182R183R181R18R183R181";
	str += "S20";
	str += "END";

	buffer.sputn( str.c_str(), str.size() );
	BOOST_CHECK_EQUAL( file.getLength(), str.size() );

	report_generator::Report report;

	comp.check( spec, file, report );

	BOOST_CHECK_EQUAL( file.isEndOfFile(), true );


	BOOST_CHECK_EQUAL( report.get( "value1"    )->_status, eStatusInvalid  );
	BOOST_CHECK_EQUAL( report.get( "value14"   )->_status, eStatusInvalid  );
	BOOST_CHECK_EQUAL( report.get( "value141"  )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value1411" )->_status, eStatusSkip     );
	BOOST_CHECK_EQUAL( report.get( "value1412" )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value1413" )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value142"  )->_status, eStatusInvalid  );

	BOOST_CHECK( checkElementStatus( report, "value1421", eStatusSkip ) );
	BOOST_CHECK( checkElementStatus( report, "value1422", eStatusValid ) );

	BOOST_CHECK_EQUAL( report.get( "value1423" )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value15"   )->_status, eStatusValid    );
	
	BOOST_CHECK( checkElementStatus( report, "value151", eStatusValid ) );

	BOOST_CHECK_EQUAL( report.get( "value152"  )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value153"  )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "value16"   )->_status, eStatusValid    );
	BOOST_CHECK_EQUAL( report.get( "valueEnd"  )->_status, eStatusValid    );
}

BOOST_AUTO_TEST_CASE( comparator_test_comparator_3 )
{
	LOG_INFO( "\n>>> comparator_test_comparator 3 <<<" );
	std::string jsonString = R"*(
			{
				"content": [
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

BOOST_AUTO_TEST_CASE( comparator_test_comparator_4 )
{
	LOG_INFO( "\n>>> comparator_test_comparator 4 <<<" );
	std::string jsonString = R"*(
			{
				"content": [
					{
						"id": "value1",
						"label": "Value1",
						"type": "ascii",
						"values": "A1",
						"groupSize": "6",
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

BOOST_AUTO_TEST_CASE( comparator_test_comparator_word )
{
	LOG_INFO( "\n>>> comparator_test_comparator word <<<" );
	std::string jsonString = R"*(
			{
				"content": [
					{
						"id": "value1",
						"label": "Value1",
						"type": "ascii"
					},
					{
						"id": "value2",
						"label": "Value2",
						"type": "ascii",
						"endsWith": "x"
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
	BOOST_CHECK_EQUAL( node->getId(),                  "value1"   );
	BOOST_CHECK_EQUAL( node->next()->getId(),          "value2"  );
	BOOST_CHECK_EQUAL( node->next()->next()->getId(),  "valueEnd" );

	Comparator comp;

	std::stringbuf buffer;
	file_reader::FileReader file( &buffer );

	std::string str { 'H','E', 'L', 'L', 'O', '\0' };
	str += { 'W', 'O', 'R', 'L', 'D', 'x' };
	str += { 'E', 'N', 'D' };

	buffer.sputn( str.c_str(), str.size() );
	BOOST_CHECK_EQUAL( file.getLength(), str.size() );

	report_generator::Report report;

	comp.check( spec, file, report );

	BOOST_CHECK_EQUAL( file.isEndOfFile(), true );

	BOOST_CHECK_EQUAL( report.get( "value1"   )->_status, eStatusValid );
	BOOST_CHECK_EQUAL( report.get( "value2"   )->_status, eStatusValid );
	BOOST_CHECK_EQUAL( report.get( "valueEnd" )->_status, eStatusValid );
}

BOOST_AUTO_TEST_CASE( comparator_test_comparator_5 )
{
	LOG_INFO( "\n>>> comparator_test_comparator 5 <<<" );
	std::string jsonString = R"*(
			{
				"content": [
					{
						"id": "value0",
						"label": "Value0"
					},
					{
						"id": "value1",
						"label": "Value1",
						"group": [
							{
								"id": "value11",
								"label": "Value11",
								"type": "ascii",
								"values": "ONE"
							},
							{
								"id": "value12",
								"label": "Value12",
								"group": [
									{
										"id": "value121",
										"label": "Value121",
										"type": "ascii",
										"values": "end"
									}
								]
							}
						]
					},
					{
						"id": "value2",
						"label": "Value2",
						"group": [
							{
								"id": "value21",
								"label": "Value21",
								"type": "ascii",
								"values": "TWO"
							},
							{
								"id": "value22",
								"label": "Value22",
								"type": "ascii",
								"values": "end",
								"caseSensitive": true
							}
						]
					},
					{
						"id": "value3",
						"label": "Value3",
						"group": [
							{
								"id": "value31",
								"label": "Value31",
								"type": "ascii",
								"values": "THREE",
								"optional": true
							},
							{
								"id": "value32",
								"label": "Value32",
								"type": "ascii",
								"values": "EERHT",
								"optional": true
							}
						]
					},
					{
						"id": "value4",
						"label": "Value4",
						"optional": true,
						"group": [
							{
								"id": "value41",
								"label": "Value41",
								"type": "ascii",
								"values": "DNE"
							}
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

	Comparator comp;

	std::stringbuf buffer;
	file_reader::FileReader file( &buffer );

	std::string str = "ONEENDTWOENDEND";

	buffer.sputn( str.c_str(), str.size() );
	BOOST_CHECK_EQUAL( file.getLength(), str.size() );

	report_generator::Report report;

	comp.check( spec, file, report );

	BOOST_CHECK_EQUAL( file.isEndOfFile(), true );

	BOOST_CHECK_EQUAL( report.get( "value0"   )->_status, eStatusUnknown );

	BOOST_CHECK_EQUAL( report.get( "value1"   )->_status, eStatusUnknown );
	BOOST_CHECK_EQUAL( report.get( "value11"  )->_status, eStatusValid   );
	BOOST_CHECK_EQUAL( report.get( "value12"  )->_status, eStatusUnknown );
	BOOST_CHECK_EQUAL( report.get( "value121" )->_status, eStatusValid   );

	BOOST_CHECK_EQUAL( report.get( "value2"   )->_status, eStatusInvalid );
	BOOST_CHECK_EQUAL( report.get( "value21"  )->_status, eStatusValid   );
	BOOST_CHECK_EQUAL( report.get( "value22"  )->_status, eStatusInvalid );

	BOOST_CHECK_EQUAL( report.get( "value3"   )->_status, eStatusSkip    );
	BOOST_CHECK_EQUAL( report.get( "value31"  )->_status, eStatusSkip    );
	BOOST_CHECK_EQUAL( report.get( "value32"  )->_status, eStatusSkip    );

	BOOST_CHECK_EQUAL( report.get( "value4"   )->_status, eStatusSkip    );
	BOOST_CHECK_EQUAL( report.get( "value41"  )->_status, eStatusSkip    );

	BOOST_CHECK_EQUAL( report.get( "valueEnd" )->_status, eStatusValid   );
}

BOOST_AUTO_TEST_CASE( comparator_test_comparator_6 )
{
	LOG_INFO( "\n>>> comparator_test_comparator 6 <<<" );
	std::string jsonString = R"*(
			{
				"content": [
					{ "id": "value0", "label": "Value0" },
					{ "id": "value1", "label": "Value1", "ordered": false, "group": [
							{ "id": "value11", "label": "Value11", "type": "ascii", "values": "ONE" },
							{ "id": "value12", "label": "Value12", "group": [
									{ "id": "value121", "label": "Value121", "type": "ascii", "values": "end" } ] }
					] },
					{ "id": "value2", "label": "Value2", "group": [ 
							{ "id": "value21", "label": "Value21", "type": "ascii", "values": "TWO" },
							{ "id": "value22", "label": "Value22", "type": "ascii", "values": "end", "caseSensitive": true }
					] },
					{ "id": "value3", "label": "Value3", "group": [ 
							{ "id": "value31", "label": "Value31", "type": "ascii", "values": "THREE", "optional": true },
							{ "id": "value32", "label": "Value32", "type": "ascii", "values": "EERHT", "optional": true }
					] },
					{ "id": "value4", "label": "Value4", "optional": true, "group": [
							{ "id": "value41", "label": "Value41", "type": "ascii", "values": "DNE" }
					] },
					{ "id": "value5", "label": "Value5", "ordered": false, "group": [
							{ "id": "value51", "label": "Value51", "group": [
									{ "id": "value511", "label": "Value511", "type": "ascii", "values": "00" },
									{ "id": "value512", "label": "Value512", "type": "ascii", "values": "22" }
							] },
							{ "id": "value52", "label": "Value52", "group": [
									{ "id": "value521", "label": "Value521", "type": "ascii", "values": "11" },
									{ "id": "value522", "label": "Value522", "type": "ascii", "values": "22" }
							] }
					] },
					{ "id": "valueEnd", "label": "ValueEnd", "type": "ascii", "values": "END" }
				] }
		)*";

	spec_reader::Specification spec;
	spec.setFromString( jsonString );
	std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

	Comparator comp;

	std::stringbuf buffer;
	file_reader::FileReader file( &buffer );

	std::string str = "ENDONETWOEND";
	str += "11220022";
	str += "END";

	buffer.sputn( str.c_str(), str.size() );
	BOOST_CHECK_EQUAL( file.getLength(), str.size() );

	report_generator::Report report;

	comp.check( spec, file, report );

	BOOST_CHECK_EQUAL( file.isEndOfFile(), true );

	BOOST_CHECK_EQUAL( report.get( "value0"   )->_status, eStatusUnknown );

	BOOST_CHECK_EQUAL( report.get( "value1"   )->_status, eStatusUnknown   );
	BOOST_CHECK( checkElementStatus( report, "value11", eStatusValid ) );

	BOOST_CHECK_EQUAL( report.get( "value12"  )->_status, eStatusUnknown   );
	BOOST_CHECK_EQUAL( report.get( "value121" )->_status, eStatusValid   );

	BOOST_CHECK_EQUAL( report.get( "value2"   )->_status, eStatusInvalid );
	BOOST_CHECK_EQUAL( report.get( "value21"  )->_status, eStatusValid   );
	BOOST_CHECK_EQUAL( report.get( "value22"  )->_status, eStatusInvalid );

	BOOST_CHECK_EQUAL( report.get( "value3"   )->_status, eStatusSkip    );
	BOOST_CHECK_EQUAL( report.get( "value31"  )->_status, eStatusSkip    );
	BOOST_CHECK_EQUAL( report.get( "value32"  )->_status, eStatusSkip    );

	BOOST_CHECK_EQUAL( report.get( "value4"   )->_status, eStatusSkip    );
	BOOST_CHECK_EQUAL( report.get( "value41"  )->_status, eStatusSkip    );

	BOOST_CHECK_EQUAL( report.get( "value5"   )->_status, eStatusUnknown );
	BOOST_CHECK( checkElementStatus( report, "value51",  eStatusUnknown ) );
	BOOST_CHECK( checkElementStatus( report, "value511", eStatusValid   ) );
	BOOST_CHECK_EQUAL( report.get( "value512" )->_status, eStatusValid   );
	BOOST_CHECK_EQUAL( report.get( "value52"  )->_status, eStatusUnknown );
	BOOST_CHECK_EQUAL( report.get( "value521" )->_status, eStatusValid   );
	BOOST_CHECK_EQUAL( report.get( "value522" )->_status, eStatusValid   );

	BOOST_CHECK_EQUAL( report.get( "valueEnd" )->_status, eStatusValid   );
}

BOOST_AUTO_TEST_CASE( comparator_test_comparator_7 )
{
	LOG_INFO( "\n>>> comparator_test_comparator 7 <<<" );
	std::string jsonString = R"*(
			{
				"content": [
					{ "id": "value0", "label": "Value0" },
					{ "id": "value6", "label": "Value6", "ordered": false, "group": [
							{ "id": "value61", "label": "Value61", "group": [
									{ "id": "value611", "label": "Value611", "ordered": false, "group": [
											{ "id": "value6111", "label": "Value6111", "type": "ascii", "values": "123" },
											{ "id": "value6112", "label": "Value6112", "type": "ascii", "values": "321" }
									] },
									{ "id": "value612", "label": "Value612", "type": "ascii", "values": "22" },
									{ "id": "value613", "label": "Value613", "ordered": false, "group": [
											{ "id": "value6131", "label": "Value6131", "type": "ascii", "values": "456" },
											{ "id": "value6132", "label": "Value6132", "type": "ascii", "values": "654" }
									] }
							] },
							{ "id": "value62", "label": "Value62", "ordered": false, "group": [
									{ "id": "value621", "label": "Value621", "type": "ascii", "values": "11" },
									{ "id": "value622", "label": "Value622", "type": "ascii", "values": "22" }
							] }
					] },
					{ "id": "valueEnd", "label": "ValueEnd", "type": "ascii", "values": "END" }
				] }
		)*";

	spec_reader::Specification spec;
	spec.setFromString( jsonString );
	std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

	Comparator comp;

	std::stringbuf buffer;
	file_reader::FileReader file( &buffer );

	std::string str = "221132112322654456";
	str += "END";

	buffer.sputn( str.c_str(), str.size() );
	BOOST_CHECK_EQUAL( file.getLength(), str.size() );

	report_generator::Report report;

	comp.check( spec, file, report );

	BOOST_CHECK_EQUAL( file.isEndOfFile(), true );

	BOOST_CHECK_EQUAL( report.get(    "value0"   )->_status, eStatusUnknown );
	BOOST_CHECK_EQUAL( report.get(    "value6"   )->_status, eStatusUnknown );
	BOOST_CHECK( checkElementStatus( report, "value61",             eStatusUnknown ) );
	BOOST_CHECK( checkElementStatus( report, "value611",            eStatusUnknown ) );
	BOOST_CHECK( checkElementStatus( report, "value6111",           eStatusValid   ) );
	BOOST_CHECK( checkElementStatus( report, "value6112",           eStatusValid   ) );
	BOOST_CHECK_EQUAL( report.get(    "value612" )->_status, eStatusValid   );
	BOOST_CHECK( checkElementStatus( report, "value613",            eStatusUnknown ) );
	BOOST_CHECK( checkElementStatus( report, "value6131",           eStatusValid   ) );
	BOOST_CHECK( checkElementStatus( report, "value6132",           eStatusValid   ) );
	
	BOOST_CHECK_EQUAL( report.get(    "value62"  )->_status, eStatusUnknown );
	BOOST_CHECK( checkElementStatus( report, "value621",            eStatusValid ) );
	BOOST_CHECK( checkElementStatus( report, "value622",            eStatusValid   ) );

	BOOST_CHECK_EQUAL( report.get( "valueEnd" )->_status, eStatusValid   );
}

BOOST_AUTO_TEST_SUITE_END()
