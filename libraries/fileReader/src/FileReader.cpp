#include "FileReader.hpp"

namespace filereader
{

FileReader::FileReader( std::streambuf* inStream ):
	_fileBuffer ( inStream )
{
}


/*
bool FileReader::readData( char* data, const size_t size )
{
	//LOG_INFO( "read pos: " << _file.tellg() );
	_file.read( data, size );
	//	for (size_t i=0; i<size; ++i)
	//	std::cout << "read : " << i << "  " << (unsigned char)data[i] << std::endl;

	//GET_VAR2(_file.gcount(), (int)size);

	return ( _file.gcount() == (int)size );
}

void FileReader::goToBegin()
{
	_file.seekg( std::ios_base::beg );
	//LOG_INFO( "pos: " << _file.tellg() );
}

void FileReader::goBack( const std::size_t size )
{
	_file.seekg( - size, std::ios::cur );
	//LOG_INFO( "pos: " << _file.tellg() );
}

void FileReader::goForward( const std::size_t size )
{
	_file.seekg( size, std::ios::cur );
	//LOG_INFO( "pos: " << _file.tellg() );
}

bool FileReader::endOfFile( )
{
	if( getPosition() == -1 )
		return true;
	return _file.eof();
}

int FileReader::getPosition()
{
	int position;
	std::stringstream sstr;
	sstr << _file.tellg();
	sstr >> position;
	return position;
	// LOG_INFO( "pos: " << _file.tellg() );
}

void FileReader::resetFile()
{
	_file.close();
	_file.open ( _filename.c_str(), std::ios::in | std::ios::binary );
	_file.seekg ( std::ios::beg);
}

std::string FileReader::getFilename()
{
	if ( _file.is_open() )
		return _filename;
	else
		return "None";
}*/

}
