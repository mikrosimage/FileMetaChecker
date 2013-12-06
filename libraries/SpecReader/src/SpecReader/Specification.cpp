#include "Specification.hpp"

#include <Common/common.hpp>
#include <Common/log.hpp>
#include <Common/color.hpp>

// #include <rapidjson/allocators.h>
// #include <rapidjson/stringbuffer.h>
// #include <rapidjson/writer.h>

#include <fstream>

namespace spec_reader
{

Specification::Specification()
{
}

Specification::~Specification()
{
}


void Specification::setFromString( const std::string& jsonString )
{
	try
	{
		_specDoc.Parse<0>( jsonString.c_str() );
		if( _specDoc.HasParseError() )
			throw std::runtime_error( std::string( _specDoc.GetParseError() ) + "  @char:# " + std::to_string( _specDoc.GetErrorOffset() ) );
	}
	catch( const std::runtime_error& ex )
	{
		LOG_ERROR( "[specification] set from string: "<<  ex.what() );
		throw;
	}
}

bool Specification::setFromFile( const std::string& filepath )
{
	try
	{
		std::ifstream file( filepath );
		if( ! file.good() )
			throw std::runtime_error( "[specification] Invalid specification file" );

		std::string content( ( std::istreambuf_iterator<char>( file ) ), std::istreambuf_iterator<char>() );
		_specDoc.Parse<0>( content.c_str() );
		file.close();

		if( _specDoc.HasParseError() )
		{
			LOG_ERROR( std::string( _specDoc.GetParseError() ) + "  @char:# " + std::to_string( _specDoc.GetErrorOffset() ) );
			return false;
		}
		return true;
	}
	catch( const std::exception& ex )
	{
		LOG_ERROR( "[specification] set from file ("<< filepath << "): "<<  ex.what() );
		throw;
	}
}

std::string Specification::getId( )
{
	return getSpecInfo( kId );
}

std::string Specification::getLabel( )
{
	return getSpecInfo( kLabel );
}

std::string Specification::getType( )
{
	return getSpecInfo( kType );
}

std::vector< std::string > Specification::getSupportedExtensions( )
{
	try
	{
		rapidjson::Value::Member* description;
		if( ! ( description = _specDoc.FindMember( std::string( kDescription ).c_str() ) ) )
			throw std::runtime_error( "[specification] Invalid specification file: must contain a format description field" );
		
		rapidjson::Value::Member* extensions;
		if( ! ( extensions = description->value.FindMember( std::string( kExtensions ).c_str() ) ) )
			throw std::runtime_error( "[specification] No extensions" );

		std::vector< std::string > list;
		for( rapidjson::Value::ConstValueIterator itr  = extensions->value.Begin(); itr != extensions->value.End(); ++itr )
			list.push_back( std::string( itr->GetString() ) );
		
		return list;
	}
	catch( const std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

std::shared_ptr< spec_reader::SpecNode > Specification::getFirstNode()
{
	try
	{
		rapidjson::Value::Member* content;
		if( ! ( content = _specDoc.FindMember( std::string( kContent ).c_str() ) ) )
			throw std::runtime_error( "[specification] Invalid specification file: must contain a content field" );

		return std::make_shared< spec_reader::SpecNode >( this, content->value.Begin() );
	}
	catch( const std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

rapidjson::Value::ConstValueIterator Specification::end() const
{
	try
	{
		const rapidjson::Value::Member* content;
		if( ! ( content = _specDoc.FindMember( std::string( kContent ).c_str() ) ) )
			throw std::runtime_error( "[specification] Invalid specification file: must contain a content field" );

		return content->value.End();
	}
	catch( const std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

std::string Specification::getSpecInfo( const std::string& key )
{
	try
	{
		rapidjson::Value::Member* description;
		if( ! ( description = _specDoc.FindMember( std::string( kDescription ).c_str() ) ) )
			throw std::runtime_error( "[specification] Invalid specification file: must contain a format description field" );
		
		rapidjson::Value::Member* field;
		if( ! ( field = description->value.FindMember( std::string( key ).c_str() ) ) )
			throw std::runtime_error( "[specification] No such node (" + key + ")" );
		
		return std::string( field->value.GetString() );
	}
	catch( const std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

}
