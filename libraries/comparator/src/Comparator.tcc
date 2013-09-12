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
	if( _file->getLength() == 0 )
		throw std::runtime_error( "Empty file, cannot be checked." );
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
	element->setMap( node.getMap() );
	element->setRanges( node.getRange() );

	size_t size = element->getSize();
	char buffer[ size ];
	_file->readData( buffer, size );
	element->setData( buffer, size );
	element->checkData();

	_elementList.insert( std::make_pair( node.getId(), element ) );
	LOG_TRACE( "_elementList: " << _elementList.size() );

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
	element->setDisplayType( node.getDisplayType() );

	size_t size = element->getSize();
	if( size == 0 && ! node.getCount().empty() )
	{
		be::expression_parser::ExpressionParser sizeParser( _elementList );
		size = sizeParser.getExpressionResult< size_t >( node.getCount() );
	}

	if( size == 0 && node.getCount().empty() )
		throw std::runtime_error( node.getId() + " element size cannot be found" );

	char buffer[ size ];
	_file->readData( buffer, size );
	element->setData( buffer, size );
	element->checkData();
	return element;
}


void Comparator::compare( const std::string& specId, rg::Report& report )
{
	LOG_TRACE( "COMPARE !");
	_report = &report;

	sr::Specification spec = _specs.getSpec( specId );
	sr::SpecNode      node = spec.getFirstNode();
	rg::ReportNode    reportNode;
	checkNode( node, reportNode );
}

void Comparator::checkNode( const sr::SpecNode& node, report_generator::ReportNode& reportNode, const bool& isFirstChild )
{
	size_t initPos = _file->getPosition();
	if( _file->isEndOfFile() )
	{
		LOG_ERROR( "End Of File !" );
		return;
	}

	std::string nodeId = node.getId();

	// check node
	std::shared_ptr< be::Element > element( getElementFromNode( node ) );

	// iterate node
	if( _elementIter.count( nodeId ) == 0 )
	{
		ElementIter iteration;
		iteration.iter = 1;
		if( node.parent() != NULL )
		{
			iteration.parentId = node.parent()->getId();
			_elementIter.at( iteration.parentId ).childrenId.push_back( nodeId );
		}
		_elementIter.insert( std::make_pair( nodeId, iteration ) );
	}
	else
		_elementIter.at( nodeId ).iter++;

	LOG_TRACE( ">> CHECK NODE " << nodeId << " | " << _elementIter.at( nodeId ).iter );
	
	// get repetition
	size_t repetNumber = 1;
	size_t elemIter = _elementIter.at( nodeId ).iter;
	Vector< size_t >::Pair repetRange;
	extractRepetition( repetNumber, repetRange, node.getRepetition() );

	sr::SpecNode nextNode = node.next();
	
	bool sentToReport = true;

	// check repetitions
	if( elemIter < repetNumber )
	{
		nextNode = node;
	}

	if( repetRange.size() == 0 && ( elemIter > 1 && elemIter <= repetNumber ) && element->getStatus() == be::Element::eStatusInvalid ) 
	{
		std::stringstream error;
		error << "Out of repetition range (" << --_elementIter.at( nodeId ).iter << "/" << repetNumber << ")";
		reportNode.getSecond()->begin()->second.data()->addErrorLabel( error.str() );
		LOG_ERROR( error.str() );
		sentToReport = false;
		_file->goBack( element->getSize() );
	}

	// check if optional
	if( node.isOptional() && element->getStatus() != be::Element::eStatusValid && elemIter == 1 )
	{
		LOG_TRACE( "Optional & Not Valid -> Go back: " << element->getSize() );
		sentToReport = false;
		_file->goBack( element->getSize() );
		nextNode = node.next();
	}

	bool inRangeCheck = false;
	if( repetRange.size() > 0 )
	{
		size_t min = repetRange.at(0).first;
		size_t max = repetRange.at(0).second;
		for( std::pair< size_t, size_t > range : repetRange )
		{
			min = std::min( range.first,  min );
			max = ( range.second * max == 0 )? 0 : std::max( range.second, max );	
		}
		LOG_TRACE( "\tRanges : min = " << min << " | max = " << max );

		if( element->getStatus() == be::Element::eStatusValid )
		{
			inRangeCheck = true;
			nextNode = node;
		}
		else if( ! node.isOptional() || elemIter > 1 )
		{
			if( _file->getLength() - initPos > 0 && _file->getLength() - initPos < element->getSize() )
				_file->goBack( _file->getLength() - initPos );
			else
				_file->goBack( element->getSize() );

			sentToReport = false;

			elemIter = --_elementIter.at( nodeId ).iter;
			
			bool isInRange = false;
			for( std::pair< size_t, size_t > range : repetRange )
			{
				if( elemIter >= range.first && range.second == 0 )
					isInRange = true;
				else if( elemIter >= range.first && elemIter <= range.second )
					isInRange = true;
			}
			if( ! isInRange )
			{
				std::stringstream error;
				error << "Out of repetition range (" << elemIter << " iterations) ";
				reportNode.getSecond()->begin()->second.data()->addErrorLabel( error.str() );
				LOG_ERROR( error.str() );
			}
		}
	}

	// node to report
	rg::ReportNode* nextReportNode;
	if( isFirstChild && sentToReport )
	{
		rg::ReportNode newReportNode = reportNode.appendChild( element );
		nextReportNode = &newReportNode;
	}
	else if( sentToReport )
	{
		if( node.parent() == NULL )
			reportNode = _report->addRootElement( element );
		else
			reportNode = reportNode.appendNext( element );
		nextReportNode = &reportNode;
	}
	else
	{
		nextReportNode = &reportNode;
	}

	// children nodes check
	if( node.hasGroup() && sentToReport )
		checkNode( node.firstChild(), *nextReportNode, true );
	
	// check group size
	if( node.hasGroup() && ! node.getGroupSize().empty() )
	{
		int sizeDiff = extractGroupSize( node.getGroupSize() ) - ( _file->getPosition() - ( initPos + element->getSize() ) );
		std::stringstream warning;
		if( sizeDiff > 0 )
			warning << "Group size difference: " << sizeDiff << " missing bytes ";
		if( sizeDiff < 0 )
			warning << "Group size difference: " << abs( sizeDiff ) << " unexpected bytes ";
		if( ! warning.str().empty() )
		{
			nextReportNode->getSecond()->begin()->second.data()->addWarningLabel( warning.str() );
			LOG_WARNING( warning.str() );
		}
	}

	// next node check
	if( ! node.isLastNode() )
		checkNode( nextNode, *nextReportNode, ( isFirstChild && ! sentToReport ) );
	else if( nextNode.getId() == nodeId && ! _file->isEndOfFile() && inRangeCheck )
		checkNode( nextNode, *nextReportNode, ( isFirstChild && ! sentToReport ) );
	else if( _elementIter.at( nodeId ).iter < repetNumber && sentToReport )
		checkNode( node, *nextReportNode, ( isFirstChild && ! sentToReport ) );
	else
		LOG_TRACE( "\t IS LAST NODE !" );

	// remove iteration
	_elementIter.at( nodeId ).iter = 0;
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

void Comparator::extractRepetition( size_t& repetNumber, Vector< size_t >::Pair& repetRange, const Vector< std::string >::Pair& nodeRepetitions )
{
	if( nodeRepetitions.size() > 0 )
	{
		for( std::pair< std::string, std::string > repetPair : nodeRepetitions )
		{
			if( repetPair.first == repetPair.second )
			{
				be::expression_parser::ExpressionParser repetParser( _elementList );
				repetNumber = repetParser.getExpressionResult< size_t >( repetPair.first );
			}
			else
			{
				be::expression_parser::ExpressionParser repetParser( _elementList );
				size_t repetMin = 0;
				size_t repetMax = 0;

				if( ! repetPair.first.empty() )
					repetMin = repetParser.getExpressionResult< size_t >( repetPair.first );
				if( ! repetPair.second.empty() )
					repetMax = repetParser.getExpressionResult< size_t >( repetPair.second );

				if( repetMax != 0 && repetMin > repetMax )
					repetRange.push_back( std::make_pair( repetMax, repetMin ) );
				else
					repetRange.push_back( std::make_pair( repetMin, repetMax ) );
			}
		}
	}
}

size_t Comparator::extractGroupSize( const std::string& groupSizeExpr )
{
	be::expression_parser::ExpressionParser repetParser( _elementList );
	return repetParser.getExpressionResult< size_t >( groupSizeExpr );
}

}

