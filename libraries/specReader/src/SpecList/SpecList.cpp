#include "SpecList.hpp"

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

namespace spec_reader
{

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
	
	if( dirList.size() == 0 )
		LOG_WARNING( "initDirectoryPaths: No specification file found.");

	for( std::string& dirPath : dirList )
		addDirectoryPath( dirPath );
}

void SpecList::addDirectoryPath( const std::string& directoryPath )
{
	bfs::path path( directoryPath );
	try
	{
		if( ! exists( path ) )
			throw std::runtime_error( " does not exist" );

		if( ! is_directory( path ) )
			throw std::runtime_error( " is not a directory" );

		_directories.push_back( directoryPath );
	}
	catch( const bfs::filesystem_error& ex )
	{
		LOG_ERROR( "Init Directory Path: " <<  ex.what() );
	}
	catch( const std::runtime_error& ex )
	{
		LOG_ERROR( "Init Directory Path: "<< path.string() <<  ex.what() );
	}
}

void SpecList::addSpecFromDirectories()
{
	for( std::string& filepath : _directories )
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

Specification SpecList::getSpec( const std::string& specId )
{
	try
	{
		for( Specification& spec : _specifications )
		{
			if( spec.getId() == specId )
				return spec;
		}
		throw std::runtime_error( "No fitting specification found." );
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

void SpecList::getSpecList( std::map< std::string, std::string >& specIds )
{
	for( Specification& spec : _specifications )
	{
		specIds[ spec.getId() ] = spec.getLabel();
	}
}

size_t SpecList::getSpecNumber() const
{
	return _specifications.size();
}

}
