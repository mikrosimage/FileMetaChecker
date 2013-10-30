#include "Comparator.hpp"

#include <iostream>

#include <BasicElement/Element.hpp>

#include <SpecReader/Specification.hpp>
#include <FileReader/FileReader.hpp>
#include <ReportGenerator/Report.hpp>

Comparator::Comparator()
{
}
	
std::shared_ptr< basic_element::Element > Comparator::getElement( const std::shared_ptr< spec_reader::SpecNode > node, const std::shared_ptr< basic_element::Element > previous )
{
	// switch( node->getType() )
	// {
	// 	case eTypeData:
	// 	{
	// 		std::shared_ptr< basic_element::Data > d( new basic_element::Data( node, previous ) );
	// 		return static_cast< std::shared_ptr< basic_element::Element > >( d );
	// 	}
	// 	case eTypeNumber:
	// 	{
	// 		std::shared_ptr< basic_element::Number > n( new basic_element::Number( node, previous ) );
	// 		return static_cast< std::shared_ptr< basic_element::Element > >( n );
	// 	}
	// 	default: break;
	// }
	return NULL;
}
	
void Comparator::check( spec_reader::Specification& spec, file_reader::FileReader& reader, report_generator::Report& report )
{
	std::cout << "check start" << std::endl;
	
	// std::shared_ptr< spec_reader::SpecNode > s = NULL;
	// std::shared_ptr< basic_element::Number > prev;
	// std::shared_ptr< basic_element::Element > e = getElement( spec.getFirstNode( ), prev );		// get first element

	// while( ( s = e->next() ) != NULL )	// if end of specification : stop
	// {
	// 	e = getElement( s, e );			// get an element
	// 	// e->check();						// check it

	// 	if( e->getStatus() == eStatusInvalidButOptional ||
	// 		e->getStatus() == eStatusInvalidButSkip )
	// 	{
	// 		std::cout << "go back in raw file" << std::endl;
	// 		continue;
	// 	}

	// 	report.addElement( e );			// add the element to report

	// 	if( e->getUniqueId() > 20 )		// @todelete
	// 		break;
	// }
	
	std::cout << "check end" << std::endl;
}

