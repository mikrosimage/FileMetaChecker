#include "SpecNode.hpp"
#include "Specification.hpp"
#include <Common/Element.hpp>
#include <Common/log.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>


namespace be  = basic_element;
namespace bpt = boost::property_tree;

namespace spec_reader
{

size_t SpecNode::_globalIndex = 0;

SpecNode::SpecNode( const Specification* spec,
	                const bpt::ptree::const_iterator node,
	                const SpecNode* parent )
	: _uId ( _globalIndex++ )
	, _node( node )
	, _parent( parent )
	, _specification( spec )
{
	LOG_FATAL( "SPECNODE: " << &*this << " # parent:" << &*_parent );
}

std::string SpecNode::getId() const
{
	return getProperty( kId );
}

std::string SpecNode::getLabel() const
{
	return getProperty( kLabel );
}

EType SpecNode::getType() const
{
	try
	{
		return typeMap.at( getProperty( kType ) );
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
		return displayTypeMap.at( getProperty( kDisplayType, "" ) );
	}
	catch( const std::out_of_range& oor )
	{
		LOG_WARNING( "Unknown displayType" );
		return eDisplayTypeDefault;
	}
}

std::string SpecNode::getCount() const
{
	return getProperty( kCount, "" );
}

std::string SpecNode::getRequirement() const
{
	return getProperty( kRequired, "" );
}

std::string SpecNode::getGroupSize() const
{
	return getProperty( kGroupSize, "" );
}

bool SpecNode::isGroup() const
{
	return _node->second.get_child_optional( kGroup );
}

bool SpecNode::isOrdered() const
{
	return ( getProperty( kOrdered, kTrue ) == kTrue );
}

bool SpecNode::isOptional() const
{
	return ( getProperty( kOptional, kFalse ) == kTrue );
}

bool SpecNode::isBigEndian() const
{
	return ( getProperty( kEndian, kEndianBig ) == kEndianBig );
}

size_t SpecNode::isRepeated() const
{
	if( _uId == 4 )
		return 3;

	if( _uId == 5 )
		return 2;
	
	return 1;
}

std::vector< std::string > SpecNode::getValues() const
{
	std::vector< std::string > values;
	if( boost::optional< const bpt::ptree& > valuesNode = _node->second.get_child_optional( kValues ) )
	{
		std::string stringValue = _node->second.get< std::string >( kValues );
		if( stringValue.empty() )
		{
			for( const bpt::ptree::value_type& value : valuesNode.get( ) )
				values.push_back( value.second.get_value< std::string >() );
		}
		else
		{
			values.push_back( stringValue );
		}
	}
	return values;
}

std::vector< std::pair< std::string, std::string > > SpecNode::getRange() const
{
	std::vector< std::pair< std::string, std::string > > ranges;
	if( boost::optional< const bpt::ptree& > rangeNode = _node->second.get_child_optional( kRange ) )
	{
		for( const bpt::ptree::value_type& rangeElement : rangeNode.get() )
		{
			std::pair< std::string, std::string > range { "", "" };
			if( rangeElement.second.get_child_optional( kMin ) )
				range.first = rangeElement.second.get< std::string >( kMin );

			if( rangeElement.second.get_child_optional( kMax ) )
				range.second = rangeElement.second.get< std::string >( kMax );

			if( range.first != "" || range.second != "" )
				ranges.push_back( range );
		}
	}
	return ranges;
}

std::vector< std::pair< std::string, std::string > > SpecNode::getRepetitions() const
{
	std::vector< std::pair< std::string, std::string > > repetitions;
	if( boost::optional< const bpt::ptree& > repetitionNode = _node->second.get_child_optional( kRepetition ) )
	{
		std::string repetitionExpr = _node->second.get< std::string >( kRepetition );
		if( ! repetitionExpr.empty() )
		{
			repetitions.push_back( std::make_pair( repetitionExpr, repetitionExpr ) );
		}
		else
		{
			for( const bpt::ptree::value_type& repetitionElement : repetitionNode.get() )
			{
				std::pair< std::string, std::string > repetition { "", "" };
				if( ! repetitionElement.second.get_child_optional( kMin ) && ! repetitionElement.second.get_child_optional( kMax ) )
				{
					std::string repetitionExpr = repetitionElement.second.data();
					repetition.first  = repetitionExpr;
					repetition.second = repetitionExpr;
				}

				if( repetitionElement.second.get_child_optional( kMin ) )
					repetition.first = repetitionElement.second.get< std::string >( kMin );

				if( repetitionElement.second.get_child_optional( kMax ) )
					repetition.second = repetitionElement.second.get< std::string >( kMax );

				repetitions.push_back( repetition );
			}
		}
	}
	return repetitions;
}

std::map< std::string, std::string > SpecNode::getMap() const
{
	std::map< std::string, std::string > map;

	if( boost::optional< const bpt::ptree& > mapNode = _node->second.get_child_optional( kMap ) )
	{
		for( const bpt::ptree::value_type& mapElements : mapNode.get() )
		{
			std::string index = mapElements.second.ordered_begin()->first;
			map[ index ] = mapElements.second.get< std::string >( index );
		}
	}
	return map;
}

std::shared_ptr< spec_reader::SpecNode > SpecNode::next() const
{
	bpt::ptree::const_iterator node = _node;
	++node;

	if( _parent != nullptr && _parent->getIterator()->second.get_child( kGroup ).end() == node )
	{
		LOG_WARNING( "SpecNode::next " << getId() << ": Last Element (group)" );
		return nullptr;
	}

	if( node == _specification->end() )
	{
		LOG_WARNING( "SpecNode::next " << getId() << ": Last Element (specification)" );
		return nullptr;
	}
	
	LOG_WARNING( "SpecNode::next " << getId() << ": Next" );
	return std::make_shared< SpecNode >( _specification, node, _parent );
}

std::shared_ptr< spec_reader::SpecNode > SpecNode::firstChild() const
{
	try
	{
		LOG_WARNING( "SpecNode::firstChild " << getId() );
		if( ! isGroup() )
			throw std::runtime_error( "SpecNode::firstChild: This node has no child." );
		bpt::ptree::const_iterator node = _node->second.get_child( kGroup ).begin();
		const SpecNode* thisOne = new SpecNode( *this );
		return std::make_shared< SpecNode >( _specification, node, thisOne );
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

size_t SpecNode::getChildrenNumber() const
{
	if( ! isGroup() )
		return 0;
	return _node->second.get_child( kGroup ).size();
}

std::set< std::string > SpecNode::getChildrenNodes() const
{
	try
	{
		if( ! isGroup() )
			throw std::runtime_error( "SpecNode::getChildrenNodes: This node has no child." );
		
		std::set< std::string > list;
		for( const bpt::ptree::value_type& child : _node->second.get_child( kGroup ) )
		{
			list.insert( child.second.get< std::string >( kId ) );
		}
		return list;
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}


std::string SpecNode::getProperty( const std::string& prop ) const
{
	try 
	{
		return _node->second.get< std::string >( prop );
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

std::string SpecNode::getProperty( const std::string& prop, const std::string& defaultValue ) const
{
	return _node->second.get< std::string >( prop, defaultValue );
}

}
