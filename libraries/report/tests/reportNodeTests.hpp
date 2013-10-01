#include <boost/property_tree/json_parser.hpp>

BOOST_AUTO_TEST_SUITE( report_test_report_node )

BOOST_AUTO_TEST_CASE( report_report_node )
{
	LOG_INFO( ">>> report_report_node <<<" );
	{
		ReportTree tree;
		ReportTree node1, node2;
		std::shared_ptr< ben::Number >  numPtr( new ben::Number( id, eSubTypeInt32 ) );
		std::shared_ptr< bed::Data   > dataPtr( new bed::Data( id ) );

		node1.add( "0", ( std::shared_ptr< be::Element > ) numPtr );
		tree.add_child( "report.", node1 );

		ReportNode rptNode1( tree.get_child( "report" ).begin(), 0, &tree );
		BOOST_CHECK_EQUAL( rptNode1.getIndex(), 0 );
		BOOST_CHECK_EQUAL( rptNode1.getSecond()->begin()->first, "0" );

		bool parent = true;
		if( rptNode1.parent() == NULL ) parent = false;
		BOOST_CHECK_EQUAL( parent, false );


		ReportNode rptNode2 = rptNode1.appendNext( dataPtr );
		BOOST_CHECK_EQUAL( rptNode2.getIndex(), 1 );
		BOOST_CHECK_EQUAL( rptNode2.getSecond()->begin()->first, "1" );

		parent = true;
		if( rptNode2.parent() == NULL ) parent = false;
		BOOST_CHECK_EQUAL( parent, false );
		

		ReportNode rptNode3 = rptNode2.appendChild( numPtr );
		BOOST_CHECK_EQUAL( rptNode3.getIndex(), 0 );
		BOOST_CHECK_EQUAL( rptNode3.getSecond()->begin()->first, "1-0" );

		BOOST_CHECK_EQUAL( rptNode2.getSecond()->begin()->first, "1" );
		BOOST_CHECK_EQUAL( rptNode3.parent()->getSecond()->begin()->first, "1" );
	}
	{
		ReportTree tree;
		ReportTree node1, node2;
		std::shared_ptr< ben::Number >  numPtr( new ben::Number( id, eSubTypeInt32 ) );
		std::shared_ptr< bed::Data   > dataPtr( new bed::Data( id ) );

		node1.add( "0", ( std::shared_ptr< be::Element > ) numPtr );
		tree.add_child( "report.", node1 );

		ReportNode node( tree.get_child( "report" ).begin(), 0, &tree );
		BOOST_CHECK_EQUAL( node.getIndex(), 0 );
		BOOST_CHECK_EQUAL( node.getSecond()->begin()->first, "0" );

		node = node.appendNext( dataPtr );
		BOOST_CHECK_EQUAL( node.getIndex(), 1 );
		BOOST_CHECK_EQUAL( node.getSecond()->begin()->first, "1" );

		node = node.appendNext( dataPtr );
		BOOST_CHECK_EQUAL( node.getIndex(), 2 );
		BOOST_CHECK_EQUAL( node.getSecond()->begin()->first, "2" );

		ReportNode child = node.appendChild( numPtr );
		BOOST_CHECK_EQUAL( child.getIndex(), 0 );
		BOOST_CHECK_EQUAL( child.getSecond()->begin()->first, "2-0" );
		BOOST_CHECK_EQUAL(  node.getSecond()->begin()->first, "2"   );
		BOOST_CHECK_EQUAL( child.parent()->getSecond()->begin()->first, "2" );

		child = child.appendNext( dataPtr );
		BOOST_CHECK_EQUAL( child.getIndex(), 1 );
		BOOST_CHECK_EQUAL( child.getSecond()->begin()->first, "2-1" );
		BOOST_CHECK_EQUAL(  node.getSecond()->begin()->first, "2"   );
		BOOST_CHECK_EQUAL( child.parent()->getSecond()->begin()->first, "2" );

		ReportNode subchild = child.appendChild( dataPtr );
		BOOST_CHECK_EQUAL( subchild.getIndex(), 0 );
		BOOST_CHECK_EQUAL( subchild.getSecond()->begin()->first, "2-1-0" );
		BOOST_CHECK_EQUAL(    child.getSecond()->begin()->first, "2-1"   );
		BOOST_CHECK_EQUAL( subchild.parent()->getSecond()->begin()->first, "2-1" );
		BOOST_CHECK_EQUAL( subchild.parent()->parent()->getSecond()->begin()->first, "2" );

		bool parent = true;
		if( subchild.parent()->parent()->parent() == NULL ) parent = false;
		BOOST_CHECK_EQUAL( parent, false );
	}
}

BOOST_AUTO_TEST_SUITE_END()
