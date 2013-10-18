#ifndef _BASIC_ELEMENT_TRANSLATOR_TRANSLATOR_HPP_
#define _BASIC_ELEMENT_TRANSLATOR_TRANSLATOR_HPP_

#include <cstring>
#include <Common/Element.hpp>

template< typename NumberType >
union NumberData
{
	char       data[0];
	NumberType value;
};

class Translator
{
public:
	Translator( const basic_element::Element* elem )
		: ref( elem )
	{
	}

	template< typename OutType >
	OutType get();

private:
	const basic_element::Element* ref;
};

#include "Translator.tcc"

#endif
