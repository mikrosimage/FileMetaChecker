
// contains range, map, displayType, values and count tests

BOOST_AUTO_TEST_SUITE( comparator_test_suite_features )

BOOST_AUTO_TEST_CASE( comparator_comparator_validation_1 )
{
	LOG_WARNING( ">>> comparator_comparator_validation_1 <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1"]
			},
			"header": [
				{
					"id": "file",
					"label": "It is written FILE",
					"type": "ascii",
					"values": "FILE"
				},
				{
					"id": "space",
					"label": "It is a space character",
					"type": "uint8",
					"range" : [
						{ "min": 31, "max": 33 }
					]
				},
				{
					"id": "reader",
					"label": "It is a written reader",
					"type": "ascii",
					"values": "reader"
				},
				{
					"id": "number",
					"label": "It is a number",
					"type": "uint8",
					"endian": "little",
					"map" : [
						{ "32" : "Value of the number" }
					]
				},
				{
					"id": "otherNumber",
					"label": "It is an other number",
					"type": "uint8",
					"map" : [
						{ "33" : "Something" }
					],
					"range" : [
						{ "min": 31, "max": 33 }
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
		buffer.str( "FILE reader  " );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		rg::Export exporter( tr.transformTree( rg::Transform::eReportTypeXml ) );
		
		LOG_INFO( "\n==== REPORT ====" );
		// exporter.writeXmlFile( "test_validation_1.xml" );
		LOG_INFO( exporter.getXmlString() );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );

		std::vector< std::string >  xmlIds;
		std::vector< std::string > jsonIds;

		fillVectorXml( xmlReport, xmlIds, "<xmlattr>.id" );
		fillVectorJson( jsonReport.get_child( "header" ), jsonIds, "id" );

		BOOST_CHECK_EQUAL( xmlIds.size(), jsonIds.size() );
		for( size_t i = 0; i < xmlIds.size(); ++i )
			BOOST_CHECK_EQUAL( xmlIds.at(i), jsonIds.at(i) );
	}
}

BOOST_AUTO_TEST_CASE( comparator_comparator_validation_2 )
{
	LOG_WARNING( ">>> comparator_comparator_validation_2 <<<" );
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
					"label": "It is written FILE",
					"type": "ascii",
					"values": "FILE"
				},
				{
					"id": "space",
					"label": "It is a space character",
					"type": "uint8",
					"range": [
						{ "min": 31, "max": 33 }
					]
				},
				{
					"id": "reader",
					"label": "It is a written reader",
					"type": "ascii",
					"values": "reader"
				},
				{
					"id": "number",
					"label": "It is a number",
					"type": "uint8",
					"endian": "little",
					"map": [
						{ "32" : "Value of the number" }
					]
				},
				{
					"id": "hexaSpace",
					"label": "It is an other space character",
					"type": "hexa",
					"values": "20"
				},
				{
					"id": "rawData",
					"label": "There is some raw data",
					"type": "raw",
					"count": "7",
					"displayType": "ascii"
				},
				{
					"id": "hexaToAscii",
					"label": "There are hexa characters",
					"type": "hexa",
					"values": "40202e",
					"displayType": "ascii"
				},
				{
					"id": "hexaToRaw",
					"label": "There are other hexa characters",
					"type": "hexa",
					"values": "2f2c2b",
					"displayType": "raw"
				},
				{
					"id": "zeroToHexa",
					"label": "It is a zero character",
					"type": "ascii",
					"values": "0",
					"displayType": "hexa"
				},
				{
					"id": "digitsToHexa",
					"label": "There are some digit characters",
					"type": "ascii",
					"count": "5",
					"displayType": "hexa"
				},
				{
					"id": "charactersToRaw",
					"label": "There are some characters",
					"type": "ascii",
					"count": "8",
					"displayType": "raw"
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		buffer.str( "FILE reader  nothing@ ./,+0031560?12!3#4" );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		rg::Export exporter( tr.transformTree( rg::Transform::eReportTypeXml ) );
		
		LOG_INFO( "\n==== REPORT ====" );
		// exporter.writeXmlFile( "test_validation_2.xml" );
		LOG_INFO( exporter.getXmlString() );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );

		std::vector< std::string >  xmlIds;
		std::vector< std::string > jsonIds;

		fillVectorXml( xmlReport, xmlIds, "<xmlattr>.id" );
		fillVectorJson( jsonReport.get_child( "header" ), jsonIds, "id" );

		BOOST_CHECK_EQUAL( xmlIds.size(), jsonIds.size() );
		for( size_t i = 0; i < xmlIds.size(); ++i )
			BOOST_CHECK_EQUAL( xmlIds.at(i), jsonIds.at(i) );
	}
}


BOOST_AUTO_TEST_CASE( comparator_comparator_validation_count_expression )
{
	LOG_WARNING( ">>> comparator_comparator_validation_count_expression <<<" );
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
					"id": "length",
					"label": "Sixteen times length of FILE",
					"type": "uint8"
				},
				{
					"id": "file",
					"label": "FILE",
					"type": "ascii",
					"count": "length / 16"
				},
				{
					"id": "size",
					"label": "Eleven times size HEY",
					"type": "uint8"
				},
				{
					"id": "hey",
					"label": "HEY",
					"type": "ascii",
					"count": "size / 11"
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		buffer.str( "@FILE!HEY" );
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

		std::vector< std::string >  xmlIds;
		std::vector< std::string > jsonIds;

		fillVectorXml( xmlReport, xmlIds, "<xmlattr>.id" );
		fillVectorJson( jsonReport.get_child( "header" ), jsonIds, "id" );

		BOOST_CHECK_EQUAL( xmlIds.size(), jsonIds.size() );
		for( size_t i = 0; i < xmlIds.size(); ++i )
			BOOST_CHECK_EQUAL( xmlIds.at(i), jsonIds.at(i) );
	}
}

BOOST_AUTO_TEST_SUITE_END()
