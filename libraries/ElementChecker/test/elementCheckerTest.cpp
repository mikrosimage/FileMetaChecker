#define BOOST_TEST_MODULE qc_element_checker

#include <ElementChecker/Checker/Checker.hpp>
#include <Common/log.hpp>
#include <SpecReader/Specification.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::unit_test;
namespace bpt = boost::property_tree;

using namespace element_checker;

BOOST_AUTO_TEST_SUITE( element_checker_test )

BOOST_AUTO_TEST_CASE( element_checker_test_init )
{
	common::level = common::eLogTrace;
}

BOOST_AUTO_TEST_SUITE_END()

// #include "translatorTest.hpp"
// #include "rangesTest.hpp"
// #include "expressionParserTest.hpp"
#include "checkerTest.hpp"
#include "checkerTestRepetitions.hpp"
#include "checkerTestUnordered.hpp"
#include "checkerTestCount.hpp"
