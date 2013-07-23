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
	}
}


std::string Specification::getId( )
{
	return _specTree.get< std::string >( kStandard + "." + kId );
}

std::string Specification::getLabel( )
{
	return _specTree.get< std::string >( kStandard + "." + kLabel );
}

std::string Specification::getType( )
{
	return _specTree.get< std::string >( kStandard + "." + kType );
}

std::vector< std::string > Specification::getSupportedExtensions( )
{
	std::vector< std::string > list;
	BOOST_FOREACH( bpt::ptree::value_type &node, _specTree.get_child( kStandard + "." + kExtension ) )
	{
		list.push_back( node.second.data() );
	}
	return list;
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
	return SpecNode( getBody(), 0, getBodySize() );
}

