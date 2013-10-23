#define BOOST_TEST_MODULE qc_spec_reader

#include <Common/log.hpp>
#include <SpecReader/Specification.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::unit_test;
using namespace basic_element;

std::string id    = "id";
std::string label = "label";

BOOST_AUTO_TEST_SUITE( basic_element_test_suite01 )

BOOST_AUTO_TEST_CASE( basic_element_init )
{
	common::level = common::eLogTrace;
	// LOG_INFO( common::level );
}

BOOST_AUTO_TEST_SUITE_END()

#include "dataTest.hpp"
