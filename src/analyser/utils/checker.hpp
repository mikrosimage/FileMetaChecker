#ifndef _UTILS_CHECKER_HPP
#define _UTILS_CHECKER_HPP

#include "generic.hpp"

class Checker
{
public:
	Checker()
		: nodeStatus( eSegmentStatusNotValid )
	{
	}
	
	virtual bool isValid( Node &n, char* buffer, size_t size ) = 0;
	
	SegmentStatus getStatus( )
	{
		return nodeStatus;
	}
	
protected:
	SegmentStatus nodeStatus;
};

#endif
