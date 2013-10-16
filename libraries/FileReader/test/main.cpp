#include <FileReader/FileReader.hpp>

#include <iostream>
#include <limits>

#define BOOST_TEST_MODULE qc_extractor_translator
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;
using namespace std;

// boost::shared_ptr<common::formatters::Formatter> formatter( common::formatters::Formatter::get() );
// boost::shared_ptr<common::Color>                 color( common::Color::get() );

BOOST_AUTO_TEST_SUITE( fileReader_tests_suite01 )

BOOST_AUTO_TEST_CASE( fileReader_init )
{
	// formatter->init_logging();
	// color->enable();
}

// BOOST_AUTO_TEST_CASE( fileReader_test_streambuffer )
// {
// 	{
// 		// LOG_INFO( " >>> fileReader_test_streambuffer <<< " );
// 		bool ret;
// 		std::stringbuf buffer;
// 		file_reader::FileReader fr( &buffer );

// 		BOOST_CHECK_EQUAL( fr.getLength(), 0 );
// 		buffer.sputc( 'W' );
// 		buffer.sputc( 'A' );
// 		buffer.sputc( 'V' );
// 		buffer.sputc( 'E' );
// 		BOOST_CHECK_EQUAL( fr.getLength(), 4 );

// 		BOOST_CHECK_EQUAL( fr.getPosition(), 0 );

// 		char out[4] = { 0, 0, 0, 0 };

// 		ret= fr.readData( out, 2 );
// 		BOOST_CHECK_EQUAL( ret, true );

// 		BOOST_CHECK_EQUAL( out[0], 'W' );
// 		BOOST_CHECK_EQUAL( out[1], 'A' );
// 		BOOST_CHECK_EQUAL( out[2],  0  );
// 		BOOST_CHECK_EQUAL( out[3],  0  );

// 		BOOST_CHECK_EQUAL( fr.getPosition(), 2 );
// 		BOOST_CHECK_EQUAL( fr.getLength(),   4 );
// 		BOOST_CHECK_EQUAL( fr.getPosition(), 2 );

// 		ret = fr.readData( out, 2 );
// 		BOOST_CHECK_EQUAL( ret, true );

// 		BOOST_CHECK_EQUAL( out[0], 'V' );
// 		BOOST_CHECK_EQUAL( out[1], 'E' );
// 		BOOST_CHECK_EQUAL( out[2],  0  );
// 		BOOST_CHECK_EQUAL( out[3],  0  );

// 		BOOST_CHECK_EQUAL( fr.getPosition(), 4 );
// 		BOOST_CHECK_EQUAL( fr.getLength(),   4 );
// 		BOOST_CHECK_EQUAL( fr.getPosition(), 4 );
// 		BOOST_CHECK_EQUAL( fr.isEndOfFile(), true );

// 		ret = fr.readData( out, 2 );
// 		BOOST_CHECK_EQUAL( ret, 0 );

// 		BOOST_CHECK_EQUAL( fr.isEndOfFile(), true );
// 		fr.goToBegin();
// 		BOOST_CHECK_EQUAL( fr.isEndOfFile(), false );
// 	}
// 	{
// 		bool ret;
// 		std::stringbuf buffer;
// 		file_reader::FileReader fr( &buffer );

// 		BOOST_CHECK_EQUAL( fr.getLength(), 0 );
// 		buffer.sputc( 'W' );
// 		buffer.sputc( 'A' );
// 		buffer.sputc( 'V' );
// 		buffer.sputc( 'E' );
// 		BOOST_CHECK_EQUAL( fr.getLength(), 4 );

// 		BOOST_CHECK_EQUAL( fr.getPosition(), 0 );

// 		char out1[1] = { 0 };

// 		ret= fr.readData( out1, 1 );
// 		BOOST_CHECK_EQUAL( ret, true );
// 		BOOST_CHECK_EQUAL( out1[0], 'W' );
// 		BOOST_CHECK_EQUAL( fr.getPosition(), 1 );

// 		fr.goForward( 2 );
// 		ret= fr.readData( out1, 1 );
// 		BOOST_CHECK_EQUAL( ret, true );
// 		BOOST_CHECK_EQUAL( out1[0], 'E' );
// 		BOOST_CHECK_EQUAL( fr.getPosition(), 4 );
// 		BOOST_CHECK_EQUAL( fr.isEndOfFile(), true );

// 		fr.goBack( 1 );
// 		ret= fr.readData( out1, 1 );
// 		BOOST_CHECK_EQUAL( ret, true );
// 		BOOST_CHECK_EQUAL( out1[0], 'E' );
// 		BOOST_CHECK_EQUAL( fr.getPosition(), 4 );
// 		BOOST_CHECK_EQUAL( fr.isEndOfFile(), true );

// 		fr.goBack( 3 );
// 		ret= fr.readData( out1, 1 );
// 		BOOST_CHECK_EQUAL( ret, true );
// 		BOOST_CHECK_EQUAL( out1[0], 'A' );
// 		BOOST_CHECK_EQUAL( fr.getPosition(), 2 );

// 		fr.goBack( 2 );
// 		ret= fr.readData( out1, 1 );
// 		BOOST_CHECK_EQUAL( ret, true );
// 		BOOST_CHECK_EQUAL( out1[0], 'W' );
// 		BOOST_CHECK_EQUAL( fr.getPosition(), 1 );

// 		fr.goForward( 1 );
// 		ret= fr.readData( out1, 1 );
// 		BOOST_CHECK_EQUAL( ret, true );
// 		BOOST_CHECK_EQUAL( out1[0], 'V' );
// 		BOOST_CHECK_EQUAL( fr.getPosition(), 3 );

// 		fr.goBack( 3 );
// 		BOOST_CHECK_EQUAL( fr.getPosition(), 0 );

// 		char out2[4] = { 0, 0, 0, 0 };
// 		ret= fr.readData( out2, 4 );
// 		BOOST_CHECK_EQUAL( ret, true );
// 		BOOST_CHECK_EQUAL( strcmp( out2, "WAVE" ), 0 );
// 		BOOST_CHECK_EQUAL( fr.getPosition(), 4 );
// 		BOOST_CHECK_EQUAL( fr.isEndOfFile(), true );
// 	}
// }

/*
BOOST_AUTO_TEST_CASE( fileReader_test_filebuffer )
{
	LOG_INFO( " >>> fileReader_test_filebuffer <<< " );

	std::string content = "FILE reader";

	ofstream myfile;
	myfile.open( "test.txt" );
	myfile << content;
	myfile.close();

	std::ifstream is;
	std::filebuf* fb = is.rdbuf();

	fb->open( "test.txt", std::ios::in );
	BOOST_CHECK_EQUAL( fb->is_open(), true );
	BOOST_CHECK_EQUAL( fb->in_avail(), content.size() );

	filereader::FileReader fr( fb );

	BOOST_CHECK_EQUAL( fr.getLength(),  content.size() );
	BOOST_CHECK_EQUAL( fr.getPosition(), 0 );

	char buffer[ content.size() + 1 ];
	std::memset( buffer, 0, content.size() + 1 );	// "+ 1" to compensate the NULL char ending std::strings
	bool ret;

	ret= fr.readData( buffer, content.size() );
	BOOST_CHECK_EQUAL( ret, true );

	BOOST_CHECK_EQUAL( strcmp( content.c_str(), buffer ), 0 );

	BOOST_CHECK_EQUAL( fr.getPosition(), content.size() );
	BOOST_CHECK_EQUAL( fr.getLength(),   content.size() );
	BOOST_CHECK_EQUAL( fr.getPosition(), content.size() );
	BOOST_CHECK_EQUAL( fr.isEndOfFile(), true );

	ret = fr.readData( buffer, 1 );
	BOOST_CHECK_EQUAL( ret, 0 );

	BOOST_CHECK_EQUAL( fr.isEndOfFile(), true );
	fr.goToBegin();
	BOOST_CHECK_EQUAL( fr.isEndOfFile(), false );

	fb->close();
}*/

BOOST_AUTO_TEST_SUITE_END()
