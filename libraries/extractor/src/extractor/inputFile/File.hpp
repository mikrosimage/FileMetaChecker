#ifndef _EXTRACTOR_INPUT_FILE_FILE_HPP_
#define _EXTRACTOR_INPUT_FILE_FILE_HPP_

#include <common/global.hpp>

#include <fstream>
#include <cstdio>
#include <vector>
#include <string>

class File
{
public:
	File()
	{
	}
	
	bool open( const std::string& filename )
	{
		_file.open ( filename.c_str(), std::ios::in | std::ios::binary );
		_file.seekg (0, std::ios::beg);
		return _file.is_open();
	}
	
	void close()
	{
		_file.close();
	}

	bool readData( char* data, const size_t size )
	{
		_file.read( data, size );
		//_file >> data;
/*
		for (size_t i=0; i<size; ++i)
			std::cout << "read : " << i << "  " << (unsigned char)data[i] << std::endl;
*/
		return true;
	}

	void goBack( const std::size_t size)
	{
		_file.seekg( _file.tellg() - (int)size );
	}
	
private:
	std::ifstream _file;
};

#endif
