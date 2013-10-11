#ifndef _QICHE_ELEMENT_HPP_
#define _QICHE_ELEMENT_HPP_

#include "common.hpp"

#include <set>
#include <set>

class Data;
class Number;

class Element
{
public:
	Element( const SpecNode* node, const Element* prev )
		: id       ( node->getId() )
		, index    ( node->getIndex() )
		, type     ( node->getType() )
		, specNode ( node )
		, status   ( eStatusNotCheck )
		, parent   ( node->getParent() )
		, checkedGroup( false )
		, iteration   ( 1 )
		, previous    ( prev )
	{
		if( node->isRepeated() > 1 && ( previous != NULL ) )
		{
			const Element* e = previous;
			while( e != NULL )
			{
				if( e->getId() == node->getId() )
				{
					iteration = e->getIteration() + 1;
					break;
				}
				else
				{
					e = e->getParent();
				}
			}
		}
	}

	virtual void check() = 0;
	
	EStatus getStatus() { return status; }
	
	const SpecNode* next( )
	{
		if( status == eStatusNotCheck )
		{
			return specNode;
		}
		
		if( specNode->isOptional() &&
			status == eStatusInvalid )
		{
			return specNode->next( parent );
		}
		
		if( status == eStatusInvalidButSkip &&
			parent != NULL &&
			( ! parent->specNode->isOrdered() ) )
		{
			std::cout << "Unordered, check one another baby !" << std::endl;
			status = eStatusInvalidButSkip;
			
			return parent->specNode->firstChild( parent );
		}
		
		if( specNode->isGroup() && ! checkedGroup )
		{
			checkedGroup = true;
			return specNode->firstChild( this );
		}
		
		size_t count = specNode->isRepeated();

		if( count > 1 )
		{
			if( iteration < count )
				return specNode;
		}
		
		const SpecNode* sn = specNode->next( parent );
		
		if( sn == NULL && parent != NULL )
		{
			if( ! parent->specNode->isOrdered() )
			{
				std::set< std::string > childIds;
				
				childIds = parent->specNode->getChildNodes();
				
				const Element* p = previous;
				while( p->getId() != parent->getId() )
				{
					for( auto id : childIds )
					{
						if( p->getId() == id )
						{
							childIds.erase( id );
						}
					}
					p = p->previous;
				}
				
				
				std::cout << id << "   " << index << std::endl;
				std::cout << "End of check Unordered " << childIds.size() << std::endl;
				
				if( childIds.size() != 0 )
				{
					std::cout << "prout" << std::endl;
					//status = eStatusInvalid;
					parent->status = eStatusInvalidForUnordered;
				}
			}
			return parent->next( );
		}
		
		return sn;
	}
	
	std::string getId()    const { return id; }
	size_t      getIndex() const { return index; }
	EType       getType()  const { return type; }
	size_t      getIteration()  const { return iteration; }
	
	std::string getStringStatus() const
	{
		switch( status )
		{
			case eStatusValid       : return "valid";
			case eStatusInvalid     : return "invalid";
			case eStatusInvalidButOptional : return "invalid but optional";
			case eStatusInvalidForUnordered : "invalid for unordered";
			case eStatusPassOverData: return "pass over data";
			case eStatusSkip        : return "skip";
		}
		return "";
	}
	
	Element* getParent() const { return parent; }
	
private:
	std::string id;
	char*       data;
	EType       type;
	
	bool        checkedGroup;
	
protected:
	size_t      index;
	size_t      iteration;
	EStatus     status;
	
	const SpecNode* specNode;
	Element*  parent;
	const Element* previous;
};

#endif
