#ifndef _EXTRACTOR_SPECS_NUMBER_SPECIFICATION_HPP_
#define _EXTRACTOR_SPECS_NUMBER_SPECIFICATION_HPP_

template< typename NumberType >
std::string getStringForType();

template<typename NumberType >
NumberType getValueFromString( const std::string& string );

template<typename NumberType >
inline bool isEqual( const NumberType& val1, const NumberType& val2 );

template< typename NumberType >
bool getRange( SubSpec& subSpec, const NumberType value );

template< typename NumberType >
bool isValidNumber( File* _file, std::string& message, const std::string& type, const bool isBigEndian, SubSpec& subSpec, NumberType& value );

#include "NumbersSpecification.tcc"

#endif
