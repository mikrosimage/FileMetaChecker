#include "NodeSpecification.hpp"

#include <extractor/inputFile/File.hpp>
#include <extractor/inputFile/Translator.hpp>

#include <common/global.hpp>
#include <iomanip>

const std::string kId    = "id";
const std::string kLabel = "label";

const std::string kAscii = "ascii";
const std::string kHexa  = "hexa";
const std::string kType  = "type";

NodeSpecification::NodeSpecification( File* file, SubSpec& subSpec )
	: _file ( file )
	, _subSpec ( subSpec )
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


template< typename IntType >
bool isValidInt( File* _file, std::string& message, const std::string& type, const bool isBigEndian )
{
	if( type == getStringForType<IntType>() )
	{
		IntType value;
		size_t size = sizeof( IntType );
		char buffer[ size ];
		_file->readData( buffer, size );

		Translator<IntType> tr;
		value = tr.translate( buffer, size, isBigEndian );

		message += " = " + getPrintable( value );
		return true;
	}
	return false;
}

bool NodeSpecification::isValid()
{
	bool isValid = false;
	std::string message;
	std::string id = _subSpec.second.get< std::string >( kId );
	std::string label = _subSpec.second.get< std::string >( kLabel, "" );
	std::string asciiValue = _subSpec.second.get< std::string >( kAscii, "" );
	std::string hexaValue = _subSpec.second.get< std::string >( kHexa, "" );
	std::string type = _subSpec.second.get< std::string >( "type", "" );
	bool endianValue = ( _subSpec.second.get<std::string>( "endian", "big" ) == "big" );

	if( asciiValue != "" )
	{
		message += kAscii + " => " + asciiValue;
		size_t size = asciiValue.size();
		Ascii value;

		//COMMON_COUT( "read data " << size );
		char buffer[ size ];
		_file->readData( buffer, size );

		Translator<Ascii> tr;
		value = tr.translate( buffer, size );

		isValid = ( asciiValue ==  value.value );

		//COMMON_COUT_VAR2( asciiValue, value.value );
	}

	if( hexaValue != "" )
	{
		message += kHexa + " => " + hexaValue;
		size_t size = 0.5 * hexaValue.size() ;
		Hexa value;

		//COMMON_COUT( "read data " << size );
		char buffer[ size ];
		_file->readData( buffer, size );

		Translator<Hexa> tr;
		value = tr.translate( buffer, size );

		isValid = ( hexaValue ==  value.value );

		//COMMON_COUT_VAR2( hexaValue, value.value );
	}

	std::string typeValue = _subSpec.second.get< std::string >( kType, "" );
	if( typeValue != "" )
	{
		message += kType + " => " + typeValue;

		bool validUInt8  = isValidInt<uint8> ( _file, message, type, endianValue );
		bool validInt8   = isValidInt<int8>  ( _file, message, type, endianValue );
		bool validUInt16 = isValidInt<uint16>( _file, message, type, endianValue );
		bool validInt16  = isValidInt<int16> ( _file, message, type, endianValue );
		bool validUInt32 = isValidInt<uint32>( _file, message, type, endianValue );
		bool validInt32  = isValidInt<int32> ( _file, message, type, endianValue );

		isValid = validUInt8 | validInt8 | validUInt16 | validInt16 | validUInt32 | validInt32;

	}




	COMMON_COUT( ( isValid ? common::details::kColorGreen : common::details::kColorRed ) << "\t" << std::left << std::setw(40) << ( label + " - " + id ) << "\t" << common::details::kColorStd << message );
	return isValid;
}
