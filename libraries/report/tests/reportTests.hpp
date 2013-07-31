
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
		BOOST_CHECK_THROW( node1.parent(), std::runtime_error );
		
		node1 = node1.appendNext( dataPtr );
		BOOST_CHECK_EQUAL( report.getSize(), 2 );
		BOOST_CHECK_EQUAL( node1.getIndex(), 1 );
		BOOST_CHECK_EQUAL( node1.getSecond()->begin()->first, "1" );
		BOOST_CHECK_THROW( node1.parent(), std::runtime_error );

		ReportNode child1 = node1.appendChild( numPtr2 );
		BOOST_CHECK_EQUAL( report.getSize(), 2 );
		BOOST_CHECK_EQUAL( child1.getIndex(), 0 );
		BOOST_CHECK_EQUAL( child1.getSecond()->begin()->first, "1-0" );
		BOOST_CHECK_EQUAL( child1.parent()->getSecond()->begin()->first, "1" );
		BOOST_CHECK_THROW( child1.parent()->parent(), std::runtime_error );

		child1 = child1.appendNext( numPtr1 );
		BOOST_CHECK_EQUAL( report.getSize(), 2 );
		BOOST_CHECK_EQUAL( child1.getIndex(), 1 );
		BOOST_CHECK_EQUAL( child1.getSecond()->begin()->first, "1-1" );
		BOOST_CHECK_EQUAL( child1.parent()->getSecond()->begin()->first, "1" );
		BOOST_CHECK_THROW( child1.parent()->parent(), std::runtime_error );

		ReportNode child2 = child1.appendChild( dataPtr );
		BOOST_CHECK_EQUAL( report.getSize(), 2 );
		BOOST_CHECK_EQUAL( child2.getIndex(), 0 );
		BOOST_CHECK_EQUAL( child2.getSecond()->begin()->first, "1-1-0" );
		BOOST_CHECK_EQUAL( child2.parent()->getSecond()->begin()->first, "1-1" );
		BOOST_CHECK_EQUAL( child2.parent()->parent()->getSecond()->begin()->first, "1" );
		BOOST_CHECK_THROW( child2.parent()->parent()->parent(), std::runtime_error );

		ReportNode node2 = report.addRootElement( numPtr2 );
		BOOST_CHECK_EQUAL( report.getSize(), 3 );
		BOOST_CHECK_EQUAL( node2.getIndex(), 2 );
		BOOST_CHECK_EQUAL( node2.getSecond()->begin()->first, "2" );
		BOOST_CHECK_THROW( node2.parent(), std::runtime_error );
	}
}

BOOST_AUTO_TEST_SUITE_END()

