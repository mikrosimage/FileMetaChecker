#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ostream>

#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

struct FileInfo
{
	size_t      size;
	std::string name;
	std::string extension;
	std::string permissions;
	std::string status;
};


class FileReader
{
	public:
		FileReader();
		FileReader( const std::string& filePath );

		void setFilePath( const std::string& filePath );

		void extractFileInfo();

		void openFile();

	private:
		std::string _filePath;

	public:
		FileInfo     _fileInfo;
		std::fstream _file;
};

std::ostream& operator<<( std::ostream& out, const bfs::file_type& fileType );
std::ostream& operator<<( std::ostream& out, const FileInfo& fileReader );
std::ostream& operator<<( std::ostream& out, const FileReader& fileReader );

#endif
