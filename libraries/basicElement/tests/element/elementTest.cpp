#define BOOST_TEST_MODULE qc_basic_element

#include <boost/test/unit_test.hpp>

#include <Element.hpp>
#include <Data/Data.hpp>
#include <Number/Number.hpp>

using namespace boost::unit_test;
namespace be = basic_element;
namespace dbe = basic_element::data_element;
namespace nbe = basic_element::number_element;

boost::shared_ptr<common::formatters::Formatter> formatter( common::formatters::Formatter::get() );
boost::shared_ptr<common::Color>                 color( common::Color::get() );

std::string id    = "id";
std::string label = "label";

BOOST_AUTO_TEST_SUITE( basic_element_test_suite01 )

BOOST_AUTO_TEST_CASE( basic_element_element )
{
	formatter->init_logging();
	color->enable();
}

BOOST_AUTO_TEST_SUITE_END()

#include "dataTests.hpp"
#include "numberTests.hpp"
