#include "FileReader.hpp"

namespace file_reader
{

FileReader::FileReader( std::streambuf* inStream )
	: _fileBuffer ( inStream )
{
}

size_t FileReader::getLength()
{
	size_t currentPosition, length;
	currentPosition = _fileBuffer.tellg();

	_fileBuffer.seekg( 0, _fileBuffer.end );
	length = _fileBuffer.tellg();
	
	_fileBuffer.seekg( currentPosition, _fileBuffer.beg );
	return length;
}

size_t FileReader::getPosition()
{
	return _fileBuffer.tellg();
}

bool FileReader::readData( std::vector< char >& data, const size_t size )
{
	data.resize( size );
	_fileBuffer.readsome( &data[0], size );
	return ( _fileBuffer.gcount() == (int)size );
}

bool FileReader::readWord( std::vector< char >& data, const char endOfString )
{
	char character;
	_fileBuffer.readsome( &character, 1 );
	data.push_back( character );
	
	while( endOfString != character )
	{
		_fileBuffer.readsome( &character, 1 );
		data.push_back( character );
	}
	return true;
}

void FileReader::goToBegin()
{
	_fileBuffer.seekg( 0, _fileBuffer.beg );
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
	return getPosition() == getLength();
}

}
