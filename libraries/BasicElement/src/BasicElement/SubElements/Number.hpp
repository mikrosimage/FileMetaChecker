#ifndef _BASIC_ELEMENT_SUBELEMENTS_NUMBER_HPP_
#define _BASIC_ELEMENT_SUBELEMENTS_NUMBER_HPP_

#include <Common/Element.hpp>

namespace basic_element
{

typedef          char       int8;
typedef unsigned char      uint8;
typedef          short      int16;
typedef unsigned short     uint16;
typedef          int        int32;
typedef unsigned int       uint32;
typedef   signed long long  int64;
typedef unsigned long long uint64;

typedef long double  ieeeExtended;    // 80 bits IEEE Standard 754 floating point

class Number : public Element
{
	
public:
	Number( const spec_reader::SpecNode* node, const std::shared_ptr< Element > previous = std::shared_ptr< Element >() );

};

}

#endif