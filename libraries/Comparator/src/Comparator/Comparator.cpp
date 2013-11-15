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
	PtrSpecNode node = spec.getFirstNode();
	PtrElement element( new basic_element::Element( node ) );

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
		PtrElement previous = element;
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
		LOG_FATAL( "Size: " << size );
		char buffer[ size ];
		if( ! file.readData( buffer, size ) )
			throw std::runtime_error( "End of file, cannot read data" );
		element->set( buffer, size );

		checker.check( element );
		LOG_WARNING( "COMPARATOR : " << element->_id << " << Prev: " << previous << " - " << statusMap.at( element->_status ) << " | " << element->_dispValue );

		checkGroupSize( element, file );

		parent = getNextParent( element, node );
	}

	report.init( checker.getElementList() );
	// report.print();								// @todelete !
}

bool Comparator::isInUnorderedGroup( const std::shared_ptr< basic_element::Element > element )
{
	PtrElement parent = element->getParent();
	while( parent != nullptr )
	{
		if( ! parent->_isOrdered )
			return true;
	}
	return false;
}

Comparator::PtrElement Comparator::getNextParent( const PtrElement element, const PtrSpecNode node )
{
	PtrElement parent = element->getParent();
	bool isLastInGroup = ( node->next() == nullptr && ( parent == nullptr || ( parent->_isOrdered || ( ! parent->_isOrdered && element->_status == eStatusInvalidButSkip ) ) ) );

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


void Comparator::checkGroupSize( const PtrElement element, file_reader::FileReader& file )
{
	PtrElement parent   = element->getParent();
	PtrElement previous = element->getPrevious();
	
	if( parent != nullptr && previous != nullptr && element->getSpecNode()->next() == nullptr && ! parent->_groupSizeExpr.empty() )
	{
		size_t groupSize = element->_size;

		while( previous != nullptr || previous->_id != parent->_id )
		{
			groupSize += previous->_size;
			if( previous->_id == parent->_id )
				break;
			previous = previous->getPrevious();
		}

		int sizeDiff = parent->_groupSize - groupSize;
		if( sizeDiff != 0 )
		{
			if( sizeDiff > 0 )
			{
				std::stringstream errorMessage;
				errorMessage << "Group size difference: " << sizeDiff << " missing bytes - ";
				parent->_error = errorMessage.str();
				throw std::runtime_error( errorMessage.str() );
			}
			if( sizeDiff < 0 )
			{
				std::stringstream warningMessage;
				warningMessage << "Group size difference: " << abs( sizeDiff ) << " unexpected bytes - ";
				parent->_warning += warningMessage.str();
				file.goForward( abs( sizeDiff ) );
				LOG_WARNING( warningMessage.str() << "go forward..." );
			}
		}
	}
	return;
}

}
