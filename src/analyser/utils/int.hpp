#ifndef _UTILS_INT_HPP
#define _UTILS_INT_HPP

#include <sstream>
#include <cstdlib>
#include <iomanip>

#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

#include "generic.hpp"
#include "checker.hpp"

namespace bpt = boost::property_tree;

typedef bpt::ptree::value_type Node;

class IntChecker
	: public Checker
{
public:
	IntChecker()
		: Checker()
	{
	}

	bool isValidInt( Node &n, int value )
	{
		if( n.second.get_child_optional( kValue ) )
		{
			if( value == n.second.get<int>( kValue ) )
			{
				nodeStatus = eSegmentStatusValid;
			}
			return true;
		}
		return false;
	}
	
	
	bool isValidChoice( Node &n, int value )
	{
		if( boost::optional<bpt::ptree &> choice = n.second.get_child_optional( "choice" ) )
		{
			BOOST_FOREACH( Node& m, *choice )
			{
				if( value == m.second.get_value<int>() )
				{
					nodeStatus = eSegmentStatusValid;
				}
			}
			return true;
		}
		return false;
	}
	
	bool isValidInterval( Node &n, int value )
	{
		if( boost::optional<bpt::ptree &> choice = n.second.get_child_optional("interval") ) 
		{
			std::vector<int> minMax;
			BOOST_FOREACH( Node &m, *choice )
			{
				minMax.push_back( m.second.get_value<int>() );
			}
			
			if( minMax.size() != 2 )
			{
				return true;
			}
			
			if( minMax.at(0) < value && value < minMax.at(1) )
			{
				nodeStatus = eSegmentStatusValid;
			}
			return true;
		}
		return false;
	}
	
	
	bool isValid( Node &n, char* buffer, size_t size )
	{
		std::stringstream ss;
		//for( size_t i = 0; i < size; ++i )
		if( n.second.get<std::string>( kEndian ) == kEndianLittle )
		{
			for( int i = size - 1; i >= 0; --i )
			{
				ss << std::hex << std::setw( 2 ) << std::setfill( '0' ) << (int)(unsigned char)buffer[i];
			}
		}
		else
		{
			for( size_t i = 0; i < size; ++i )
			{
				ss << std::hex << std::setw( 2 ) << std::setfill( '0' ) << (int)(unsigned char)buffer[i];
			}
		}
		int buff;
		ss >> buff;
		COMMON_COUT( kTypeInt << " : " << buff << " : " << ss.str() << " : " << size );
		

		if( ! isValidInt( n, buff ) )
		{
			if( !isValidChoice( n, buff ) )
			{
				if( ! isValidInterval( n, buff ) )
				{
					nodeStatus = eSegmentStatusPass;
				}
			}
		}
		
		n.second.put( kValue, buff );
		return true;
	}
};
#endif
