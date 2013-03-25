#include "FileSpecificationLoader.hpp"
#include "JsonLoader.hpp"

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

FileSpecificationLoader::FileSpecificationLoader()
{
}

void FileSpecificationLoader::load( )
{
	JsonParser jsonParser;
	
	// get environment options and parse them
	if( const char* env_options = std::getenv("QC_SPECS_DIR") )
	{
		std::vector<std::string> vecPaths;
		boost::algorithm::split( vecPaths, env_options, boost::algorithm::is_any_of(" ") );
		BOOST_FOREACH( std::string path, vecPaths )
		{
			std::vector<bpt::ptree> trees = jsonParser.load( path );
			
			_ptreeList.reserve( _ptreeList.size() + trees.size() );
			BOOST_FOREACH( bpt::ptree tree, trees )
			{
				_ptreeList.push_back( tree );
			}
		}
	}
	//_ptreeList = jsonParser.load( "fileSpecification" );
}
