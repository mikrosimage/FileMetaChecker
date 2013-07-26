
BOOST_AUTO_TEST_SUITE( report_test_report )

BOOST_AUTO_TEST_CASE( report_report )
{
	Report report;

	// Number< int > num;
	// std::shared_ptr< ben::Number< int > > ptr( num );

	std::shared_ptr< ben::Number< int > > ptr( new ben::Number< int > );
	report.addBasicElement( ptr );

	std::shared_ptr< be::Element > retPtr = report.getBasicElement( ptr->getUniqueId() );
	

	// BOOST_CHECK_EQUAL( ptr, retPtr );
	// BOOST_CHECK_EQUAL( ptr->getUniqueId(), retPtr->getUniqueId() );
}

BOOST_AUTO_TEST_SUITE_END()
