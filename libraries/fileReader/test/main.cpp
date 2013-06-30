#include <FileReader.hpp>

#include <iostream>
#include <limits>

#define BOOST_TEST_MODULE qc_extractor_translator
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;
using namespace std;

BOOST_AUTO_TEST_SUITE( fileReader_tests_suite01 )

BOOST_AUTO_TEST_CASE( fileReader_test_streambuffer )
{
	std::stringbuf buffer;
	filereader::FileReader fr( &buffer );


}


BOOST_AUTO_TEST_CASE( fileReader_test_filebuffer )
{
	std::ifstream is;
	std::filebuf* fb = is.rdbuf();

	fb->open( "test.txt", std::ios::out | std::ios::app );


	filereader::FileReader fr( fb );

	fb->close();
}

BOOST_AUTO_TEST_SUITE_END()
