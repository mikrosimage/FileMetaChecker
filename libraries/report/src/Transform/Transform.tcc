
#include <basicElement/Number/Number.hpp>
#include <basicElement/Data/Data.hpp>

#include <memory>

namespace be  = basic_element;
namespace ben = be::number_element;
namespace bed = be::data_element;

namespace report_generator
{

Transform::Transform()
{
}

Transform::Transform( const Report& report )
{
	_basicElementTree = report.getReportTree();
}


Transform::~Transform()
{
}

void Transform::setBasicElementReport( const Report& report )
{
	_basicElementTree = report.getReportTree();
}

bpt::ptree Transform::transformTree( const EReportType& type )
{
	_type = type;
	for( ReportTree::value_type& rootNode : _basicElementTree.get_child( kReport ) )
	{
		_report.push_back( bpt::ptree::value_type( kSpecification, translate( rootNode ) ) );
	}
	return _report;
}

bpt::ptree Transform::translate( ReportTree::value_type& elementNode )
{
	bpt::ptree elemNode;
	for( ReportTree::value_type& elem : elementNode.second )
	{
		if( elem.first == kGroup )
		{
			for( ReportTree::value_type& child : elem.second )
				elemNode.push_back( bpt::ptree::value_type( kElement, translate( child ) ) );
		}
		else
		{
			elemNode = extractElement( elem.second.data() );
		}
	}
	return elemNode;
}

bpt::ptree Transform::extractElement( std::shared_ptr< be::Element > element )
{
	bpt::ptree elemNode;
	std::vector< std::pair< std::string, std::string > > elemInfo;

	switch( element->getType() )
	{
		case eTypeUnknown : LOG_WARNING( "Element " << element->getUniqueId() << ": unknown type." ); break;
		case eTypeNumber  : 
		{
			switch( element->getSubType() )
			{
				case eSubTypeUnknown      : LOG_WARNING( "Number " << element->getUniqueId()  << ": unknown type." ); break;
				case eSubTypeInt8         : 
				case eSubTypeUInt8        : 
				case eSubTypeInt16        : 
				case eSubTypeUInt16       : 
				case eSubTypeInt32        : 
				case eSubTypeUInt32       : 
				case eSubTypeInt64        : 
				case eSubTypeUInt64       : 
				case eSubTypeFloat        : 
				case eSubTypeDouble       : 
				case eSubTypeIeeeExtended : elemInfo = translateElement< ben::Number >( element )->getElementInfo(); break;
				case eSubTypeAscii        :
				case eSubTypeHexa         :
				case eSubTypeRaw          : LOG_WARNING( "Number " << element->getUniqueId()  << ": invalid type." );  break;
			}
			LOG_WARNING( "Number " << element->getUniqueId()  << ": not supported !!" ); // @todelete !!
		} break;
		case eTypeExif    : LOG_INFO( "Exif !");                      break;
		case eTypeData    : elemInfo = translateElement< bed::Data >( element )->getElementInfo(); break;
		case eTypeKlv     : LOG_INFO( "Klv !");                       break;
	}

	switch( _type )
	{
		case eReportTypeXml  : elemNode = toXml( elemInfo ); break;
		case eReportTypeJson : LOG_WARNING( "Json report not available yet." ); break;
	}

	return elemNode;
}

bpt::ptree Transform::toXml( std::vector< std::pair< std::string, std::string > > elementInfo )
{
	bpt::ptree xmlNode;
	for( std::pair< std::string, std::string >& pair : elementInfo )
	{
		if( pair.first == kData || pair.first == kValue )
		{
			xmlNode.put_value( pair.second );
			continue;
		}
		xmlNode.put( "<xmlattr>." + pair.first, pair.second );
	}
	return xmlNode;
}

template < typename ElementType >
std::shared_ptr< ElementType > Transform::translateElement( std::shared_ptr< be::Element > element )
{
	return std::static_pointer_cast< ElementType >( element );
}

}
