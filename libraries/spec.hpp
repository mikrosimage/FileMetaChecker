#ifndef _QICHE_SPEC_HPP_
#define _QICHE_SPEC_HPP_

#include "specNode.hpp"

class Spec
{
public:
	Spec()
	{
	}
	
	SpecNode* getFirstNode( )
	{
		SpecNode* s = new SpecNode();
		s->setId( "firstNode" );
		s->setType( eTypeNumber );
		return s;
	}
};

#endif
