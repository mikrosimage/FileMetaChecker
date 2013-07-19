#include "SpecList.hpp"

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

SpecList::SpecList()
{
}

SpecList::~SpecList()
{
}

void SpecList::initDirectoryPaths()
{
	clearDirectories();
	std::vector< std::string > dirList;
	if( const char* env_options = std::getenv( "QC_SPECS_DIR" ) )
		boost::algorithm::split( dirList, env_options, boost::algorithm::is_any_of(" ") );

	BOOST_FOREACH( std::string dirPath, dirList )
		addDirectoryPath( dirPath );
}

void SpecList::addDirectoryPath( const std::string& directoryPath )
{
	bfs::path path( directoryPath );
	try
	{
		if( !exists( path ) )
			throw std::runtime_error( " does not exist" );

		if ( !is_directory( path ) )
			throw std::runtime_error( " is not a directory" );

		_directories.push_back( directoryPath );
	}
	catch( const std::runtime_error& ex )
	{
		LOG_WARNING( "Init Directory Path: "<< path.string() <<  ex.what() );
	}
	catch( const bfs::filesystem_error& ex )
	{
		LOG_ERROR( "Init Directory Path: " <<  ex.what() );
	}
}

void SpecList::addSpecFromDirectories()
{
	BOOST_FOREACH( std::string filepath, _directories )
	{
		LOG_INFO( "Searching specs in path : " << filepath );
		bfs::path path( filepath );
		bfs::directory_iterator end_itr;
		for( bfs::directory_iterator itr( path ); itr != end_itr; ++itr )
		{
			Specification spec;
			if( spec.setFromFile( itr->path().string() ) )
				addSpecification( spec );
		}
	}
	LOG_TRACE( getSpecNumber() << " specification file(s) loaded" );
}

void SpecList::addSpecification( const Specification& spec )
{
	_specifications.push_back( spec );
}


void SpecList::clearSpecifications()
{
	_specifications.clear();
}

void SpecList::clearDirectories()
{
	_directories.clear();
}

void SpecList::getSpec( Specification& spec, const size_t index ) const
{
	spec = _specifications.at( index );
}

void SpecList::getSpecList( std::vector< Specification >& specs ) const
{
	specs = _specifications;
}

size_t SpecList::getSpecNumber() const
{
	return _specifications.size();
}
