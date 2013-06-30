#include "FileReader.hpp"

namespace filereader
{

FileReader::FileReader( std::streambuf* inStream )
	: _fileBuffer ( inStream )
{
}

size_t FileReader::getLength()
{
	size_t position = _fileBuffer.tellg();
	_fileBuffer.seekg( 0, _fileBuffer.end );
	size_t length = _fileBuffer.tellg();
	_fileBuffer.seekg( position, _fileBuffer.beg );
	return length;
}

size_t FileReader::getPosition()
{
	return _fileBuffer.tellg();
}

bool FileReader::readData( char* data, const size_t size )
{
	_fileBuffer.read( data, size );
	return ( _fileBuffer.gcount() == (int)size );
}

void FileReader::goToBegin()
{
	_fileBuffer.seekg( std::ios_base::beg );
}

void FileReader::goBack( const std::size_t size )
{
	_fileBuffer.seekg( - size, std::ios::cur );
}

void FileReader::goForward( const std::size_t size )
{
	_fileBuffer.seekg( size, std::ios::cur );
}

bool FileReader::isEndOfFile( )
{
	return _fileBuffer.eof();
}

}
