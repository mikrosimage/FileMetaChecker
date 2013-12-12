#include "Checker.hpp"

#include <ElementChecker/Translator/Translator.hpp>
#include <ElementChecker/Ranges/Ranges.hpp>
#include <ElementChecker/ExpressionParser/ExpressionParser.hpp>
#include <ElementChecker/Map/Map.hpp>
#include <SpecReader/SpecNode.hpp>

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

void Checker::check( const ShPtrElement element )
{
	ShPtrElement parent   = element->getParent();
	ShPtrElement previous = element->getPrevious();
	
	EStatus status = eStatusInvalid;
	
	element->_dispValue = Translator( element ).get( element->_displayType );

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
	
	if( ! isRequirementValid( element ) )
	{
		LOG_TRACE( "[checker] " << element->_id << " : requirement not valid -> skipped" );
		status = eStatusSkip;
	}
	
	if( element->_isOptional && status == eStatusInvalid && element->_iteration == 1 )
	{
		LOG_TRACE( "[checker] " << element->_id << " : invalid but optional -> invalid but skipped" );
		status = eStatusSkip;
	}

	if( parent != nullptr && ! parent->_isOrdered && status == eStatusInvalid )
	{
		LOG_TRACE( "[checker] " << element->_id << " : unordered group -> invalid but skipped" );
		status = eStatusSkip;
	}

	if( ! element->_repetExpr.empty() )
	{
		LOG_TRACE( "[checker] " << element->_id << " : check repetitions" );
		std::string errorMessage;
		if( status == eStatusValid && ! continueRepetition( element ) )
			status = eStatusInvalid;
		if( status == eStatusInvalid )
		{
			status = eStatusSkip;
			ShPtrElement previous = element->getPrevious();
			if( ! isIterationValid( previous, errorMessage ) )
			{
				LOG_ERROR( errorMessage << " (" << element->_id << " )" );
				element->_error.push_back( errorMessage );
				status = eStatusInvalid;
			}
		}
	}

	if( element->getSpecNode()->next() == nullptr && status == eStatusSkip && parent != nullptr && ! parent->_isOrdered )
		checkLastUnorderedElement( element );

	element->_status = status;

	_exprParser->addElementToContext( element );

	setParentGroupSize( element );

	LOG_TRACE( "[checker] " << element->_id << " : return status = " << statusMap.at( status ) );
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

void Checker::setParentGroupSize( const ShPtrElement element )
{
	ShPtrElement parent = element->getParent();

	if( parent == nullptr                         ||
		element->getPrevious() == nullptr         ||
		element->getSpecNode()->next() != nullptr ||
		parent->_groupSizeExpr.empty()            )
		return;
	
	parent->_specGroupSize = _exprParser->getExpressionResult< size_t >( parent->_groupSizeExpr );
	LOG_TRACE( "[checker] set " << element->_id << "'s parent (" << parent->_id << ") groupSize (" << parent->_groupSizeExpr << "): " << parent->_specGroupSize );
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
	for( std::pair< std::string, std::string > repetPair : element->_repetExpr )
	{
		if( repetPair.first == repetPair.second )
		{
			size_t repetNumber = _exprParser->getExpressionResult< size_t >( repetPair.first );
			LOG_TRACE( "[checker] repetition : " << element->_iteration << " / " << repetNumber );
			if( element->_iteration == repetNumber )
				return true;
			error << element->_iteration << " / " << repetNumber;
		}
		else
		{
			size_t repetMin = 0;
			size_t repetMax = 0;

			if( ! repetPair.first.empty() )
				repetMin = _exprParser->getExpressionResult< size_t >( repetPair.first );
			if( ! repetPair.second.empty() )
				repetMax = _exprParser->getExpressionResult< size_t >( repetPair.second );

			LOG_TRACE( "[checker] repetitions : " << element->_iteration << " / [" << repetMin << ", " << repetMax << "]" );
			if( repetMin <= element->_iteration )
				if( repetMax == 0 || repetMax >= element->_iteration )
					return true;
			error << element->_iteration << " / [" << repetMin << ", " << repetMax << "]";
		}
	}
	errorMessage = "[checker] Out of repetition range (" + error.str() + ") ";
	return false;
}

bool Checker::isRequirementValid( const ShPtrElement element )
{
	if( element->_requiredExpr.empty() )
		return true;

	bool requirement = _exprParser->getExpressionResult< bool >( element->_requiredExpr );
	LOG_TRACE(  "[checker] " << element->_id << ": requirement = " << requirement );
	return requirement;
}

void Checker::checkLastUnorderedElement( const ShPtrElement element )
{
	// LOG_COLOR( common::details::kColorMagenta, "checkLastUnorderedElement" << std::endl );
	if( element->getPrevious() == nullptr )
		throw std::runtime_error( "[checker] Invalid tree" );

	ShPtrElement parent = element->getParent();
	ShPtrElement prev   = element->getPrevious();
	std::set< std::string > childIds = parent->getSpecNode()->getChildrenNodes();

	while( prev->_id != parent->_id )
	{
		for( std::string id : childIds )
		{
			if( prev->_id == id && prev->_status == eStatusValid )
			{
				// LOG_TRACE( "[checker] childId's: " << id );
				std::string errorMessage;
				if( ! prev->_repetExpr.empty() && ! isIterationValid( prev, errorMessage ) )
				{
					LOG_ERROR( "(" << prev->_id << ") " << errorMessage );
					prev->_error.push_back( errorMessage );
					_exprParser->addElementToContext( prev );
					prev->getParent()->_status = eStatusInvalid;
				}

				childIds.erase( id );
			}

			for( ShPtrElement child : parent->getChildren() )
			{
				if( child->_id == id && ( child->_isOptional || child->_status == eStatusSkip ) )
				{
					childIds.erase( id );
					break;
				}
			}
		}
		prev = prev->getPrevious();
	}


	if( childIds.size() != 0 )
	{
		LOG_WARNING( "[checker] " << element->_id << ": End of unordered group, remaining children: " << childIds.size() );
		parent->_status = eStatusInvalid;
	}
	else
	{
		LOG_TRACE( "[checker] " << element->_id << ": End of unordered group, remaining children: " << childIds.size() );
	}
}

}

