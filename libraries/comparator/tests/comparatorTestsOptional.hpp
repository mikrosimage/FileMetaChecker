
BOOST_AUTO_TEST_SUITE( comparator_test_suite_optional )

BOOST_AUTO_TEST_CASE( comparator_comparator_validation_optional )
{
	LOG_WARNING( ">>> comparator_comparator_validation_optional <<<" );
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
					"id": "root",
					"label": "Root",
					"type": "ascii",
					"values": "root",
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
							"values": "1",
							"optional": true
						},
						{
							"id": "element2",
							"label": "Element 2",
							"type": "ascii",
							"values": "2",
							"optional": true,
							"group": [
								{
									"id": "element2.1",
									"label": "Element 2.1",
									"type": "hexa",
									"values": "20",
									"optional": true,
									"repeated": "2"
								},
								{
									"id": "element2.2",
									"label": "Element 2.2",
									"type": "hexa",
									"values": "61",
									"optional": true,
									"repeated": [
										{ "min": 2, "max": 4 }
									]
								},
								{
									"id": "element2.3",
									"label": "Element 2.3",
									"type": "hexa",
									"values": "62",
									"optional": true,
									"repeated": [
										{ "min": 2, "max": 4 }
									]
								},
								{
									"id": "element2.4",
									"label": "Element 2.4",
									"type": "ascii",
									"values": "*",
									"optional": true,
									"repeated": "2"
								},
								{
									"id": "element2.5",
									"label": "Element 2.5",
									"type": "hexa",
									"values": "63",
									"optional": true,
									"repeated": [
										{ "min": 2, "max": 4 }
									]
								},
								{
									"id": "element2.6",
									"label": "Element 2.6",
									"type": "hexa",
									"values": "64",
									"optional": true,
									"repeated": [
										{ "min": 2, "max": 4 }
									],
									"group": [
										{
											"id": "element2.6.1",
											"label": "Element 2.6.1",
											"type": "hexa",
											"values": "66",
											"optional": true,
											"repeated": "3"
										}
									]
								},
								{
									"id": "element2.7",
									"label": "Element 2.7",
									"type": "ascii",
									"values": "c", 
									"optional": true
								}
							]
						},
						{
							"id": "elementEndGroup",
							"label": "Element End Group",
							"type": "ascii",
							"values": "end"
						}
					]
				},
				{
					"id": "root2",
					"label": "Root 2",
					"type": "ascii",
					"values": "again",
					"optional": true,
					"repeated": [
						{ "min": 3, "max": 4 }
					]
				},
				{
					"id": "root3",
					"label": "Root 3",
					"type": "ascii",
					"values": "re",
					"optional": true,
					"repeated": 4
				},
				{
					"id": "elementEnd",
					"label": "Element End",
					"type": "ascii",
					"values": "end",
					"optional": true
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		std::string str = "root";
		str += " ";
		str += "1";
		str += "2  bbbb**c";
		str += "dfffddfffdfffc";
		str += "end";
		str += "again";
		str += "again";
		str += "again";
		str += "again";
		str += "again";
		str += "re";
		str += "re";
		str += "re";
		str += "   ";
		buffer.str( str );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		bpt::ptree xmlTransReport = tr.transformTree( rg::Transform::eReportTypeXml );
		rg::Export exporter( xmlTransReport );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.get< rg::Export::eExportTypeXml >() );

		BOOST_CHECK_EQUAL( xmlTransReport.end()->second.data(), "root3" );
		BOOST_CHECK_EQUAL( xmlTransReport.back().second.data(), "re" );

		std::istringstream  xmlStream( exporter.get< rg::Export::eExportTypeXml >() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), 9 );
	}
}


BOOST_AUTO_TEST_CASE( comparator_comparator_validation_optional_empty )
{
	LOG_WARNING( ">>> comparator_comparator_validation_optional_empty <<<" );
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
					"id": "root",
					"label": "Root",
					"type": "ascii",
					"values": "root",
					"optional": true
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		std::string str = "";
		buffer.str( str );
		fr::FileReader file( &buffer );

		BOOST_CHECK_THROW( Comparator comp( &file, specList ), std::runtime_error );
	}
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
					"id": "root",
					"label": "Root",
					"type": "ascii",
					"values": "root",
					"optional": true
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		std::string str = " ";
		buffer.str( str );
		fr::FileReader file( &buffer );
		
		Comparator comp( &file, specList );

		rg::Report report;
		comp.compare( "test", report );
	
		rg::Transform tr( report );
		BOOST_CHECK_THROW( tr.transformTree( rg::Transform::eReportTypeXml ), std::runtime_error );
	}
}

BOOST_AUTO_TEST_SUITE_END()
