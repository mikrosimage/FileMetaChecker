#include "Element.hpp"
#include <Common/log.hpp>
#include <SpecReader/SpecNode.hpp>

#include <iostream>

namespace sr = spec_reader;

namespace basic_element
{

Element::Element( const std::shared_ptr< sr::SpecNode > node,
	              const std::shared_ptr< Element > previous,
	              const std::shared_ptr< Element > parent )
	: _parent        ( parent )
	, _previous      ( previous )
	, _specNode      ( node )
	, _id            ( node->getId() )
	, _label         ( node->getLabel() )
	, _uId           ( node->getUId() )
	, _size          ( 0 )
	, _iteration     ( 1 )
	, _countExpr     ( node->getCount() )
	, _requiredExpr  ( node->getRequirement() )
	, _groupSizeExpr ( node->getGroupSize() )
	, _values        ( node->getValues() )
	, _map           ( node->getMap() )
	, _rangeExpr     ( node->getRange() )
	, _repetExpr     ( node->getRepetitions() )
	, _type          ( node->getType() )
	, _displayType   ( node->getDisplayType() )
	, _status        ( eStatusNotChecked )
	, _error         ( "" )
	, _warning       ( "" )
	, _isGroup       ( node->isGroup() )
	, _isOrdered     ( node->isOrdered() )
	, _isOptional    ( node->isOptional() )
	, _isBigEndian   ( node->isBigEndian() )
	, _checkedGroup  ( false )
	, _data          ( nullptr )
{
	LOG_ERROR( "____________________" << _id <<  "____________________" );
	if( ! _repetExpr.empty() )
	{
		std::shared_ptr< Element > prev = previous;
		while( prev != nullptr || ( parent != nullptr && prev->_id != parent->_id ) )
		{
			if( prev->_id == _id && ( prev->_status == eStatusValid || prev->_status == eStatusPassOver ) )
			{
				_iteration = prev->_iteration + 1;
				// LOG_ERROR( "ELEMENT: >>>> prev: " << prev->_id << " @" << &*prev );
				break;
			}

			// LOG_ERROR( "ELEMENT: prev: " << prev->_id << " @" << &*prev );
			if( parent != nullptr && prev->_id == parent->_id )
				break;

			prev = prev->getPrevious();
		}
	}

	// if( ! _previous.expired() )
	// 	LOG_ERROR( "ELEMENT: " << _id << "'s previous: " << previous->_id );

	if( ! _parent.expired() )
		_parent.lock()->_children.push_back( std::make_shared< Element >( *this ) );

	LOG_ERROR( "ELEMENT: " << _id << ": " << &*this << " - Parent: " << &*_parent.lock() << " - Previous: " << &( *( _previous.lock().get() ) ) << " - SpecNode: " << &*_specNode.get() << " - Iteration: " << _iteration );
}

std::shared_ptr< spec_reader::SpecNode > Element::next( )
{
	// if element has been checked
	if( _status == eStatusNotChecked )
	{
		return _specNode;
	}

	std::shared_ptr< Element > parent;
	// if parent exists (?)
	if( _parent.use_count() != 0 )
	{
		// copy the parent
		parent = _parent.lock();
	}
	
	// Optional element :
	// if element optional & invalid
	if( _isOptional && _status == eStatusInvalid )
	{
		// go to next SpecNode
		return _specNode->next();
	}
	
	// Unordered groups : if element status = Valid and parent is not ordered
	if( _status == eStatusValid && _parent.use_count() != 0 && ( ! parent->_isOrdered ) )
	{
		// go to the first SpecNode of the childhood
		return parent->_specNode->firstChild();
	}
	
	// Groups :
	// if element has a group not already checked and is valid or first time parsed
	if( _specNode->isGroup() && ! _checkedGroup && ( _iteration == 1 || _status == eStatusValid ) )
	{
		LOG_ERROR( "Element::next " << _id << ": IsGroup" );
		// it becomes checked
		_checkedGroup = true;
		// creates a pointer to this current element
		std::shared_ptr< spec_reader::SpecNode > child( _specNode->firstChild() );
		// go to the first child SpecNode
		return child;
	}
	
	// if repeated element
	if( ! _repetExpr.empty() && _status == eStatusValid )
	{
		LOG_ERROR( ">>>>>>>>>>>>>> Element::next " << _id << ": Repeated: " << _iteration );
		// go to the same SpecNode
		return _specNode;
	}

	// creates a pointer to the next SpecNode
	std::shared_ptr< sr::SpecNode > nextNode = _specNode->next();

	// in Unoredered groups : check if every nodes have been checked
	// if their is no more SpecNode after and parent exists
	if( nextNode == nullptr && _parent.use_count() != 0 )
	{
		// go to the node after the parent
		LOG_ERROR( "Element::next " << _id << ": last element -> parent's (" << _parent.lock()->_id << ") next !" );
		return parent->next( );
	}
	LOG_ERROR( ">>> Element::next " << _id << ": Next ! " << nextNode );
	// go to the next node
	return nextNode;
}


void Element::set( const char* data, const size_t& size )
{
	_data = new char [size];
	_size = size;
	
	std::memcpy( _data, data, _size );
}

}

