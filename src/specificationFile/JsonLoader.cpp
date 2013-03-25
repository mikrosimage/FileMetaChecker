#include "JsonLoader.hpp"

#include <common/global.hpp>

#include <boost/filesystem.hpp>

#include <iterator>

namespace bfs = boost::filesystem;

JsonParser::JsonParser()
{
}

std::vector<bpt::ptree> JsonParser::load( const std::string& folderPath )
{
	std::vector<bpt::ptree> ptreeList;
	bfs::path path( folderPath );
	try
	{
		if( exists( path ) )
		{
			if( is_directory( path ) )
			{
				bfs::directory_iterator end_itr;
				for( bfs::directory_iterator itr( path ); itr != end_itr; ++itr )
				{
					if ( is_regular( itr->status() ) )
					{
						bpt::ptree pt;
						bpt::read_json( itr->path().string(), pt );
						ptreeList.push_back( pt );
					}
				}
			}
			else
			{
				COMMON_CERR( "Json loader: " << path << " exists, but is neither a regular file nor a directory" );
			}
		}
		else
		{
			COMMON_CERR( "Json loader: " << path << " does not exist" );
		}
	}
	catch( const bfs::filesystem_error& ex )
	{
		COMMON_CERR( "Json loader: " <<  ex.what() );
	}
	return ptreeList;
}
