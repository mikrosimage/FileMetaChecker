#include <boost/property_tree/json_parser.hpp>

BOOST_AUTO_TEST_SUITE( report_test_report_node )

BOOST_AUTO_TEST_CASE( report_report_node )
{
	LOG_INFO( ">>> report_report_node <<<" );
	{
		ReportTree tree;
		ReportTree node1, node2;
		std::shared_ptr< ben::Number< int > >  numPtr( new ben::Number< int > );
		std::shared_ptr< bed::Data >          dataPtr( new bed::Data );

		node1.add( "0", ( std::shared_ptr< be::Element > )  numPtr );
		node2.add( "1", ( std::shared_ptr< be::Element > ) dataPtr );
		tree.add_child( "report.", node1 );
		tree.add_child( "report.", node2 );

		ReportNode node( tree.get_child( "report" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 1 );
	}
}

BOOST_AUTO_TEST_CASE( report_report_node_next )
{
	LOG_INFO( ">>> report_report_node_next <<<" );
	{
		ReportTree tree;
		ReportTree node1, node2, node3, node4;
		std::shared_ptr< ben::Number< int > >  numPtr1( new ben::Number< int > );
		std::shared_ptr< ben::Number< int > >  numPtr2( new ben::Number< int > );
		std::shared_ptr< bed::Data >          dataPtr1( new bed::Data );
		std::shared_ptr< bed::Data >          dataPtr2( new bed::Data );

		node1.add( "0", ( std::shared_ptr< be::Element > )  numPtr1 );
		node2.add( "1", ( std::shared_ptr< be::Element > )  numPtr2 );
		node3.add( "2", ( std::shared_ptr< be::Element > ) dataPtr1 );
		node4.add( "3", ( std::shared_ptr< be::Element > ) dataPtr2 );
		tree.add_child( "report.", node1 );
		tree.add_child( "report.", node2 );
		tree.add_child( "report.", node3 );
		tree.add_child( "report.", node4 );

		ReportNode node( tree.get_child( "report" ).begin(), 0, 4 );
		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      1 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      2 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      3 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(), 4 );
		BOOST_CHECK_EQUAL( node.getIndex(), node.getIndexTotal() );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(), 4 );
		BOOST_CHECK_EQUAL( node.getIndex(), node.getIndexTotal() );
	}
}

BOOST_AUTO_TEST_CASE( report_report_node_first_child )
{
	LOG_INFO( "\n>>> report_report_node_first_child <<<" );
	{
		ReportTree tree;
		ReportTree group;
		ReportTree node1, node2, node3, node4;
		std::shared_ptr< ben::Number< int > >  numPtr1( new ben::Number< int > );
		std::shared_ptr< ben::Number< int > >  numPtr2( new ben::Number< int > );
		std::shared_ptr< bed::Data >          dataPtr1( new bed::Data );
		std::shared_ptr< bed::Data >          dataPtr2( new bed::Data );

		node1.add( "0", ( std::shared_ptr< be::Element > )  numPtr1 );
		node2.add( "1", ( std::shared_ptr< be::Element > )  numPtr2 );
		node3.add( "2", ( std::shared_ptr< be::Element > ) dataPtr1 );
		node4.add( "3", ( std::shared_ptr< be::Element > ) dataPtr2 );

		node1.add_child( "group.", node2 );
		node1.add_child( "group.", node3 );
		node1.add_child( "group.", node4 );

		tree.add_child( "report.", node1 );

		ReportNode node( tree.get_child( "report" ).begin(), 0, 1 );

		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 1 );
		BOOST_CHECK_EQUAL( node.hasGroup(),   true );

		node = node.firstChild();
		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );
		BOOST_CHECK_EQUAL( node.hasGroup(),  false );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      1 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );
		BOOST_CHECK_EQUAL( node.hasGroup(),  false );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      2 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );
		BOOST_CHECK_EQUAL( node.hasGroup(),  false );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      3 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );
		BOOST_CHECK_EQUAL( node.hasGroup(),  false );
	}
	{
		ReportTree tree;
		ReportTree node1, node2, node3, node4;
		std::shared_ptr< ben::Number< int > >  numPtr1( new ben::Number< int > );
		std::shared_ptr< ben::Number< int > >  numPtr2( new ben::Number< int > );
		std::shared_ptr< bed::Data >          dataPtr1( new bed::Data );
		std::shared_ptr< bed::Data >          dataPtr2( new bed::Data );

		node1.add( "0", ( std::shared_ptr< be::Element > )  numPtr1 );
		node2.add( "1", ( std::shared_ptr< be::Element > )  numPtr2 );
		node3.add( "2", ( std::shared_ptr< be::Element > ) dataPtr1 );
		node4.add( "3", ( std::shared_ptr< be::Element > ) dataPtr2 );
		tree.add_child( "report.", node1 );
		tree.add_child( "report.", node2 );
		tree.add_child( "report.", node3 );
		tree.add_child( "report.", node4 );

		ReportNode node( tree.get_child( "report" ).begin(), 0, 4 );
		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
		BOOST_CHECK_EQUAL( node.hasGroup(),  false );
		BOOST_CHECK_THROW( node.firstChild(), std::runtime_error );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      1 );
		BOOST_CHECK_EQUAL( node.hasGroup(),  false );
		BOOST_CHECK_THROW( node.firstChild(), std::runtime_error );
		
		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      2 );
		BOOST_CHECK_EQUAL( node.hasGroup(),  false );
		BOOST_CHECK_THROW( node.firstChild(), std::runtime_error );
		
		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      3 );
		BOOST_CHECK_EQUAL( node.hasGroup(),  false );
		BOOST_CHECK_THROW( node.firstChild(), std::runtime_error );
	}
}

BOOST_AUTO_TEST_CASE( report_report_node_parent )
{
	LOG_INFO( ">>> report_report_node_parent <<<" );
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

		ReportNode node( tree.get_child( "report" ).begin(), 0, 4 );
		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      1 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );

		ReportNode child = node.firstChild();
		BOOST_CHECK_EQUAL( child.getIndex(),      0 );
		BOOST_CHECK_EQUAL( child.getIndexTotal(), 1 );
		BOOST_CHECK_EQUAL( child.parent()->getIndex(),      1 );
		BOOST_CHECK_EQUAL( child.parent()->getIndexTotal(), 4 );

		node = child.parent()->next();
		BOOST_CHECK_EQUAL( node.getIndex(),      2 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      3 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );
		BOOST_CHECK_THROW( node.parent(), std::runtime_error );
	}
}

BOOST_AUTO_TEST_CASE( report_report_node_get_element_pointer )
{
	LOG_INFO( ">>> report_report_node_get_element_pointer <<<" );
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

		ReportNode node( tree.get_child( "report" ).begin(), 0, 4 );
		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );
		BOOST_CHECK_EQUAL( node.getElementPointer()->getType(), 1 );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      1 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );
		BOOST_CHECK_EQUAL( node.getElementPointer()->getType(), 1 );

		ReportNode child = node.firstChild();
		BOOST_CHECK_EQUAL( child.getIndex(),      0 );
		BOOST_CHECK_EQUAL( child.getIndexTotal(), 1 );
		BOOST_CHECK_EQUAL( child.getElementPointer()->getType(), 3 );
		BOOST_CHECK_EQUAL( child.parent()->getIndex(),      1 );
		BOOST_CHECK_EQUAL( child.parent()->getIndexTotal(), 4 );

		node = child.parent()->next();
		BOOST_CHECK_EQUAL( node.getIndex(),      2 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );
		BOOST_CHECK_EQUAL( node.getElementPointer()->getType(), 3 );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      3 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );
		BOOST_CHECK_EQUAL( node.getElementPointer()->getType(), 3 );
	}
}

BOOST_AUTO_TEST_SUITE_END()
