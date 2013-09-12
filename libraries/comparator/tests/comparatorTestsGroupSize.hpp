
BOOST_AUTO_TEST_SUITE( comparator_test_suite_group_size )

BOOST_AUTO_TEST_CASE( comparator_comparator_validation_group_size )
{
	LOG_WARNING( ">>> comparator_comparator_validation_group_size <<<" );
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
					"id": "root1",
					"label": "Root1",
					"type": "ascii",
					"values": "root1",
					"groupSize": 10,
					"group": [
						{
							"id": "element1",
							"label": "Element 1",
							"type": "uint8"
						},
						{
							"id": "element2",
							"label": "Element 2",
							"type": "ascii",
							"values": "1"
						},
						{
							"id": "element3",
							"label": "Element 3",
							"type": "uint64"
						}
					]
				},
				{
					"id": "root2",
					"label": "Root2",
					"type": "ascii",
					"values": "root2",
					"groupSize": 1,
					"group": [
						{
							"id": "element4",
							"label": "Element 4",
							"type": "uint8"
						},
						{
							"id": "element5",
							"label": "Element 5",
							"type": "ascii",
							"values": "1"
						}
					]
				},
				{
					"id": "root3",
					"label": "Root3",
					"type": "ascii",
					"values": "root3",
					"groupSize": 3,
					"group": [
						{
							"id": "element6",
							"label": "Element 6",
							"type": "uint16"
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
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		std::string str = "root1";
		str += " ";
		str += "1";
		str += "00000001";
		str += "root2";
		str += " ";
		str += "1";
		str += "root3";
		str += "  ";
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
					"id": "root1",
					"label": "Root1",
					"type": "ascii",
					"values": "root1",
					"groupSize": 11,
					"group": [
						{
							"id": "element1",
							"label": "Element 1",
							"type": "uint8",
							"groupSize": 10,
							"group" : [
								{
									"id": "element1.1",
									"label": "Element 1.1",
									"type": "ascii",
									"values": "w"
								},
								{
									"id": "element12",
									"label": "Element 1.2",
									"type": "uint64",
									"groupSize": "element1 / 32",
									"group": [
										{
											"id": "element1.2.1",
											"label": "Element 1.2.1",
											"type": "ascii",
											"values": "0"
										}
									]
								}
							]
						}
					]
				},
				{
					"id": "root2",
					"label": "Root2",
					"type": "ascii",
					"values": "root2",
					"groupSize": 7,
					"group": [
						{
							"id": "element2",
							"label": "Element 2",
							"type": "hexa",
							"values": "61"
						},
						{
							"id": "element3",
							"label": "Element 3",
							"type": "ascii",
							"values": "1",
							"groupSize": 2,
							"repeated": 2,
							"group": [
								{
									"id": "element31",
									"label": "Element 3.1",
									"type": "uint16"
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
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		std::string str = "root1";
		str += " ";
		str += "w";
		str += "        ";
		str += "0";
		str += "root2";
		str += "a";
		str += "1";
		str += " A";
		// str += "1";
		// str += " A";
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
