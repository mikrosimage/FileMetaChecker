
BOOST_AUTO_TEST_SUITE( report_test_report )

BOOST_AUTO_TEST_CASE( report_report )
{
	LOG_INFO( ">>> report_report <<<" );
	{
		LOG_INFO( ">>> report_report <<<" );
		Report report;

		std::shared_ptr< ben::Number< int > > numPtr1( new ben::Number< int > );
		std::shared_ptr< ben::Number< int > > numPtr2( new ben::Number< int > );
		std::shared_ptr< bed::Data > dataPtr( new bed::Data );
		report.addBasicElement( numPtr1 );
		report.addBasicElement( dataPtr );
		report.addBasicElement( numPtr2 );

		BOOST_CHECK_EQUAL( report.getSize(), 3 );

		ReportNode node = report.getFirstNode();
		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );
		BOOST_CHECK_EQUAL( node.getElementPointer()->getType(), be::Element::eTypeNumber );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      1 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );
		BOOST_CHECK_EQUAL( node.getElementPointer()->getType(), be::Element::eTypeData );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      2 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );
		BOOST_CHECK_EQUAL( node.getElementPointer()->getType(), be::Element::eTypeNumber );
	}
	{
		ReportTree tree;
		ReportTree group;
		ReportTree node1, node2, node3, node4, node5;
		std::shared_ptr< ben::Number< int > >  numPtr1( new ben::Number< int > );
		std::shared_ptr< ben::Number< int > >  numPtr2( new ben::Number< int > );
		std::shared_ptr< bed::Data >          dataPtr1( new bed::Data );
		std::shared_ptr< bed::Data >          dataPtr2( new bed::Data );
		std::shared_ptr< bed::Data >          dataPtr3( new bed::Data );

		node1.add( "0", ( std::shared_ptr< be::Element > )  numPtr1 );
		node2.add( "1", ( std::shared_ptr< be::Element > )  numPtr2 );
		node3.add( "2", ( std::shared_ptr< be::Element > ) dataPtr1 );
		node4.add( "3", ( std::shared_ptr< be::Element > ) dataPtr2 );
		node5.add( "4", ( std::shared_ptr< be::Element > ) dataPtr3 );

		node2.add_child( "group.", node5 );
		
		tree.add_child( "report.", node1 );
		tree.add_child( "report.", node2 );
		tree.add_child( "report.", node3 );
		tree.add_child( "report.", node4 );

		Report report( tree );

		BOOST_CHECK_EQUAL( report.getSize(), 4 );

		ReportNode node = report.getFirstNode();
		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );
		BOOST_CHECK_EQUAL( node.getElementPointer()->getType(), be::Element::eTypeNumber );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      1 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );
		BOOST_CHECK_EQUAL( node.getElementPointer()->getType(), be::Element::eTypeNumber );

		ReportNode child = node.firstChild();
		BOOST_CHECK_EQUAL( child.getIndex(),      0 );
		BOOST_CHECK_EQUAL( child.getIndexTotal(), 1 );
		BOOST_CHECK_EQUAL( child.parent()->getIndex(),      1 );
		BOOST_CHECK_EQUAL( child.parent()->getIndexTotal(), 4 );
		BOOST_CHECK_EQUAL( child.getElementPointer()->getType(), be::Element::eTypeData );

		node = child.parent()->next();
		BOOST_CHECK_EQUAL( node.getIndex(),      2 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );
		BOOST_CHECK_EQUAL( node.getElementPointer()->getType(), be::Element::eTypeData );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      3 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );
		BOOST_CHECK_EQUAL( node.getElementPointer()->getType(), be::Element::eTypeData );
		BOOST_CHECK_THROW( node.parent(), std::runtime_error );
	}
}

BOOST_AUTO_TEST_SUITE_END()

