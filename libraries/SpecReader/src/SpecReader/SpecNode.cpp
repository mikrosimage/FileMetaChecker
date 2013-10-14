#include "SpecNode.hpp"
#include <BasicElement/Element.hpp>

namespace be = basic_element;

namespace spec_reader
{

size_t SpecNode::_globalIndex = 0;

SpecNode::SpecNode( be::Element* p )
	: _index( _globalIndex++ )
	, _parent( p )
{
}

SpecNode* SpecNode::next( be::Element* parent ) const
{
	if( _index == 8 ||
		_index == 10 ||
		_index == 12 ||
		_index == 17 ||
		_index == 18 )
		return NULL;
	
	SpecNode* s = new SpecNode( parent );
	s->setId( "next " + _index );
	s->setType( eTypeData );
	return s;
}

bool SpecNode::isGroup() const
{
	if( _index == 2 ||
		_index == 5 ||
		_index == 14 )
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

SpecNode* SpecNode::firstChild( be::Element* e ) const
{
	SpecNode* s = new SpecNode( e );
	s->setId( "child" );
	s->setType( eTypeNumber );
	return s;
}

std::set< std::string > SpecNode::getChildNodes() const
{
	std::set< std::string > list;
	list.insert( "child" );
	list.insert( "prout" );
	return list;
}

}
