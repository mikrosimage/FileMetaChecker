#ifndef _BASIC_ELEMENT_TOOLS_RANGE_HPP_
#define _BASIC_ELEMENT_TOOLS_RANGE_HPP_

#include <common/global.hpp>

namespace basic_element
{

template< typename ValueType >
class Range
{

public:
	Range();

	~Range()
	{
	}

	void setMinimum( const ValueType& min );
	void setMaximum( const ValueType& max );
	void setRange  ( const ValueType& min, const ValueType& max );

	bool isInRange( const ValueType& value );

private:
	ValueType _min;
	ValueType _max;
};

}

#include "Range.tcc"

#endif
