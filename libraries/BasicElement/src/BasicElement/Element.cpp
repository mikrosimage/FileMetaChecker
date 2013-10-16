#include <Common/Element.hpp>
#include <SpecReader/SpecNode.hpp>

#include <iostream>

namespace sr = spec_reader;

namespace basic_element
{

Element::Element( const sr::SpecNode* node, const std::shared_ptr< Element > prev )
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
	if( node->isRepeated() > 1 && ( prev.use_count() != 0 ) )
	{
		std::shared_ptr< Element > e = _previous.lock();
		while( e.use_count() != 0 )
		{
			if( e->getId() == node->getId() )
			{
				_iteration = e->getIteration() + 1;
				break;
			}
			else
			{
				if( e->getParent().use_count() == 0 )
					break;
				e = e->getParent().lock();
			}
		}
	}
}

const sr::SpecNode* Element::next( )
{
	if( _status == eStatusNotCheck )
		return _specNode;

	std::shared_ptr< Element > p;

	if( _parent.use_count() != 0 )
		p = _parent.lock();
	
	if( _specNode->isOptional() && _status == eStatusInvalid )
	{
		return _specNode->next( p );
	}
	
	if( _status == eStatusInvalidButSkip && _parent.use_count() != 0 && ( ! p->_specNode->isOrdered() ) )
	{
		std::cout << "Unordered, check one another baby !" << std::endl;
		_status = eStatusInvalidButSkip;
		
		return p->_specNode->firstChild( p );
	}
	
	if( _specNode->isGroup() && ! _checkedGroup )
	{
		_checkedGroup = true;
		std::shared_ptr< Element > t( this );
		return _specNode->firstChild( t );
	}
	
	size_t count = _specNode->isRepeated();

	if( count > 1 )
	{
		if( _iteration < count )
			return _specNode;
	}
	
	const sr::SpecNode* sn = _specNode->next( p );
	
	if( sn == NULL && _parent.use_count() != 0 )
	{
		if( ! p->_specNode->isOrdered() )
		{
			std::set< std::string > childIds;
			
			childIds = p->_specNode->getChildrenNodes();
			
			std::shared_ptr< Element > prev;
			if( _previous.use_count() != 0 )
			{
				prev = _previous.lock();
				while( prev->getId() != p->getId() )
				{
					for( auto id : childIds )
					{
						if( prev->getId() == id )
						{
							childIds.erase( id );
						}
					}
					prev = prev->_previous.lock();
				}
			}			
			
			std::cout << _id << "   " << _index << std::endl;
			std::cout << "End of check Unordered " << childIds.size() << std::endl;
			
			if( childIds.size() != 0 )
			{
				std::cout << "prout" << std::endl;		// Error or Warning ?
				//status = eStatusInvalid;
				p->_status = eStatusInvalidForUnordered;
			}
		}
		return p->next( );
	}
	
	return sn;
}

std::string Element::getStringStatus() const
{
	switch( _status )
	{
		case eStatusNotCheck           : return "not check";
		case eStatusSkip               : return "skip";
		case eStatusPassOverData       : return "pass over data";
		case eStatusValid              : return "valid";
		case eStatusInvalid            : return "invalid";
		case eStatusInvalidButSkip     : return "invalid but skip";
		case eStatusInvalidButOptional : return "invalid but optional";
		case eStatusInvalidForUnordered: return "invalid for unordered";
	}
	return "";
}


}
