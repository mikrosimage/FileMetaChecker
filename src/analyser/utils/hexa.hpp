#ifndef _UTILS_HEXA_HPP
#define _UTILS_HEXA_HPP

#include <sstream>
#include <cstdlib>
#include <iomanip>

#include "generic.hpp"
#include "checker.hpp"

class HexaChecker
	: public Checker
{
public:
	HexaChecker()
		: Checker()
	{
	}

	bool isValid( Node &n, char* buffer, size_t size )
	{
		std::stringstream ss;
		for( size_t i = 0; i < size; ++i)
		{
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)buffer[i];
		}
		
		std::string value = n.second.get<std::string>( kValue, "" );
		
		COMMON_COUT_DEBUG( kTypeHexa << " : " << ss.str() << " | " << value );
		
		if( value != "" )
		{
			if( ss.str() == value )
			{
				nodeStatus = eSegmentStatusValid;
			}
		}
		else
		{
			nodeStatus = eSegmentStatusPass;
		}
		n.second.put( kValue, ss.str() );
		return true;
	}
};
#endif
