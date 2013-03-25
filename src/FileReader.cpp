#include "common/global.hpp"
#include "FileReader.hpp"

#include <sstream>

FileReader::FileReader()
	: _filePath( "" )
{
}

FileReader::FileReader( const std::string& filePath )
	: _filePath( filePath )
{
}

void FileReader::setFilePath( const std::string& filePath )
{
	_filePath = filePath;
}

void FileReader::extractFileInfo()
{
	// File extraction
	bfs::path filepath( _filePath );

	// File name
	_fileInfo.name = filepath.stem().string();

	// Define file status
	bfs::file_status fileStatus = bfs::status( filepath );

	std::stringstream ssStatus;
	ssStatus << fileStatus.type();
	_fileInfo.status = ssStatus.str();


	// Get informations
	_fileInfo.extension = filepath.extension().string();
	if( is_regular( fileStatus ) )
	{
		_fileInfo.size = bfs::file_size( filepath );
	}

	std::stringstream ss;
	ss << std::oct << fileStatus.permissions();
	_fileInfo.permissions = ss.str();
}


void FileReader::openFile()
{
	_file.open( _filePath.c_str(), std::ios::in | std::ios::binary );
}

std::ostream& operator<<( std::ostream& out, const bfs::file_type& fileType )
{
	switch( fileType )
	{
		case bfs::status_unknown:              out << "Unknown status"; break;
		case bfs::file_not_found:              out << "Not found"; break;
		case bfs::regular_file:                out << "File"; break;
		case bfs::directory_file:              out << "Directory"; break;
		case bfs::symlink_file:                out << "Symlink file"; break;
		case bfs::block_file:                  out << "Block file"; break;
		case bfs::character_file:              out << "Character file"; break;
		case bfs::fifo_file:                   out << "Fifo file"; break;
		case bfs::socket_file:                 out << "Socket file"; break;
		case bfs::type_unknown:                out << "Unknown type"; break;
		case bfs::reparse_file:                out << "Reparse file"; break;
		case bfs::_detail_directory_symlink:   out << "Directory symlink"; break;
	}
	return out;
}

std::ostream& operator<<( std::ostream& out, const FileInfo& fileReader )
{
	out << "INPUT FILE :" << std::endl;
	out << "name          " << fileReader.name << std::endl;
	out << "extension     " << fileReader.extension << std::endl;
	out << "size          " << fileReader.size << std::endl;
	out << "permissions   " << fileReader.permissions << std::endl;
	out << "status        " << fileReader.status << std::endl;

	return out;
}

std::ostream& operator<<( std::ostream& out, const FileReader& fileReader )
{
	out << fileReader._fileInfo;

	return out;
}

