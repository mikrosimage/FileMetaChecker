#ifndef _EXTRACTOR_EXTRACTOR_HPP_
#define _EXTRACTOR_EXTRACTOR_HPP_

#include <boost/property_tree/ptree.hpp>

#include <string>
#include <vector>

namespace bpt = boost::property_tree;

class Report;
class File;
class Specifications;

static const std::string kStatus   = "status";
static const std::string kSpec     = "specification";
static const std::string kStream   = "stream";
static const std::string kMetadata = "metadata";

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
	std::string getDate();
	
private:
	File*                      _file;
	Specifications*            _specs;
	bpt::ptree                 _statusReport;
	bpt::ptree                 _streamReport;
	std::vector< bpt::ptree >  _specReports;
	std::string                _ext;
};

#endif
