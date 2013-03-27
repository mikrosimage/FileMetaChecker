#ifndef _SYSTEM_INFO_FILE_SYSTEM_INFO_HPP_
#define _SYSTEM_INFO_FILE_SYSTEM_INFO_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ostream>

#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

class FileSystemInfo
{
public:
	FileSystemInfo();
	FileSystemInfo( const std::string& filePath );

	~FileSystemInfo();
	
	void setFilePath( const std::string& filePath );

	void extractFileInfo();

	std::string getFilename() const;
	std::string getExt() const;
	size_t      getSize() const;
	std::string getFileStatus() const;
	std::string getPermissions() const;
	
	void getOwnerPermissions( bool& read, bool& write, bool& exec ) const;
	void getGroupPermissions( bool& read, bool& write, bool& exec ) const;
	void getOtherPermissions( bool& read, bool& write, bool& exec ) const;
	
private:
	char getStatus( bool status, char c ) const;
	
private:
	bfs::path    _filePath;
	std::string  _filename;
	std::string  _extension;
	std::string  _permissions;
	std::string  _status;
	std::fstream _file;
	size_t       _size;
};

std::ostream& operator<<( std::ostream& out, const bfs::file_type& fileType );
std::ostream& operator<<( std::ostream& out, const FileSystemInfo& fileReader );

#endif
