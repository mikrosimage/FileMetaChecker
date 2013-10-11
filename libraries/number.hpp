#ifndef _QICHE_NUMBER_HPP_
#define _QICHE_NUMBER_HPP_

#include "element.hpp"

class Number : public Element
{
	
public:
	Number( const SpecNode* node, const Element* previous )
	 : Element( node, previous )
	{
	}
	
	void check()
	{
		std::cout << index << "  check number" << std::endl;
		status = eStatusPassOverData;
	}
};

#endif
