#include "Comparator.hpp"

#include <iostream>

#include <BasicElement/Element.hpp>
#include <BasicElement/SubElements/Data.hpp>
#include <BasicElement/SubElements/Number.hpp>

#include <SpecReader/Specification.hpp>
#include <FileReader/FileReader.hpp>
#include <ReportGenerator/Report.hpp>

Comparator::Comparator()
{
}
	
std::shared_ptr< basic_element::Element > Comparator::getElement( const spec_reader::SpecNode* node, const std::shared_ptr< basic_element::Element > previous )
{
	switch( node->getType() )
	{
		case eTypeData:
		{
			std::shared_ptr< basic_element::Data > d( new basic_element::Data( node, previous ) );
			return static_cast< std::shared_ptr< basic_element::Element > >( d );
			break;
		}
		case eTypeNumber:
		{
			std::shared_ptr< basic_element::Number > n( new basic_element::Number( node, previous ) );
			return static_cast< std::shared_ptr< basic_element::Element > >( n );
			break;
		}
	}
	return NULL;
}
	
void Comparator::check( spec_reader::Specification& spec, file_reader::FileReader& reader, report_generator::Report& report )
{
	std::cout << "check start" << std::endl;
	
	const spec_reader::SpecNode* s = NULL;
	std::shared_ptr< basic_element::Number > wElem;
	std::shared_ptr< basic_element::Element > e = getElement( spec.getFirstNode( ), wElem );

	while( ( s = e->next() ) != NULL )
	{
		e = getElement( s, e );
		e->check();

		if( e->getStatus() == eStatusInvalidButOptional ||
			e->getStatus() == eStatusInvalidButSkip )
		{
			std::cout << "go back in raw file" << std::endl;
			continue;
		}

		report.addElement( e );

		if( e->getIndex() > 20 )
			break;
	}
	
	std::cout << "check end" << std::endl;
}

