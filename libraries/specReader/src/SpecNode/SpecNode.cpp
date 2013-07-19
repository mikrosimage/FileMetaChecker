#include "SpecNode.hpp"
#include <boost/foreach.hpp>

SpecNode::SpecNode()
{
}

SpecNode::SpecNode( const bpt::ptree::value_type& node )
	: _node( node.second )
{
}

SpecNode::~SpecNode()
{
}

void SpecNode::setNode( const bpt::ptree::value_type& node  )
{
	_node = node.second;
}



std::string SpecNode::getId()
{
	return _node.get< std::string >( kId );
}

std::string SpecNode::getLabel()
{
	return _node.get< std::string >( kLabel );
}

std::string SpecNode::getType()
{
	return _node.get< std::string >( kType );
}

std::string SpecNode::getDisplayType()
{
	return _node.get< std::string >( kDisplayType, "" );
}


std::string SpecNode::getCount()
{
	return _node.get< std::string >( kCount, "" );
}

std::string SpecNode::getRequired()
{
	return _node.get< std::string >( kRequired, "" );
}


std::vector< std::string > SpecNode::getValues()
{
	std::vector< std::string > values;
	if( boost::optional< bpt::ptree& > valuesNode = _node.get_child_optional( kValues ) )
	{
		std::string stringValue = _node.get< std::string >( kValues );
		if( stringValue.empty() )
		{
			BOOST_FOREACH( bpt::ptree::value_type& value, valuesNode.get( ) )
				values.push_back( value.second.get_value< std::string >() );
		}
		else
		{
			values.push_back( stringValue );
		}
	}
	return values;
}



bool SpecNode::isBigEndian()
{
	return ( _node.get<std::string>( kEndian, kEndianBig ) == kEndianBig );
}

bool SpecNode::isOptional()
{
	return ( _node.get<std::string>( kOptional, kOptionalFalse ) == kOptionalTrue );
}

bool SpecNode::isOrdered()
{
	return ( _node.get<std::string>( kOrdered, kOrderedTrue ) == kOrderedTrue );
}

bool SpecNode::hasGroup()
{
	return _node.get_child_optional( kGroup );
}
