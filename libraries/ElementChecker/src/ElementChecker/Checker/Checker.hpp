#ifndef _ELEMENT_CHECKER_CHECKER_HPP_
#define _ELEMENT_CHECKER_CHECKER_HPP_

#include <Common/common.hpp>
#include <BasicElement/Element.hpp>
#include <SpecReader/SpecNode.hpp>

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
	typedef std::shared_ptr< spec_reader::SpecNode >  ShPtrSpecNode;
	
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
	
	/**
	 * Get the following Element's parent.
	 * @param element Current Element reference.
	 * @return        Next Element's parent reference.
	 */
	ShPtrElement getNextParent    ( const ShPtrElement element );

	/**
	 * Get the invalid Elements size to go back in the file.
	 * @param element Current Element reference.
	 * @return        Offset to go back.
	 */
	size_t getBackOffset          ( const ShPtrElement element );

	/**
	 * Check if the group size is valid.
	 * @param element Current Element reference.
	 * @return        Size difference.
	 */
	size_t checkGroupSize         ( const ShPtrElement element );

private:
	bool isRequirementValid       ( const ShPtrElement element );

	void checkRequirement         ( const ShPtrElement element, EStatus& status );
	void checkValue               ( const ShPtrElement element, EStatus& status );
	void checkSpecialCases        ( const ShPtrElement element, EStatus& status );
	void checkRepetition          ( const ShPtrElement element, EStatus& status );
	void checkLastUnorderedElement( const ShPtrElement element, EStatus& status );
	void updateParent             ( const ShPtrElement element );

	bool isLastInGroup            ( const ShPtrElement element );

	void addElementToParent       ( const ShPtrElement element );
	void setParentGroupSize       ( const ShPtrElement element );
	void updateParentSize         ( const ShPtrElement element );
	void updateParentStatus       ( const ShPtrElement element );

	bool continueRepetition       ( const ShPtrElement element );
	bool isIterationValid         ( const ShPtrElement element, std::string& errorMessage );

private:
	ExpressionParser* _exprParser;
};

}

#endif
