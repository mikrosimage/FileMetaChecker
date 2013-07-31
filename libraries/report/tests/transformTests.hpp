
BOOST_AUTO_TEST_SUITE( report_test_transform )

BOOST_AUTO_TEST_CASE( report_transform )
{
	LOG_INFO( ">>> report_transform <<<" );
	Report elementReport;

	std::shared_ptr< ben::Number< int >   > numPtr1( new ben::Number< int > );
	std::shared_ptr< bed::Data            > dataPtr( new bed::Data );
	std::shared_ptr< ben::Number< short > > numPtr2( new ben::Number< short > );

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

}

BOOST_AUTO_TEST_SUITE_END()
