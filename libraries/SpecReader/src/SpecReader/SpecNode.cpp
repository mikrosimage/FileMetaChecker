#include "SpecNode.hpp"
#include <Common/Element.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>


namespace be  = basic_element;
namespace bpt = boost::property_tree;

namespace spec_reader
{

size_t SpecNode::_globalIndex = 0;

SpecNode::SpecNode( const bpt::ptree::const_iterator node, std::shared_ptr< be::Element > p )
	: _index ( _globalIndex++ )
	, _node( node )
	, _parent( p )
{
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
	switch( getSubType() )
	{
		case eSubTypeInt8         :
		case eSubTypeUInt8        :
		case eSubTypeInt16        :
		case eSubTypeUInt16       :
		case eSubTypeInt32        :
		case eSubTypeUInt32       :
		case eSubTypeInt64        :
		case eSubTypeUInt64       :
		case eSubTypeFloat        :
		case eSubTypeDouble       :
		case eSubTypeIeeeExtended : return eTypeNumber;
		case eSubTypeAscii        :
		case eSubTypeHexa         :
		case eSubTypeRaw          : return eTypeData;
		default                   : break;
	}
	return eTypeUnknown;
}

ESubType SpecNode::getSubType() const
{
	try
	{
		return subTypeMap.at( getProperty( kType ) );
	}
	catch( const std::out_of_range& oor )
	{
		std::cout << "Warning : Unknown subtype" << std::endl;
		return eSubTypeUnknown;
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
		std::cout << "Warning : Unknown displayType" << std::endl;
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
	if( _index == 2 ||
		_index == 5 ||
		_index == 14 )
		return true;
	return false;
}

bool SpecNode::isOrdered() const
{
	if( _index == 14 )
		return false;
	return true;
}

bool SpecNode::isOptional() const
{
	if( _index == 7 )
		return true;
	return false;
}

size_t SpecNode::isRepeated() const
{
	if( _index == 4 )
		return 3;

	if( _index == 5 )
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


SpecNode* SpecNode::next( std::shared_ptr< be::Element > parent ) const
{
	if( _index == 8 ||
		_index == 10 ||
		_index == 12 ||
		_index == 17 ||
		_index == 18 )
		return NULL;

	bpt::ptree::const_iterator node = _node;
	SpecNode* s = new SpecNode( ++node, parent );
	// std::ostringstream osstrId;
	// osstrId << "next " << _index; 
	// s->setId( osstrId.str() );
	// s->setType( eTypeData );
	return s;
}

SpecNode* SpecNode::firstChild( std::shared_ptr< be::Element > e ) const
{
	try
	{
		if( ! isGroup() )
			throw std::runtime_error( "firstChild: This node has no child." );
		SpecNode* s = new SpecNode( _node->second.get_child( kGroup ).begin(), e );
		return s;
	}
	catch( std::runtime_error& e )
	{
		std::cout << "Error: " << e.what();
		throw;
	}
}

std::set< std::string > SpecNode::getChildrenNodes() const
{
	// @todo : parse element children in ptree (and their Id ? or pointer ?)
	std::set< std::string > list;
	list.insert( "child" );
	list.insert( "prout" );
	return list;
}


std::string SpecNode::getProperty( const std::string& prop ) const
{
	try 
	{
		return _node->second.get< std::string >( prop );
	}
	catch( std::runtime_error& e )
	{
		std::cout << "Error: " << e.what();
		throw;
	}
}

std::string SpecNode::getProperty( const std::string& prop, const std::string& defaultValue ) const
{
	return _node->second.get< std::string >( prop, defaultValue );
}

}
