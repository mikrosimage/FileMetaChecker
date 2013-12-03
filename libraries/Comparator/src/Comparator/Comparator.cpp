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
	ShPtrSpecNode node = spec.getFirstNode();
	ShPtrElement element( new basic_element::Element( node ) );

	element_checker::Checker checker;
	size_t size = checker.getSize( element );

	char buffer1[ size ];
	if( ! file.readData( buffer1, size ) )
		throw std::runtime_error( "[comparator] End of file, cannot read data" );
	element->set( buffer1, size );

	checker.check( element );
	LOG_TRACE( "[comparator] checked:" << element->_id << " = " << element->_dispValue << " (" << statusMap.at( element->_status ) << ")" );

	std::shared_ptr< basic_element::Element > parent;
	parent = getNextParent( element, node );
	report.add( element );

	while( ( node = element->next() ) != nullptr && ! file.isEndOfFile() )
	{
		ShPtrElement previous = element;
		switch( element->_status )
		{
			case eStatusInvalidButOptional  :
			case eStatusInvalidButSkip      :
			case eStatusInvalidForIteration :
			case eStatusSkip :
			{
				LOG_TRACE( "[comparator] Go back in file (" << size << " bytes)" );
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
			LOG_TRACE( "[comparator] Critical remaining file data size: " << size << "/" << file.getLength() - file.getPosition() );
			size = file.getLength() - file.getPosition();
		}

		char buffer[ size ];
		if( ! file.readData( buffer, size ) )
			throw std::runtime_error( "[comparator] End of file, cannot read data" );
		element->set( buffer, size );

		checker.check( element );
		LOG_TRACE( "[comparator] checked: " << element->_id << " = " << element->_dispValue << " (" << statusMap.at( element->_status ) << ") @ " << element << " << Previous: " << previous << " << Parent: " << parent );

		checkGroupSize( element, file );

		report.add( element );
		report.update( parent );
		parent = getNextParent( element, node );

		if( parent != nullptr )
		{
			LOG_TRACE( "[comparator] next parent: id = " << parent->_id );
		}
		else
			LOG_TRACE( "[comparator] next parent: null" );
		report.print( element );
	}

	if( ! file.isEndOfFile() )
		LOG_WARNING( "Did not reach the end of file, remaining " << file.getLength() - file.getPosition() << " bytes." );
}

bool Comparator::isInUnorderedGroup( const std::shared_ptr< basic_element::Element > element )
{
	ShPtrElement parent = element->getParent();
	while( parent != nullptr )
	{
		if( ! parent->_isOrdered )
			return true;
	}
	return false;
}

Comparator::ShPtrElement Comparator::getNextParent( const ShPtrElement element, const ShPtrSpecNode node )
{
	ShPtrElement parent = element->getParent();
	bool isLastInGroup = ( node->next() == nullptr && ( parent == nullptr || ( parent->_isOrdered || ( ! parent->_isOrdered && element->_status == eStatusInvalidButSkip ) ) ) );
	bool isNotSkipped = ( element->_status != eStatusInvalidButOptional && element->_status != eStatusInvalidButSkip && element->_status != eStatusInvalidForIteration );

	if( element->_isGroup && ! element->_checkedGroup && isNotSkipped )
		return element;
	
	if( isLastInGroup && element->getParent() != nullptr )
	{
		parent = element->getParent();
		while( parent->getSpecNode()->next() == nullptr && parent->getParent() != nullptr )
		{
			parent = parent->getParent();
		}
		return parent->getParent();
	}
	return element->getParent();
}

void Comparator::checkGroupSize( const ShPtrElement element, file_reader::FileReader& file )
{
	ShPtrElement parent   = element->getParent();
	ShPtrElement previous = element->getPrevious();
	
	if( parent != nullptr && previous != nullptr && element->getSpecNode()->next() == nullptr && ! parent->_groupSizeExpr.empty() )
	{
		size_t groupSize = element->_size;

		while( previous != nullptr || ( previous != nullptr && previous->_id != parent->_id ) )
		{
			LOG_TRACE( "[comparator] \tGroupSize Loop: from " << element->_id << " to " << previous->_id << " ?= parent " << parent->_id << " >> " << groupSize << "/" << parent->_groupSize << " (" << parent->_groupSizeExpr << ")");
			if( previous->_id == parent->_id )
				break;
			groupSize += previous->_size;
			previous = previous->getPrevious();
		}

		int sizeDiff = parent->_groupSize - groupSize;
		if( sizeDiff != 0 )
		{
			if( sizeDiff > 0 )
			{
				std::stringstream errorMessage;
				errorMessage << "[comparator] Group size difference: " << sizeDiff << " missing bytes ";
				parent->_error = errorMessage.str();
				throw std::runtime_error( errorMessage.str() );
			}
			if( sizeDiff < 0 )
			{
				std::stringstream warningMessage;
				warningMessage << "[comparator] Group size difference: " << abs( sizeDiff ) << " unexpected bytes ";
				parent->_warning += warningMessage.str();
				file.goForward( abs( sizeDiff ) );
				LOG_WARNING( warningMessage.str() << ": go forward..." );
			}
		}
	}
	return;
}

}
