#ifndef _EXTRACTOR_EXTRACTOR_HPP_
#define _EXTRACTOR_EXTRACTOR_HPP_

#include <boost/property_tree/ptree.hpp>

#include <string>
#include <vector>

namespace bpt = boost::property_tree;

class Report;
class File;
class Specifications;

class Extractor
{
public:
	Extractor();
	~Extractor();
	
	void init( );
	void uninit();
	
	bool openFilename( const std::string& filename );
	void closeFilename( );
	
	void analyse( );

	void getReport( Report* report );
	
private:
	File*                _file;
	Specifications*       _specs;
	std::vector< bpt::ptree >  _report;
	std::string           _ext;
};

#endif
