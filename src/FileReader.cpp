#include "FileReader.hpp"

#include <sstream>

#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;

FileReader::FileReader(): _filePath ( "" )
{}

FileReader::FileReader( const std::string& filePath ): _filePath ( filePath )
{}

void FileReader::setFilePath( const std::string& filePath )
{
	_filePath = filePath;
}

void FileReader::extractFileInfo()
{
// File extraction :
	bfs::path fp( _filePath );

// File name :
	_fi.name = fp.stem().string();

// Define file status :
	bfs::file_status fs = bfs::status( fp );
	_fi.status = bfs::is_regular_file( fp );


// Get informations :
	if ( _fi.status )
	{
		_fi.extension = fp.extension().string();
		_fi.size = bfs::file_size( fp );

		std::stringstream ss;
		ss << std::oct << fs.permissions();
		_fi.permissions = ss.str();
	}
	else 	// other (for the moment)
	{
		std::cout << "Unsupported file type" << std::endl;
	}
}

void FileReader::printFileInfo()
{
	std::cout << "INPUT FILE :" << std::endl;
	std::cout << "name : " << _fi.name << std::endl;
	std::cout << "extension : " << _fi.extension << std::endl;
	std::cout << "size : " << _fi.size << std::endl;
	std::cout << "permissions : " << _fi.permissions << std::endl;
	std::cout << "status : " << _fi.status << std::endl;
}

void FileReader::openFile()
{
	_file.open( _filePath.c_str(), std::ios::in | std::ios::binary );
}

// std::string FileReader::readHeaderSegment( size_t length )
// {
// 	char buffer[length];
// 	std::string sBuffer;

// 	if (_file.is_open()) 
// 	{	
// 		_file.read(buffer, length);
// 	}

// 	for (int i = 0; i < length; ++i)
// 	{
// 		sBuffer.push_back(buffer[i]);
// 	}

// 	return sBuffer;
// }