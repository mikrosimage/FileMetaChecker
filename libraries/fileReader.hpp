#ifndef _QICHE_FILEREADER_HPP_
#define _QICHE_FILEREADER_HPP_

class FileReader
{
public:
	FileReader()
	{
	}
	
	void read( char* data, const size_t size )
	{
		for( size_t i = 0; i < size; ++i )
		{
			data[i] = index++;
		}
	}
private:
	static unsigned char index;
};

unsigned char FileReader::index = 0;

#endif
