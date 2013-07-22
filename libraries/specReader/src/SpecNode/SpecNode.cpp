#include "SpecNode.hpp"
#include <boost/foreach.hpp>

SpecNode::SpecNode()
	: _parent( NULL )
{
}

SpecNode::SpecNode( const bpt::ptree::const_iterator node )
	: _node( node )
	, _parent( NULL )
{
}

SpecNode::~SpecNode()
{
}

void SpecNode::setNode( const bpt::ptree::const_iterator node  )
{
	_node = node;
}



std::string SpecNode::getId()
{
	return _node->second.get< std::string >( kId );
}

std::string SpecNode::getLabel()
{
	return _node->second.get< std::string >( kLabel );
}

std::string SpecNode::getType()
{
	return _node->second.get< std::string >( kType );
}

std::string SpecNode::getDisplayType()
{
	return _node->second.get< std::string >( kDisplayType, "" );
}


std::string SpecNode::getCount()
{
	return _node->second.get< std::string >( kCount, "" );
}

std::string SpecNode::getRequired()
{
	return _node->second.get< std::string >( kRequired, "" );
}


std::vector< std::string > SpecNode::getValues()
{
	std::vector< std::string > values;
	if( boost::optional< const bpt::ptree& > valuesNode = _node->second.get_child_optional( kValues ) )
	{
		std::string stringValue = _node->second.get< std::string >( kValues );
		if( stringValue.empty() )
		{
			BOOST_FOREACH( const bpt::ptree::value_type& value, valuesNode.get( ) )
				values.push_back( value.second.get_value< std::string >() );
		}
		else
		{
			values.push_back( stringValue );
		}
	}
	return values;
}

std::vector< std::pair< std::string, std::string > > SpecNode::getRange()
{
	std::vector< std::pair< std::string, std::string > > ranges;
	if( boost::optional< const bpt::ptree& > rangeNode = _node->second.get_child_optional( kRange ) )
	{
		BOOST_FOREACH( const bpt::ptree::value_type& r, rangeNode.get() )
		{
			std::pair< std::string, std::string > range;
			if( r.second.get_child_optional( kMin ) )
				range.first = r.second.get< std::string >( kMin );

			if( r.second.get_child_optional( kMax ) )
				range.second = r.second.get< std::string >( kMax );

			ranges.push_back( range );
		}
	}
	return ranges;
}

std::vector< std::pair< std::string, std::string > > SpecNode::getRepetition()
{
	std::vector< std::pair< std::string, std::string > > repetitions;
	if( boost::optional< const bpt::ptree& > repetitionNode = _node->second.get_child_optional( kRepetition ) )
	{
		std::string repetitionExpr = _node->second.get< std::string >( kRepetition );
		if( !repetitionExpr.empty() )
		{
			LOG_TRACE( " --- CASE EXPRESSION --- " );
			std::pair< std::string, std::string > repetition;
			repetition.first  = repetitionExpr;
			repetition.second = repetitionExpr;
			repetitions.push_back( repetition );
		}
		else
		{
			LOG_TRACE( " --- CASE MULTIPLE --- " );
			BOOST_FOREACH( const bpt::ptree::value_type& r, repetitionNode.get() )
			{
				std::pair< std::string, std::string > repetition;
				if( !r.second.get_child_optional( kMin ) && !r.second.get_child_optional( kMax ) )
				{
					std::string repetitionExpr = r.second.data();
					repetition.first  = repetitionExpr;
					repetition.second = repetitionExpr;
				}

				if( r.second.get_child_optional( kMin ) )
					repetition.first = r.second.get< std::string >( kMin );

				if( r.second.get_child_optional( kMax ) )
					repetition.second = r.second.get< std::string >( kMax );

				repetitions.push_back( repetition );
			}
		}
	}
	return repetitions;
}

std::map< std::string, std::string > SpecNode::getMap()
{
	std::map< std::string, std::string > map;
	if( boost::optional< const bpt::ptree& > mapNode = _node->second.get_child_optional( kMap ) )
	{
		BOOST_FOREACH( const bpt::ptree::value_type& m, mapNode.get() )
		{
			std::string index = m.second.ordered_begin()->first;
			map[ index ] = m.second.get< std::string >( index );
		}
	}
	return map;
}


bool SpecNode::isBigEndian()
{
	return ( _node->second.get<std::string>( kEndian, kEndianBig ) == kEndianBig );
}

bool SpecNode::isOptional()
{
	return ( _node->second.get<std::string>( kOptional, kOptionalFalse ) == kOptionalTrue );
}

bool SpecNode::isOrdered()
{
	return ( _node->second.get<std::string>( kOrdered, kOrderedTrue ) == kOrderedTrue );
}

bool SpecNode::hasGroup()
{
	return _node->second.get_child_optional( kGroup );
}

std::string SpecNode::getGroupSize()
{
	return _node->second.get< std::string >( kGroupSize, "" );
}

SpecNode SpecNode::next()
{
	bpt::ptree::const_iterator node = _node;
	// if( node != _parent.getLastChild() )		// @todo
	return SpecNode( ++node );
}

