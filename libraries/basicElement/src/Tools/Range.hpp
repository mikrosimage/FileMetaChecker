#ifndef _BASIC_ELEMENT_TOOLS_RANGE_HPP_
#define _BASIC_ELEMENT_TOOLS_RANGE_HPP_

namespace basic_element
{

template< typename ValueType >
class Range
{
public:
	Range()
	{
	}

	Range( const std::vector< ValueType >& range )
		: _range( range )
	{
	}

	~Range()
	{
	}

	void setRange( const ValueType& min, const ValueType& max )
	{
		LOG_INFO( " => Range: \tSET RANGE " );
		_range.push_back( min );
		_range.push_back( max );
	}

	bool isInRange( const ValueType& value )
	{
		LOG_INFO( " => Range: \tIS IN RANGE " );
		if( _range.at(0) <= value && _range.at(1) >= value )
			return true;
		return false;
	}

	const ValueType& at( const size_t& index )
	{
		LOG_INFO( " => Range: \tGET VALUE " );
		return _range.at( index );
	}

	size_t getSize()
	{
		LOG_INFO( " => Range: \tGET SIZE " );
		return _range.size();
	}

private:
	std::vector< ValueType > _range;
};


template<>
bool Range< std::string >::isInRange( const std::string& value )
{
	LOG_INFO( " => Range: \tIS IN RANGE (string) " );
	size_t intValue;
	size_t minValue;
	size_t maxValue;

	std::stringstream ssInt( value );
	std::stringstream ssMin( _range.at(0) );
	std::stringstream ssMax( _range.at(1) );

	ssInt >> std::hex >> intValue;
	ssMin >> std::hex >> minValue;
	ssMax >> std::hex >> maxValue;

	LOG_INFO( " \t value : " << value << " ==> intValue: " << intValue );
	LOG_INFO( " \t min   : " << _range.at(0) << " ==> minValue: " << minValue );
	LOG_INFO( " \t max   : " << _range.at(1) << " ==> maxValue: " << maxValue );

	if( minValue <= intValue && maxValue >= intValue )
		return true;
	return false;
}


}

#endif
