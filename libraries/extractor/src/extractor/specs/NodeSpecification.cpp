#include "NodeSpecification.hpp"

#include <extractor/inputFile/File.hpp>
#include <extractor/inputFile/Translator.hpp>
#include <extractor/expressionParser/ExpressionParser.hpp>


#include <boost/foreach.hpp>

#include <common/global.hpp>
#include <iomanip>
#include <cstdlib>
#include <cmath>

const std::string kId     = "id";
const std::string kLabel  = "label";

const std::string kAscii  = "ascii";
const std::string kHexa   = "hexa";
const std::string kType   = "type";
const std::string kCount  = "count";
const std::string kMap    = "map";

const std::string kEndian        = "endian";
const std::string kEndianBig     = "big";
const std::string kEndianLittle  = "little";

const std::string kOptional      = "optional";
const std::string kOptionalTrue  = "true";
const std::string kOptionalFalse = "false";

NodeSpecification::NodeSpecification( File* file )
	: _file ( file )
{

}

template< typename IntType >
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


template< typename Type >
std::string getPrintable( Type value )
{
	std::ostringstream os;
	os << value;
	return os.str();
}

template< >
std::string getPrintable<uint8>( uint8 value )
{
	std::ostringstream os;
	os << (int)value;
	return os.str();
}

template< >
std::string getPrintable<int8>( int8 value )
{
	std::ostringstream os;
	os << (int)value;
	return os.str();
}


template<typename Type>
Type getValueFromString( const std::string& string )
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

template<typename Type>
inline bool isEqual( const Type& val1, const Type& val2 )
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


template< typename IntType >
bool isValidInt( File* _file, std::string& message, const std::string& type, const bool isBigEndian, SubSpec& subSpec, IntType& value )
{
	if( type == getStringForType<IntType>() )
	{
		size_t size = sizeof( IntType );
		char buffer[ size ];
		Translator<IntType> tr;
		std::map< IntType, std::string > map;
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
		return true;
	}
	return false;
}


template< typename RealType >
bool isValidReal( File* _file, std::string& message, const std::string& type, const bool isBigEndian, SubSpec& subSpec, RealType& value )
{
	if( type == getStringForType<RealType>() )
	{
		size_t size = sizeof( RealType );
		char buffer[ size ];
		Translator<RealType> tr;
		std::map< RealType, std::string > map;
		std::string stringValue;
		
		_file->readData( buffer, size );
		value = tr.translate( buffer, size, isBigEndian );
		stringValue = getMap( subSpec, map, size, isBigEndian, value );

		message += " = ";

		if( stringValue == "" )
		{
			message += getPrintable<RealType>(value);
		}
		else
		{
			message += stringValue + " (" + getPrintable(value) + ")";
		}
		return true;
	}
	return false;
}


template< typename Type >
std::vector< Type > getMultipleValues( SubSpec& subSpec, const std::string& nodeName )
{
	std::vector< Type > vector;
	if( boost::optional< const Spec& > valuesNode = subSpec.second.get_child_optional( nodeName ) )
	{
		vector.push_back( subSpec.second.get< Type >( nodeName ) );

		if( vector.size() == 1 && vector[0] == "" )
		{
			vector.clear();
			BOOST_FOREACH( SubSpec& value, valuesNode.get( ) )
			{
				vector.push_back( value.second.get_value< Type >() );
			}
		}
	}
	return vector;
}


bool NodeSpecification::isValid( SubSpec& subSpec )
{
	bool isValid = false;
	std::string message;
	std::string id         = subSpec.second.get< std::string >( kId );
	std::string label      = subSpec.second.get< std::string >( kLabel, "" );
	std::vector< std::string > asciiValues = getMultipleValues< std::string >( subSpec, kAscii );
	std::vector< std::string > hexaValues  = getMultipleValues< std::string >( subSpec, kHexa  );
	std::string typeValue  = subSpec.second.get< std::string >( kType, "" );
	std::string count      = subSpec.second.get< std::string >( kCount, "" );

	bool endianValue = ( subSpec.second.get<std::string>( kEndian, kEndianBig ) == kEndianBig );
	bool optional    = ( subSpec.second.get<std::string>( kOptional, kOptionalFalse ) == kOptionalTrue );

	// COMMON_COUT( "label " << label );
	
	if( asciiValues.size() != 0 )
	{
		message += kAscii + " => ";

		size_t size = asciiValues[0].size();
		for( size_t i = 0; i < asciiValues.size(); i++ )
		{
			Ascii value;

			if( i > 0 )
			{
				_file->goBack( size );
			}

			//COMMON_COUT( "read data " << size );
			char buffer[ size ];
			if( ! _file->readData( buffer, size ) )
				return optional;
			
			Translator<Ascii> tr;
			value = tr.translate( buffer, size );

			if( asciiValues[i] ==  value.value )
			{
				isValid = true;
				message += asciiValues[i];
			}
			// COMMON_COUT_VAR2( asciiValues[i], value.value );
		}
		
		if( optional && !isValid )
		{
			_file->goBack( size );
			return true;
		}
	}

	if( hexaValues.size() != 0 )
	{
		message += kHexa + " => ";
		size_t size = 0.5 * hexaValues[0].size() ;
		
		for( size_t i = 0; i < hexaValues.size(); i++ )
		{
			Hexa value;

			if( i > 0 )
			{
				_file->goBack( size );
			}

			//COMMON_COUT( "read data " << size );
			char buffer[ size ];
			if( ! _file->readData( buffer, size ) )
				return optional;

			Translator<Hexa> tr;
			value = tr.translate( buffer, size );

			if( hexaValues[i] ==  value.value )
			{
				isValid = true;
				message += hexaValues[i];
			}
			// COMMON_COUT_VAR2( hexaValues[i], value.value );
		}
	}

	if( typeValue != "" )
	{
		message += kType + " => " + typeValue;

		uint8  uint8Val  = 0;
		int8   int8Val   = 0;
		uint16 uint16Val = 0;
		int16  int16Val  = 0;
		uint32 uint32Val = 0;
		int32  int32Val  = 0;

		float  floatVal  = 0;
		double doubleVal = 0;
		
		bool validUInt8  = isValidInt<uint8> ( _file, message, typeValue, endianValue, subSpec, uint8Val );
		bool validInt8   = isValidInt<int8>  ( _file, message, typeValue, endianValue, subSpec, int8Val );
		bool validUInt16 = isValidInt<uint16>( _file, message, typeValue, endianValue, subSpec, uint16Val );
		bool validInt16  = isValidInt<int16> ( _file, message, typeValue, endianValue, subSpec, int16Val );
		bool validUInt32 = isValidInt<uint32>( _file, message, typeValue, endianValue, subSpec, uint32Val );
		bool validInt32  = isValidInt<int32> ( _file, message, typeValue, endianValue, subSpec, int32Val );

		bool validFloat   = isValidReal<float> ( _file, message, typeValue, endianValue, subSpec, floatVal );
		bool validDouble  = isValidReal<double>( _file, message, typeValue, endianValue, subSpec, doubleVal );

		bool validData = false;
		
		if( validUInt8 )
		{
			_headerElements[ id ] = uint8Val;
			// COMMON_COUT( "add id : " << id << " = " << _headerElements[ id ] );
		}
		if( validInt8 )
		{
			_headerElements[ id ] = int8Val;
		}
		if( validUInt16 )
		{
			_headerElements[ id ] = uint16Val;
		}
		if( validInt16 )
		{
			_headerElements[ id ] = int16Val;
		}
		if( validUInt32 )
		{
			_headerElements[ id ] = uint32Val;
		}
		if( validInt32 )
		{
			_headerElements[ id ] = int32Val;
		}

		if( validFloat )
		{
			_headerElements[ id ] = floatVal;
		}
		if( validDouble )
		{
			_headerElements[ id ] = doubleVal;
		}
		
		
		if( typeValue == "data" )
		{
			size_t size = 0;
			
			ExpressionParser ep = ExpressionParser();
			ep.setVariables( _headerElements );

			if( count != "" )
			{
				size = ep.parseExpression<size_t>( count );
			}
			
			message += " ( size = " + getPrintable( size ) + " )";
			
			char buffer[ size ];
			if( ! _file->readData( buffer, size ) )
			{
				return optional;
			}
			
			validData = true;
		}
		
		isValid = validUInt8 | validInt8 | validUInt16 | validInt16 | validUInt32 | validInt32 | validFloat | validDouble | validData;
	}
	
	COMMON_COUT( ( isValid ? common::details::kColorGreen : common::details::kColorRed ) << "\t" << std::left << std::setw(40) << ( label + " - " + id ) << "\t" << common::details::kColorStd << message );
	return isValid;
}
