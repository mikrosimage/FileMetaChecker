#ifndef _ELEMENT_CHECKER_RANGES_RANGE_HPP_
#define _ELEMENT_CHECKER_RANGES_RANGE_HPP_

namespace element_checker
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
	void setRange  ( const std::string& min, const std::string& max );

	bool isInRange( const ValueType& value );

	ValueType fromString( const std::string& value );

private:
	ValueType _min;
	ValueType _max;
};

}

#include "Range.tcc"

#endif
