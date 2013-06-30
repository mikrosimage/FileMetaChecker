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
	bool ret;
	std::stringbuf buffer;
	filereader::FileReader fr( &buffer );

	BOOST_CHECK_EQUAL( fr.getLength(), 0 );
	buffer.sputc( 'W' );
	buffer.sputc( 'A' );
	buffer.sputc( 'V' );
	buffer.sputc( 'E' );
	BOOST_CHECK_EQUAL( fr.getLength(), 4 );

	BOOST_CHECK_EQUAL( fr.getPosition(), 0 );

	char out[4] = { 0, 0, 0, 0};

	ret= fr.readData( out, 2 );

	BOOST_CHECK_EQUAL( ret, 1 );
	BOOST_CHECK_EQUAL( out[0], 'W' );
	BOOST_CHECK_EQUAL( out[1], 'A' );
	BOOST_CHECK_EQUAL( out[2],  0  );
	BOOST_CHECK_EQUAL( out[3],  0  );
	BOOST_CHECK_EQUAL( fr.getPosition(), 2 );
	BOOST_CHECK_EQUAL( fr.getLength(), 4 );
	BOOST_CHECK_EQUAL( fr.getPosition(), 2 );

	ret = fr.readData( out, 2 );

	BOOST_CHECK_EQUAL( ret, 1 );
	BOOST_CHECK_EQUAL( out[0], 'V' );
	BOOST_CHECK_EQUAL( out[1], 'E' );
	BOOST_CHECK_EQUAL( out[2],  0  );
	BOOST_CHECK_EQUAL( out[3],  0  );
	BOOST_CHECK_EQUAL( fr.getPosition(), 4 );
	BOOST_CHECK_EQUAL( fr.getLength(), 4 );
	BOOST_CHECK_EQUAL( fr.getPosition(), 4 );
	BOOST_CHECK_EQUAL( fr.isEndOfFile(), true );

	ret = fr.readData( out, 2 );
	BOOST_CHECK_EQUAL( ret, 0 );

	BOOST_CHECK_EQUAL( fr.isEndOfFile(), true );
	fr.goToBegin();
	BOOST_CHECK_EQUAL( fr.isEndOfFile(), false );
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
