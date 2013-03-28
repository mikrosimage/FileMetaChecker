#ifndef _EXTRACTOR_INPUT_FILE_FILE_HPP_
#define _EXTRACTOR_INPUT_FILE_FILE_HPP_

#include "Translator.hpp"

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
		pFile = fopen( filename.c_str() , "r" );
		return pFile != NULL;
	}
	
	void close()
	{
		fclose( pFile );
	}
	
	template< typename DataType >
	DataType getData()
	{
		DataType t;
		
		size_t size = sizeof( DataType );
		char buffer[ size ];
		readData( buffer, size );
		
		Translator<DataType> tr;
		t = translate( buffer, size );
		
		return t;
	}
	
	
	template< typename DataType >
	std::vector<DataType> getData( size_t number )
	{
		std::vector< DataType > t;
		return t;
	}
	
private:
	bool readData( char* data, size_t size )
	{
		while( ! feof( pFile ) )
		{
			if( fgets( data, size , pFile ) == NULL ) break;
			fputs( data, stdout );
		}
		return true;
	}
	
private:
	FILE * pFile;
};

#include "File.tcc"

#endif
