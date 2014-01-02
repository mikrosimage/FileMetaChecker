#ifndef _FILEREADER_FILEREADER_HPP_
#define _FILEREADER_FILEREADER_HPP_

#include <sstream>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

namespace file_reader
{

/**
 *  FileReader, to extract raw data from file.
 */
class FileReader
{
public:

	/**
	 * FileReader's constructor.
	 * @param inStream Stream buffer from input file.
	 */
	FileReader( std::streambuf* inStream );

	/**
	 * Get the file length.
	 * @return File buffer length (in byte).
	 */
	size_t getLength();

	/**
	 * Get the pointer position within the file.
	 * @return Offset from the beginning of the file (in byte).
	 */
	size_t getPosition();

	/**
	 * Extract data from file
	 * @param[out] data Buffer to fill.
	 * @param[in]  size Buffer size (in byte) to copy.
	 * @return     Whether data have been correctly read and copied from file, or not.
	 */
	bool readData( std::vector< char >& data, const size_t size );

	/**
	 * Extract word from file
	 * @param[out] data Buffer to fill.
	 * @param[in]  caracter of end of string.
	 * @return     Whether data have been correctly read and copied from file, or not.
	 */
	bool readWord( std::vector< char >& data, const char endOfString );

	/**
	 * Set the pointer at the beginning of the file.
	 */
	void goToBegin();

	/**
	 * Move the pointer position backward in the stream.
	 * @param size Number of bytes to take backward.
	 */
	void goBack   ( const std::size_t size );

	/**
	 * Move the pointer position forward in the stream.
	 * @param size Number of bytes to take forward.
	 */
	void goForward( const std::size_t size );

	/**
	 * Indicate whether the pointer is at the end of the file stream.
	 * @return Whether the pointer position corresponds to the end of file, or not.
	 */
	bool isEndOfFile( );

private:
	std::istream _fileBuffer;
};

}

#endif
