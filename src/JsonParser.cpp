#include "JsonParser.hpp"

#include <dirent.h>

JsonParser::JsonParser()
{}

void JsonParser::parseJsonFile ( const std::string& filename )
{

	bpt::ptree pt;
	bpt::read_json(filename, pt);
	// std::cout << "Json file name : " << pt.get<std::string>("fileType.name") << std::endl;
	_ptreeList.push_back( pt );
}

void JsonParser::readJsonFolder( const std::string& folderPath )
{
	DIR* jsonFolder = opendir( folderPath.c_str() );
	dirent* pdir;
	std::string filename;
	while ( pdir = readdir( jsonFolder ) )
	{
		filename = pdir->d_name;
		if ( filename.size() > 5 && filename.compare(filename.size()-5, 5, ".json") == 0 )
		{
			filename = folderPath + "/" + filename;
			parseJsonFile( filename );
		}
	}
}