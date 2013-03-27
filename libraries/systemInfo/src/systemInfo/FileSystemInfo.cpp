#include <common/global.hpp>
#include "FileSystemInfo.hpp"

#include <sstream>

FileSystemInfo::FileSystemInfo()
	: _filePath( "" )
{
}

FileSystemInfo::FileSystemInfo( const std::string& filePath )
	: _filePath( filePath )
{
	_filename = _filePath.stem().string();
}

FileSystemInfo::~FileSystemInfo()
{
	if( _file.is_open() )
	{
		_file.close();
	}
}

void FileSystemInfo::setFilePath( const std::string& filePath )
{
	_filePath = bfs::path( filePath );
}

void FileSystemInfo::extractFileInfo()
{
	_status = getFileStatus();
	_extension = getExt();
	_size = getSize();
	_permissions = getPermissions();
}

std::string FileSystemInfo::getFilename() const
{
	return _filename;
}

std::string FileSystemInfo::getExt() const
{
	return _filePath.extension().string();
}

size_t FileSystemInfo::getSize() const
{
	if( is_regular( bfs::status( _filePath ) ) )
	{
		return bfs::file_size( _filePath );
	}

	return 0;
}

std::string FileSystemInfo::getFileStatus() const
{
	std::stringstream status;
	status << bfs::status( _filePath ).type();
	return status.str();
}

char FileSystemInfo::getStatus( bool status, char c ) const
{
	return ( status ? c : '-' );
}

std::string FileSystemInfo::getPermissions() const
{
	bool ownerRead, ownerWrite, ownerExe;
	bool groupRead, groupWrite, groupExe;
	bool othersRead, othersWrite, othersExe;
	std::string rights;
	
	getOwnerPermissions( ownerRead, ownerWrite, ownerExe );
	getGroupPermissions( groupRead, groupWrite, groupExe );
	getOtherPermissions( othersRead, othersWrite, othersExe );
	
	rights += getStatus( ownerRead,  'r' );
	rights += getStatus( ownerWrite, 'w' );
	rights += getStatus( ownerExe,   'x' );
	
	rights += getStatus( groupRead,  'r' );
	rights += getStatus( groupWrite, 'w' );
	rights += getStatus( groupExe,   'x' );
	
	rights += getStatus( othersRead,  'r' );
	rights += getStatus( othersWrite, 'w' );
	rights += getStatus( othersExe,   'x' );
	
	return rights;
}

void FileSystemInfo::getOwnerPermissions( bool& read, bool& write, bool& exec ) const
{
	bfs::perms perms = bfs::status( _filePath ).permissions();
	read  = ( bfs::owner_read  & perms ) != 0;
	write = ( bfs::owner_write & perms ) != 0;
	exec  = ( bfs::owner_exe   & perms ) != 0;
}

void FileSystemInfo::getGroupPermissions( bool& read, bool& write, bool& exec ) const
{
	bfs::perms perms = bfs::status( _filePath ).permissions();
	read  = ( bfs::group_read  & perms ) != 0;
	write = ( bfs::group_write & perms ) != 0;
	exec  = ( bfs::group_exe   & perms ) != 0;
}

void FileSystemInfo::getOtherPermissions( bool& read, bool& write, bool& exec ) const
{
	bfs::perms perms = bfs::status( _filePath ).permissions();
	read  = ( bfs::others_read  & perms ) != 0;
	write = ( bfs::others_write & perms ) != 0;
	exec  = ( bfs::others_exe   & perms ) != 0;
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

std::ostream& operator<<( std::ostream& out, const FileSystemInfo& fileReader )
{
	out << "name          " << fileReader.getFilename() << std::endl;
	out << "extension     " << fileReader.getExt() << std::endl;
	out << "size          " << fileReader.getSize() << std::endl;
	out << "permissions   " << fileReader.getPermissions() << std::endl;
	out << "status        " << fileReader.getFileStatus() << std::endl;

	return out;
}

