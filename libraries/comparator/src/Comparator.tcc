#include <Data/Data.hpp>
#include <Number/Number.hpp>
#include <FileReader.hpp>
#include <Tools/ExpressionParser.hpp>

namespace rg = report_generator;
namespace sr = spec_reader;
namespace be = basic_element;
namespace fr = filereader;

namespace comparator
{

Comparator::Comparator( fr::FileReader* file, const sr::SpecList& specs )
	: _file( file )
	, _specs( specs )
{
}

Comparator::~Comparator()
{
}

template< >
EDataType Comparator::getNodeSubType< EDataType >( const std::string& nodeType )
{
	if( nodeType == kAscii  ) return eDataTypeAscii;
	if( nodeType == kHexa   ) return eDataTypeHexa;
	if( nodeType == kRaw    ) return eDataTypeRaw;
	return eDataTypeUnknown;
}

template< >
ENumberType Comparator::getNodeSubType< ENumberType >( const std::string& nodeType )
{
	if( nodeType == kInt8         ) return eNumberTypeInt8;
	if( nodeType == kUInt8        ) return eNumberTypeUInt8;
	if( nodeType == kInt16        ) return eNumberTypeInt16;
	if( nodeType == kUInt16       ) return eNumberTypeUInt16;
	if( nodeType == kInt32        ) return eNumberTypeInt32;
	if( nodeType == kUInt32       ) return eNumberTypeUInt32;
	if( nodeType == kInt64        ) return eNumberTypeInt64;
	if( nodeType == kUInt64       ) return eNumberTypeUInt64;
	if( nodeType == kFloat        ) return eNumberTypeFloat;
	if( nodeType == kDouble       ) return eNumberTypeDouble;
	if( nodeType == kIeeeExtended ) return eNumberTypeIeeeExtended;
	return eNumberTypeUnknown;
}

template< typename ElementType >
std::shared_ptr< ElementType > Comparator::getElement( const sr::SpecNode& node )
{
	std::shared_ptr< ElementType > element( new ElementType() );
	element->setId( node.getId() );
	element->setLabel( node.getLabel() );
	element->setBigEndianness( node.isBigEndian() );
	// element->setData( /*fromFileData*/, element->getSize() );		// @todo: when file reading supported!
	element->setRanges( node.getRange() );

	element->checkData();
	return element;
}

template< >
std::shared_ptr< be::data_element::Data > Comparator::getElement< be::data_element::Data >( const sr::SpecNode& node )
{
	EDataType subtype = getNodeSubType< EDataType >( node.getType() );
	std::shared_ptr< be::data_element::Data > element( new be::data_element::Data( subtype ) );
	element->setId( node.getId() );
	element->setLabel( node.getLabel() );
	element->setBigEndianness( node.isBigEndian() );
	element->setSpecData( node.getValues() );

	// size_t size = element->getSize();
	// if( size == 0 && ! node.getCount().empty() )						// @todo: when variables map is got!
	// {
	// 	be::expression_parser::ExpressionParser< size_t > sizeParser( /*variablesMap*/ );
	// 	size = sizeParser.getExpressionResult< size_t >( node.getCount() );
	// }
	// element->setData( /*fromFileData*/, size );						// @todo: when file reading supported!

	element->checkData();
	return element;
}


void Comparator::compare( const std::string& specId, rg::Report& report )
{
	// @todo: init ExpressionParser variable map with first pass! or not...
	LOG_TRACE( "COMPARE !");
	_report = &report;
	
	sr::Specification spec = _specs.getSpec( specId );
	sr::SpecNode      node = spec.getFirstNode();
	rg::ReportNode    reportNode;
	checkNode( node, reportNode );
}

void Comparator::checkNode( const sr::SpecNode& node, report_generator::ReportNode& reportNode, const bool& isFirstChild )
{
	LOG_TRACE( "CHECK NODE " << node.getId() );
	std::shared_ptr< be::Element > element( getElementFromNode( node ) );

	if( isFirstChild )
	{
		rg::ReportNode newReportNode = reportNode.appendChild( element );

		if( node.hasGroup() )
			checkNode( node.firstChild(), newReportNode, true );

		if( ! node.isLastNode() )
			checkNode( node.next(), newReportNode );
		return;
	}

	if( node.parent() == NULL )
		reportNode = _report->addRootElement( element );
	else
		reportNode = reportNode.appendNext( element );

	if( node.hasGroup() )
		checkNode( node.firstChild(), reportNode, true );

	if( ! node.isLastNode() )
		checkNode( node.next(), reportNode );

}

std::shared_ptr< be::Element > Comparator::getElementFromNode( const sr::SpecNode& node )
{
	if( getNodeSubType< EDataType >( node.getType() ) != eDataTypeUnknown )
		return getElement< be::data_element::Data >( node );

	switch( getNodeSubType< ENumberType >( node.getType() ) )
	{
		case eNumberTypeUnknown      : break;
		case eNumberTypeInt8         : return getElement< be::number_element::Number< be::number_element::int8         > >( node ); break;
		case eNumberTypeUInt8        : return getElement< be::number_element::Number< be::number_element::uint8        > >( node ); break;
		case eNumberTypeInt16        : return getElement< be::number_element::Number< be::number_element::int16        > >( node ); break;
		case eNumberTypeUInt16       : return getElement< be::number_element::Number< be::number_element::uint16       > >( node ); break;
		case eNumberTypeInt32        : return getElement< be::number_element::Number< be::number_element::int32        > >( node ); break;
		case eNumberTypeUInt32       : return getElement< be::number_element::Number< be::number_element::uint32       > >( node ); break;
		case eNumberTypeInt64        : return getElement< be::number_element::Number< be::number_element::int64        > >( node ); break;
		case eNumberTypeUInt64       : return getElement< be::number_element::Number< be::number_element::uint64       > >( node ); break;
		case eNumberTypeFloat        : return getElement< be::number_element::Number< float                            > >( node ); break;
		case eNumberTypeDouble       : return getElement< be::number_element::Number< double                           > >( node ); break;
		case eNumberTypeIeeeExtended : return getElement< be::number_element::Number< be::number_element::ieeeExtended > >( node ); break;
	}
	throw std::runtime_error( "getElementFromNode: Unknown type" );
}

}

