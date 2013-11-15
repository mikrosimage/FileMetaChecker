#include "Report.hpp"

#include <iostream>
#include <sstream>

#include <BasicElement/Element.hpp>

#include <Common/color.hpp>

#include <rapidxml/rapidxml_print.hpp>

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

void addXmlNodeAttribute( rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const std::string& attrName, const std::string& attrValue )
{
	std::string name  = attrName;
	std::string value = attrValue;
	node->append_attribute( doc.allocate_attribute( name.c_str(), value.c_str() ) );
}


void Report::writeXml()
{
	rapidxml::xml_document<> doc;
	
	rapidxml::xml_node<>* prevNode = nullptr;
	rapidxml::xml_node<>* node     = nullptr;

	for( std::shared_ptr< basic_element::Element > element : _elementList )
	{
		if( element->_status == eStatusSkip
		 || element->_status == eStatusInvalidButSkip
		 || element->_status == eStatusInvalidButOptional
		 || element->_status == eStatusNotChecked         )
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
		sizeSstr << element->_size;
		addXmlNodeAttribute( doc, node, kSize,        sizeSstr.str()                    );
		addXmlNodeAttribute( doc, node, kValue,       element->_dispValue               );
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
	rapidxml::print( std::cout, doc, 0 );
}




}
