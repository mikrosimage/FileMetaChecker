#ifndef _QICHE_COMPARATOR_HPP_
#define _QICHE_COMPARATOR_HPP_

#include "element.hpp"
#include "data.hpp"
#include "number.hpp"
#include "specNode.hpp"

class Comparator
{
public:
	Comparator()
	{
	}
	
private:
	
	Element* getElement( const SpecNode* node, const Element* previous )
	{
		switch( node->getType() )
		{
			case eTypeData:
			{
				Data* d = new Data( node, previous );
				return static_cast< Element* >( d );
				break;
			}
			case eTypeNumber:
			{
				Number* n = new Number( node, previous );
				return static_cast< Element* >( n );
				break;
			}
		}
		return NULL;
	}
	
public:
	void check( Spec& spec, FileReader& reader, Report& report )
	{
		std::cout << "check start" << std::endl;
		
		const SpecNode* s = NULL;
		Element* e = getElement( spec.getFirstNode( ), NULL );
		
		while( ( s = e->next() ) != NULL )
		{
			e = getElement( s, e );
			e->check();
			
			if( e->getStatus() == eStatusInvalidButOptional ||
				e->getStatus() == eStatusInvalidButSkip )
			{
				std::cout << "go back in raw file" << std::endl;
				continue;
			}
			
			report.addElement( e );
			
			if( e->getIndex() > 20 )
				break;
		}
		
		std::cout << "check end" << std::endl;
	}
};

#endif
