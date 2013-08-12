
BOOST_AUTO_TEST_SUITE( report_test_report )

BOOST_AUTO_TEST_CASE( report_report )
{
	LOG_INFO( ">>> report_report <<<" );
	{
		Report report;

		std::shared_ptr< ben::Number< int > > numPtr( new ben::Number< int > );
		std::shared_ptr< bed::Data >          dataPtr( new bed::Data );

		BOOST_CHECK_EQUAL( report.getSize(), 0 );
		
		ReportNode node1 = report.addRootElement( numPtr );
		BOOST_CHECK_EQUAL( report.getSize(), 1 );
		BOOST_CHECK_EQUAL( node1.getIndex(), 0 );
		
		ReportNode node2 = report.addRootElement( dataPtr );
		BOOST_CHECK_EQUAL( report.getSize(), 2 );
		BOOST_CHECK_EQUAL( node2.getIndex(), 1 );
	}
}

BOOST_AUTO_TEST_CASE( report_report_and_node_report )
{
	LOG_INFO( ">>> report_report_and_node_report <<<" );
	{
		Report report;

		std::shared_ptr< ben::Number< int >   > numPtr1( new ben::Number< int > );
		std::shared_ptr< bed::Data            > dataPtr( new bed::Data );
		std::shared_ptr< ben::Number< short > > numPtr2( new ben::Number< short > );

		BOOST_CHECK_EQUAL( report.getSize(), 0 );
		
		ReportNode node1 = report.addRootElement( numPtr1 );
		BOOST_CHECK_EQUAL( report.getSize(), 1 );
		BOOST_CHECK_EQUAL( node1.getIndex(), 0 );
		BOOST_CHECK_EQUAL( node1.getSecond()->begin()->first, "0" );
		
		bool parent = true;
		if( node1.parent() == NULL ) parent = false;
		BOOST_CHECK_EQUAL( parent, false );


		node1 = node1.appendNext( dataPtr );
		BOOST_CHECK_EQUAL( report.getSize(), 2 );
		BOOST_CHECK_EQUAL( node1.getIndex(), 1 );
		BOOST_CHECK_EQUAL( node1.getSecond()->begin()->first, "1" );
		
		parent = true;
		if( node1.parent() == NULL ) parent = false;
		BOOST_CHECK_EQUAL( parent, false );


		ReportNode child1 = node1.appendChild( numPtr2 );
		BOOST_CHECK_EQUAL( report.getSize(), 2 );
		BOOST_CHECK_EQUAL( child1.getIndex(), 0 );
		BOOST_CHECK_EQUAL( child1.getSecond()->begin()->first, "1-0" );
		BOOST_CHECK_EQUAL( child1.parent()->getSecond()->begin()->first, "1" );
		
		parent = true;
		if( child1.parent()->parent() == NULL ) parent = false;
		BOOST_CHECK_EQUAL( parent, false );


		child1 = child1.appendNext( numPtr1 );
		BOOST_CHECK_EQUAL( report.getSize(), 2 );
		BOOST_CHECK_EQUAL( child1.getIndex(), 1 );
		BOOST_CHECK_EQUAL( child1.getSecond()->begin()->first, "1-1" );
		BOOST_CHECK_EQUAL( child1.parent()->getSecond()->begin()->first, "1" );
		parent = true;
		if( child1.parent()->parent() == NULL ) parent = false;
		BOOST_CHECK_EQUAL( parent, false );


		ReportNode child2 = child1.appendChild( dataPtr );
		BOOST_CHECK_EQUAL( report.getSize(), 2 );
		BOOST_CHECK_EQUAL( child2.getIndex(), 0 );
		BOOST_CHECK_EQUAL( child2.getSecond()->begin()->first, "1-1-0" );
		BOOST_CHECK_EQUAL( child2.parent()->getSecond()->begin()->first, "1-1" );
		BOOST_CHECK_EQUAL( child2.parent()->parent()->getSecond()->begin()->first, "1" );
		
		parent = true;
		if( child2.parent()->parent()->parent() == NULL ) parent = false;
		BOOST_CHECK_EQUAL( parent, false );
		

		ReportNode node2 = report.addRootElement( numPtr2 );
		BOOST_CHECK_EQUAL( report.getSize(), 3 );
		BOOST_CHECK_EQUAL( node2.getIndex(), 2 );
		BOOST_CHECK_EQUAL( node2.getSecond()->begin()->first, "2" );

		parent = true;
		if( node2.parent() == NULL ) parent = false;
		BOOST_CHECK_EQUAL( parent, false );
	}
}

BOOST_AUTO_TEST_SUITE_END()

