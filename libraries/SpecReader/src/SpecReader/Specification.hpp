#ifndef _SPEC_READER_SPECIFICATION_HPP_
#define _SPEC_READER_SPECIFICATION_HPP_

#include "SpecNode.hpp"

namespace spec_reader
{

class Specification
{
public:
	Specification()
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

}

#endif
