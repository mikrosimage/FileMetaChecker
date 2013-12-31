#include "Checker.hpp"

#include <ElementChecker/Translator/Translator.hpp>
#include <ElementChecker/Ranges/Ranges.hpp>
#include <ElementChecker/ExpressionParser/ExpressionParser.hpp>
#include <ElementChecker/Map/Map.hpp>

#include <Common/log.hpp>

#include <algorithm>

namespace be = basic_element;

namespace element_checker
{

namespace utils
{
	template< typename NumberType >
	EStatus checkNumberElement( const Checker::ShPtrElement element )
	{
		NumberType value = Translator( element ).get< NumberType >();

		element->_mapValue = Map< NumberType >( element->_map ).getLabel( value );
		
		if( ! Ranges< NumberType >( element->_rangeExpr ).isInRanges( value ) )
			return eStatusInvalid;
		return eStatusValid;
	}
}

Checker::Checker()
	: _exprParser( new ExpressionParser )
{
}

size_t Checker::getSize( const ShPtrElement element )
{
	try
	{
		size_t size = 0;
		if( ! element->_values.empty() )
		{
			size = element->_values.at( 0 ).size();
			for( std::string value : element->_values )
				if( value.size() != size )
					throw std::runtime_error( "[checker] Values must have the same size (" +  element->_id + ")" );
		}

		switch( element->_type )
		{
			case eTypeHexa         : size = size >> 1; break;
			case eTypeInt8         :
			case eTypeUInt8        : return  1;
			case eTypeInt16        :
			case eTypeUInt16       : return  2;
			case eTypeInt32        :
			case eTypeUInt32       :
			case eTypeFloat        : return  4;
			case eTypeInt64        :
			case eTypeUInt64       :
			case eTypeDouble       : return  8;
			case eTypeIeeeExtended : return 10;
			default: break;
		}

		if( ! element->_countExpr.empty() && size == 0 )
		{
			size = _exprParser->getExpressionResult< size_t >( element->_countExpr );
			LOG_TRACE( "[checker] get " << element->_id << "'s size: " << size );
		}

		return size;
	}
	catch( std::runtime_error e )
	{
		LOG_ERROR( "[checker] " << e.what() << " (" << element->_id << ")" );
		throw;
	}
}

Checker::ShPtrElement Checker::getNextParent( const ShPtrElement element )
{
	ShPtrElement parent = element->getParent();

	if( element->_isGroup && ! element->_checkedGroup && element->_status != eStatusSkip )
		return element;
	
	ShPtrSpecNode next = element->next();
	if( next == nullptr )
		return nullptr;

	while( parent != nullptr )
	{
		std::set< std::string > childIds = parent->getSpecNode()->getChildrenNodes();
		for( std::string child : childIds )
		{
			if( child == next->getId() )
				return parent;
		}
		parent = parent->getParent();
	}
	return parent;
}

size_t Checker::getBackOffset( const ShPtrElement element )
{
	if( element == nullptr )
		return 0;
	
	size_t backOffset = 0;
	ShPtrElement parent = element->getParent();

	if( element->_status == eStatusSkip )
		backOffset = element->_data.size();

	if( parent != nullptr && parent->_status == eStatusSkip )
		backOffset = ( parent->_childrenSize == 0 )? backOffset : parent->_childrenSize;

	if( backOffset > 0 )
		LOG_TRACE( "[checker] Go back in file (" << backOffset << " bytes)" );
	return backOffset;
}

size_t Checker::checkGroupSize( const ShPtrElement element )
{
	try 
	{
		if( element->getSpecNode()->next() != nullptr || element->_isGroup )
			return 0;

		ShPtrElement parent = element->getParent();
		size_t offset = 0;
		while(  parent != nullptr )
		{
			if( parent->_groupSizeExpr.empty() )
			{
				parent = parent->getParent();
				continue;
			}

			int sizeDiff = parent->_childrenSize - parent->_specGroupSize;
			LOG_TRACE( "[comparator] group size difference (" << parent->_id << ") : " << parent->_specGroupSize << " - " << parent->_childrenSize << " = "<< sizeDiff );
			if( sizeDiff != 0 )
			{
				if( sizeDiff > 0 )
				{
					std::stringstream errorMessage;
					errorMessage << "[comparator] Group size difference: " << sizeDiff << " missing bytes ";
					parent->_error.push_back( errorMessage.str() );
					throw std::runtime_error( errorMessage.str() );
				}
				if( sizeDiff < 0 )
				{
					std::stringstream warningMessage;
					warningMessage << "[comparator] Group size difference: " << abs( sizeDiff ) << " unexpected bytes ";
					parent->_warning.push_back( warningMessage.str() );
					offset = abs( sizeDiff );
					LOG_WARNING( warningMessage.str() );
				}
			}
			parent = parent->getParent();
		}
		return offset;
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

void Checker::check( const ShPtrElement element )
{
	ShPtrElement parent   = element->getParent();
	ShPtrElement previous = element->getPrevious();
	
	EStatus status = eStatusInvalid;
	
	element->_dispValue = Translator( element ).get( element->_displayType );

	checkRequirement ( element, status );
	checkValue       ( element, status );
	checkSpecialCases( element, status );

	if( parent == nullptr || parent->_isOrdered )
		checkRepetition( element, status );

	element->_status = status;

	switch( element->_status )
	{
		case eStatusUnknown :
		case eStatusValid   : _exprParser->addElementToContext( element ); break;
		case eStatusInvalid :
		case eStatusSkip    : break;
	}

	updateParent( element );

	LOG_TRACE( "[checker] " << element->_id << " : return status = " << statusMap.at( status ) );
}

bool Checker::isRequirementValid( const ShPtrElement element )
{
	if( element->_requiredExpr.empty() )
		return true;
	return _exprParser->getExpressionResult< bool >( element->_requiredExpr );
}

void Checker::checkRequirement( const ShPtrElement element, EStatus& status )
{
	if( element->_requiredExpr.empty() )
		return;
	if( ! isRequirementValid( element ) )
	{
		LOG_TRACE( "[checker] " << element->_id << " : requirement not valid -> skipped" );
		status = eStatusSkip;
	}
}

void Checker::checkValue( const ShPtrElement element, EStatus& status )
{
	if( status == eStatusSkip )
		return;

	switch( element->_type )
	{
		case eTypeUnknown      : status = eStatusUnknown; break;
		
		case eTypeInt8         : status = utils::checkNumberElement< be::int8         >( element ); break;
		case eTypeUInt8        : status = utils::checkNumberElement< be::uint8        >( element ); break;
		case eTypeInt16        : status = utils::checkNumberElement< be::int16        >( element ); break;
		case eTypeUInt16       : status = utils::checkNumberElement< be::uint16       >( element ); break;
		case eTypeInt32        : status = utils::checkNumberElement< be::int32        >( element ); break;
		case eTypeUInt32       : status = utils::checkNumberElement< be::uint32       >( element ); break;
		case eTypeInt64        : status = utils::checkNumberElement< be::int64        >( element ); break;
		case eTypeUInt64       : status = utils::checkNumberElement< be::uint64       >( element ); break;
		case eTypeFloat        : status = utils::checkNumberElement< float            >( element ); break;
		case eTypeDouble       : status = utils::checkNumberElement< double           >( element ); break;
		case eTypeIeeeExtended : status = utils::checkNumberElement< be::ieeeExtended >( element ); break;
		
		case eTypeAscii :
		{
			std::string orig = Translator( element ).get();
			std::string lowCase = orig;
			std::transform( lowCase.begin(), lowCase.end(), lowCase.begin(), ::tolower );
		
			for( std::string value : element->_values )
				if( value == orig || ( ! element->_isCaseSensitive && value == lowCase ) )
					status = eStatusValid;

			element->_mapValue = Map< std::string >( element->_map ).getLabel( orig );
			if( element->_mapValue.empty() && ! element->_isCaseSensitive )
				element->_mapValue = Map< std::string >( element->_map ).getLabel( lowCase );

			if( element->_values.empty() && ! element->_mapValue.empty() && getSize( element ) )
				status = eStatusValid;

			if( element->_values.empty() && getSize( element ) == 0 && element->_data.back() == element->_endChar )
				status = eStatusValid;

			if( status == eStatusInvalid )
				element->_error.push_back( "[checker] Invalid value " );
			break;
		}

		case eTypeHexa  :
		{
			std::string rawValue = Translator( element ).get();
			std::string lowCase;

			for( std::string value : element->_values )
			{
				lowCase = value;
				std::transform( lowCase.begin(), lowCase.end(), lowCase.begin(), ::tolower );
				if( value == rawValue || ( ! element->_isCaseSensitive && lowCase == rawValue ) )
				{
					status = eStatusValid;
					break;
				}
			}

			element->_mapValue = Map< std::string >( element->_map ).getLabel( rawValue );
			if( element->_mapValue.empty() && ! element->_isCaseSensitive )
				element->_mapValue = Map< std::string >( element->_map ).getLabel( lowCase );

			if( element->_values.empty() && ! element->_mapValue.empty() && getSize( element ) )
				status = eStatusValid;

			if( status == eStatusInvalid )
				element->_error.push_back( "[checker] Invalid value " );
			break;
		}

		case eTypeRaw   :
		{
			if( element->_data.size() == getSize( element ) )
				status = eStatusValid;
			break;
		}
	}
}

void Checker::checkSpecialCases( const ShPtrElement element, EStatus& status )
{
	ShPtrElement parent = element->getParent();

	if( element->_isOptional && status == eStatusInvalid && element->_iteration == 1 )
	{
		LOG_TRACE( "[checker::checkSpecialCases] " << element->_id << " : invalid but optional -> invalid but skipped" );
		status = eStatusSkip;
	}

	if( parent != nullptr && ! parent->_isOrdered && status == eStatusInvalid )
	{
		LOG_TRACE( "[checker::checkSpecialCases] " << element->_id << " : unordered group -> invalid but skipped" );
		status = eStatusSkip;
	}
}

void Checker::checkRepetition( const ShPtrElement element, EStatus& status )
{
	if( element->_repetExpr.empty() )
		return;

	LOG_TRACE( "[checker] " << element->_id << " : check repetitions " << statusMap.at( status ) << " / " << element->_iteration );
	
	std::string errorMessage;
	
	if( ( status == eStatusValid || status == eStatusUnknown ) && ! continueRepetition( element ) )
		status = eStatusInvalid;

	if( status == eStatusInvalid )
	{
		status = eStatusSkip;
		if( ! isIterationValid( element, errorMessage ) )
		{
			LOG_ERROR( "[checker::checkRepetition] " << errorMessage << " (" << element->_id << " )" );
			element->_error.push_back( errorMessage );
			status = eStatusInvalid;
		}
	}

	if( status == eStatusUnknown && continueRepetition( element ) )
		status = eStatusValid;
}

void Checker::updateParent( const ShPtrElement element )
{
	addElementToParent( element );
	setParentGroupSize( element );
	updateParentSize( element );
	updateParentStatus( element );
}

bool Checker::isLastInGroup( const ShPtrElement element )
{
	ShPtrElement parent = element->getParent();
	return ( element->getSpecNode()->next() == nullptr && ( parent == nullptr || ( parent->_isOrdered || ( ! parent->_isOrdered && element->_status == eStatusSkip ) ) ) );
}

void Checker::addElementToParent( const ShPtrElement element )
{
	ShPtrElement parent = element->getParent();
	if( parent == nullptr )
		return;
	parent->addChild( element );
}

void Checker::setParentGroupSize( const ShPtrElement element )
{
	ShPtrElement parent   = element->getParent();
	ShPtrElement previous = element->getPrevious();


	if( parent == nullptr || previous == nullptr || ! isLastInGroup( element ) || parent->_groupSizeExpr.empty() )
		return;

	while( true )
	{
		if( parent == nullptr || ( parent->_groupSizeExpr.size() && parent->_specGroupSize > 0 ) )
			break;
		if( parent->_groupSizeExpr.size() )
		{
			parent->_specGroupSize = _exprParser->getExpressionResult< size_t >( parent->_groupSizeExpr );
			LOG_TRACE( "[checker] >>> set " << element->_id << "'s parent (" << parent->_id << ") groupSize (" << parent->_groupSizeExpr << "): " << parent->_specGroupSize );
		}
		parent = parent->getParent();
	}
}

void Checker::updateParentSize( const ShPtrElement element )
{
	ShPtrElement parent = element->getParent();
	while( parent != nullptr && element->_status != eStatusSkip  )
	{
		parent->_childrenSize += element->_data.size();
		LOG_TRACE( "[checker] " << parent->_id << "'s children size : " << parent->_childrenSize );
		parent = parent->getParent();
	}
}

void Checker::updateParentStatus( const ShPtrElement element )
{
	ShPtrElement parent = element->getParent();
	EStatus status = element->_status;
	if( status == eStatusUnknown || parent == nullptr )
		return;

	if( isLastInGroup( element ) && ! parent->_isOrdered )
	{
		for( ShPtrElement child : parent->getChildren() )
		{
			if( child->_status != eStatusValid )
				continue;

			std::string errorMessage;
			if( ! isIterationValid( child, errorMessage ) )
			{
				LOG_ERROR( "[checker::updateParentStatus] " << errorMessage << " (" << child->_id << " )" );
				child->_error.push_back( errorMessage );
				child->_status = eStatusInvalid;
				parent->_status = eStatusInvalid;
				updateParent( parent );
				return;
			}
		}
	}

	if( status == eStatusInvalid )
	{
		parent->_status = eStatusInvalid;
		checkSpecialCases( parent, parent->_status );
		checkRepetition( parent, parent->_status );
		if( parent->_status == eStatusSkip )
		{
			ShPtrElement prev = element;
			while( prev != nullptr && prev->_id != parent->_id )
			{
				prev->_status = eStatusSkip;
				prev = prev->getPrevious();
			}
			if( parent->getParent() != nullptr )
				parent->getParent()->_childrenSize -= parent->_childrenSize;
		}

		LOG_INFO( "[checker::updateParentStatus] " << element->_id << " parent's (" << parent->_id << ") status: " << statusMap.at( parent->_status ) );
		updateParent( parent );
		return;
	}

	if( isLastInGroup( element ) && element->_status == eStatusSkip )
	{
		ShPtrElement prev = element;

		bool everyChildrenSkip = true;

		for( ShPtrElement child : parent->getChildren() )
		{
			LOG_TRACE( child->_id << " -> " << statusMap.at( child->_status ) );
			if( child->_status != eStatusSkip )
				everyChildrenSkip = false;
		}

		if( everyChildrenSkip && parent->_status == eStatusUnknown && parent->_isOrdered )
		{
			parent->_status = eStatusSkip;
			LOG_INFO( "[checker::updateParentStatus] " << parent->_id << ": " << statusMap.at( parent->_status ) );
			updateParent( parent );
			return;
		}

		if( ! parent->_isOrdered )
		{
			if( everyChildrenSkip )
			{
				parent->_status = eStatusInvalid;
				LOG_INFO( "[checker::updateParentStatus] " << parent->_id << ": " << statusMap.at( parent->_status ) );
				checkSpecialCases( parent, parent->_status );
				checkRepetition( parent, parent->_status );
				updateParent( parent );
				return;
			}
			
			std::set< std::string > childIds = parent->getSpecNode()->getChildrenNodes();
			for( ShPtrElement child : parent->getChildren() )
			{
				for( std::string id : childIds )
					if( child->_id == id && ( child->_status == eStatusValid || child->_status == eStatusUnknown || child->_isOptional || ! isRequirementValid( element ) ) )
						childIds.erase( id );
			}
			
			if( childIds.size() )
			{
				LOG_TRACE( "[checker] " << element->_id << ": End of unordered group, remaining children: " << childIds.size() );
				parent->_status = eStatusInvalid;
				checkSpecialCases( parent, parent->_status );
				checkRepetition( parent, parent->_status );
				updateParent( parent );
				return;
			}
		}
	}
}

bool Checker::continueRepetition( const ShPtrElement element )
{
	if( element->_repetExpr.empty() )
		return true;
	size_t repetMax = 0;
	for( std::pair< std::string, std::string > repetPair : element->_repetExpr )
	{
		if( repetPair.first != repetPair.second )
			continue;
		repetMax = std::max( _exprParser->getExpressionResult< size_t >( repetPair.first ), repetMax );
	}

	if( repetMax != 0 && element->_iteration > repetMax )
	{
		LOG_TRACE( "[checker] repetition : " << element->_iteration << " / " << repetMax << ": continue...");
		return false;
	}
	return true;
}

bool Checker::isIterationValid( const ShPtrElement element, std::string& errorMessage )
{
	if( element->_repetExpr.empty() )
		return true;

	std::stringstream error;
	error << element->_id << ": ";
	ShPtrElement parent = element->getParent();
	ShPtrElement lastIterationElement = nullptr;

	if( parent == nullptr )
	{
		ShPtrElement previous = element->getPrevious();
		while( previous != nullptr )
		{
			if( previous->_id == element->_id )
			{
				lastIterationElement = previous;
				break;
			}
			previous = previous->getParent();
		}
	}
	else
	{
		size_t maxIteration = 0;
		for( ShPtrElement child : parent->getChildren() )
		{
			if( child->_id == element->_id && child->_status == eStatusValid && child->_iteration > maxIteration )
			{
				lastIterationElement = child;
				maxIteration = lastIterationElement->_iteration;
			}
		}
	}

	if( lastIterationElement == nullptr )
		return false;

	for( std::pair< std::string, std::string > repetPair : lastIterationElement->_repetExpr )
	{
		if( repetPair.first == repetPair.second )
		{
			size_t repetNumber = _exprParser->getExpressionResult< size_t >( repetPair.first );
			LOG_TRACE( "[checker] repetition : " << lastIterationElement->_iteration << " / " << repetNumber );
			if( lastIterationElement->_iteration == repetNumber )
				return true;
			error << lastIterationElement->_iteration << " / " << repetNumber;
		}
		else
		{
			size_t repetMin = 0;
			size_t repetMax = 0;

			if( repetPair.first.size() )
				repetMin = _exprParser->getExpressionResult< size_t >( repetPair.first );
			if( repetPair.second.size() )
				repetMax = _exprParser->getExpressionResult< size_t >( repetPair.second );

			LOG_TRACE( "[checker] repetitions : " << lastIterationElement->_iteration << " / [" << repetMin << ", " << repetMax << "]" );

			if( repetMin <= lastIterationElement->_iteration )
				if( repetMax == 0 || repetMax >= lastIterationElement->_iteration )
					return true;
			error << lastIterationElement->_iteration << " / [" << repetMin << ", " << repetMax << "]";
		}
	}
	errorMessage = "[checker] Out of repetition range (" + error.str() + ") ";
	return false;
}

}

