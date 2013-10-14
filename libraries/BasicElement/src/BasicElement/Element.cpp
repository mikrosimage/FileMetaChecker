#include "Element.hpp"
#include <SpecReader/SpecNode.hpp>

#include <iostream>

namespace sr = spec_reader;

namespace basic_element
{

Element::Element( const sr::SpecNode* node, const Element* prev )
	: _id       ( node->getId() )
	, _index    ( node->getIndex() )
	, _type     ( node->getType() )
	, _specNode ( node )
	, _status   ( eStatusNotCheck )
	, _parent   ( node->getParent() )
	, _checkedGroup( false )
	, _iteration   ( 1 )
	, _previous    ( prev )
{
	if( node->isRepeated() > 1 && ( _previous != NULL ) )
	{
		const Element* e = _previous;
		while( e != NULL )
		{
			if( e->getId() == node->getId() )
			{
				_iteration = e->getIteration() + 1;
				break;
			}
			else
			{
				e = e->getParent();
			}
		}
	}
}

const sr::SpecNode* Element::next( )
{
	if( _status == eStatusNotCheck )
	{
		return _specNode;
	}
	
	if( _specNode->isOptional() &&
		_status == eStatusInvalid )
	{
		return _specNode->next( _parent );
	}
	
	if( _status == eStatusInvalidButSkip &&
		_parent != NULL &&
		( ! _parent->_specNode->isOrdered() ) )
	{
		std::cout << "Unordered, check one another baby !" << std::endl;
		_status = eStatusInvalidButSkip;
		
		return _parent->_specNode->firstChild( _parent );
	}
	
	if( _specNode->isGroup() && ! _checkedGroup )
	{
		_checkedGroup = true;
		return _specNode->firstChild( this );
	}
	
	size_t count = _specNode->isRepeated();

	if( count > 1 )
	{
		if( _iteration < count )
			return _specNode;
	}
	
	const sr::SpecNode* sn = _specNode->next( _parent );
	
	if( sn == NULL && _parent != NULL )
	{
		if( ! _parent->_specNode->isOrdered() )
		{
			std::set< std::string > childIds;
			
			childIds = _parent->_specNode->getChildNodes();
			
			const Element* p = _previous;
			while( p->getId() != _parent->getId() )
			{
				for( auto id : childIds )
				{
					if( p->getId() == id )
					{
						childIds.erase( id );
					}
				}
				p = p->_previous;
			}
			
			
			std::cout << _id << "   " << _index << std::endl;
			std::cout << "End of check Unordered " << childIds.size() << std::endl;
			
			if( childIds.size() != 0 )
			{
				std::cout << "prout" << std::endl;
				//status = eStatusInvalid;
				_parent->_status = eStatusInvalidForUnordered;
			}
		}
		return _parent->next( );
	}
	
	return sn;
}

std::string Element::getStringStatus() const
{
	switch( _status )
	{
		case eStatusValid              : return "valid";
		case eStatusInvalid            : return "invalid";
		case eStatusInvalidButOptional : return "invalid but optional";
		case eStatusInvalidForUnordered: return "invalid for unordered";
		case eStatusPassOverData       : return "pass over data";
		case eStatusSkip               : return "skip";
		default: break;
	}
	return "";
}


}
