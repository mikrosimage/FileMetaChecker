#include "Specification.hpp"

#include <Common/common.hpp>
#include <Common/log.hpp>
#include <Common/color.hpp>

#include <fstream>

namespace spec_reader
{

Specification::Specification()
{
}

Specification::~Specification()
{
}


void Specification::setFromString( const std::string& string )
{
	try
	{
		_specDoc.Parse<0>( string.c_str() );
		if( _specDoc.HasParseError() )
			throw std::runtime_error( std::string( _specDoc.GetParseError() ) + "  @char:# " + std::to_string( _specDoc.GetErrorOffset() ) );
	}
	catch( const std::exception& ex )
	{
		LOG_ERROR( "Specification from string: "<<  ex.what() );
		throw;
	}
}

bool Specification::setFromFile( const std::string& filepath )
{
	try
	{
	    std::ifstream file( filepath );
	    bool ret = false;
		if( file.good() )
		{
			std::string content( ( std::istreambuf_iterator<char>( file ) ),
			                       std::istreambuf_iterator<char>()       );
			_specDoc.Parse<0>( content.c_str() );
			if( _specDoc.HasParseError() )
				throw std::runtime_error( std::string( _specDoc.GetParseError() ) + "  @char:# " + std::to_string( _specDoc.GetErrorOffset() ) );
			file.close();
			ret = true;
		}
		return ret;
	}
	catch( const std::exception& ex )
	{
		LOG_ERROR( "Specification from file ("<< filepath << "): "<<  ex.what() );
		throw;
	}
	return false;
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
	std::vector< std::string > list;
	std::string standardStr  = kStandard;
	std::string extensionStr = kExtension;

	for( rapidjson::Value::ConstValueIterator itr  = _specDoc[ standardStr.c_str() ][ extensionStr.c_str() ].Begin();
		                                      itr != _specDoc[ standardStr.c_str() ][ extensionStr.c_str() ].End();
		                                      ++itr )
		list.push_back( std::string( itr->GetString() ) );
	return list;
}

std::shared_ptr< spec_reader::SpecNode > Specification::getFirstNode()
{
	try
	{
		std::string headerStr  = kHeader;
		rapidjson::Value::ConstValueIterator begin = _specDoc.FindMember( headerStr.c_str() )->value.Begin();
		return std::make_shared< spec_reader::SpecNode >( this, begin );
	}
	catch( const std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

rapidjson::Value::ConstValueIterator Specification::end() const
{
	std::string headerStr  = kHeader;
	return _specDoc.FindMember( headerStr.c_str() )->value.End();
}

std::string Specification::getSpecInfo( const std::string& key )
{
	std::string standardStr = kStandard;
	std::string keyStr = key;
	return std::string( _specDoc[ standardStr.c_str() ][ keyStr.c_str() ].GetString() );
}

}
