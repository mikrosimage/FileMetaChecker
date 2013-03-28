#include <common/global.hpp>

#include <report/Report.hpp>

#include "FileSystemInfo.hpp"

#include <sstream>

FileSystemInfo::FileSystemInfo( const std::string& filePath )
	: _filePath( filePath )
{
	_status = bfs::status( _filePath );
	_permissions = _status.permissions();
}

FileSystemInfo::~FileSystemInfo()
{
}

inline
std::string to_string( const bool b )
{
	return b ? "True" : "False";
}

inline
std::string to_string( const size_t size )
{
	std::ostringstream ss;
	ss << size;
	return ss.str();
}

void FileSystemInfo::getReport( Report* report )
{
	const std::string si     = "systemInfo";
	const std::string perm   = "permissions";
	const std::string owner  = "owner";
	const std::string group  = "group";
	const std::string others = "others";
	const std::string read   = "read";
	const std::string write  = "write";
	const std::string exe    = "exe";
	bool ownerRead, ownerWrite, ownerExe;
	bool groupRead, groupWrite, groupExe;
	bool othersRead, othersWrite, othersExe;
	
	getOwnerPermissions( ownerRead, ownerWrite, ownerExe );
	getGroupPermissions( groupRead, groupWrite, groupExe );
	getOtherPermissions( othersRead, othersWrite, othersExe );
	
	
	report->add( si + ".filename", getFilename() );
	report->add( si + ".absolutePath", getAbsoluteFilename() );
	report->add( si + ".extension", getExt() );
	report->add( si + ".size",  to_string( getSize() ) );

	report->add( si + "." + perm + "." + owner + "." + read , to_string( ownerRead  ) );
	report->add( si + "." + perm + "." + owner + "." + write, to_string( ownerWrite ) );
	report->add( si + "." + perm + "." + owner + "." + exe  , to_string( ownerExe   ) );
	
	report->add( si + "." + perm + "." + group + "." + read , to_string( ownerRead  ) );
	report->add( si + "." + perm + "." + group + "." + write, to_string( ownerWrite ) );
	report->add( si + "." + perm + "." + group + "." + exe  , to_string( ownerExe   ) );
	
	report->add( si + "." + perm + "." + others + "." + read , to_string( ownerRead  ) );
	report->add( si + "." + perm + "." + others + "." + write, to_string( ownerWrite ) );
	report->add( si + "." + perm + "." + others + "." + exe  , to_string( ownerExe   ) );
	
	
	report->add( si + ".filestatus", getFileStatus() );
}

std::string FileSystemInfo::getFilename() const
{
	return _filePath.stem().string();
}

std::string FileSystemInfo::getAbsoluteFilename() const
{
	return absolute( _filePath ).string();
}

std::string FileSystemInfo::getExt() const
{
	return _filePath.extension().string();
}

size_t FileSystemInfo::getSize() const
{
	if( is_regular( _status ) )
		return bfs::file_size( _filePath );
	return 0;
}

std::string FileSystemInfo::getFileStatus() const
{
	std::stringstream status;
	status << _status.type();
	return status.str();
}

char FileSystemInfo::getRightStatus( bool permission, char c ) const
{
	return ( permission ? c : '-' );
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
	
	rights += getRightStatus( ownerRead,  'r' );
	rights += getRightStatus( ownerWrite, 'w' );
	rights += getRightStatus( ownerExe,   'x' );
	
	rights += getRightStatus( groupRead,  'r' );
	rights += getRightStatus( groupWrite, 'w' );
	rights += getRightStatus( groupExe,   'x' );
	
	rights += getRightStatus( othersRead,  'r' );
	rights += getRightStatus( othersWrite, 'w' );
	rights += getRightStatus( othersExe,   'x' );
	
	return rights;
}

void FileSystemInfo::getOwnerPermissions( bool& read, bool& write, bool& exec ) const
{
	read  = ( bfs::owner_read  & _permissions ) != 0;
	write = ( bfs::owner_write & _permissions ) != 0;
	exec  = ( bfs::owner_exe   & _permissions ) != 0;
}

void FileSystemInfo::getGroupPermissions( bool& read, bool& write, bool& exec ) const
{
	read  = ( bfs::group_read  & _permissions ) != 0;
	write = ( bfs::group_write & _permissions ) != 0;
	exec  = ( bfs::group_exe   & _permissions ) != 0;
}

void FileSystemInfo::getOtherPermissions( bool& read, bool& write, bool& exec ) const
{
	read  = ( bfs::others_read  & _permissions ) != 0;
	write = ( bfs::others_write & _permissions ) != 0;
	exec  = ( bfs::others_exe   & _permissions ) != 0;
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
	out << "filename           " << fileReader.getFilename() << std::endl;
	out << "absolute filename  " << fileReader.getAbsoluteFilename() << std::endl;
	out << "extension          " << fileReader.getExt() << std::endl;
	out << "size               " << fileReader.getSize() << std::endl;
	out << "permissions        " << fileReader.getPermissions() << std::endl;
	out << "status             " << fileReader.getFileStatus() << std::endl;

	return out;
}

