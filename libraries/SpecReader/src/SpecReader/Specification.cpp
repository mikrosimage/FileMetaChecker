#include "Specification.hpp"

#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace bpt = boost::property_tree;
namespace bfs = boost::filesystem;

namespace spec_reader
{

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

		bpt::read_json( path.string(), _specTree );
		return true;
	}
	catch( const bfs::filesystem_error& ex )
	{
		std::cout << "Error: " <<  "Specification from file: " <<  ex.what() << std::endl;
	}
	catch( const std::runtime_error& ex )
	{
		std::cout << "Warning: " << "Specification from file: "<< path.string() <<  ex.what() << std::endl;
	}
	return false;
}

void Specification::setFromString( const std::string& string )
{
	std::istringstream isstream( string );
	try
	{
		bpt::read_json( isstream, _specTree );
	}
	catch( const std::exception& ex )
	{
		std::cout << "Error: " <<  "Specification from String: " <<  ex.what() << std::endl;
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
		std::cout << "Error: " <<  e.what() << std::endl;
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
		std::cout << "Error: " <<  e.what() << std::endl;
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
		std::cout << "Error: " <<  e.what() << std::endl;
		throw;
	}
}

std::vector< std::string > Specification::getSupportedExtensions( )
{
	try
	{
		std::vector< std::string > list;
		for( bpt::ptree::value_type& node : _specTree.get_child( kStandard + "." + kExtension ) )
		{
			list.push_back( node.second.data() );
		}
		return list;
	}
	catch( const std::runtime_error& e )
	{
		std::cout << "Error: " <<  e.what() << std::endl;
		throw;
	}
}

SpecNode* Specification::getFirstNode()
{
	try
	{
		SpecNode* s = new SpecNode( _specTree.get_child( kHeader ).begin() );
		return s;
	}
	catch( const std::runtime_error& e )
	{
		std::cout << "Error: " <<  e.what() << std::endl;
		throw;
	}
}

}

