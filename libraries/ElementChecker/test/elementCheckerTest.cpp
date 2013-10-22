#define BOOST_TEST_MODULE qc_element_checker

#include <ElementChecker/Checker/Checker.hpp>
#include <Common/log.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::unit_test;
namespace bpt = boost::property_tree;

BOOST_AUTO_TEST_SUITE( element_checker_test )

BOOST_AUTO_TEST_CASE( element_checker_test_init )
{
	common::level = common::eLogTrace;
}

BOOST_AUTO_TEST_SUITE_END()

#include "translatorTest.hpp"
#include "checkerTest.hpp"
