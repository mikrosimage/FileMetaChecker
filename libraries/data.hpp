#ifndef _QICHE_DATA_HPP_
#define _QICHE_DATA_HPP_

#include "element.hpp"

class Data : public Element
{
	
public:
	Data( const SpecNode* node, const Element* previous )
	 : Element( node, previous )
	{
	}
	
	void check()
	{
		std::cout << index << "  check data" << std::endl;
		
		if( index == 7 )
		{
			status = eStatusInvalidButOptional;
			return;
		}
		
		if( index == 16 )
		{
			status = eStatusInvalidButSkip;
			return;
		}
		
		status = eStatusValid;
	}
};

#endif
