#ifndef _BASIC_ELEMENT_TOOLS_ELEMENT_TRANSLATOR_HPP_
#define _BASIC_ELEMENT_TOOLS_ELEMENT_TRANSLATOR_HPP_

#include <Element.hpp>
#include <Number/Number.hpp>
#include <Data/Data.hpp>

namespace be = basic_element;
namespace dbe = basic_element::data_element;
namespace nbe = basic_element::number_element;

namespace basic_element
{
namespace translator_tool
{

template< typename InputType, typename OutputType >
class ElementTranslator
{
public:
	ElementTranslator()
	{
	}

	~ElementTranslator()
	{
	}

	OutputType translate( const InputType& element );
};

}
}

#include "ElementTranslator.tcc"

#endif
