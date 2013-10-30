#include <Common/Element.hpp>
#include <Common/log.hpp>
#include <SpecReader/SpecNode.hpp>

#include <iostream>

namespace sr = spec_reader;

namespace basic_element
{

Element::Element( const std::shared_ptr< sr::SpecNode > node,
	              const std::shared_ptr< Element > previous,
	              const std::shared_ptr< Element > parent )
	: _parent      ( parent )
	, _previous    ( previous )
	, _specNode    ( node )
	, _prop        ( { node->getId(),			// id
	                   node->getLabel(),		// label
	                   node->getUId(),			// uId
	                   0,						// size
	                   1,						// iteration
	                   node->getCount(),		// countExpr
	                   node->getRequirement(),	// requiredExpr
	                   node->getGroupSize(),	// groupSizeExpr
	                   node->getValues(),		// values
	                   node->getRange(),		// rangeExpr
	                   node->getRepetitions(),	// repetExpr
	                   node->getMap(),			// map
	                   node->getType(),			// type
	                   node->getSubType(),		// subType
	                   node->getDisplayType(),	// displayType
	                   eStatusNotChecked,		// status
	                   nullptr,					// data
	                   node->isGroup(),			// isGroup
	                   node->isOrdered(),		// isOrdered
	                   node->isOptional(),		// isOptional
	                   node->isBigEndian(),		// bigEndianData
	                   "",						// error
	                   ""						// warning
	                } )
	, _checkedGroup( false )
{
	if( node->isRepeated() > 1 && ( previous.use_count() != 0 ) )
	{
		std::shared_ptr< Element > prev = previous;
		while( prev.use_count() != 0 )
		{
			if( prev->getId() == node->getId() )
			{
				_prop.iteration = prev->getIteration() + 1;
				break;
			}
			else
			{
				if( prev->getParent().use_count() == 0 )
					break;
				prev = prev->getParent();
			}
		}
	}

	if( ! _previous.expired() )
		LOG_FATAL( getId() << ": " << previous->getId() );

	if( ! _parent.expired() )
		_parent.lock()->_children.push_back( std::make_shared< Element >( *this ) );

	LOG_ERROR( getId() << ": " << &*this << " - Parent: " << &*_parent.lock() << " - Previous: " << &( *( _previous.lock().get() ) ) );
}

std::shared_ptr< spec_reader::SpecNode > Element::next( )
{
	// if element has been checked
	if( _prop.status == eStatusNotChecked )
	{
		return _specNode.lock();
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
	if( _prop.isOptional && _prop.status == eStatusInvalid )
	{
		// go to next SpecNode
		return _specNode.lock()->next();
	}
	
	// Unordered groups : if element status = Valid and parent is not ordered
	if( _prop.status == eStatusValid && _parent.use_count() != 0 && ( ! parent->_specNode.lock()->isOrdered() ) )
	{
		// go to the first SpecNode of the childhood
		return parent->_specNode.lock()->firstChild();
	}
	
	// Groups :
	// if element has a group not already checked
	if( _specNode.lock()->isGroup() && ! _checkedGroup )
	{
		LOG_WARNING( "Element::next " << getId() << ": IsGroup" );
		// it becomes checked
		_checkedGroup = true;
		// creates a pointer to this current element
		std::weak_ptr< spec_reader::SpecNode > child( _specNode.lock()->firstChild() );
		// go to the first child SpecNode
		return child.lock();
	}
	
	// Repetitions
	// get repetitions
	size_t count = _specNode.lock()->isRepeated();
	
	// if repeated element
	if( count > 1 )
	{
		LOG_WARNING( "Element::next " << getId() << ": Repeated: " << _prop.iteration );
		// and not repeated enough yet
		if( _prop.iteration < count )
		{
			// go to the same SpecNode
			return _specNode.lock();
		}
	}
	
	// creates a pointer to the next SpecNode
	std::shared_ptr< sr::SpecNode > nextNode = _specNode.lock()->next();
	
	// in Unoredered groups : check if every nodes have been checked
	// if their is no more SpecNode after and parent exists
	if( nextNode == nullptr && _parent.use_count() != 0 )
	{
		LOG_WARNING( "Element::next " << getId() << ": Last Element" );
		
		// if the current group (parent's children) is not ordered
		if( ! _parent.lock()->_specNode.lock()->isOrdered() )
		{
			LOG_WARNING( "Element::next " << getId() << ": Unordered group check" );
			
			// create a list with the children IDs
			std::set< std::string > childIds = parent->_specNode.lock()->getChildrenNodes();

			// if the previous element exists (the current element is not the first)
			if( _previous.lock().get() != nullptr )
			{
				// creates a pointer to the previous element
				std::shared_ptr< Element > prev = _previous.lock();
				// while the previous element is not the parent
				while( prev->getId() != parent->getId() )
				{
					// for each ID in the children ID list
					for( auto id : childIds )				
					{
						// if the previous element's ID is equal to one ID of the list
						if( prev->getId() == id && prev->getStatus() == eStatusValid )
						{
							LOG_WARNING( "Element::next " << "childIds: " << id );
							// erase this ID from the list
							childIds.erase( id );
						}
					}
					// go to the previous element of the previous, etc..
					prev = prev->_previous.lock();
				}
			}			
			LOG_WARNING( "Element::next " << getId() << ": End of check Unordered, remaining children: " << childIds.size() );
			// if it remains some IDs in the list
			if( childIds.size() != 0 )
			{
				LOG_ERROR( "Element::next " << _parent.lock()->getId() );
				LOG_ERROR( "Element::next " << &*_parent.lock() );
				// every nodes haven't been checked : the parent is not valid
				_parent.lock()->setStatus( eStatusInvalidForUnordered );
			}
		}
		// go to the node after the parent
		return parent->next( );
	}
	LOG_WARNING( "Element::next " << getId() << ": Next !" );
	// go to the next node
	return nextNode;
}

std::string Element::getStringStatus() const
{
	switch( _prop.status )
	{
		case eStatusNotChecked         : return "not checked";
		case eStatusSkip               : return "skip";
		case eStatusPassOver           : return "pass over";
		case eStatusValid              : return "valid";
		case eStatusInvalid            : return "invalid";
		case eStatusInvalidButSkip     : return "invalid but skip";
		case eStatusInvalidButOptional : return "invalid but optional";
		case eStatusInvalidForUnordered: return "invalid for unordered";
	}
	return "";
}

char* Element::getData() const
{
	return _prop.data;
}

void Element::set( const char* data, const size_t& size )
{
	_prop.data = new char [size];
	_prop.size = size;
	
	std::memcpy( _prop.data, data, _prop.size );
}

void Element::addErrorLabel( const std::string& error )
{
	if( _prop.error.empty() )
		_prop.error = error;
	else
		_prop.error += " / " + error;
}

void Element::addWarningLabel( const std::string& warning )
{
	if( _prop.warning.empty() )
		_prop.warning = warning;
	else
		_prop.warning += " / " + warning;
}

std::string Element::getErrorLabel()
{
	return _prop.error;
}

std::string Element::getWarningLabel()
{
	return _prop.warning;
}

void Element::setStatus( const EStatus status )
{
	_prop.status = status;
}

}


