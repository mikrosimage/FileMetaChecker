#include <Common/Element.hpp>
#include <SpecReader/SpecNode.hpp>

#include <iostream>

namespace sr = spec_reader;

namespace basic_element
{

Element::Element( const sr::SpecNode* node, const std::shared_ptr< Element > previous )
	: _parent      ( node->getParent() )
	, _previous    ( previous )
	, _specNode    ( node )
	, _checkedGroup( false )
{
	_prop.id        = node->getId();
	_prop.type      = node->getType();
	_prop.uId       = node->getIndex();
	_prop.iteration = 1;
	_prop.status    = eStatusNotCheck;

	if( node->isRepeated() > 1 && ( previous.use_count() != 0 ) )
	{
		std::shared_ptr< Element > e = _previous.lock();
		while( e.use_count() != 0 )
		{
			if( e->getId() == node->getId() )
			{
				_prop.iteration = e->getIteration() + 1;
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
	if( _prop.status == eStatusNotCheck )
		return _specNode;

	std::shared_ptr< Element > parent;

	if( _parent.use_count() != 0 )
		parent = _parent.lock();
	
	if( _specNode->isOptional() && _prop.status == eStatusInvalid )
	{
		return _specNode->next( parent );
	}
	
	if( _prop.status == eStatusInvalidButSkip && _parent.use_count() != 0 && ( ! parent->_specNode->isOrdered() ) )
	{
		std::cout << "Unordered, check one another baby !" << std::endl;
		_prop.status = eStatusInvalidButSkip;
		
		return parent->_specNode->firstChild( parent );
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
		if( _prop.iteration < count )
			return _specNode;
	}
	
	const sr::SpecNode* sn = _specNode->next( parent );
	
	if( sn == NULL && _parent.use_count() != 0 )
	{
		if( ! parent->_specNode->isOrdered() )
		{
			std::set< std::string > childIds;
			
			childIds = parent->_specNode->getChildrenNodes();
			
			std::shared_ptr< Element > prev;
			if( _previous.use_count() != 0 )
			{
				prev = _previous.lock();
				while( prev->getId() != parent->getId() )
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
			
			std::cout << _prop.id << "   " << _prop.uId << std::endl;
			std::cout << "End of check Unordered " << childIds.size() << std::endl;
			
			if( childIds.size() != 0 )
			{
				std::cout << "prout" << std::endl;		// Error or Warning ?
				//status = eStatusInvalid;
				parent->_prop.status = eStatusInvalidForUnordered;
			}
		}
		return parent->next( );
	}
	
	return sn;
}

std::string Element::getStringStatus() const
{
	switch( _prop.status )
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

size_t Element::getChildrenNumber() const
{
	return _specNode->getChildrenNumber();
}

void Element::getEndianOrderedData( char* buffer, const char* data ) const
{
	if( ! _specNode->isBigEndian() )
		std::reverse_copy( data, data + _prop.size, buffer );
	else
		std::memcpy( buffer, data, _prop.size );
}

char* Element::get() const
{
	if( _prop.data == NULL )
		throw std::runtime_error( "Undefined data" );
	return _prop.data;
}

}
