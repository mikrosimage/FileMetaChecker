#include "Comparator.hpp"

#include <iostream>
#include <Common/log.hpp>

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
	size_t fileLength = file.getLength();
	bool   end = false;

	while( ! end )
	{
		previous = element;
		element = std::make_shared< basic_element::Element >( node, previous, parent );
		size = checker.getSize( element );

		size_t remainingData = fileLength - file.getPosition();

		if( size > remainingData && ( isInUnorderedGroup( element ) || element->_isOptional ) )
		{
			LOG_TRACE( "[comparator] Critical remaining file data size: " << size << "/" << remainingData );
			if( ( size = remainingData ) == 0 )
				break;
		}

		std::vector< char > buffer;

		if( size == 0 )
		{
			if( ( element->_type != eTypeUnknown )
				&& ( ! file.readWord( buffer, element->_endChar ) ) )
				throw std::runtime_error( "[comparator] End of file, cannot read word" );

			if( buffer.empty() )
				element->_warning.push_back( "[comparator] Null data size" );
		}
		else
		{
			if( ( element->_type == eTypeRaw ) && ( element->_displayType == eDisplayTypeDefault ) && ! element->_isDetailed )
			{
				file.goForward( size );
			}
			else
			{
				if( ! file.readData( buffer, size ) )
					throw std::runtime_error( "[comparator] End of file, cannot read data" );
			}
		}
		
		element->set( buffer );

		checker.check( element );

		for( char c : element->_data )
			LOG_TRACE( element->_id << ": " << c );

		if( element->_isDetailed )
		{
			std::string str( &element->_data[0], element->_data.size() );
			std::stringbuf buff( str );
			
			file_reader::FileReader elementData( &buff );
			report_generator::Report detailsReport;
			spec_reader::Specification detailsNodes;
			detailsNodes.setFromNode( element->getSpecNode(), kDetails );

			Comparator detailsComp;
			
			detailsComp.check( detailsNodes, elementData, detailsReport );

			for( ShPtrElement elem : detailsReport.getElementList() )
				report.add( elem );
		}

		size_t fileMoveLength = checker.checkGroupSize( element );
		if( fileMoveLength > 0 )
		{
			LOG_WARNING( "go forward... (" << fileMoveLength << " bytes)" );
			file.goForward( fileMoveLength );
		}

		report.add( element );
		file.goBack( checker.getBackOffset( element ) );
		parent = checker.getNextParent( element );
		
		if( common::level <= common::eLogInfo )
			displayElement( element, file );

		node = element->next();
		end = ( node == nullptr );
	}

	if( ! file.isEndOfFile() )
		LOG_WARNING( "Did not reach the end of file, remaining " << fileLength - file.getPosition() << " bytes." );
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

void  Comparator::displayElement( const ShPtrElement element, file_reader::FileReader& file )
{
	std::string color;
	switch( element->_status )
	{
		case eStatusUnknown : color = common::details::kColorBlue;  break;
		case eStatusValid   : color = common::details::kColorGreen; break;
		case eStatusInvalid : color = common::details::kColorRed;   break;
		case eStatusSkip    : color = common::details::kColorCyan;  break;
	}

	std::string rawData;
	if( element->_data.size() < 30 )
		rawData.assign( &element->_data[0], element->_data.size() );

	ShPtrElement parent = element->getParent();
	size_t fileOffset = file.getPosition() - element->_data.size();

	LOG_COLOR( color, "\t[comparator] @ " << std::setfill( ' ' ) << std::setw( 11 ) << fileOffset << " | " );
	LOG_COLOR( color, " i. " << std::setfill( ' ' ) << std::setw( 11 ) << element->_iteration << " | " );
	LOG_COLOR( color, std::setfill( ' ' ) << std::setw( 20 ) << element->_id << ( ( element->_dispValue.size() <= 30 )? ": " + element->_dispValue + " = " : "" ) );
	LOG_COLOR( color, rawData << " ( " << statusMap.at( element->_status ) << " )" );
	LOG_COLOR( color, ", parent: " << ( ( parent == nullptr )? "null": parent->_id + " (" + statusMap.at( parent->_status ) + ")" ) << std::endl );
}


}
