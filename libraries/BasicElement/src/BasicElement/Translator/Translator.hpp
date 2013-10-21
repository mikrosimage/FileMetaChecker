#ifndef _BASIC_ELEMENT_TRANSLATOR_TRANSLATOR_HPP_
#define _BASIC_ELEMENT_TRANSLATOR_TRANSLATOR_HPP_

#include <cstring>
#include <Common/Element.hpp>
#include <Common/common.hpp>
#include <vector>
#include <sstream> 

namespace basic_element
{

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
		: _ref( elem )
	{
	}

	template< typename OutType >
	OutType get();

	std::string get( const EDisplayType dispType );

	template< EDisplayType >
	void fillStream( std::ostringstream& osstr );

private:
	template< typename NumberType >
	std::vector< NumberType > convertToVector() const;

private:
	const basic_element::Element* _ref;
};
	
}

#include "Translator.tcc"
#include "TranslatorFillStream.tcc"
#include "TranslatorGetDisplayType.tcc"

#endif
