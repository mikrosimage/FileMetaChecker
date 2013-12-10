#ifndef _ELEMENT_CHECKER_RANGES_RANGES_HPP_
#define _ELEMENT_CHECKER_RANGES_RANGES_HPP_

#include "Range.hpp"

namespace element_checker
{

template< typename ValueType >
class Ranges
{

public:
	Ranges()
	{
	}

	Ranges( const std::vector< std::pair< std::string, std::string > >& ranges )
	{
		for( std::pair< std::string, std::string > rangePair : ranges )
		{
			Range< ValueType > range;
			range.setRange( rangePair.first, rangePair.second );
			_ranges.push_back( range );
		}
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
		if( ! _ranges.size() )
			return true;

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
