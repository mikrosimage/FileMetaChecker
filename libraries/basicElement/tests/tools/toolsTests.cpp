#define BOOST_TEST_MODULE qc_basic_element_tools

#include <boost/test/unit_test.hpp>
#include <basicElement/Tools/ExpressionParser.hpp>

#include <basicElement/Data/Data.hpp>
#include <basicElement/Number/Number.hpp>

using namespace boost::unit_test;
namespace be = basic_element;
namespace bep = basic_element::expression_parser;

boost::shared_ptr<common::formatters::Formatter> formatter( common::formatters::Formatter::get() );
boost::shared_ptr<common::Color>                 color( common::Color::get() );

static const std::string id = "id";

BOOST_AUTO_TEST_SUITE( basic_element_test_suite02 )

BOOST_AUTO_TEST_CASE( basic_element_init )
{
	formatter->init_logging();
	color->enable();
}

BOOST_AUTO_TEST_SUITE_END()

#include "mapTests.hpp"
#include "rangeTests.hpp"
#include "expressionParserTests.hpp"
