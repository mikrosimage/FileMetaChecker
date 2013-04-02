#ifndef _EXTRACTOR_EXTRACTOR_HPP_
#define _EXTRACTOR_EXTRACTOR_HPP_

#include <string>

class Report;
class File;
class Specifications;

class Extractor
{
public:
	Extractor();
	~Extractor();
	
	void init();
	void uninit();
	
	bool openFilename( const std::string& filename );
	void closeFilename( );
	
	void analyse( );

	void getReport( Report* report );

private:
	File*           _file;
	Specifications* _specs;
	std::string     _ext;
};

#endif
