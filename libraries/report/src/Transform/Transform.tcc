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

void Transform::transform()
{
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
			extractElement( pair.second.data() );
	}

	return node;
}

void Transform::extractElement( std::shared_ptr< be::Element > element )
{
	LOG_INFO( "be::Element::eType: " << element->getType() );
	switch( element->getType() )
	{
		case be::Element::eTypeUnknown : LOG_WARNING( "Element " << element->getUniqueId() << ": unknown type." ); break;
		
		case be::Element::eTypeNumber  : 
		{
			LOG_INFO( "be::Element::eNumberType: " << element->getNumberSubType() );
			switch( element->getNumberSubType() )
			{
				case be::Element::eNumberTypeUnknown      : LOG_WARNING( "Number " << element->getUniqueId()  << ": unknown type." ); break;
				case be::Element::eNumberTypeInt8         : translateElement< ben::Number< ben::int8         > >( element ); break;
				case be::Element::eNumberTypeUInt8        : translateElement< ben::Number< ben::uint8        > >( element ); break;
				case be::Element::eNumberTypeInt16        : translateElement< ben::Number< ben::int16        > >( element ); break;
				case be::Element::eNumberTypeUInt16       : translateElement< ben::Number< ben::uint16       > >( element ); break;
				case be::Element::eNumberTypeInt32        : translateElement< ben::Number< ben::int32        > >( element ); break;
				case be::Element::eNumberTypeUInt32       : translateElement< ben::Number< ben::uint32       > >( element ); break;
				case be::Element::eNumberTypeInt64        : translateElement< ben::Number< ben::int64        > >( element ); break;
				case be::Element::eNumberTypeUInt64       : translateElement< ben::Number< ben::uint64       > >( element ); break;
				case be::Element::eNumberTypeFloat        : translateElement< ben::Number< float             > >( element ); break;
				case be::Element::eNumberTypeDouble       : translateElement< ben::Number< double            > >( element ); break;
				case be::Element::eNumberTypeIeeeExtended : translateElement< ben::Number< ben::ieeeExtended > >( element ); break;
			}
		} break;

		case be::Element::eTypeExif    : LOG_INFO( "Exif !");                      break;
		case be::Element::eTypeData    : translateElement< bed::Data >( element ); break;
		case be::Element::eTypeKlv     : LOG_INFO( "Klv !");                       break;
	}
}


template < typename ElementType >
std::shared_ptr< ElementType > Transform::translateElement( std::shared_ptr< be::Element > element )
{
	return std::static_pointer_cast< ElementType >( element );
}

}
