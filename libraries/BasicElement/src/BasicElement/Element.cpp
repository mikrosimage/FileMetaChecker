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
	, _mapValue      ( "" )
	, _dispValue     ( "" )
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
	initSize();
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

	if( ! _parent.expired() )
		_parent.lock()->_children.push_back( std::make_shared< Element >( *this ) );

	LOG_ERROR( "ELEMENT: " << _id << ": " << &*this << " - Parent: " << &*_parent.lock() << " - Previous: " << &( *( _previous.lock().get() ) ) << " - SpecNode: " << &*_specNode.get() << " - Iteration: " << _iteration );
}

std::shared_ptr< spec_reader::SpecNode > Element::next( )
{
	// if element has been checked, back to the same SpecNode
	if( _status == eStatusNotChecked )
		return _specNode;

	std::shared_ptr< Element > parent;
	// if parent exists, copy it
	if( _parent.use_count() != 0 )
		parent = _parent.lock();
	
	// Optional: if element optional & invalid, got to the next SpecNode
	if( _isOptional && _status == eStatusInvalid )
		return _specNode->next();
	
	// Unordered Groups: if element valid and parent is unordered, go to the first child of the parent
	if( _status == eStatusValid && _parent.use_count() != 0 && ( ! parent->_isOrdered ) )
		return parent->_specNode->firstChild();
	
	// Groups: if element has a group not already checked and is valid or first time parsed, go to the first child
	if( _specNode->isGroup() && ! _checkedGroup && ( _iteration == 1 || _status == eStatusValid ) )
	{
		// LOG_ERROR( "Element::next " << _id << ": IsGroup" );
		_checkedGroup = true;
		std::shared_ptr< spec_reader::SpecNode > child( _specNode->firstChild() );
		return child;
	}
	
	// Repetition: if repeated element, go to the same SpecNode
	if( ! _repetExpr.empty() && _status == eStatusValid )
	{
		// LOG_ERROR( ">>>>>>>>>>>>>> Element::next " << _id << ": Repeated: " << _iteration );
		return _specNode;
	}

	std::shared_ptr< sr::SpecNode > nextNode = _specNode->next();

	// Last Element: if their is no more SpecNode after and parent exists, go to the node after the parent
	if( nextNode == nullptr && _parent.use_count() != 0 )
	{
		// LOG_ERROR( "Element::next " << _id << ": last element -> parent's (" << _parent.lock()->_id << ") next !" );
		return parent->next( );
	}

	// LOG_ERROR( ">>> Element::next " << _id << ": Next ! " << nextNode );
	return nextNode;
}

void Element::set( const char* data, const size_t& size )
{
	_data = new char [size];
	_size = size;
	
	std::memcpy( _data, data, _size );
}

void Element::initSize()
{
	try
	{
		if( ! _values.empty() )
		{
			_size = _values.at( 0 ).size();
			for( std::string value : _values )
				if( value.size() != _size )
					throw std::runtime_error( "Values must have the same size" );
		}

		switch( _type )
		{
			case eTypeInt8         : _size =  1; break;
			case eTypeUInt8        : _size =  1; break;
			case eTypeInt16        : _size =  2; break;
			case eTypeUInt16       : _size =  2; break;
			case eTypeInt32        : _size =  4; break;
			case eTypeUInt32       : _size =  4; break;
			case eTypeInt64        : _size =  8; break;
			case eTypeUInt64       : _size =  8; break;
			case eTypeFloat        : _size =  4; break;
			case eTypeDouble       : _size =  8; break;
			case eTypeIeeeExtended : _size = 10; break;
			default: break;
		}
	}
	catch( std::runtime_error e )
	{
		LOG_ERROR( "(" << _id << ") " << e.what() );
	}
}

}

