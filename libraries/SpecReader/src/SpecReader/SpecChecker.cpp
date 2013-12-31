#include "SpecChecker.hpp"

#include <Common/log.hpp>

#include <rapidjson/allocators.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <fstream>
#include <stdexcept>

namespace spec_reader
{

SpecChecker::SpecChecker( const std::string& filepath )
	: _filepath ( filepath )
	, _document ( new rapidjson::Document )
{
}

bool SpecChecker::check()
{
	try
	{
		LOG_TRACE( "[speccheker] filepath: " << _filepath );
		std::ifstream file( _filepath );

		if( ! file.good() )
			throw std::runtime_error( "[specchecker] Invalid specification file" );

		std::string content( ( std::istreambuf_iterator<char>( file ) ), std::istreambuf_iterator<char>() );
		_document->Parse<0>( content.c_str() );
		file.close();

		if( _document->HasParseError() )
			throw std::runtime_error( std::string( _document->GetParseError() ) + "  @char:# " + std::to_string( _document->GetErrorOffset() ) );
		
		LOG_TRACE( "[speccheker] _document OK !" );
		checkSubFile( _document->FindMember( std::string( kContent ).c_str() ) );
		return true;
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		return false;
	}
}

void SpecChecker::checkSubFile( rapidjson::Value::Member* member )
{
	LOG_TRACE( "[speccheker] checkSubFile" );
	if( member == nullptr )
		throw std::runtime_error( "[speccheker] Invalid specification file: must contain a content field" );

	for( rapidjson::Value::ValueIterator node = member->value.Begin(); node != member->value.End(); ++node )
	{
		for( rapidjson::Value::ConstMemberIterator itr = node->MemberBegin(); itr != node->MemberEnd(); ++itr )
		{
			std::string key( itr->name.GetString() );
			if( key != kFile )
				continue;

			std::string subfilename( itr->value.GetString() );
			
			rapidjson::Document subDoc;
			std::string dirname = _filepath.substr( 0, _filepath.find_last_of( "/" ) + 1 );
			std::ifstream file( dirname + subfilename + ".json" );
			if( file.good() )
			{
				std::string content( ( std::istreambuf_iterator< char >( file ) ), std::istreambuf_iterator< char >() );
				subDoc.Parse<0>( content.c_str() );
				file.close();
			}

			node->RemoveMember( std::string( kFile ).c_str() );
			rapidjson::Value::Member* description = subDoc.FindMember( std::string( kDescription ).c_str() );
			rapidjson::Value::Member* id    = description->value.FindMember( std::string( kId    ).c_str() );
			rapidjson::Value::Member* label = description->value.FindMember( std::string( kLabel ).c_str() );
			node->AddMember( std::string( kId    ).c_str(),    id->value.GetString(), _document->GetAllocator() );
			node->AddMember( std::string( kLabel ).c_str(), label->value.GetString(), _document->GetAllocator() );

			rapidjson::Value extNodes( rapidjson::kArrayType );
			node->AddMember( std::string( kGroup ).c_str(), extNodes, _document->GetAllocator() );

			rapidjson::Value::Member* subMember = subDoc.FindMember( std::string( kContent ).c_str() );
			for( rapidjson::Value::ConstValueIterator subNode = subMember->value.Begin(); subNode != subMember->value.End(); ++subNode )
				includeExtNode( node->FindMember( std::string( kGroup ).c_str() ), subNode );
			break;
		}

		if( node->HasMember( std::string( kGroup ).c_str() ) )
			checkSubFile( node->FindMember( std::string( kGroup ).c_str() ) );
	}
}

void SpecChecker::includeExtNode( rapidjson::Value::Member* member, rapidjson::Value::ConstValueIterator subNode )
{
	LOG_TRACE( "[speccheker] includeExtNode " << member->name.GetString() << std::endl );
	rapidjson::Value object( rapidjson::kObjectType );

	for( rapidjson::Value::ConstMemberIterator itr = subNode->MemberBegin(); itr != subNode->MemberEnd(); ++itr )
	{
		switch( itr->value.GetType() )
		{
			case rapidjson::kNullType  : object.AddMember( itr->name.GetString(), std::string( kNull ).c_str(), _document->GetAllocator() ); break;
			case rapidjson::kFalseType :
			case rapidjson::kTrueType  :
			{
				object.AddMember( itr->name.GetString(), itr->value.GetBool(), _document->GetAllocator() );
				break;
			}
			case rapidjson::kObjectType : break;
			case rapidjson::kArrayType  :
			{
				rapidjson::Value array( rapidjson::kArrayType );
				object.AddMember( itr->name.GetString(), array, _document->GetAllocator() );

				for( rapidjson::Value::ConstValueIterator child = itr->value.Begin(); child != itr->value.End(); ++child )
				{
					if( child->GetType() != rapidjson::kObjectType )
					{
						switch( child->GetType() )
						{	
							case rapidjson::kNullType :
							{
								object.FindMember( itr->name.GetString() )->value.PushBack( std::string( kNull ).c_str(), _document->GetAllocator() );
								break;
							}
							case rapidjson::kFalseType :
							case rapidjson::kTrueType :
							{
								object.FindMember( itr->name.GetString() )->value.PushBack( child->GetBool(), _document->GetAllocator() );
								break;
							}
							case rapidjson::kStringType :
							{
								object.FindMember( itr->name.GetString() )->value.PushBack( child->GetString(), _document->GetAllocator() );
								break;
							}
							case rapidjson::kNumberType :
							{
								if( itr->value.IsInt() )
									object.FindMember( itr->name.GetString() )->value.PushBack( child->GetInt(), _document->GetAllocator() );
								if( itr->value.IsDouble() )
									object.FindMember( itr->name.GetString() )->value.PushBack( child->GetDouble(), _document->GetAllocator() );
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
				object.AddMember( itr->name.GetString(), itr->value.GetString(), _document->GetAllocator() );
				break;
			}
			case rapidjson::kNumberType :
			{
				if( itr->value.IsInt() )
					object.AddMember( itr->name.GetString(), itr->value.GetInt(), _document->GetAllocator() );
				if( itr->value.IsDouble() )
					object.AddMember( itr->name.GetString(), itr->value.GetDouble(), _document->GetAllocator() );
				break;
			} 
		}
	}
	member->value.PushBack( object, _document->GetAllocator() );
}

std::string SpecChecker::getSpecString()
{
	try
	{
		rapidjson::StringBuffer strbuf;
		rapidjson::Writer< rapidjson::StringBuffer > writer( strbuf );
		_document->Accept( writer );
		return strbuf.GetString();
	}
	catch( const std::exception& e )
	{
		LOG_ERROR( "[speccheker] displaySpec : " << e.what() );
		throw;
	}
}

}
