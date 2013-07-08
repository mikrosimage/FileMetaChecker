#include <common/global.hpp>

#include <report/Report.hpp>

#include "FileSystemInfo.hpp"
#include <boost/property_tree/ptree.hpp>

#include <sstream>
#include <ctime>

namespace bpt = boost::property_tree;

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
	const std::string kSi     = "systemInfo";
	const std::string kPerm   = "permissions";
	const std::string kOwner  = "owner";
	const std::string kGroup  = "group";
	const std::string kOthers = "others";
	const std::string kRead   = "read";
	const std::string kWrite  = "write";
	const std::string kExe    = "exe";
	bool ownerRead, ownerWrite, ownerExe;
	bool groupRead, groupWrite, groupExe;
	bool othersRead, othersWrite, othersExe;
	
	getOwnerPermissions( ownerRead, ownerWrite, ownerExe );
	getGroupPermissions( groupRead, groupWrite, groupExe );
	getOtherPermissions( othersRead, othersWrite, othersExe );
	
	bpt::ptree fileSystemInfoReport;
	bpt::ptree nodeReport;


	nodeReport.put_value( getAbsoluteFilename() );
	nodeReport.put( "<xmlattr>.label", "Path" );
	fileSystemInfoReport.push_back( bpt::ptree::value_type( "absolutePath", nodeReport ));

	nodeReport.put_value( getFilename() );
	nodeReport.put( "<xmlattr>.label", "Filename" );
	fileSystemInfoReport.push_back( bpt::ptree::value_type( "filename", nodeReport ));
	
	nodeReport.put_value( getExt() );
	nodeReport.put( "<xmlattr>.label", "Extension" );
	fileSystemInfoReport.push_back( bpt::ptree::value_type( "extension", nodeReport ));

	nodeReport.put_value( to_string( getSize() ) );
	nodeReport.put( "<xmlattr>.label", "Size" );
	fileSystemInfoReport.push_back( bpt::ptree::value_type( "size", nodeReport ));

	nodeReport.put_value( getLastModifDate() );
	nodeReport.put( "<xmlattr>.label", "Last Modification" );
	fileSystemInfoReport.push_back( bpt::ptree::value_type( "lastModifDate", nodeReport ));

	nodeReport.clear();
	nodeReport.put_value( getFileStatus() );
	nodeReport.put( "<xmlattr>.label", "Status" );
	fileSystemInfoReport.put( "<xmlattr>.label", "File System Info" );
	fileSystemInfoReport.push_back( bpt::ptree::value_type( "status", nodeReport ));
	
	nodeReport.clear();
	nodeReport.put( "<xmlattr>.label", "Rights" );
	

	bpt::ptree userPermNode, readNode, writeNode, exeNode;
	
	readNode.put(  "<xmlattr>.status", to_string( ownerRead  ) );
	writeNode.put( "<xmlattr>.status", to_string( ownerWrite ) );
	exeNode.put(   "<xmlattr>.status", to_string( ownerExe   ) );
	userPermNode.push_back( bpt::ptree::value_type( kRead , readNode  ) );
	userPermNode.push_back( bpt::ptree::value_type( kWrite, writeNode ) );
	userPermNode.push_back( bpt::ptree::value_type( kExe  , exeNode   ) );
	userPermNode.put( "<xmlattr>.label", "Owner" );
	nodeReport.push_back( bpt::ptree::value_type( kOwner, userPermNode ) );

	userPermNode.clear();
	readNode.clear();
	writeNode.clear();
	exeNode.clear();

	readNode.put(  "<xmlattr>.status", to_string( groupRead  ) );
	writeNode.put( "<xmlattr>.status", to_string( groupWrite ) );
	exeNode.put(   "<xmlattr>.status", to_string( groupExe   ) );
	userPermNode.push_back( bpt::ptree::value_type( kRead , readNode  ) );
	userPermNode.push_back( bpt::ptree::value_type( kWrite, writeNode ) );
	userPermNode.push_back( bpt::ptree::value_type( kExe  , exeNode   ) );
	userPermNode.put( "<xmlattr>.label", "Group" );
	nodeReport.push_back( bpt::ptree::value_type( kGroup, userPermNode ) );

	userPermNode.clear();
	readNode.clear();
	writeNode.clear();
	exeNode.clear();

	readNode.put(  "<xmlattr>.status", to_string( othersRead  ) );
	writeNode.put( "<xmlattr>.status", to_string( othersWrite ) );
	exeNode.put(   "<xmlattr>.status", to_string( othersExe   ) );
	userPermNode.push_back( bpt::ptree::value_type( kRead , readNode  ) );
	userPermNode.push_back( bpt::ptree::value_type( kWrite, writeNode ) );
	userPermNode.push_back( bpt::ptree::value_type( kExe  , exeNode   ) );
	userPermNode.put( "<xmlattr>.label", "Others" );
	nodeReport.push_back( bpt::ptree::value_type( kOthers, userPermNode ) );

	fileSystemInfoReport.push_back( bpt::ptree::value_type( kPerm, nodeReport ) );

	report->add( fileSystemInfoReport, kSi );
}

std::string FileSystemInfo::getFilename() const
{
	std::string filenameStr = _filePath.stem().string();
	filenameStr.append( _filePath.extension().string() );
	return filenameStr;
}

std::string FileSystemInfo::getAbsoluteFilename() const
{
	return canonical( _filePath ).string();
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

std::string FileSystemInfo::getLastModifDate() const
{
	time_t timer;
	struct tm *timeptr;
	char buffer[32];
	std::string date = "";

	timer = bfs::last_write_time( _filePath );
	timeptr = gmtime( &timer );
	if( std::strftime( buffer, 32, "%a, %d.%m.%Y %H:%M:%S", timeptr ) != 0 )
	{
		date.assign( buffer );
	}
	return date;
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
	out << "last modification  " << fileReader.getLastModifDate() << std::endl;
	out << "permissions        " << fileReader.getPermissions() << std::endl;
	out << "status             " << fileReader.getFileStatus() << std::endl;

	return out;
}

