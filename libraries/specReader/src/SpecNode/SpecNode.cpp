#include "SpecNode.hpp"
#include <boost/foreach.hpp>

namespace spec_reader
{

SpecNode::SpecNode( const bpt::ptree::const_iterator node, const size_t& index, const size_t& indexTotal )
	: _node( node )
	, _index( index )
	, _indexTotal( indexTotal )
{
}

SpecNode::~SpecNode()
{
}


std::string SpecNode::getId()
{
	return getProperty( kId );
}

std::string SpecNode::getLabel()
{
	return getProperty( kLabel );
}

std::string SpecNode::getType()
{
	return getProperty( kType );
}

std::string SpecNode::getDisplayType()
{
	return getProperty( kDisplayType, "" );
}


std::string SpecNode::getCount()
{
	return getProperty( kCount, "" );
}

std::string SpecNode::getRequired()
{
	return getProperty( kRequired, "" );
}

std::string SpecNode::getGroupSize()
{
	return getProperty( kGroupSize, "" );
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
	return ( getProperty( kEndian, kEndianBig ) == kEndianBig );
}

bool SpecNode::isOptional()
{
	return ( getProperty( kOptional, kOptionalFalse ) == kOptionalTrue );
}

bool SpecNode::isOrdered()
{
	return ( getProperty( kOrdered, kOrderedTrue ) == kOrderedTrue );
}

bool SpecNode::hasGroup()
{
	return _node->second.get_child_optional( kGroup );
}

SpecNode SpecNode::next()
{
	bpt::ptree::const_iterator node = _node;
	size_t index = _index;

	if( _index >= _indexTotal - 1 )
		return SpecNode( node, _indexTotal, _indexTotal );
	return SpecNode( ++node, ++index, _indexTotal );
}

SpecNode SpecNode::firstChild()
{
	try
	{
		if( !hasGroup() )
			throw std::runtime_error( "firstChild: This node has no child." );
		return SpecNode( _node->second.get_child( kGroup ).begin(), 0, _node->second.get_child( kGroup ).size() );
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

size_t SpecNode::getIndex()
{
	return _index;
}

size_t SpecNode::getIndexTotal()
{
	return _indexTotal;
}


std::string SpecNode::getProperty( const std::string& prop )
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

std::string SpecNode::getProperty( const std::string& prop, const std::string& defaultValue )
{
	return _node->second.get< std::string >( prop, defaultValue );
}

}




