#include "Comparator.hpp"

#include <iostream>
#include <Common/log.hpp>
#include <Common/common.hpp>

#include <BasicElement/Element.hpp>
#include <ElementChecker/Checker/Checker.hpp>

#include <SpecReader/Specification.hpp>
#include <FileReader/FileReader.hpp>
#include <Report/Report.hpp>

extern common::ELogLevel common::level;

namespace comparator
{

Comparator::Comparator()
{
}

void Comparator::check( spec_reader::Specification& spec, file_reader::FileReader& file, report_generator::Report& report )
{
	ShPtrSpecNode node    = spec.getFirstNode();
	ShPtrElement element  = nullptr;
	ShPtrElement parent   = nullptr;
	ShPtrElement previous = nullptr;
	element_checker::Checker checker;
	size_t size = 0;
	bool end = false;

	while( ! end )
	{
		// LOG_FATAL( node->getId() );
		previous = element;
		
		if( previous != nullptr && previous->_status == eStatusSkip )
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

		if( size == 0 )
		{
			getWord( element, file, buffer );
			if( buffer.empty() )
				element->_warning.push_back( "[comparator] Null data size " );
		}
		else
		{
			if( ! file.readData( buffer, size ) )
				throw std::runtime_error( "[comparator] End of file, cannot read data" );
		}
		element->set( buffer );

		checker.check( element );
		LOG_TRACE( "[comparator] checked: " << element->_id << " = " << element->_dispValue << " (" << statusMap.at( element->_status ) << ") @ " << element << " << Previous: " << previous << " << Parent: " );
		// LOG_COLOR( common::details::kColorMagenta, "[comparator] checked: " << element->_id << " = " << element->_dispValue << " (" << statusMap.at( element->_status ) << ") @ " << element << " << Previous: " << previous << " << Parent: " << parent << std::endl );

		updateParentSize( element );

		checkGroupSize( element, file );

		report.add( element );

		parent = getNextParent( element, node );

		LOG_TRACE( "[comparator] next parent: " << ( ( parent == nullptr ) ? "null" : "id = " + parent->_id ) );
		report.print( element, file.getPosition() - size );

		node = element->next();
		end = ( node == nullptr );
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

void Comparator::getWord( const ShPtrElement element, file_reader::FileReader& file, std::vector<char>& word )
{
	if( element->_type != eTypeAscii )
		return;

	std::vector<char> buff;

	while( 1 )
	{
		if( ! file.readData( buff, 1 ) )
			throw std::runtime_error( "[comparator] End of file, cannot read data" );
		
		word.push_back( buff.at( 0 ) );

		if( element->_endChar == buff.at( 0 ) )
		{
			LOG_TRACE( "[comparator] " << element->_id << " is a " << word.size() << "-char word" );
			return;
		}
	}
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
