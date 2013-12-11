#include "Report.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#include <BasicElement/Element.hpp>

#include <Common/color.hpp>
#include <Common/log.hpp>

#include <rapidxml/rapidxml_print.hpp>

namespace report_generator 
{

Report::Report( const std::vector< ShPtrElement >& elementList )
	: _verbosity( eReportDisplayComments )
{
	add( elementList );
}

void Report::setPrintVerbosity( const EReportDisplayLevel level )
{
	_verbosity = level;
}

void Report::add( const ShPtrElement element )
{
	_elementList.push_back( element );
}

void Report::add( const std::vector< ShPtrElement >& elementList )
{
	for( ShPtrElement element : elementList )
	{
		_elementList.push_back( element );
	}
}

Report::ShPtrElement Report::get( const std::string& elementId, const size_t& iteration )
{
	for( ShPtrElement element : _elementList )
	{
		if( element->_id == elementId && element->_iteration == iteration )
			return element;
	}
	return nullptr;
}


void Report::update( const ShPtrElement newElement )
{
	if( newElement == nullptr )
		return;
	for( ShPtrElement element : _elementList )
	{
		if( element->_uId == newElement->_uId )
		{
			element = newElement;
			return;
		}
	}
}

void Report::printHelper()
{
	if( _verbosity == eReportDisplayNone )
		return;
	LOG_COLOR( common::details::kColorBoldWhite, basic_element::Element::getLabelsLegend() );
	LOG_COLOR( common::details::kColorBoldWhite, basic_element::Element::getElementPropertiesLegend() );
}

void Report::print()
{
	if( _verbosity == eReportDisplayNone )
		return;
	size_t offset = 0;
	for( ShPtrElement element : _elementList )
	{
		print( element, offset );
		offset += element->_data.size();
	}
}

void Report::print( const ShPtrElement element, const size_t& fileOffset )
{
	if( ! isPrintable( element ) || _verbosity == eReportDisplayNone )
		return;
	
	if( _verbosity == eReportDisplayComments )
		LOG( element->getPropertiesFlag() << " | " );

	std::string color;
	switch( element->_status )
	{
		case eStatusValid    : color = common::details::kColorGreen; break;
		case eStatusInvalid  : color = common::details::kColorRed;   break;
		case eStatusUnknown  : color = common::details::kColorCyan;  break;
		default: return;
	}
	
	if( _verbosity >= eReportDisplayStatus )
	{
		LOG( "S: " );
		LOG_COLOR( color, tabulation( 14, statusMap.at( element->_status ) ) );
	}

	if( _verbosity >= eReportDisplayIteration )
		LOG( ".i: " << std::setfill( ' ' ) << std::setw( 4 ) << element->_iteration << " " );

	if( _verbosity >= eReportDisplayType )
		LOG( ".t: " << std::setfill( ' ' ) << std::setw( 12 ) << typeStringMap.at( element->_type ) << " " );

	if( _verbosity >= eReportDisplaySize )
		LOG( ".s: " << std::setfill( ' ' ) << std::setw( 12 ) << element->_data.size() << " " );

	if( _verbosity >= eReportDisplayOffset )
		LOG( ".@: " << std::setfill( ' ' ) << std::setw( 9 ) << fileOffset << " " );

	if( _verbosity >= eReportDisplayID )
	{
		LOG( "." );
		LOG_COLOR( color, tabulation( getDisplayOffset( element ) ) << element->_label );
	}

	if( _verbosity >= eReportDisplayValue && ( element->_type != eTypeRaw || element->_displayType != eDisplayTypeDefault ) && ! element->_dispValue.empty() )
	{
		LOG( " => v: " );
		LOG_COLOR( color, element->_dispValue );
		if( ! element->_mapValue.empty() )
			LOG_COLOR( color, " (" << element->_mapValue << ")" );
		if( element->_specGroupSize != 0 )
			LOG( " ( group size: " << element->_specGroupSize << " )" << std::endl );
	}

	if( ! element->_error.empty() )
	{
		LOG( " >> " );
		LOG_COLOR( common::details::kColorRed, "E: " );
		for( std::string e : element->_error )
			LOG_COLOR( common::details::kColorRed, e );
	}

	if( ! element->_warning.empty() )
	{
		LOG( " >> " );
		LOG_COLOR( common::details::kColorYellow, "W: " );
		for( std::string w : element->_warning )
			LOG_COLOR( common::details::kColorYellow, w );
	}

	LOG_ENDL();
}

size_t Report::getDisplayOffset( const ShPtrElement element )
{
	size_t tab = 0;
	std::shared_ptr< basic_element::Element > elemCopy( element );
	while( 1 )
	{
		tab++;
		std::weak_ptr< basic_element::Element > parent = elemCopy->getParent();
		if( parent.lock() == nullptr )
			break;
		elemCopy = parent.lock();
	}
	return tab;
}

std::string Report::tabulation( size_t tabNum )
{
	std::stringstream tab;
	tab << std::setfill( ' ' ) << std::setw( tabNum ) << "";
	return tab.str();
}

std::string Report::tabulation( size_t tabNum, const std::string& str )
{
	std::stringstream tab;
	tab << str << std::setfill( ' ' ) << std::setw( tabNum - str.size() ) << "";
	return tab.str();
}

bool Report::isPrintable( const ShPtrElement element )
{
	bool ret = false;
	switch( element->_status )
	{
		case eStatusUnknown  :
		case eStatusValid    :
		case eStatusInvalid  : ret = true;
		default: break;
	}
	return ret;
}

void addXmlNodeAttribute( rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const std::string& attrName, const std::string& attrValue )
{
	std::string name  = attrName;
	std::string value = attrValue;
	node->append_attribute( doc.allocate_attribute( name.c_str(), value.c_str() ) );
}

void addXmlNodeAttribute( rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const std::string& attrName, const std::vector< std::string >& attrValues )
{
	std::string name  = attrName;
	std::string value;
	for( std::string v : attrValues )
		value += v;
	node->append_attribute( doc.allocate_attribute( name.c_str(), value.c_str() ) );
}

void Report::writeXml( const std::string& filename )
{
	rapidxml::xml_document<> doc;

	rapidxml::xml_node<>* declaration = doc.allocate_node( rapidxml::node_declaration );
	declaration->append_attribute( doc.allocate_attribute( "version",  "1.0"   ) );
	declaration->append_attribute( doc.allocate_attribute( "encoding", "utf-8" ) );
	doc.append_node( declaration );
	
	rapidxml::xml_node<>* prevNode = nullptr;
	rapidxml::xml_node<>* node     = nullptr;

	for( std::shared_ptr< basic_element::Element > element : _elementList )
	{
		if( element->_status == eStatusSkip )
			continue;

		std::shared_ptr< basic_element::Element > parent   = element->getParent();
		std::shared_ptr< basic_element::Element > previous = element->getPrevious();


		// create xml node
		std::string elementStr = kElement;
		node = doc.allocate_node( rapidxml::node_element, elementStr.c_str() );

		addXmlNodeAttribute( doc, node, kId,          element->_id                      );
		addXmlNodeAttribute( doc, node, kLabel,       element->_label                   );
		addXmlNodeAttribute( doc, node, kType,        typeStringMap.at( element->_type) );
		
		std::stringstream sizeSstr;
		sizeSstr << element->_data.size();
		addXmlNodeAttribute( doc, node, kSize,        sizeSstr.str()                    );

		std::string value = "";
		if( element->_displayType != eDisplayTypeDefault || element->_type != eTypeRaw )
			value = element->_dispValue;
		addXmlNodeAttribute( doc, node, kValue,       value                             );

		addXmlNodeAttribute( doc, node, kMap,         element->_mapValue                );
		addXmlNodeAttribute( doc, node, kStatus,      statusMap.at( element->_status )  );

		std::string endianStr = kEndianLittle;
		if( element->_isBigEndian )
			endianStr = kEndianBig;
		addXmlNodeAttribute( doc, node, kEndian,      endianStr );

		std::string optionStr = kFalse;
		if( element->_isOptional )
			optionStr = kTrue;
		addXmlNodeAttribute( doc, node, kOptional,    optionStr );

		std::string orderedStr = kFalse;
		if( element->_isOrdered )
			orderedStr = kTrue;
		addXmlNodeAttribute( doc, node, kOrdered,     orderedStr );
		addXmlNodeAttribute( doc, node, kDisplayType, displayTypeStringMap.at( element->_displayType ) );
		addXmlNodeAttribute( doc, node, kError,       element->_error );
		addXmlNodeAttribute( doc, node, kWarning,     element->_warning );
		
		// insert it in the xml doc
		if( parent == nullptr )
		{
			doc.append_node( node );
		}
		else if( previous == parent && prevNode != nullptr )
		{
			prevNode->append_node( node );
		}
		else
		{
			rapidxml::xml_node<>* nodeParent = prevNode->parent();
			std::string idStr = kId;
			while( nodeParent != 0 && nodeParent->first_attribute( idStr.c_str() )->value() != parent->_id )
			{
				nodeParent = nodeParent->parent();
			}
			nodeParent->append_node( node );
		}		

		prevNode = node;
	}

	// rapidxml::print( std::cout, doc, 0 );

	std::ofstream file( filename );
	file << doc;
	file.close();
}

}
