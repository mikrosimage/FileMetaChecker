#ifndef _EXTRACTOR_SPECS_VALUE_SPECIFICATION_HPP_
#define _EXTRACTOR_SPECS_VALUE_SPECIFICATION_HPP_

#include <common/global.hpp>

#include <boost/foreach.hpp>

template<typename NumberType>
NumberType getValueFromString( const std::string& string )
{
	return atoi( string.c_str() );
}

template< >
float getValueFromString<float>( const std::string& string )
{
	return atof( string.c_str() );
}

template< >
double getValueFromString<double>( const std::string& string )
{
	return atof( string.c_str() );
}

template<typename NumberType>
inline bool isEqual( const NumberType& val1, const NumberType& val2 )
{
	return val1 == val2;
}

template< >
inline bool isEqual<float>( const float& val1, const float& val2 )
{
	return val1 == val2 || ( std::isnan( val1 ) && std::isnan( val2 ) );
}

template< >
inline bool isEqual<double>( const double& val1, const double& val2 )
{
	return val1 == val2 || ( std::isnan( val1 ) && std::isnan( val2 ) );
}

template< typename Type >
std::string getMap( SubSpec& subSpec, std::map< Type, std::string >& map, const size_t size, const bool isBigEndian, const Type value )
{
	std::string description = "";
	if( boost::optional< const Spec& > mapNode = subSpec.second.get_child_optional( kMap ) )
	{
		BOOST_FOREACH( SubSpec& m, mapNode.get() )
		{
			std::string index = m.second.ordered_begin()->first;
			try
			{
				Type i = getValueFromString< Type >( index );
				if( isEqual( value, i ) )
				{
					description = m.second.get< std::string >( index );
				}
			}
			catch(...)
			{
				COMMON_COUT( "Can not convert index of map: " << index );
			}
		}
	}
	return description;
}

template< typename Type >
std::vector< Type > getMultipleValues( SubSpec& subSpec, const std::string& nodeName )
{
	std::vector< Type > vector;
	if( boost::optional< const Spec& > valuesNode = subSpec.second.get_child_optional( nodeName ) )
	{
		std::string stringValue = subSpec.second.get< Type >( nodeName );

		if( stringValue.length() == 0 )
		{
			BOOST_FOREACH( SubSpec& value, valuesNode.get( ) )
			{
				vector.push_back( value.second.get_value< Type >() );
			}
		}
		else
		{
			vector.push_back( stringValue );
		}
	}
	return vector;
}

#endif
