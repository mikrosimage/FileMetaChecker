#include <Common/Element.hpp>
#include <Common/log.hpp>
#include <SpecReader/SpecNode.hpp>

#include <iostream>

namespace sr = spec_reader;

namespace basic_element
{

Element::Element( const std::shared_ptr< sr::SpecNode > node, const std::shared_ptr< Element > previous )
	: _parent      ( node->getParent() )
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

	// if( _parent.use_count() != 0 )
	if( _parent.lock().get() != nullptr )
	{
		_parent.lock()->_children.push_back( std::make_shared< Element >( *this ) );
	}
}

std::shared_ptr< spec_reader::SpecNode > Element::next( )
{
	if( _prop.status == eStatusNotChecked )		// if element has been checked
	{
		LOG_TRACE( getId() << ": Not Checked" );
		return _specNode;
	}

	std::shared_ptr< Element > parent;

	if( _parent.use_count() != 0 )		// if parent exists (?)
	{
		LOG_TRACE( getId() << ": Copy parent" );
		parent = _parent.lock();		// copy the parent
	}
	
	// Optional element :
	if( _prop.isOptional && _prop.status == eStatusInvalid )		// if element optional & invalid
	{
		LOG_TRACE( getId() << ": Element optional & Invalid" );
		return _specNode->next( parent );							// go to next SpecNode
	}
	
	// Unordered groups :
	if( _prop.status == eStatusInvalidButSkip 		// if element status = invalidButSkip (unordered group case)
	 && _parent.use_count() != 0 					// and if parent exists
	 && ( ! parent->_specNode->isOrdered() ) )		// and if parent is not ordered
	{
		LOG_TRACE( getId() << ": Unordered, check one another baby !" );
		return parent->_specNode->firstChild( parent );		// go to the first SpecNode of the childhood
	}
	
	// Groups :
	if( _specNode->isGroup() && ! _checkedGroup )	// if element has a group not already checked 
	{
		LOG_TRACE( getId() << ": IsGroup" );
		_checkedGroup = true;						// it becomes checked
		std::shared_ptr< Element > t = std::make_shared< Element >( *this );		// creates a pointer to this current element
		return _specNode->firstChild( t );			// go to the first child SpecNode
	}
	
	// Repetitions
	size_t count = _specNode->isRepeated();		// get repetitions
	
	if( count > 1 )								// if repeated element
	{
		LOG_TRACE( getId() << ": Repeated: " << _prop.iteration );
		if( _prop.iteration < count )			// and not repeated enough yet
			return _specNode;					// go to the same SpecNode
	}
	
	std::shared_ptr< sr::SpecNode > nextNode = _specNode->next( parent );		// creates a pointer to the next SpecNode
	
	// in Unoredered groups : check if every nodes have been checked
	if( nextNode == nullptr && _parent.use_count() != 0 )		// if their is no more SpecNode after and parent exists
	{
		LOG_TRACE( getId() << ": Last Element" );
		if( ! parent->_specNode->isOrdered() )			// if the current group (parent's children) is not ordered
		{
			LOG_TRACE( getId() << ": Unordered group check" );
			std::set< std::string > childIds = parent->_specNode->getChildrenNodes();	// create a list with the children IDs
			
			if( _previous.use_count() != 0 )		// if the previous element exists (the current element is not the first)
			{
				std::shared_ptr< Element > prev = _previous.lock();	// creates a pointer to the previous element
				while( prev->getId() != parent->getId() )	// while the previous element is not the parent
				{
					for( auto id : childIds )				// for each ID in the children ID list
					{
						if( prev->getId() == id )			// if the previous element's ID is equal to one ID of the list
							childIds.erase( id );			// erase this ID from the list
					}
					prev = prev->_previous.lock();			// go to the previous element of the previous, etc..
				}
			}			
			
			std::cout << _prop.id << "   " << _prop.uId << std::endl;
			std::cout << "End of check Unordered " << childIds.size() << std::endl;
			
			if( childIds.size() != 0 )					// if it remains some IDs in the list
			{
				std::cout << "prout" << std::endl;		// Error or Warning ?
				//status = eStatusInvalid;
				parent->_prop.status = eStatusInvalidForUnordered;	// every nodes haven't been checked : the parent is not valid
			}
		}
		return parent->next( );			// go to the node after the parent
	}
	LOG_TRACE( getId() << ": Next !" );
	return nextNode;	// go to the next node
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
