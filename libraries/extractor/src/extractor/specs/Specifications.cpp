#include "Specifications.hpp"

#include <common/global.hpp>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <iterator>

namespace bfs = boost::filesystem;

Specifications::Specifications()
{

}

void Specifications::init()
{
	paths.clear();
	if( const char* env_options = std::getenv( "QC_SPECS_DIR" ) )
	{
		boost::algorithm::split( paths, env_options, boost::algorithm::is_any_of(" ") );
	}
}

void Specifications::updateList()
{
	BOOST_FOREACH( std::string filepath, paths )
	{
		COMMON_COUT_DEBUG( "Search specs in path : " << filepath );
		bfs::path path( filepath );
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
							Specification spec;
							spec.setSpecTree( pt );
							COMMON_COUT_DEBUG( "Add spec : " << spec.getLabel() );
							specList.push_back( spec );
						}
					}
				}
				else
				{
					COMMON_CERR( "Specification load: " << path.string() << " exists, but is neither a regular file nor a directory" );
				}
			}
			else
			{
				COMMON_CERR( "Specification load: " << path.string() << " does not exist" );
			}
		}
		catch( const bfs::filesystem_error& ex )
		{
			COMMON_CERR( "Specification load: " <<  ex.what() );
		}
	}
}
