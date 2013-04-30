
#include "common.hpp"
#include "convertersTools.hpp"

#include <extractor/inputFile/File.hpp>

#include <boost/foreach.hpp>

template< typename NumberType >
std::string getStringForType()
{
	return "";
}

template< >
std::string getStringForType<uint8>()
{
	return "uint8";
}

template< >
std::string getStringForType<int8>()
{
	return "int8";
}

template< >
std::string getStringForType<uint16>()
{
	return "uint16";
}

template< >
std::string getStringForType<int16>()
{
	return "int16";
}

template< >
std::string getStringForType<uint32>()
{
	return "uint32";
}

template< >
std::string getStringForType<int32>()
{
	return "int32";
}

template< >
std::string getStringForType<float>()
{
	return "float";
}

template< >
std::string getStringForType<double>()
{
	return "double";
}

template< typename NumberType >
bool getRange( SubSpec& subSpec, const NumberType value )
{
	bool isInRange = true;
	if( boost::optional< const Spec& > rangeNode = subSpec.second.get_child_optional( kRange ) )
	{
		isInRange = false;
		BOOST_FOREACH( SubSpec& m, rangeNode.get() )
		{
			if( m.second.get_child_optional( kMin ) && m.second.get_child_optional( kMax ) )
			{
				NumberType max = m.second.get< NumberType >( kMax );
				NumberType min = m.second.get< NumberType >( kMin );
				if( value >= min && value <= max )
				{
					isInRange = true;
				}
			}
			if( !m.second.get_child_optional( kMin ) && m.second.get_child_optional( kMax ) )
			{			
				NumberType max = m.second.get< NumberType >( kMax );
				if( value <= max )
				{
					isInRange = true;
				}
			}
			if( m.second.get_child_optional( kMin ) && !m.second.get_child_optional( kMax ) )
			{			
				NumberType min = m.second.get< NumberType >( kMin );
				if( value >= min )
				{
					isInRange = true;
				}
			}
		}
	}
	if(!isInRange)
	{
		COMMON_COUT( common::details::kColorRed << "Value error : out of range" << common::details::kColorStd );
	}
	return isInRange;
}

template< typename NumberType >
bool isValidNumber( File* _file, std::string& message, const std::string& type, const bool isBigEndian, SubSpec& subSpec, NumberType& value )
{
	if( type == getStringForType<NumberType>() )
	{
		size_t size = sizeof( NumberType );
		char buffer[ size ];
		Translator<NumberType> tr;
		std::map< NumberType, std::string > map;
		std::string stringValue;
		
		_file->readData( buffer, size );
		value = tr.translate( buffer, size, isBigEndian );
		stringValue = getMap( subSpec, map, size, isBigEndian, value );

		message += " = ";
		if( stringValue == "" )
		{
			message += getPrintable( value );
		}
		else
		{
			message += stringValue + " (" + getPrintable( value ) + ")";
		}
		return getRange( subSpec, value );
	}
	return false;
}


