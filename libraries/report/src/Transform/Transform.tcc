#include <memory>

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

void Transform::transformTree( const EReportType& type )
{
	_type = type;
	for( ReportTree::value_type& rootNode : _basicElementTree.get_child( kReport ) )
	{
		LOG_INFO( "--> root node :" );
		_report.add_child( kReport + ".", translate( rootNode ) );
	}
}


bpt::ptree Transform::translate( ReportTree::value_type& elementNode )
{
	bpt::ptree node;

	for( ReportTree::value_type& pair : elementNode.second )
	{
		LOG_INFO( pair.first );
		if( pair.first != kGroup )
		{
			extractElement( pair.second.data() );
		}
		else
		{
			for( ReportTree::value_type& elem : pair.second )
				node.add_child( kGroup + ".", translate( elem ) );
		}
	}

	return node;
}

bpt::ptree Transform::extractElement( std::shared_ptr< be::Element > element )
{
	LOG_INFO( "be::Element::eType: " << element->getType() );
	bpt::ptree node;
	std::vector< std::pair< std::string, std::string > > elemInfo;
	switch( element->getType() )
	{
		case be::Element::eTypeUnknown : LOG_WARNING( "Element " << element->getUniqueId() << ": unknown type." ); break;
		
		case be::Element::eTypeNumber  : 
		{
			LOG_INFO( "be::Element::eNumberType: " << element->getNumberSubType() );
			switch( element->getNumberSubType() )
			{
				case be::Element::eNumberTypeUnknown      : LOG_WARNING( "Number " << element->getUniqueId()  << ": unknown type." ); break;
				case be::Element::eNumberTypeInt8         : elemInfo = translateElement< ben::Number< ben::int8         > >( element )->getElementInfo(); break;
				case be::Element::eNumberTypeUInt8        : elemInfo = translateElement< ben::Number< ben::uint8        > >( element )->getElementInfo(); break;
				case be::Element::eNumberTypeInt16        : elemInfo = translateElement< ben::Number< ben::int16        > >( element )->getElementInfo(); break;
				case be::Element::eNumberTypeUInt16       : elemInfo = translateElement< ben::Number< ben::uint16       > >( element )->getElementInfo(); break;
				case be::Element::eNumberTypeInt32        : elemInfo = translateElement< ben::Number< ben::int32        > >( element )->getElementInfo(); break;
				case be::Element::eNumberTypeUInt32       : elemInfo = translateElement< ben::Number< ben::uint32       > >( element )->getElementInfo(); break;
				case be::Element::eNumberTypeInt64        : elemInfo = translateElement< ben::Number< ben::int64        > >( element )->getElementInfo(); break;
				case be::Element::eNumberTypeUInt64       : elemInfo = translateElement< ben::Number< ben::uint64       > >( element )->getElementInfo(); break;
				case be::Element::eNumberTypeFloat        : elemInfo = translateElement< ben::Number< float             > >( element )->getElementInfo(); break;
				case be::Element::eNumberTypeDouble       : elemInfo = translateElement< ben::Number< double            > >( element )->getElementInfo(); break;
				case be::Element::eNumberTypeIeeeExtended : elemInfo = translateElement< ben::Number< ben::ieeeExtended > >( element )->getElementInfo(); break;
			}
		} break;

		case be::Element::eTypeExif    : LOG_INFO( "Exif !");                      break;
		case be::Element::eTypeData    : elemInfo = translateElement< bed::Data >( element )->getElementInfo(); break;
		case be::Element::eTypeKlv     : LOG_INFO( "Klv !");                       break;
	}

	switch( _type )
	{
		case eReportTypeXml  : node = toXml( elemInfo ); break;
		case eReportTypeJson : LOG_WARNING( "Json report not available yet." ); break;
	}
	return node;
}

bpt::ptree Transform::toXml( std::vector< std::pair< std::string, std::string > > elementInfo )
{
	bpt::ptree xmlNode;
	for( std::pair< std::string, std::string >& pair : elementInfo )
		LOG_INFO( pair.first << " : " << pair.second );		// @todo: make the xml node !
	return xmlNode;
}

template < typename ElementType >
std::shared_ptr< ElementType > Transform::translateElement( std::shared_ptr< be::Element > element )
{
	return std::static_pointer_cast< ElementType >( element );
}

}
