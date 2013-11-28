#include "Element.hpp"
#include <Common/log.hpp>
#include <SpecReader/SpecNode.hpp>

#include <iostream>

namespace basic_element
{

Element::Element( const ShPtrSpecNode node,
	              const ShPtrElement  previous,
	              const ShPtrElement  parent )
	: _parent        ( parent   )
	, _previous      ( previous )
	, _specNode      ( node )
	, _id            ( node->getId()    )
	, _label         ( node->getLabel() )
	, _uId           ( node->getUId()   )
	, _size          ( getElementSize( node->getId(), node->getType(), node->getValues() )            )
	, _iteration     ( getElementIteration( node->getId(), node->getRepetitions(), previous, parent ) )
	, _groupSize     ( 0 )
	, _countExpr     ( node->getCount()       )
	, _requiredExpr  ( node->getRequirement() )
	, _groupSizeExpr ( node->getGroupSize()   )
	, _values        ( node->getValues()      )
	, _map           ( node->getMap()         )
	, _mapValue      ( "" )
	, _dispValue     ( "" )
	, _rangeExpr     ( node->getRange()       )
	, _repetExpr     ( node->getRepetitions() )
	, _type          ( node->getType()        )
	, _displayType   ( node->getDisplayType() )
	, _status        ( eStatusNotChecked )
	, _error         ( "" )
	, _warning       ( "" )
	, _isGroup       ( node->isGroup()     )
	, _isOrdered     ( node->isOrdered()   )
	, _isOptional    ( node->isOptional()  )
	, _isBigEndian   ( node->isBigEndian() )
	, _checkedGroup  ( false )
	, _data          ( nullptr )
{
	if( ! _parent.expired() )
		_parent.lock()->_children.push_back( std::make_shared< Element >( *this ) );

	LOG_TRACE( "[element] Create new Element " << _id << " :"       );
	LOG_TRACE(" [element]   - Parent   : " << _parent.lock()->_id   );
	LOG_TRACE(" [element]   - Previous : " << _previous.lock()->_id );
	LOG_TRACE(" [element]   - Iteration: " << _iteration            );
}

Element::ShPtrSpecNode Element::next( )
{
	// if element has been checked, back to the same SpecNode
	if( _status == eStatusNotChecked )
		return _specNode;

	ShPtrElement  parent;
	// if parent exists, copy it
	if( _parent.use_count() != 0 )
		parent = _parent.lock();
	
	// Optional: if element optional & invalid, go to the next SpecNode
	if( _isOptional && _status == eStatusInvalidButOptional )
	{
		LOG_TRACE( "[element] Next: next node of " << _id<< " ( optional )" );
		if( _specNode->next() != nullptr )
			return _specNode->next();
		if( parent != nullptr )
			return parent->next( );
	}
	
	// Unordered Groups: if element valid and parent is unordered, go to the first child of the parent
	if( _status == eStatusValid && _parent.use_count() != 0 && ( ! parent->_isOrdered ) && ( ! _isGroup || _checkedGroup ) )
	{
		LOG_TRACE( "[element] Next: " << _id << "'s parent first child ( unordered group )" );
		return parent->_specNode->firstChild();
	}
	
	// Groups: if element has a group not already checked and is valid or first time parsed, go to the first child
	if( _specNode->isGroup() && ! _checkedGroup && ( _iteration == 1 || _status == eStatusValid ) && _status != eStatusInvalidButSkip )
	{
		_checkedGroup = true;
		ShPtrSpecNode child( _specNode->firstChild() );
		LOG_TRACE( "[element] Next: first child of " << _id << " ( group )" );
		return child;
	}
	
	// Repetition: if repeated element, go to the same SpecNode
	if( ! _repetExpr.empty() && _status == eStatusValid )
	{
		LOG_TRACE( "[element] Next: same node " << _id << " ( repetition )" );
		return _specNode;
	}

	ShPtrSpecNode nextNode = _specNode->next();

	// Last Element: if their is no more SpecNode after and parent exists, go to the node after the parent
	if( nextNode == nullptr && _parent.use_count() != 0 )
	{
		LOG_TRACE( "[element] Next: "<< _id << "'s parent next node ( end of group )" );
		return parent->next( );
	}

	LOG_TRACE( "[element] Next: next node of " << _id );
	return nextNode;
}

void Element::set( const char* data, const size_t& size )
{
	_data = new char [size];
	_size = size;
	
	std::memcpy( _data, data, _size );
}

size_t Element::getElementSize( const std::string& id, const EType type, const std::vector<std::string>& values )
{
	size_t size = 0;
	try
	{
		if( ! values.empty() )
		{
			size = values.at( 0 ).size();
			for( std::string value : values )
				if( value.size() != size )
					throw std::runtime_error( "Values must have the same size" );
		}

		switch( type )
		{
			case eTypeInt8         :
			case eTypeUInt8        : return  1;
			case eTypeInt16        :
			case eTypeUInt16       : return  2;
			case eTypeInt32        :
			case eTypeUInt32       :
			case eTypeFloat        : return  4;
			case eTypeInt64        :
			case eTypeUInt64       :
			case eTypeDouble       : return  8;
			case eTypeIeeeExtended : return 10;
			default: break;
		}
	}
	catch( std::runtime_error e )
	{
		LOG_ERROR( "[element] " << e.what() << " (" << id << ")" );
	}
	return size;
}

size_t Element::getElementIteration( const std::string& id, const ExpressionList& repetExpr, const ShPtrElement& previous, const ShPtrElement& parent )
{
	size_t iteration = 1;
	if( repetExpr.empty() )
		return iteration;
	
	ShPtrElement prev = previous;
	while( prev != nullptr || ( parent != nullptr && prev->_id != parent->_id ) )
	{
		if( prev->_id == id && ( prev->_status == eStatusValid || prev->_status == eStatusPassOver ) )
		{
			iteration = prev->_iteration + 1;
			break;
		}

		if( parent != nullptr && prev->_id == parent->_id )
			break;

		prev = prev->getPrevious();
	}
	return iteration;
}

}
