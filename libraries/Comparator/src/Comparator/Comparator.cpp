#include "Comparator.hpp"

#include <iostream>
#include <Common/log.hpp>
#include <Common/common.hpp>

#include <BasicElement/Element.hpp>
#include <ElementChecker/Checker/Checker.hpp>

#include <SpecReader/Specification.hpp>
#include <FileReader/FileReader.hpp>
#include <ReportGenerator/Report.hpp>

namespace comparator
{

Comparator::Comparator()
{
}
	
void Comparator::check( spec_reader::Specification& spec, file_reader::FileReader& file, report_generator::Report& report )
{
	std::shared_ptr<  spec_reader::SpecNode > node = spec.getFirstNode();
	std::shared_ptr< basic_element::Element > element( new basic_element::Element( node ) );

	element_checker::Checker checker;
	size_t size = checker.getSize( element );

	char buffer1[ size ];
	if( ! file.readData( buffer1, size ) )
		throw std::runtime_error( "End of file, cannot read data" );
	element->set( buffer1, size );

	checker.check( element );
	
	std::shared_ptr< basic_element::Element > parent;
	if( element->_isGroup )
		parent = element;

	LOG_ERROR( element->_id << ": " << statusMap.at( element->_status ) << " | " << element->_dispValue );

	while( ( node = element->next() ) != nullptr )	// if end of specification : stop
	{
		std::shared_ptr< basic_element::Element > previous = element;

		switch( element->_status )
		{
			case eStatusInvalidButOptional  :
			case eStatusInvalidButSkip      :
			case eStatusInvalidForIteration :
			case eStatusSkip :
			{
				LOG_WARNING( "COMPARATOR : Go BACK in file" );
				file.goBack( size );
				previous = element->getPrevious();
				break;
			}
			default: break;
		}

		element = std::make_shared< basic_element::Element >( node, previous, parent );
		size = checker.getSize( element );

		if( size > ( file.getLength() - file.getPosition() ) && ( isInUnorderedGroup( element ) || element->_isOptional ) )
		{
			LOG_WARNING( "Critical remaining file data size: " << size << "/" << file.getLength() - file.getPosition() );
			size = file.getLength() - file.getPosition();
		}

		LOG_WARNING( "Size: " << size << "/" << file.getLength() - file.getPosition() );
		char buffer[ size ];
		if( ! file.readData( buffer, size ) )
			throw std::runtime_error( "End of file, cannot read data" );
		element->set( buffer, size );

		checker.check( element );
		LOG_WARNING( "COMPARATOR : " << element->_id << " << Prev: " << previous << " - " << statusMap.at( element->_status ) << " | " << element->_dispValue );

		parent = getNextParent( element, node );
	}

	report.init( checker.getElementList() );
	report.print();		// @todelete
}

bool Comparator::isInUnorderedGroup( const std::shared_ptr< basic_element::Element > element )
{
	std::shared_ptr< basic_element::Element > parent = element->getParent();
	while( parent != nullptr )
	{
		if( ! parent->_isOrdered )
			return true;
	}
	return false;
}

std::shared_ptr< basic_element::Element > Comparator::getNextParent( const std::shared_ptr< basic_element::Element > element,
		                                                             const std::shared_ptr< spec_reader::SpecNode  > node )
{
	std::shared_ptr< basic_element::Element > parent = element->getParent();
	bool isLastInGroup = ( node->next() == nullptr && ( parent->_isOrdered || ( ! parent->_isOrdered && element->_status == eStatusInvalidButSkip ) ) );

	if( element->_isGroup
	 && ! element->_checkedGroup
	 && element->_status != eStatusInvalidButOptional
	 && element->_status != eStatusInvalidButSkip
	 && element->_status != eStatusInvalidForIteration )
	{
		LOG_WARNING( "COMPARATOR::getNextParent -  CASE 1" );
		parent = element;
	}
	else if( isLastInGroup && element->getParent() != nullptr )
	{
		LOG_WARNING( "COMPARATOR::getNextParent -  CASE 2" );
		parent = element->getParent();
		while( parent->getSpecNode()->next() == nullptr && parent->getParent() != nullptr )
		{
			LOG_WARNING( "parent of  : " << parent->_id );
			parent = parent->getParent();
		}
		parent = parent->getParent();
	}
	else
	{
		LOG_WARNING( "COMPARATOR::getNextParent -  CASE 3" );
		parent = element->getParent();
	}
	return parent;
}

}


