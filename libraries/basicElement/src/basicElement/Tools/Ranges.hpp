#ifndef _BASIC_ELEMENT_TOOLS_RANGES_HPP_
#define _BASIC_ELEMENT_TOOLS_RANGES_HPP_

#include "Range.hpp"

namespace basic_element
{

template< typename ValueType >
class Ranges
{

public:
	Ranges()
	{
	}

	~Ranges()
	{
	}

	void addRange( const Range< ValueType >& range )
	{
		_ranges.push_back( range );
	}

	bool isInRanges( const ValueType& value )
	{
		for( Range< ValueType > range : _ranges )
		{
			if( range.isInRange( value ) )
				return true;
		}
		return false;
	}

	bool empty()
	{
		return _ranges.empty();
	}

private:
	std::vector< Range< ValueType > > _ranges;

};

}

#endif
