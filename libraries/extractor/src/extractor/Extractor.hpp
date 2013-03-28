#ifndef _EXTRACTOR_EXTRACTOR_HPP_
#define _EXTRACTOR_EXTRACTOR_HPP_

#include <string>

class Report;
class File;
class Specification;

class Extractor
{
public:
	Extractor();
	~Extractor();
	
	void init();
	void uninit();
	
	bool openFilename( const std::string& filename );
	void closeFilename( );
	
	void getReport( Report* report );
	
private:
	File* _file;
	Specification* _specs;
};

#endif
