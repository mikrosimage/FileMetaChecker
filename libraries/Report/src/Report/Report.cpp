#include "Report.hpp"

#include <BasicElement/Element.hpp>

#include <Common/color.hpp>
#include <Common/log.hpp>


#include <Serializer.hpp>
#include <SerializerJson.hpp>
#include <SerializerXml.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

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

void Report::add( const ShPtrElement& element )
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

std::vector< Report::ShPtrElement > Report::get( )
{
	return _elementList;
}

Report::ShPtrElement Report::get( const std::string& elementId, const size_t iteration ) const
{
	for( ShPtrElement element : _elementList )
	{
		if( element->_id == elementId && element->_iteration == iteration )
			return element;
	}
	return nullptr;
}

size_t Report::getElementListSize() const
{
	return _elementList.size();
}

std::vector< Report::ShPtrElement > Report::getElementList() const
{
	return _elementList;
}

void Report::update( const ShPtrElement& newElement )
{
	if( newElement == nullptr )
		return;
	for( size_t i = 0; i < _elementList.size(); ++i )
	{
		if( _elementList.at(i)->_uId == newElement->_uId )
		{
			_elementList.at(i) = newElement;
			return;
		}
	}
}

size_t Report::getDisplayOffset( const ShPtrElement& element )
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
	if( element->_status == eStatusSkip )
		return false;
	return true;
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

std::string Report::serialize( EExportType exportType, bool indent )
{
	std::vector<char*> path;
	path.push_back( (char*)"report" );
	path.push_back( (char*)kElement.c_str() );

	std::vector< ShPtrElement > rootElements;
	for( auto element : _elementList )
	{
		if( element->getParent() == nullptr )
			rootElements.push_back( element );
	}

	switch( exportType )
	{
		case eExportTypeJson :
		{
			rapid_parser::SerializerJson output;
			output.indent( indent );
			serialize( ( rapid_parser::Serializer* ) &output, rootElements, path );
			return output.get();
			break;
		}
		case eExportTypeXml :
		{
			rapid_parser::SerializerXml output;
			output.indent( indent );
			serialize( ( rapid_parser::Serializer* ) &output, rootElements, path );
			return output.get();
			break;
		}
	}
	return "";
}

void Report::serialize( rapid_parser::Serializer* serializer, const std::vector< ShPtrElement >& elements, std::vector<char*>& path )
{
	for( auto element : elements )
	{
		if( element->_status == eStatusSkip )
			continue;

		serializer->addEmptyElement( path );

		serializer->add( kId.c_str(), element->_id, path );
		serializer->add( kLabel.c_str(), element->_label, path );
		serializer->add( kType.c_str(), typeStringMap.at( element->_type), path );
		serializer->add( kDisplayType.c_str(), displayTypeStringMap.at( element->_displayType ), path );

		serializer->add( kSize.c_str(), element->_data.size(), path );

		if( element->_dispValue.size() )
		{
			serializer->add( kValue.c_str(), element->_dispValue, path );
			/*
			switch( element->_type )
			{
				case eTypeUnknown:
					break;
				case eTypeInt8:
				case eTypeInt16:
				case eTypeInt32:
				{
					serializer->add( kValue.c_str(), std::stoi( element->_dispValue ), path );
					break;
				}
				case eTypeInt64:
				{
					serializer->add( kValue.c_str(), std::stol( element->_dispValue ), path );
					break;
				}
				case eTypeUInt8:
				case eTypeUInt16:
				case eTypeUInt32:
				case eTypeUInt64:
				{
					serializer->add( kValue.c_str(), std::stoul( element->_dispValue ), path );
					break;
				}
				case eTypeFloat:
				{
					serializer->add( kValue.c_str(), std::stof( element->_dispValue ), path );
					break;
				}
				case eTypeDouble:
				{
					serializer->add( kValue.c_str(), std::stod( element->_dispValue ), path );
					break;
				}
				case eTypeIeeeExtended:
				case eTypeAscii:
				case eTypeHexa:
				case eTypeRaw:
				{
					serializer->add( kValue.c_str(), element->_dispValue, path );
					break;
				}
			}*/
		}

		if( element->_mapValue.size() )
			serializer->add( kMap.c_str(), element->_mapValue, path );

		serializer->add( kStatus.c_str(), statusMap.at( element->_status ), path );

		if( element->_warning.size() )
		{
			for( auto warning : element->_warning )
				serializer->add( kWarning.c_str(), warning, path );
		}
		
		if( element->_error.size() )
		{
			for( auto error : element->_error )
				serializer->add( kError.c_str(), error, path );
		}

		if( element->getChildren().size() )
		{
			std::vector<char*> subPath = path;
			subPath.push_back( (char*)kGroup.c_str() );
			serialize( serializer, element->getChildren(), subPath );
		}
	}
}

}

