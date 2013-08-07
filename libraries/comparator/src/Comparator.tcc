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


void Comparator::compare( const std::string& specId, rg::Report& report )
{
	// @todo: init ExpressionParser variable map with first pass! or not...
	
	_report = &report;
	sr::Specification spec = _specs.getSpec( specId );
	sr::SpecNode node = spec.getFirstNode();

	LOG_TRACE( "\ncompare: Specification ID: " << spec.getId() );
	LOG_TRACE( "compare:    First node ID: "   << node.getId() );
	LOG_TRACE( "compare:      Report size: "   << _report->getSize() );
	
	checkNode( node );
}


template< >
EDataType Comparator::getNodeSubType< EDataType >( const std::string& nodeType )
{
	LOG_INFO( "EDataType" );
	if( nodeType == kAscii  ) return eDataTypeAscii;
	if( nodeType == kHexa   ) return eDataTypeHexa;
	if( nodeType == kRaw    ) return eDataTypeRaw;
	return eDataTypeUnknown;
}

template< >
ENumberType Comparator::getNodeSubType< ENumberType >( const std::string& nodeType )
{
	LOG_INFO( "ENumberType" );
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
	LOG_INFO( "°°° NUMBER ! °°°" );
	std::shared_ptr< ElementType > element( new ElementType() );
	element->setId( node.getId() );
	element->setLabel( node.getLabel() );
	element->setBigEndianness( node.isBigEndian() );
	// element->setData( /*fromFileData*/, element->getSize() );		// @todo: when file reading supported!
	element->checkData();
	return element;
}

template< >
std::shared_ptr< be::data_element::Data > Comparator::getElement< be::data_element::Data >( const sr::SpecNode& node )
{
	LOG_INFO( "°°° DATA ! °°°" );
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


void Comparator::checkNode( const sr::SpecNode& node, const bool& isFirstChild )
{
	LOG_TRACE( "\nCheck: " << node.getId() );

	checkElementFromNode( node, isFirstChild );
	if( node.hasGroup() )
		checkNode( node.firstChild(), true );
	if( ! node.isLastNode() )
		checkNode( node.next() );
}

void Comparator::checkElementFromNode( const sr::SpecNode& node, const bool& isFirstChild )
{
	if( getNodeSubType< EDataType >( node.getType() ) != eDataTypeUnknown )
		addElementToReport( node, getElement< be::data_element::Data >( node ), isFirstChild );

	switch( getNodeSubType< ENumberType >( node.getType() ) )
	{
		case eNumberTypeUnknown      : break;
		case eNumberTypeInt8         : addElementToReport( node, getElement< be::number_element::Number< be::number_element::int8         > >( node ), isFirstChild ); break;
		case eNumberTypeUInt8        : addElementToReport( node, getElement< be::number_element::Number< be::number_element::uint8        > >( node ), isFirstChild );  break;
		case eNumberTypeInt16        : addElementToReport( node, getElement< be::number_element::Number< be::number_element::int16        > >( node ), isFirstChild );  break;
		case eNumberTypeUInt16       : addElementToReport( node, getElement< be::number_element::Number< be::number_element::uint16       > >( node ), isFirstChild );  break;
		case eNumberTypeInt32        : addElementToReport( node, getElement< be::number_element::Number< be::number_element::int32        > >( node ), isFirstChild );  break;
		case eNumberTypeUInt32       : addElementToReport( node, getElement< be::number_element::Number< be::number_element::uint32       > >( node ), isFirstChild );  break;
		case eNumberTypeInt64        : addElementToReport( node, getElement< be::number_element::Number< be::number_element::int64        > >( node ), isFirstChild );  break;
		case eNumberTypeUInt64       : addElementToReport( node, getElement< be::number_element::Number< be::number_element::uint64       > >( node ), isFirstChild );  break;
		case eNumberTypeFloat        : addElementToReport( node, getElement< be::number_element::Number< float                            > >( node ), isFirstChild );  break;
		case eNumberTypeDouble       : addElementToReport( node, getElement< be::number_element::Number< double                           > >( node ), isFirstChild );    break;
		case eNumberTypeIeeeExtended : addElementToReport( node, getElement< be::number_element::Number< be::number_element::ieeeExtended > >( node ), isFirstChild );    break;
	}
}

void Comparator::addElementToReport( const sr::SpecNode& node, const std::shared_ptr< be::Element >& element, const bool& isFirstChild )
{
	LOG_TRACE( "parent  ? " << node.parent() );
	LOG_TRACE( "lastNode? " << node.isLastNode() );
	if( node.parent() == NULL )
	{
		_currentReportNode = _report->addRootElement( element );
	}
	else if( isFirstChild )
	{
		_currentReportNode = _currentReportNode.appendChild( element );
	}
	else if( node.isLastNode() )
	{
		_currentReportNode = *_currentReportNode.parent();
	}
	else
	{
		_currentReportNode = _currentReportNode.appendNext( element );
	}
	LOG_TRACE( "==> report size: " << _report->getSize() );
}



}

