#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

struct FileInfo
{
	std::string 	name;
	std::string 	extension;
	size_t      	size;
	std::string    	permissions;
	bool 			status;
};


class FileReader
{
	public:
		FileReader();
		FileReader( const std::string& filePath );

		void setFilePath( const std::string& filePath );

		void extractFileInfo();
		void printFileInfo();

		void openFile();
		// std::string readHeaderSegment( size_t length );

	private:
		std::string _filePath;

	public:
		FileInfo 	_fi;
		std::fstream _file;
};

#endif