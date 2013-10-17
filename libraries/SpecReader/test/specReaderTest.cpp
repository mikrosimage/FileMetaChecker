#define BOOST_TEST_MODULE qc_spec_reader

#include <SpecReader/Specification.hpp>
#include <Common/log.hpp>

#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;
using namespace spec_reader;

std::string id    = "id";
std::string label = "label";

BOOST_AUTO_TEST_SUITE( spec_reader_test_suite01 )

BOOST_AUTO_TEST_CASE( spec_reader_init )
{
	common::level = common::eLogTrace;
	// LOG_INFO( common::level );
}

BOOST_AUTO_TEST_SUITE_END()

#include "specNodeTest.hpp"
