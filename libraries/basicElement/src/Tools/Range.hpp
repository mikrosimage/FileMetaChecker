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
		if( min > max )
			throw std::range_error( "The 'min' value must be less than the 'max' value" );
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
	size_t hexaToUint( const std::string& hexa )
	{
		size_t value;
		std::stringstream sstr( hexa );
		sstr >> std::hex >> value;
		LOG_INFO( " \t hexa   : " << hexa << " ==> value: " << value );
		return value;
	}

private:
	std::vector< ValueType > _range;
};

// for Hexa values :
template<>
void Range< std::string >::setRange( const std::string& min, const std::string& max )
{
	LOG_INFO( " => Range: \tSET RANGE (string) " );
	size_t minValue = hexaToUint( min );
	size_t maxValue = hexaToUint( max );

	if( minValue > maxValue )
		throw std::range_error( "The 'min' value must be less than the 'max' value" );
	_range.push_back( min );
	_range.push_back( max );
}

template<>
bool Range< std::string >::isInRange( const std::string& value )
{
	LOG_INFO( " => Range: \tIS IN RANGE (string) " );
	size_t intValue = hexaToUint( value );
	size_t minValue = hexaToUint( _range.at(0) );
	size_t maxValue = hexaToUint( _range.at(1) );

	if( minValue <= intValue && maxValue >= intValue )
		return true;
	return false;
}


}

#endif
