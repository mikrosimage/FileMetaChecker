#include "Element.hpp"
#include <Common/log.hpp>
#include <SpecReader/SpecNode.hpp>

#include <iostream>

namespace basic_element
{

Element::Element( const ShPtrSpecNode node,
	              const ShPtrElement  previous,
	              const ShPtrElement  parent )
	: _parent          ( parent   )
	, _previous        ( previous )
	, _specNode        ( node )
	, _id              ( node->getId()    )
	, _label           ( node->getLabel() )
	, _uId             ( node->getUId()   )
	, _specGroupSize   ( 0 )
	, _childrenSize    ( 0 )
	, _countExpr       ( node->getCount()       )
	, _requiredExpr    ( node->getRequirement() )
	, _groupSizeExpr   ( node->getGroupSize()   )
	, _values          ( node->getValues()      )
	, _map             ( node->getMap()         )
	, _mapValue        ( "" )
	, _dispValue       ( "" )
	, _rangeExpr       ( node->getRange()       )
	, _repetExpr       ( node->getRepetitions() )
	, _type            ( node->getType()        )
	, _displayType     ( node->getDisplayType() )
	, _iteration       ( getElementIteration( _id, _repetExpr, previous, parent ) )
	, _status          ( eStatusUnknown )
	, _endChar         ( node->getEndChar()      )
	, _isGroup         ( node->isGroup()         )
	, _isOrdered       ( node->isOrdered()       )
	, _isOptional      ( node->isOptional()      )
	, _isBigEndian     ( node->isBigEndian()     )
	, _isCaseSensitive ( node->isCaseSensitive() )
	, _keepEndingChar  ( node->keepEndingChar()  )
	, _checkedGroup    ( false )
{
	if( ! _parent.expired() )
		_parent.lock()->_children.push_back( std::make_shared< Element >( *this ) );

	LOG_TRACE( "[element] Create new Element " << _id << " :"       );
	if( ! _parent.expired() )
		LOG_TRACE(" [element]   - Parent   : " << _parent.lock()->_id   );
	if( ! _previous.expired() )
		LOG_TRACE(" [element]   - Previous : " << _previous.lock()->_id );
	LOG_TRACE(" [element]   - Iteration: " << _iteration            );
}

Element::ShPtrSpecNode Element::next( )
{
	ShPtrElement  parent;
	// if parent exists, copy it
	if( _parent.use_count() != 0 )
		parent = _parent.lock();
	
	if( _status == eStatusSkip )
	{
		LOG_TRACE( "[element] Next: next node of " << _id<< " ( optional / skip )" << std::endl);
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
	if( _specNode->isGroup() && ! _checkedGroup && ( _iteration == 1 || _status == eStatusValid ) && _status != eStatusSkip )
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

void Element::set( const std::vector< char >& data )
{
	_data.resize( data.size() );
	std::memcpy( &_data[0], &data[0], data.size() );
}


std::string Element::getPropertiesFlag()
{
	std::string props;
	props += ( _isBigEndian         ) ? "B" : "l";
	props += ( _isOptional          ) ? "O" : ".";
	props += ( _isGroup             ) ? "G" : ".";
	props += ( ! _isOrdered         ) ? "U" : ".";
	props += ( ! _repetExpr.empty() ) ? "R" : ".";
	return props;
}

size_t Element::getElementIteration( const std::string& id, const ExpressionList& repetExpr, const ShPtrElement& previous, const ShPtrElement& parent )
{
	size_t iteration = 1;
	if( repetExpr.empty() )
		return iteration;
	
	ShPtrElement prev = previous;
	while( prev != nullptr || ( parent != nullptr && prev->_id != parent->_id ) )
	{
		if( prev->_id == id && ( prev->_status == eStatusValid ) )
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
