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

	std::vector< char > buffer1;
	if( ! file.readData( buffer1, size ) )
		throw std::runtime_error( "[comparator] End of file, cannot read data" );
	element->set( buffer1, size );

	checker.check( element );
	LOG_TRACE( "[comparator] checked:" << element->_id << " = " << element->_dispValue << " (" << statusMap.at( element->_status ) << ")" );

	std::shared_ptr< basic_element::Element > parent;
	parent = getNextParent( element, node );
	report.add( element );
	report.print( element, file.getPosition() - element->_data.size() );

	while( ( node = element->next() ) != nullptr )
	{
		// LOG_FATAL( node->getId() );
		ShPtrElement previous = element;
		
		if( previous->_status == eStatusSkip )
		{
			LOG_TRACE( "[comparator] Go back in file (" << size << " bytes)" );
			file.goBack( size );
			previous = element->getPrevious();
		}

		element = std::make_shared< basic_element::Element >( node, previous, parent );
		size = checker.getSize( element );

		if( size > ( file.getLength() - file.getPosition() ) && ( isInUnorderedGroup( element ) || element->_isOptional ) )
		{
			LOG_TRACE( "[comparator] Critical remaining file data size: " << size << "/" << file.getLength() - file.getPosition() );
			size = file.getLength() - file.getPosition();
			if( size == 0 )
				break;
		}

		std::vector< char > buffer;
		if( ! file.readData( buffer, size ) )
			throw std::runtime_error( "[comparator] End of file, cannot read data" );
		element->set( buffer, size );

		checker.check( element );
		LOG_TRACE( "[comparator] checked: " << element->_id << " = " << element->_dispValue << " (" << statusMap.at( element->_status ) << ") @ " << element << " << Previous: " << previous << " << Parent: " );
		// LOG_COLOR( common::details::kColorMagenta, "[comparator] checked: " << element->_id << " = " << element->_dispValue << " (" << statusMap.at( element->_status ) << ") @ " << element << " << Previous: " << previous << " << Parent: " << parent << std::endl );

		updateParentSize( element );

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
		report.print( element, file.getPosition() - size );
	}

	if( ! file.isEndOfFile() )
		LOG_WARNING( "Did not reach the end of file, remaining " << file.getLength() - file.getPosition() << " bytes." );
}

bool Comparator::isInUnorderedGroup( const ShPtrElement element )
{
	ShPtrElement parent = element->getParent();
	while( parent != nullptr )
	{
		if( ! parent->_isOrdered )
			return true;
		parent = parent->getParent();
	}
	return false;
}

void Comparator::updateParentSize( const ShPtrElement element )
{
	ShPtrElement parent = element->getParent();
	while( element->_status != eStatusSkip && parent != nullptr )
	{
		parent->_childrenSize += element->_data.size();
		// LOG_COLOR( common::details::kColorBlue, "[comparator] " << parent->_id << "'s children size : " << parent->_childrenSize << std::endl );
		parent = parent->getParent();
	}
}

Comparator::ShPtrElement Comparator::getNextParent( const ShPtrElement element, const ShPtrSpecNode node )
{
	ShPtrElement parent = element->getParent();
	bool isLastInGroup = ( node->next() == nullptr && ( parent == nullptr || ( parent->_isOrdered || ( ! parent->_isOrdered && element->_status == eStatusSkip ) ) ) );

	if( element->_isGroup && ! element->_checkedGroup && element->_status != eStatusSkip )
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
	try 
	{
		if( element->getSpecNode()->next() != nullptr || element->_isGroup )
			return;

		ShPtrElement parent = element->getParent();
		while(  parent != nullptr )
		{
			if( parent->_groupSizeExpr.empty() )
			{
				parent = parent->getParent();
				continue;
			}

			int sizeDiff = parent->_childrenSize - parent->_specGroupSize;
			LOG_TRACE( "[comparator] group size difference (" << parent->_id << ") : " << parent->_specGroupSize << " - " << parent->_childrenSize << " = "<< sizeDiff );
			if( sizeDiff != 0 )
			{
				if( sizeDiff > 0 )
				{
					std::stringstream errorMessage;
					errorMessage << "[comparator] Group size difference: " << sizeDiff << " missing bytes ";
					parent->_error.push_back( errorMessage.str() );
					throw std::runtime_error( errorMessage.str() );
				}
				if( sizeDiff < 0 )
				{
					std::stringstream warningMessage;
					warningMessage << "[comparator] Group size difference: " << abs( sizeDiff ) << " unexpected bytes ";
					parent->_warning.push_back( warningMessage.str() );
					file.goForward( abs( sizeDiff ) );
					LOG_WARNING( warningMessage.str() << ": go forward..." );
				}
			}
			parent = parent->getParent();
		}
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

}
