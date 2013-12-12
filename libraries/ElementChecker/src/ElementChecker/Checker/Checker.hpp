#ifndef _ELEMENT_CHECKER_CHECKER_HPP_
#define _ELEMENT_CHECKER_CHECKER_HPP_

#include <Common/common.hpp>
#include <BasicElement/Element.hpp>

#include <iostream>
#include <memory>

namespace element_checker
{
class ExpressionParser;

/**
 *  Basic element Checker. Compare raw data to expected values, and set Elements status.
 */
class Checker
{
	typedef std::shared_ptr< basic_element::Element > ShPtrElement;
	
public:
	/**
	 * Checker's constructor.
	 */
	Checker();

	/**
	 * Check an Element.
	 * @param element Element reference to be checked.
	 */
	void   check  ( const ShPtrElement element );

	/**
	 * Get the Element size.
	 * @param element Element reference.
	 * @return        Element's size.
	 */
	size_t getSize( const ShPtrElement element );
private:
	void setParentGroupSize       ( const ShPtrElement element );
	bool continueRepetition       ( const ShPtrElement element );
	bool isIterationValid         ( const ShPtrElement element, std::string& errorMessage );
	bool isRequirementValid       ( const ShPtrElement element );
	void checkLastUnorderedElement( const ShPtrElement element );

private:
	ExpressionParser* _exprParser;
};

}

#endif
