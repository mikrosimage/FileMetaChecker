#include "SpecNode.hpp"
#include "Specification.hpp"
#include <Common/log.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace spec_reader
{

namespace property_parser
{
	template< typename ValueType >
	ValueType getProperty( const TreeNodeIt node, const std::string& prop );

	template< >
	std::string getProperty< std::string >( const TreeNodeIt node, const std::string& prop )
	{
		try
		{
			std::string propStr = prop;
			if( node->HasMember( propStr.c_str() ) )
				return node->FindMember( propStr.c_str() )->value.GetString();
			throw std::runtime_error( "No such node (" + prop + ")" );
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
		std::string propStr = prop;
		if( node->HasMember( propStr.c_str() ) )
			return node->FindMember( propStr.c_str() )->value.GetString();
		else
			return defaultValue; 
	}

	template< >
	bool getProperty< bool >( const TreeNodeIt node, const std::string& prop, const bool& defaultValue )
	{
		std::string propStr = prop;
		if( node->HasMember( propStr.c_str() ) )
			return node->FindMember( propStr.c_str() )->value.GetBool();
		else
			return defaultValue; 
	}

	std::string valueToString( const rapidjson::Value* value )
	{
		std::string ret;
		if( value->IsInt() )
		{
			ret = std::to_string( value->GetInt() );
		}
		if( value->IsDouble() )
		{
			ret = std::to_string( value->GetDouble() );
		}
		if( value->IsString() )
		{
			ret = value->GetString();
		}
		return ret;
	}
}

size_t SpecNode::_globalIndex = 0;

SpecNode::SpecNode( const Specification* spec, const TreeNodeIt node, const SpecNode* parent )
	: _uId ( _globalIndex++ )
	, _node( node )
	, _parent( parent )
	, _specification( spec )
{
	// LOG_FATAL( "SPECNODE: " << &*this << " # parent:" << &*_parent );
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
		return typeMap.at( property_parser::getProperty< std::string >( _node, kType ) );
	}
	catch( const std::out_of_range& oor )
	{
		LOG_WARNING( "Unknown type" );
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
		LOG_WARNING( "Unknown displayType" );
		return eDisplayTypeDefault;
	}
}

std::string SpecNode::getCount() const
{
	return property_parser::getProperty< std::string >( _node, kCount, "" );
}

std::string SpecNode::getRequirement() const
{
	return property_parser::getProperty< std::string >( _node, kRequired, "" );
}

std::string SpecNode::getGroupSize() const
{
	return property_parser::getProperty< std::string >( _node, kGroupSize, "" );
}

bool SpecNode::isGroup() const
{
	std::string groupStr = kGroup;
	return _node->HasMember( groupStr.c_str() );
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

std::vector< std::string > SpecNode::getValues() const
{
	std::vector< std::string > values;
	std::string valuesStr = kValues;
	if( _node->HasMember( valuesStr.c_str() ) )
	{
		const rapidjson::Value* valuesNode = &_node->FindMember( valuesStr.c_str() )->value;
		if( valuesNode->IsArray() )
		{
			for( rapidjson::Value::ConstValueIterator itr = valuesNode->Begin(); itr != valuesNode->End(); ++itr  )
				values.push_back( property_parser::valueToString( itr ) );
		}
		else
			values.push_back( property_parser::valueToString( valuesNode ) );
	}
	return values;
}

std::vector< std::pair< std::string, std::string > > SpecNode::getRange() const
{
	std::vector< std::pair< std::string, std::string > > ranges;
	std::string rangeStr = kRange;
	std::string minStr   = kMin;
	std::string maxStr   = kMax;

	if( _node->HasMember( rangeStr.c_str() ) )
	{
		const rapidjson::Value* rangesNode = &_node->FindMember( rangeStr.c_str() )->value;
		for( rapidjson::Value::ConstValueIterator itr = rangesNode->Begin(); itr != rangesNode->End(); ++itr  )
		{
			std::pair< std::string, std::string > range { "", "" };
			if( itr->HasMember( minStr.c_str() ) )
				range.first = property_parser::valueToString( &itr->FindMember( minStr.c_str() )->value );
			
			if( itr->HasMember( maxStr.c_str() ) )
				range.second = property_parser::valueToString( &itr->FindMember( maxStr.c_str() )->value );
			
			if( range.first != "" || range.second != "" )
				ranges.push_back( range );
		}
	}
	return ranges;
}

std::vector< std::pair< std::string, std::string > > SpecNode::getRepetitions() const
{
	std::vector< std::pair< std::string, std::string > > repetitions;
	std::string repetitionStr = kRepetition;
	std::string minStr        = kMin;
	std::string maxStr        = kMax;

	if( _node->HasMember( repetitionStr.c_str() ) )
	{
		const rapidjson::Value* repetitionsNode = &_node->FindMember( repetitionStr.c_str() )->value;

		if( ! repetitionsNode->IsArray() )
		{
			std::pair< std::string, std::string > repetitionPair { "", "" };
			repetitionPair.first  = property_parser::valueToString( repetitionsNode );
			repetitionPair.second = property_parser::valueToString( repetitionsNode );
			repetitions.push_back( repetitionPair );
			return repetitions;
		}

		for( rapidjson::Value::ConstValueIterator itr = repetitionsNode->Begin(); itr != repetitionsNode->End(); ++itr  )
		{
			std::pair< std::string, std::string > repetitionRange { "", "" };
			if( ! itr->IsObject() )
			{
				repetitionRange.first  = property_parser::valueToString( itr );
				repetitionRange.second = property_parser::valueToString( itr );
				repetitions.push_back( repetitionRange );
				continue;
			}

			if( itr->HasMember( minStr.c_str() ) )
				repetitionRange.first = property_parser::valueToString( &itr->FindMember( minStr.c_str() )->value );
			
			if( itr->HasMember( maxStr.c_str() ) )
				repetitionRange.second = property_parser::valueToString( &itr->FindMember( maxStr.c_str() )->value );
			
			repetitions.push_back( repetitionRange );
		}
	}
	return repetitions;
}

std::map< std::string, std::string > SpecNode::getMap() const
{
	std::map< std::string, std::string > map;
	std::string mapStr = kMap;

	if( _node->HasMember( mapStr.c_str() ) )
	{
		const rapidjson::Value* mapNode = &_node->FindMember( mapStr.c_str() )->value;

		for( rapidjson::Value::ConstValueIterator itr = mapNode->Begin(); itr != mapNode->End(); ++itr  )
		{
			std::string index = property_parser::valueToString( itr->Begin() );
			std::string value = property_parser::valueToString( itr->End()   );
			map[ index ] = value;
		}
	}
	return map;
}

std::shared_ptr< spec_reader::SpecNode > SpecNode::next() const
{
	TreeNodeIt node = _node;
	++node;
	std::string groupStr = kGroup;

	if( _parent != nullptr && _parent->getIterator()->FindMember( groupStr.c_str() )->value.End() == node )
	{
		// LOG_WARNING( "SpecNode::next " << getId() << ": Last Element (group)" );
		return nullptr;
	}

	if( node == _specification->end() )
	{
		// LOG_WARNING( "SpecNode::next " << getId() << ": Last Element (specification)" );
		return nullptr;
	}
	
	// LOG_WARNING( "SpecNode::next " << getId() << ": Next" );
	return std::make_shared< SpecNode >( _specification, node, _parent );
}

std::shared_ptr< spec_reader::SpecNode > SpecNode::firstChild() const
{
	try
	{
		// LOG_WARNING( "SpecNode::firstChild " << getId() );
		if( ! isGroup() )
			throw std::runtime_error( "SpecNode::firstChild: This node has no child." );

		std::string groupStr = kGroup;
		
		const rapidjson::Value* groupNode = &_node->FindMember( groupStr.c_str() )->value;

		TreeNodeIt node = groupNode->Begin();
		return std::make_shared< SpecNode >( _specification, node, this );
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

// size_t SpecNode::getChildrenNumber() const
// {
// 	if( ! isGroup() )
// 		return 0;
// 	return _node->second.get_child( kGroup ).size();
// }

std::set< std::string > SpecNode::getChildrenNodes() const
{
	try
	{
		if( ! isGroup() )
			throw std::runtime_error( "SpecNode::getChildrenNodes: This node has no child." );
		
		std::set< std::string > list;
		std::string groupStr = kGroup;
		std::string idStr    = kId;

		const rapidjson::Value* groupNode = &_node->FindMember( groupStr.c_str() )->value;

		for( rapidjson::Value::ConstValueIterator itr = groupNode->Begin(); itr != groupNode->End(); ++itr  )
			list.insert( property_parser::valueToString( &itr->FindMember( idStr.c_str() )->value ) );

		return list;
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

}
