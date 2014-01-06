#include "Specification.hpp"

#include <Common/common.hpp>
#include <Common/log.hpp>
#include <Common/color.hpp>

// #include <rapidjson/allocators.h>
// #include <rapidjson/stringbuffer.h>
// #include <rapidjson/writer.h>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>

#include <fstream>

namespace spec_reader
{




Specification::Specification()
	: _rootKey ( kContent )
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

bool Specification::setFromNode( std::shared_ptr< SpecNode > node, const std::string& key )
{
	try
	{
		_rootKey = key;
		if( ! node->getIterator()->HasMember( std::string( _rootKey ).c_str() ) )
			throw std::runtime_error( "Cannot set Specification, no such key ( " + _rootKey + " )" );

		const rapidjson::Value* value = &node->getIterator()->FindMember( std::string( _rootKey ).c_str() )->value;

		rapidjson::Value array( rapidjson::kArrayType );
		_specDoc.AddMember( _rootKey.c_str(), array, _specDoc.GetAllocator() );

		for( rapidjson::Value::ConstValueIterator itr = value->Begin(); itr != value->End(); ++itr )
			includeExtNode( _specDoc.FindMember( _rootKey.c_str() ), itr );

		return true;
	}
	catch( const std::exception& ex )
	{
		LOG_ERROR( "[specification] set from node ("<< node->getId() << "): "<<  ex.what() );
		return false;
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

std::shared_ptr< SpecNode > Specification::getFirstNode()
{
	try
	{
		const rapidjson::Value::Member* content;
		if( ! ( content = _specDoc.FindMember( std::string( _rootKey ).c_str() ) ) )
			throw std::runtime_error( "[specification::getFirstNode] Invalid specification: must contain a " + _rootKey + " field" );

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
		if( ! ( content = _specDoc.FindMember( std::string( _rootKey ).c_str() ) ) )
			throw std::runtime_error( "[specification::end] Invalid specification: must contain a " + _rootKey + " field" );

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
			throw std::runtime_error( "[specification::getSpecInfo] Invalid specification file: must contain a format description field" );
		
		rapidjson::Value::Member* field;
		if( ! ( field = description->value.FindMember( std::string( key ).c_str() ) ) )
			throw std::runtime_error( "[specification::getSpecInfo] No such node (" + key + ")" );
		
		return std::string( field->value.GetString() );
	}
	catch( const std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

void Specification::includeExtNode( rapidjson::Value::Member* member, rapidjson::Value::ConstValueIterator node ) // @todo: clean ! Same as in SpecChecker...
{
	LOG_TRACE( "[Specification] includeExtNode " << member->name.GetString() << std::endl );
	rapidjson::Value object( rapidjson::kObjectType );

	for( rapidjson::Value::ConstMemberIterator itr = node->MemberBegin(); itr != node->MemberEnd(); ++itr )
	{
		switch( itr->value.GetType() )
		{
			case rapidjson::kNullType  : object.AddMember( itr->name.GetString(), std::string( kNull ).c_str(), _specDoc.GetAllocator() ); break;
			case rapidjson::kFalseType :
			case rapidjson::kTrueType  :
			{
				object.AddMember( itr->name.GetString(), itr->value.GetBool(), _specDoc.GetAllocator() );
				break;
			}
			case rapidjson::kObjectType : break;
			case rapidjson::kArrayType  :
			{
				rapidjson::Value array( rapidjson::kArrayType );
				object.AddMember( itr->name.GetString(), array, _specDoc.GetAllocator() );

				for( rapidjson::Value::ConstValueIterator child = itr->value.Begin(); child != itr->value.End(); ++child )
				{
					if( child->GetType() != rapidjson::kObjectType )
					{
						switch( child->GetType() )
						{	
							case rapidjson::kNullType :
							{
								object.FindMember( itr->name.GetString() )->value.PushBack( std::string( kNull ).c_str(), _specDoc.GetAllocator() );
								break;
							}
							case rapidjson::kFalseType :
							case rapidjson::kTrueType :
							{
								object.FindMember( itr->name.GetString() )->value.PushBack( child->GetBool(), _specDoc.GetAllocator() );
								break;
							}
							case rapidjson::kStringType :
							{
								object.FindMember( itr->name.GetString() )->value.PushBack( child->GetString(), _specDoc.GetAllocator() );
								break;
							}
							case rapidjson::kNumberType :
							{
								if( itr->value.IsInt() )
									object.FindMember( itr->name.GetString() )->value.PushBack( child->GetInt(), _specDoc.GetAllocator() );
								if( itr->value.IsDouble() )
									object.FindMember( itr->name.GetString() )->value.PushBack( child->GetDouble(), _specDoc.GetAllocator() );
								break;
							}
							default:
								break;
						}
					}
					else
					{
						includeExtNode( object.FindMember( itr->name.GetString() ), child );
					}
				}
				break;
			}
			case rapidjson::kStringType :
			{
				object.AddMember( itr->name.GetString(), itr->value.GetString(), _specDoc.GetAllocator() );
				break;
			}
			case rapidjson::kNumberType :
			{
				if( itr->value.IsInt() )
					object.AddMember( itr->name.GetString(), itr->value.GetInt(), _specDoc.GetAllocator() );
				if( itr->value.IsDouble() )
					object.AddMember( itr->name.GetString(), itr->value.GetDouble(), _specDoc.GetAllocator() );
				break;
			} 
		}
	}
	member->value.PushBack( object, _specDoc.GetAllocator() );
}

}
