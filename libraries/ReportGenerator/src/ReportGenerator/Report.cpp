#include "Report.hpp"

#include <iostream>

#include <BasicElement/Element.hpp>

#include <Common/color.hpp>

namespace report_generator 
{

void Report::init( const std::vector< std::shared_ptr< basic_element::Element > >& elementList )
{
	_elementList = elementList;
}

void Report::print()
{
	std::cout << std::setfill( '-' ) << std::setw( 231 ) << " " << std::endl;
	std::cout << "|" << std::setfill( ' ' ) << std::setw( 16 ) << "" << "Elements"  << std::setfill( ' ' ) << std::setw( 16 ) << "|"
					 << std::setfill( ' ' ) << std::setw( 16 ) << "" << "Value"     << std::setfill( ' ' ) << std::setw( 16 ) << "|"
					 << std::setfill( ' ' ) << std::setw(  2 ) << "" << "Iteration" << std::setfill( ' ' ) << std::setw(  2 ) << "|"
					 << std::setfill( ' ' ) << std::setw( 11 ) << "" << "Status"    << std::setfill( ' ' ) << std::setw( 11 ) << "|"
					 << std::setfill( ' ' ) << std::setw( 16 ) << "" << "Error"     << std::setfill( ' ' ) << std::setw( 17 ) << "|"
					 << std::setfill( ' ' ) << std::setw( 16 ) << "" << "Warning"   << std::setfill( ' ' ) << std::setw( 23 ) << "|"
					 << std::setfill( ' ' ) << std::setw( 10 ) << "" << "Comment"   << std::setfill( ' ' ) << std::setw( 10 ) << "|" << std::endl;
	std::cout << std::setfill( '-' ) << std::setw( 231 ) << " " << std::endl;

	for( std::shared_ptr< basic_element::Element > element : _elementList )
	{
		switch( element->_status )
		{
			case eStatusSkip               :
			case eStatusInvalidButSkip     :
			case eStatusInvalidButOptional :
			case eStatusNotChecked         :
				break;

			case eStatusValid    :
			case eStatusPassOver :
			{
				print( element, common::details::kColorGreen );
				break;
			}

			case eStatusInvalid                  :
			case eStatusInvalidForUnordered      :
			case eStatusInvalidForIteration      :
			case eStatusInvalidGroupForIteration :
			{
				print( element, common::details::kColorRed );
				break;
			}
		}
	}
}

void Report::print( const std::shared_ptr< basic_element::Element > element, const std::string& dispColor )
{
	size_t count = 0;
	std::shared_ptr< basic_element::Element > elemCopy( element );
	while( 1 )
	{
		count++;
		std::weak_ptr< basic_element::Element > parent = elemCopy->getParent();
		if( parent.use_count() == 0 )
			break;
		elemCopy = parent.lock();
	}

	std::string comment = "";
	if( element->_isOptional )          comment += "Optional ";
	if( element->_isGroup )             comment += "Group ";
	if( ! element->_repetExpr.empty() ) comment += "Repeated ";
	if( ! element->_isOrdered )         comment += "Unordered ";

	// element->_error = "error : this is an error !";

	std::cout
		<< dispColor
		<< std::setfill( ' ' ) << std::setw( 5 * ( count - 1 ) + 1 ) << ""
		<< element->_id
		<< std::setfill( ' ' ) << std::setw( 5*( 9 - count ) - element->_id.size() ) << ""
		<< element->_dispValue << " " << element->_mapValue
		<< std::setfill( ' ' ) << std::setw( 47 - ( element->_dispValue.size() + element->_mapValue.size() ) )
		<< element->_iteration
		<< std::setfill( ' ' ) << std::setw( 3 ) << ""
		<< statusMap.at( element->_status )
		<< common::details::kColorRed
		<< std::setfill( ' ' ) << std::setw( 28 - statusMap.at( element->_status ).size() ) << ""
		<< element->_error
		<< common::details::kColorYellow
		<< std::setfill( ' ' ) << std::setw( 38 - element->_error.size() ) << ""
		<< element->_warning
		<< common::details::kColorStd
		<< std::setfill( ' ' ) << std::setw( 46 - element->_warning.size() ) << ""
		<< ( comment.empty() ? "- " : comment )
		<< std::endl;
}

}
