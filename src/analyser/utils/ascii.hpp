#ifndef _UTILS_ASCII_HPP
#define _UTILS_ASCII_HPP

#include "generic.hpp"
#include "checker.hpp"

class AsciiChecker
	: public Checker
{
public:
	AsciiChecker()
		: Checker()
	{
		
	}

	bool isValid( Node &n, char* buffer, size_t size )
	{
		std::string str;
		
		for( size_t i = 0; i < size; ++i)
		{
			str += (unsigned char)buffer[i];
		}
		
		std::string value = n.second.get<std::string>( kValue, "" );
		
		COMMON_COUT_DEBUG( kTypeAscii << " : " << str << " | " << value );
		
		if( value != "" )
		{
			if( str == value )
			{
				nodeStatus = eSegmentStatusValid;
			}
		}
		else
		{
			nodeStatus = eSegmentStatusPass;
		}
		n.second.put( kValue, str );
		n.second.put( kStatus, nodeStatus );
		return true;
	}
};
#endif
