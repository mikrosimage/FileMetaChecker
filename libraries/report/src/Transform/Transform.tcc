#include <memory>
#include <Number/Number.hpp>
#include <Data/Data.hpp>

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
		_report.push_back( bpt::ptree::value_type( kSpecification, translate( rootNode ) ) );
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
				case eSubTypeInt8         : elemInfo = translateElement< ben::Number< ben::int8         > >( element )->getElementInfo(); break;
				case eSubTypeUInt8        : elemInfo = translateElement< ben::Number< ben::uint8        > >( element )->getElementInfo(); break;
				case eSubTypeInt16        : elemInfo = translateElement< ben::Number< ben::int16        > >( element )->getElementInfo(); break;
				case eSubTypeUInt16       : elemInfo = translateElement< ben::Number< ben::uint16       > >( element )->getElementInfo(); break;
				case eSubTypeInt32        : elemInfo = translateElement< ben::Number< ben::int32        > >( element )->getElementInfo(); break;
				case eSubTypeUInt32       : elemInfo = translateElement< ben::Number< ben::uint32       > >( element )->getElementInfo(); break;
				case eSubTypeInt64        : elemInfo = translateElement< ben::Number< ben::int64        > >( element )->getElementInfo(); break;
				case eSubTypeUInt64       : elemInfo = translateElement< ben::Number< ben::uint64       > >( element )->getElementInfo(); break;
				case eSubTypeFloat        : elemInfo = translateElement< ben::Number< float             > >( element )->getElementInfo(); break;
				case eSubTypeDouble       : elemInfo = translateElement< ben::Number< double            > >( element )->getElementInfo(); break;
				case eSubTypeIeeeExtended : elemInfo = translateElement< ben::Number< ben::ieeeExtended > >( element )->getElementInfo(); break;
				case eSubTypeAscii        :
				case eSubTypeHexa         :
				case eSubTypeRaw          : LOG_WARNING( "Number " << element->getUniqueId()  << ": invalid type." );  break;
			}
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
