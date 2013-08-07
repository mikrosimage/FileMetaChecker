#include "Comparator.hpp"

// #include <Number/Number.hpp>
#include <FileReader.hpp>
#include <Data/Data.hpp>
#include <Tools/ExpressionParser.hpp>

namespace sr = spec_reader;
namespace be = basic_element;
namespace rg = report_generator;
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
	// @todo: init ExpressionParser variable map with first pass!
	
	_report = &report;
	sr::Specification spec = _specs.getSpec( specId );
	sr::SpecNode node = spec.getFirstNode();

	LOG_TRACE( "\ncompare: Specification ID: " << spec.getId() );
	LOG_TRACE( "compare:    First node ID: "   << node.getId() );
	LOG_TRACE( "compare:      Report size: "   << _report->getSize() );
	
	checkNode( node );
}

void Comparator::checkNode( const spec_reader::SpecNode& node, const bool& isFirstChild )
{
	LOG_TRACE( "\nCheck: " << node.getId() );

	checkElementFromNode( node, isFirstChild );
	if( node.hasGroup() )
		checkNode( node.firstChild(), true );
	if( ! node.isLastNode() )
		checkNode( node.next() );
}

void Comparator::checkElementFromNode( const spec_reader::SpecNode& node, const bool& isFirstChild )
{
	EDataType subtype = getNodeSubType< EDataType >( node.getType() );
	std::shared_ptr< be::data_element::Data > element( new be::data_element::Data( subtype ) );

	LOG_TRACE( "element type: " << (int) element->getDataSubType() );
	element->setId( node.getId() );
	element->setLabel( node.getLabel() );
	element->setBigEndianness( node.isBigEndian() );
	element->setSpecData( node.getValues() );
	
	size_t size = element->getSize();
	// if( size == 0 && ! node.getCount().empty() )						// @todo: when variables map is got!
	// {
	// 	be::expression_parser::ExpressionParser< size_t > sizeParser( /*variablesMap*/ );
	// 	size = sizeParser.getExpressionResult< size_t >( node.getCount() );
	// }

	// element->setData( /*fromFileData*/, size );						// @todo: when file reading supported!

	element->checkData();
	addElementToReport( node, element, isFirstChild );
}

void Comparator::addElementToReport( const spec_reader::SpecNode& node, const std::shared_ptr< be::Element >& element, const bool& isFirstChild )
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



// be::Element Comparator::getBasicElement( const sr::SpecNode& node )
// {
// 	std::string type = node.getType();	
	
// 	

// 	if( type == kInt8         ) return be::number_element::Number< be::number_element::int8         >( eNumberTypeInt8         );
// 	if( type == kUInt8        ) return be::number_element::Number< be::number_element::uint8        >( eNumberTypeUInt8        );
// 	if( type == kInt16        ) return be::number_element::Number< be::number_element::int16        >( eNumberTypeInt16        );
// 	if( type == kUInt16       ) return be::number_element::Number< be::number_element::uint16       >( eNumberTypeUInt16       );
// 	if( type == kInt32        ) return be::number_element::Number< be::number_element::int32        >( eNumberTypeInt32        );
// 	if( type == kUInt32       ) return be::number_element::Number< be::number_element::uint32       >( eNumberTypeUInt32       );
// 	if( type == kInt64        ) return be::number_element::Number< be::number_element::int64        >( eNumberTypeInt64        );
// 	if( type == kUInt64       ) return be::number_element::Number< be::number_element::uint64       >( eNumberTypeUInt64       );
// 	if( type == kFloat        ) return be::number_element::Number< float                            >( eNumberTypeFloat        );
// 	if( type == kDouble       ) return be::number_element::Number< double                           >( eNumberTypeDouble       );
// 	if( type == kIeeeExtended ) return be::number_element::Number< be::number_element::ieeeExtended >( eNumberTypeIeeeExtended );

// 	throw std::runtime_error( "getBasicElement: Unknown type");
// 	return be::data_element::Data( eDataTypeUnknown );
// }
