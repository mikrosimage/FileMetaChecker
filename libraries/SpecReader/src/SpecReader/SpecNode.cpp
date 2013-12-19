#include "SpecNode.hpp"
#include "Specification.hpp"
#include <Common/log.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>

extern common::ELogLevel common::level;

namespace spec_reader
{

namespace property_parser
{
	std::string valueToString( const rapidjson::Value* value )
	{
		std::string ret;
		if( value->IsInt() )
			ret = std::to_string( value->GetInt() );

		if( value->IsDouble() )
			ret = std::to_string( value->GetDouble() );

		if( value->IsString() )
			ret = value->GetString();
		return ret;
	}

	template< typename ValueType >
	ValueType getProperty( const TreeNodeIt node, const std::string& prop );

	template< >
	std::string getProperty< std::string >( const TreeNodeIt node, const std::string& prop )
	{
		try
		{
			if( ! node->HasMember( std::string( prop ).c_str() ) )
				throw std::runtime_error( "[specnode] No such node (" + prop + ")" );
			return node->FindMember( std::string( prop ).c_str() )->value.GetString();
		}
		catch( std::runtime_error& e )
		{
			LOG_ERROR( e.what() );
			throw;
		}
	}

	template< typename ValueType >
	ValueType getProperty( const TreeNodeIt node, const std::string& prop, const ValueType& defaultValue );

	template< >
	std::string getProperty< std::string >( const TreeNodeIt node, const std::string& prop, const std::string& defaultValue )
	{
		if( ! node->HasMember( std::string( prop ).c_str() ) )
			return defaultValue;
		return valueToString( &node->FindMember( std::string( prop ).c_str() )->value );
	}

	template< >
	char getProperty< char >( const TreeNodeIt node, const std::string& prop, const char& defaultValue )
	{
		char ret = defaultValue;
		if( ! node->HasMember( std::string( prop ).c_str() ) )
			return ret;

		const rapidjson::Value* val = &node->FindMember( std::string( prop ).c_str() )->value;
		if( val->IsInt() )
			ret = (char) val->GetInt();
		if( val->IsString() )
			ret = (char) val->GetString()[0];
		return ret;
	}

	template< >
	bool getProperty< bool >( const TreeNodeIt node, const std::string& prop, const bool& defaultValue )
	{
		if( ! node->HasMember( std::string( prop ).c_str() ) )
			return defaultValue; 
		return node->FindMember( std::string( prop ).c_str() )->value.GetBool();
	}
}

size_t SpecNode::_globalIndex = 0;

SpecNode::SpecNode( const Specification* spec, const TreeNodeIt node, const SpecNode* parent )
	: _uId ( _globalIndex++ )
	, _node( node )
	, _parent( parent )
	, _specification( spec )
{
}

std::string SpecNode::getId() const
{
	return property_parser::getProperty< std::string >( _node, kId );
}

std::string SpecNode::getLabel() const
{
	return property_parser::getProperty< std::string >( _node, kLabel );
}

EType SpecNode::getType() const
{
	try
	{
		return typeMap.at( property_parser::getProperty< std::string >( _node, kType, "" ) );
	}
	catch( const std::out_of_range& oor )
	{
		LOG_WARNING( "[specnode] Unknown type" );
		return eTypeUnknown;
	}
}

EDisplayType SpecNode::getDisplayType() const
{
	try
	{
		return displayTypeMap.at( property_parser::getProperty< std::string >( _node, kDisplayType, "" ) );
	}
	catch( const std::out_of_range& oor )
	{
		LOG_WARNING( "[specnode] Unknown displayType" );
		return eDisplayTypeDefault;
	}
}

std::string SpecNode::getCount() const
{
	return property_parser::getProperty< std::string >( _node, kSize, "" );
}

std::string SpecNode::getRequirement() const
{
	return property_parser::getProperty< std::string >( _node, kRequirement, "" );
}

std::string SpecNode::getGroupSize() const
{
	return property_parser::getProperty< std::string >( _node, kGroupSize, "" );
}

char SpecNode::getEndChar() const
{
	return property_parser::getProperty< char >( _node, kEndsWith, 0 );
}

bool SpecNode::isGroup() const
{
	return _node->HasMember( std::string( kGroup ).c_str() );
}

bool SpecNode::isOrdered() const
{
	return property_parser::getProperty< bool >( _node, kOrdered, true );
}

bool SpecNode::isOptional() const
{
	return property_parser::getProperty< bool >( _node, kOptional, false );
}

bool SpecNode::isBigEndian() const 
{
	return ( property_parser::getProperty< std::string >( _node, kEndian, kEndianBig ) == kEndianBig );
}

bool SpecNode::isCaseSensitive() const
{
	return property_parser::getProperty< bool >( _node, kCaseSensitive, false );
}

bool SpecNode::keepEndingChar() const
{
	return property_parser::getProperty< bool >( _node, kKeepEnd, true );
}

std::vector< std::string > SpecNode::getValues() const
{
	std::vector< std::string > values;
	if( ! _node->HasMember( std::string( kValues ).c_str() ) )
		return values;

	const rapidjson::Value* valuesNode = &_node->FindMember( std::string( kValues ).c_str() )->value;
	if( ! valuesNode->IsArray() )
	{
		values.push_back( property_parser::valueToString( valuesNode ) );
		return values;
	}

	for( rapidjson::Value::ConstValueIterator itr = valuesNode->Begin(); itr != valuesNode->End(); ++itr  )
		values.push_back( property_parser::valueToString( itr ) );
	return values;
}

std::vector< std::pair< std::string, std::string > > SpecNode::getRange() const
{
	try
	{
		std::vector< std::pair< std::string, std::string > > ranges;
		if( ! _node->HasMember( std::string( kRange ).c_str() ) )
			return ranges;

		const rapidjson::Value* rangesNode = &_node->FindMember( std::string( kRange ).c_str() )->value;

		if( ! rangesNode->IsArray() )
			throw std::runtime_error( "[specnode] Invalid range definition" );

		for( rapidjson::Value::ConstValueIterator itr = rangesNode->Begin(); itr != rangesNode->End(); ++itr  )
		{
			std::pair< std::string, std::string > range { "", "" };
			if( itr->HasMember( std::string( kMin ).c_str() ) )
				range.first = property_parser::valueToString( &itr->FindMember( std::string( kMin ).c_str() )->value );
			
			if( itr->HasMember( std::string( kMax ).c_str() ) )
				range.second = property_parser::valueToString( &itr->FindMember( std::string( kMax ).c_str() )->value );
			
			if( range.first != "" || range.second != "" )
				ranges.push_back( range );
		}
		return ranges;
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

std::vector< std::pair< std::string, std::string > > SpecNode::getRepetitions() const
{
	std::vector< std::pair< std::string, std::string > > repetitions;

	if( ! _node->HasMember( std::string( kRepetition ).c_str() ) )
		return repetitions;

	const rapidjson::Value* repetitionsNode = &_node->FindMember( std::string( kRepetition ).c_str() )->value;

	if( ! repetitionsNode->IsArray() )
	{
		std::pair< std::string, std::string > repetitionPair {
			property_parser::valueToString( repetitionsNode ),
			property_parser::valueToString( repetitionsNode )
			};

		repetitions.push_back( repetitionPair );
		return repetitions;
	}

	for( rapidjson::Value::ConstValueIterator itr = repetitionsNode->Begin(); itr != repetitionsNode->End(); ++itr  )
	{
		if( ! itr->IsObject() )
		{
			std::string value = property_parser::valueToString( itr );
			std::pair< std::string, std::string > repetitionRange { value, value };
			repetitions.push_back( repetitionRange );
			continue;
		}

		std::pair< std::string, std::string > repetitionRange { "", "" };
		
		if( itr->HasMember( std::string( kMin ).c_str() ) )
			repetitionRange.first = property_parser::valueToString( &itr->FindMember( std::string( kMin ).c_str() )->value );
		
		if( itr->HasMember( std::string( kMax ).c_str() ) )
			repetitionRange.second = property_parser::valueToString( &itr->FindMember( std::string( kMax ).c_str() )->value );
		
		repetitions.push_back( repetitionRange );
	}
	return repetitions;
}

std::map< std::string, std::string > SpecNode::getMap() const
{
	std::map< std::string, std::string > map;

	if( ! _node->HasMember( std::string( kMap ).c_str() ) )
		return map;

	const rapidjson::Value* mapNode = &_node->FindMember( std::string( kMap ).c_str() )->value;

	for( rapidjson::Value::ConstValueIterator itr = mapNode->Begin(); itr != mapNode->End(); ++itr  )
	{
		std::string index = property_parser::valueToString( itr->Begin() );
		std::string value = property_parser::valueToString( itr->End()   );
		map[ index ] = value;
	}
	return map;
}

std::shared_ptr< spec_reader::SpecNode > SpecNode::next() const
{
	TreeNodeIt node = _node;
	++node;
	if( _parent != nullptr && _parent->getIterator()->FindMember( std::string( kGroup ).c_str() )->value.End() == node )
	{
		LOG_TRACE( "[specnode] " << getId() << ": Last Element (group)" );
		return nullptr;
	}

	if( node == _specification->end() )
	{
		LOG_TRACE( "[specnode] " << getId() << ": Last Element (specification)" );
		return nullptr;
	}
	
	LOG_TRACE( "[specnode] " << getId() << ": Next" );
	return std::make_shared< SpecNode >( _specification, node, _parent );
}

std::shared_ptr< spec_reader::SpecNode > SpecNode::firstChild() const
{
	try
	{
		if( ! isGroup() )
			throw std::runtime_error( "[specnode] firstChild: This node has no child." );

		const rapidjson::Value* groupNode = &_node->FindMember( std::string( kGroup ).c_str() )->value;
		return std::make_shared< SpecNode >( _specification, groupNode->Begin(), this );
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

std::set< std::string > SpecNode::getChildrenNodes() const
{
	try
	{
		if( ! isGroup() )
			throw std::runtime_error( "SpecNode::getChildrenNodes: This node has no child." );
		
		std::set< std::string > list;
		const rapidjson::Value* groupNode = &_node->FindMember( std::string( kGroup ).c_str() )->value;

		for( rapidjson::Value::ConstValueIterator itr = groupNode->Begin(); itr != groupNode->End(); ++itr  )
			list.insert( property_parser::valueToString( &itr->FindMember( std::string( kId ).c_str() )->value ) );

		return list;
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

}
