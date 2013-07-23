#include "Specification.hpp"

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

const std::string kExtension = "extension";
const std::string kFooter    = "footer";
const std::string kHeader    = "header";
const std::string kStandard  = "standard";

Specification::Specification()
{
}

Specification::~Specification()
{
}

void Specification::setFromTree( const bpt::ptree& spec )
{
	_specTree = spec;
}

bool Specification::setFromFile( const std::string& filepath )
{
	bfs::path path( filepath );
	try
	{
		if( !exists( path ) )
			throw std::runtime_error( " does not exist" );

		if ( !is_regular( path ) )
			throw std::runtime_error( " is not a regular file" );

		if ( path.extension() != ".json" )
			throw std::runtime_error( " - Invalid extension: '.json' expected" );

		LOG_TRACE( path.string() << ": Reading JSON..." );
		bpt::read_json( path.string(), _specTree );
		return true;
	}
	catch( const std::runtime_error& ex )
	{
		LOG_WARNING( "Specification from file: "<< path.string() <<  ex.what() );
	}
	catch( const bfs::filesystem_error& ex )
	{
		LOG_ERROR( "Specification from file: " <<  ex.what() );
	}
	return false;
}

void Specification::setFromString( const std::string& string )
{
	std::istringstream isstream( string );
	try
	{
		LOG_TRACE( "Reading JSON..." );
		bpt::read_json( isstream, _specTree );
	}
	catch( const std::exception& ex )
	{
		LOG_ERROR( "Specification from String: " <<  ex.what() );
		throw;
	}
}


std::string Specification::getId( )
{
	try
	{
		return _specTree.get< std::string >( kStandard + "." + kId );
	}
	catch( const std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

std::string Specification::getLabel( )
{
	try
	{
		return _specTree.get< std::string >( kStandard + "." + kLabel );
	}
	catch( const std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

std::string Specification::getType( )
{
	try
	{
		return _specTree.get< std::string >( kStandard + "." + kType );
	}
	catch( const std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

std::vector< std::string > Specification::getSupportedExtensions( )
{
	try
	{
		std::vector< std::string > list;
		BOOST_FOREACH( bpt::ptree::value_type &node, _specTree.get_child( kStandard + "." + kExtension ) )
		{
			list.push_back( node.second.data() );
		}
		return list;
	}
	catch( const std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

bpt::ptree::const_iterator Specification::getBody( )
{
	return _specTree.get_child( kHeader ).begin();
}

size_t Specification::getBodySize( )
{
	return _specTree.get_child( kHeader ).size();
}

SpecNode Specification::getFirstNode()
{
	try
	{
		return SpecNode( getBody(), 0, getBodySize() );
	}
	catch( const std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

