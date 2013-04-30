#ifndef _EXTRACTOR_SPECS_CONVERTERS_TOOLS_HPP_
#define _EXTRACTOR_SPECS_CONVERTERS_TOOLS_HPP_

#include <extractor/inputFile/Translator.hpp>

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

#endif
