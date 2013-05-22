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
		//COMMON_COUT( "read pos: " << _file.tellg() );
		_file.read( data, size );
/*		for (size_t i=0; i<size; ++i)
			std::cout << "read : " << i << "  " << (unsigned char)data[i] << std::endl; */
		
		//COMMON_COUT_VAR2(_file.gcount(), (int)size);
		
		return ( _file.gcount() == (int)size );
	}

	void goToBegin()
	{
		_file.seekg( std::ios_base::beg );
		//COMMON_COUT( "pos: " << _file.tellg() );
	}

	void goBack( const std::size_t size )
	{
		_file.seekg( - size, std::ios::cur );
		//COMMON_COUT( "pos: " << _file.tellg() );
	}

	void goForward( const std::size_t size )
	{
		_file.seekg( size, std::ios::cur );
		//COMMON_COUT( "pos: " << _file.tellg() );
	}

	bool endOfFile( )
	{
		return _file.eof();
	}

	int getPosition()
	{
		int position;
		std::stringstream sstr;
		sstr << _file.tellg();
		sstr >> position;
		return position;
		// LOG_INFO( "pos: " << _file.tellg() );
	}
	
private:
	std::ifstream _file;
};

#endif
