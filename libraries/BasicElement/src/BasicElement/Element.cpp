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
	, _isDetailed      ( node->isDetailed()      )
	, _checkedGroup    ( false )
{
	LOG_TRACE( "[element] Create new Element " << _id << " :"       );
	if( ! _parent.expired() )
		LOG_TRACE(" [element]   - Parent   : " << _parent.lock()->_id   );
	if( ! _previous.expired() )
		LOG_TRACE(" [element]   - Previous : " << _previous.lock()->_id );
	LOG_TRACE(" [element]   - Iteration: " << _iteration            );
}

Element::ShPtrSpecNode Element::next( )
{
	ShPtrElement parent = ( _parent.use_count() != 0 )? _parent.lock() : nullptr ;
	ShPtrSpecNode nextNode = _specNode->next();

	if( parent != nullptr && parent->_status == eStatusSkip )
		return parent->next( );

	if( _status == eStatusSkip )
	{
		LOG_TRACE( "[element] Next: next node of " << _id<< " ( optional / skip )" );
		if( nextNode != nullptr  && ( parent == nullptr || parent->_status != eStatusSkip ) )
			return nextNode;
		if( parent != nullptr )
			return parent->next( );
	}
	
	// Unordered Groups: if element valid and parent is unordered, go to the first child of the parent
	if( ( _status == eStatusValid || _status == eStatusUnknown ) && parent != nullptr && ! parent->_isOrdered && ( ! _isGroup || _checkedGroup ) )
	{
		LOG_TRACE( "[element] Next: " << _id << "'s parent first child ( unordered group )" );
		return parent->_specNode->firstChild();
	}
	
	// Groups: if element has a group not already checked and is valid or first time parsed, go to the first child
	if( _isGroup && ! _checkedGroup && ( _iteration == 1 || _status == eStatusValid ) && _status != eStatusSkip )
	{
		_checkedGroup = true;
		ShPtrSpecNode child( _specNode->firstChild() );
		LOG_TRACE( "[element] Next: first child of " << _id << " ( group )" );
		return child;
	}
	
	// Repetition: if repeated element, go to the same SpecNode
	if( _repetExpr.size() && _status == eStatusValid )
	{
		LOG_TRACE( "[element] Next: same node " << _id << " ( repetition )" );
		return _specNode;
	}

	// Last Element: if their is no more SpecNode after and parent exists, go to the node after the parent
	if( nextNode == nullptr && parent != nullptr )
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

void Element::addChild( const ShPtrElement& element )
{
	if( element->getParent()->_uId != _uId )
	{
		LOG_TRACE( "[element] wrong parent, cannot add child." );
		return;
	}
	_children.push_back( element );
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

std::string Element::getLabelsLegend()
{
	std::stringstream legend;
	legend << "========= LEGEND =========" << std::endl;
	legend << " v = value                " << std::endl;
	legend << " t = type                 " << std::endl;
	legend << " i = iteration            " << std::endl;
	legend << " S = status               " << std::endl;
	legend << " s = size                 " << std::endl;
	legend << " @ = address              " << std::endl;
	legend << " E = Error                " << std::endl;
	legend << " W = Warning              " << std::endl;
	legend                                 << std::endl;
	return legend.str();
}

std::string Element::getElementPropertiesLegend()
{
	std::stringstream props;
	props << "=== ELEMENT PROPERTIES ===" << std::endl;
	props << "B.... = Big endian        " << std::endl;
	props << "l.... = little endian     " << std::endl;
	props << ".O... = Optional          " << std::endl;
	props << "..G.. = Group             " << std::endl;
	props << "...U. = Unordered         " << std::endl;
	props << "....R = Repeated          " << std::endl;
	props                                 << std::endl;
	return props.str();
}

}
