#ifndef _FILEREADER_FILE_HPP_
#define _FILEREADER_FILE_HPP_

#include <common/global.hpp>

#include <sstream>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

namespace filereader
{

class FileReader
{
public:
	FileReader( std::streambuf* inStream );


/*
	bool readData( char* data, const size_t size );

	void goToBegin();

	void goBack( const std::size_t size );

	void goForward( const std::size_t size );

	bool endOfFile( );

	int  getPosition();

	void resetFile();

	std::string getFilename();
*/
private:
	std::istream _fileBuffer;
	std::string  _filename;
};

}

#endif


