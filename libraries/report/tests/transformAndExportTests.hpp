
BOOST_AUTO_TEST_SUITE( report_test_transform_and_export )

BOOST_AUTO_TEST_CASE( report_transform_and_export )
{
	LOG_INFO( ">>> report_transform_and_export <<<" );
	Report elementReport;

	std::shared_ptr< ben::Number< int >   > numPtr1( new ben::Number< int > );
	std::shared_ptr< ben::Number< short > > numPtr2( new ben::Number< short > );
	std::shared_ptr< bed::Data            > dataPtr( new bed::Data );

	numPtr1->setId(   "intElem" );
	numPtr2->setId( "shortElem" );
	dataPtr->setId(  "dataElem" );

	ReportNode node1 = elementReport.addRootElement( numPtr1 );
	node1 = node1.appendNext( dataPtr );

	ReportNode child1 = node1.appendChild( numPtr2 );
	child1 = child1.appendNext( numPtr1 );
	child1.appendChild( dataPtr );

	elementReport.addRootElement( numPtr2 );
	
	{
		Transform tr;
		tr.setBasicElementReport( elementReport );
	}
	{
		LOG_INFO( ">>> report_transform_transform <<<" );
		Transform tr( elementReport );
		tr.transformTree( Transform::eReportTypeXml );
	}
	{
		LOG_INFO( ">>> report_transform_and_export <<<" );
		Transform tr( elementReport );
		Export exporter( tr.transformTree( Transform::eReportTypeXml ) );
		exporter.writeXmlFile( "report.xml" );
		exporter.writeXmlFile( "report.xml", true );
	}
	{
		LOG_INFO( ">>> report_transform_and_export <<<" );
		Transform tr( elementReport );
		Export exporter( tr.transformTree( Transform::eReportTypeXml ) );
		LOG_INFO( exporter.getXmlString() );
	}
}


BOOST_AUTO_TEST_CASE( report_export_xml_json )
{
	LOG_INFO( ">>> report_export_xml_json <<<" );
	std::string jsonString =R"*(
	{
		"standard":
		{
			"id": "test",
			"extension": [
			"ext1",
			"ext2",
			"ext3"]
		},
		"header": [
			{
				"id": "test1",
				"label": "Test 1",
				"type": "unknown"
			}
		]
	})*";

	bpt::ptree tree;
	std::istringstream isstream( jsonString );
	bpt::read_json( isstream, tree );
	
	Export exporter( tree );
	LOG_INFO( exporter.getXmlString() );
	LOG_INFO( exporter.getXmlString( true ) );
	LOG_INFO( exporter.getJsonString() );
	LOG_INFO( exporter.getJsonString( true ) );
	exporter.writeJsonFile( "report.json" );
	exporter.writeJsonFile( "report.json", true );
}


BOOST_AUTO_TEST_SUITE_END()
