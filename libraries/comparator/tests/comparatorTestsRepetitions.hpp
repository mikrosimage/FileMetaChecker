
BOOST_AUTO_TEST_SUITE( comparator_test_suite_repetition )

BOOST_AUTO_TEST_CASE( comparator_comparator_validation_repetition_expr_simple )
{
	LOG_WARNING( ">>> comparator_comparator_validation_repetition_expr_simple <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1"
				]
			},
			"header": [
				{
					"id": "file",
					"label": "FILE",
					"type": "ascii",
					"values": "group",
					"group": [
						{
							"id": "element1",
							"label": "Element 1",
							"type": "ascii",
							"values": "digit",
							"repeated": "3"
						},
						{
							"id": "twoTimesElement",
							"label": "Two Times Element",
							"type": "ascii",
							"values": "word",
							"repeated": "2"
						},
						{
							"id": "element2",
							"label": "Element 2",
							"type": "ascii",
							"values": "digit"
						}
					]
				},
				{
					"id": "end",
					"label": "End",
					"type": "ascii",
					"values": "end",
					"repeated": "2"
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		buffer.str( "groupdigitdigitdigitwordworddigitendend" );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		rg::Export exporter( tr.transformTree( rg::Transform::eReportTypeXml ) );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.getXmlString() );
	}
}

BOOST_AUTO_TEST_CASE( comparator_comparator_validation_repetition_expr_group )
{
	LOG_WARNING( ">>> comparator_comparator_validation_repetition_expr_group <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1"
				]
			},
			"header": [
				{
					"id": "file",
					"label": "FILE",
					"type": "ascii",
					"values": "group",
					"group": [
						{
							"id": "repetition",
							"label": "Repetition",
							"type": "uint8"
						},
						{
							"id": "element1",
							"label": "Element 1",
							"type": "ascii",
							"values": "digit",
							"repeated": "2",
							"group": [
								{
									"id": "thingBegin",
									"label": "Thing Begin",
									"type": "ascii",
									"values": "begin",
									"repeated": "3"
								},
								{
									"id": "thing",
									"label": "Thing",
									"type": "ascii",
									"values": "thing",
									"repeated": "2"
								},
								{
									"id": "thingEnd",
									"label": "Thing End",
									"type": "ascii",
									"values": "end",
									"group": [
										{
											"id": "end1",
											"label": "End 1",
											"type": "ascii",
											"values": "end1",
											"repeated": "repetition / 16"
										}
									]
								}
							]
						},
						{
							"id": "elementEnd",
							"label": "Element End",
							"type": "ascii",
							"values": "end"
						}
					]
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		std::string str = "group";
		str += " ";
		str += "digit";
		str += "beginbeginbegin";
		str += "thingthing";
		str += "end";
		str += "end1end1";
		str += "digit";
		str += "beginbeginbegin";
		str += "thingthing";
		str += "end";
		str += "end1end1";
		str += "end";
		buffer.str( str );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		rg::Export exporter( tr.transformTree( rg::Transform::eReportTypeXml ) );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.getXmlString() );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );
	}
}

BOOST_AUTO_TEST_CASE( comparator_comparator_validation_repetition_range_simple )
{
	LOG_WARNING( ">>> comparator_comparator_validation_repetition_range_simple <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1"
				]
			},
			"header": [
				{
					"id": "file",
					"label": "FILE",
					"type": "ascii",
					"values": "group",
					"group": [
						{
							"id": "repetition",
							"label": "Repetition",
							"type": "uint8"
						},
						{
							"id": "element1",
							"label": "Element 1",
							"type": "ascii",
							"values": "n",
							"repeated": [
								{ "min": "2", "max": "4" }
							]
						},
						{
							"id": "element2",
							"label": "Element 2",
							"type": "ascii",
							"values": "?",
							"repeated": [
								{ "min": "2", "max": "repetition / 8" }
							]
						},
						{
							"id": "element3",
							"label": "Element 3",
							"type": "ascii",
							"values": "digit",
							"repeated": [
								{ "min": "2" }
							]
						},
						{
							"id": "element4",
							"label": "Element 4",
							"type": "ascii",
							"values": "max",
							"repeated": [
								{ "max": "4" }
							]
						},
						{
							"id": "elementEnd",
							"label": "Element End",
							"type": "ascii",
							"values": "end",
							"repeated": [
								{ "min": "2" }
							]
						}
					]
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		std::string str = "group";
		str += " ";
		str += "nnn";
		str += "????";
		str += "digit";
		str += "digit";
		str += "digit";
		str += "max";
		str += "max";
		str += "endendend";
		buffer.str( str );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		rg::Export exporter( tr.transformTree( rg::Transform::eReportTypeXml ) );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.getXmlString() );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );
	}
}

BOOST_AUTO_TEST_CASE( comparator_comparator_validation_repetition_range_group )
{
	LOG_WARNING( ">>> comparator_comparator_validation_repetition_range_group <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1"
				]
			},
			"header": [
				{
					"id": "file",
					"label": "FILE",
					"type": "ascii",
					"values": "group",
					"group": [
						{
							"id": "repetition",
							"label": "Repetition",
							"type": "uint8"
						},
						{
							"id": "element1",
							"label": "Element 1",
							"type": "ascii",
							"values": "n",
							"repeated": [
								{ "min": "2", "max": "4" }
							],
							"group": [
								{
									"id": "element2",
									"label": "Element 2",
									"type": "ascii",
									"values": "?",
									"repeated": [
										{ "min": "2", "max": "4" }
									],
									"group": [
										{
											"id": "element2.1",
											"label": "Element 2.1",
											"type": "ascii",
											"values": "!",
											"repeated": [
												{ "min": "2", "max": "4" }
											],
											"group": [
												{
													"id": "element2.1.1",
													"label": "Element 2.1.1",
													"type": "ascii",
													"values": "*",
													"repeated": [
														{ "min": "1", "max": "3" }
													]
												}
											]
										}
									]
								},
								{
									"id": "element3",
									"label": "Element 3",
									"type": "ascii",
									"values": "digit",
									"repeated": [
										{ "min": "1", "max": "3" }
									],
									"group": [
										{
											"id": "element3.1",
											"label": "Element 3.1",
											"type": "ascii",
											"values": "°"
										},
										{
											"id": "element3.2",
											"label": "Element 3.2",
											"type": "ascii",
											"values": "+",
											"repeated": [
												{ "min": "2", "max": "4" }
											],
											"group": [
												{
													"id": "element3.2.1",
													"label": "Element 3.2.1",
													"type": "ascii",
													"values": "-",
													"repeated": [
														{ "min": "1", "max": "3" }
													]
												}
											]
										},
										{
											"id": "element3.3",
											"label": "Element 3.3",
											"type": "ascii",
											"values": "/"
										}
									]
								},
								{
									"id": "element4",
									"label": "Element 4",
									"type": "ascii",
									"values": "max",
									"repeated": [
										{ "min": "1", "max": "5" }
									],
									"group": [
										{
											"id": "element4.1",
											"label": "Element 4.1",
											"type": "ascii",
											"values": "$",
											"repeated": [
												{ "min": "2", "max": "4" }
											],
											"group": [
												{
													"id": "element4.1.1",
													"label": "Element 4.1.1",
													"type": "ascii",
													"values": "£",
													"repeated": [
														{ "min": "1", "max": "3" }
													]
												}
											]
										}
									]
								}
							]
						},
						{
							"id": "elementEnd",
							"label": "Element End",
							"type": "ascii",
							"values": "end",
							"repeated": [
								{ "min": "2" }
							]
						}
					]
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		std::string str = "group";
		str += " ";
		str += "n";
		str += "?!*!*";
		str += "?!*!*!**";
		str += "?!*!**!**!*";
		str += "digit°+-+--+-+--/";
		str += "digit°+-+---/";
		str += "max$£$£££";
		str += "max$£$£££$££";
		str += "max$££$£$£";
		str += "max$£$£££$££";
		str += "max$£$£££";
		str += "n";
		str += "?!**!*!**!*";
		str += "?!*!***";
		str += "digit°+--+-+--/";
		str += "max$£££$£$££$£";
		str += "n";
		str += "?!*!**!*";
		str += "?!*!**";
		str += "?!**!*!*";
		str += "?!***!**";
		str += "digit°+--+---/";
		str += "max$££$£$££";
		str += "n";
		str += "?!*!***!**!*";
		str += "?!***!*!**!*";
		str += "digit°+-+---/";
		str += "digit°+--+-+--/";
		str += "digit°+--+-+-+-/";
		str += "max$££$£$££";
		str += "max$£$£££";
		str += "max$£$££$£$£";
		str += "endendend";
		buffer.str( str );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		rg::Export exporter( tr.transformTree( rg::Transform::eReportTypeXml ) );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.getXmlString() );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );
	}
}

BOOST_AUTO_TEST_CASE( comparator_comparator_validation_repetition_range_group_complex )
{
	LOG_WARNING( ">>> comparator_comparator_validation_repetition_range_group_complex <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1"
				]
			},
			"header": [
				{
					"id": "file",
					"label": "FILE",
					"type": "ascii",
					"values": "group",
					"group": [
						{
							"id": "repetition",
							"label": "Repetition",
							"type": "uint8"
						},
						{
							"id": "elementMinOut",
							"label": "Element Min Out",
							"type": "ascii",
							"values": "n",
							"repeated": [
								{ "min": "2", "max": "4" }
							]
						},
						{
							"id": "elementMaxOut",
							"label": "Element Max Out",
							"type": "ascii",
							"values": "p",
							"repeated": [
								{ "min": "2", "max": "4" }
							]
						},
						{
							"id": "elementMulti",
							"label": "Element Multi",
							"type": "ascii",
							"values": "m",
							"repeated": [
								{ "min": "4" },
								{ "max": "2" }
							]
						},
						{
							"id": "elementMultiOut",
							"label": "Element Multi Out",
							"type": "ascii",
							"values": "o",
							"repeated": [
								{ "min": "4" },
								{ "max": "2" }
							]
						},
						{
							"id": "elementDoubleMulti",
							"label": "Element Double Multi",
							"type": "ascii",
							"values": "d",
							"repeated": [
								{ "min": 5, "max": 6 },
								{ "min": 2, "max": 3 }
							]
						},
						{
							"id": "elementDoubleMultiOut",
							"label": "Element Double Multi Out",
							"type": "ascii",
							"values": "u",
							"repeated": [
								{ "min": 5, "max": 6 },
								{ "min": 2, "max": 3 }
							]
						},
						{
							"id": "elementEnd",
							"label": "Element End",
							"type": "ascii",
							"values": "end",
							"repeated": [
								{ "min": "2" }
							]
						}
					]
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		std::string str = "group";
		str += " ";
		str += "n";
		str += "ppppp";
		str += "mmmm";
		str += "ooo";
		str += "dddddd";
		str += "uuuu";
		str += "end";
		buffer.str( str );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		rg::Export exporter( tr.transformTree( rg::Transform::eReportTypeXml ) );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.getXmlString() );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );
	}
}

BOOST_AUTO_TEST_SUITE_END()
