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
		EStatus status = eStatusInvalid;
		
		if( element->_rangeExpr.empty() )
			status = eStatusPassOver;

		NumberType value = Translator( element ).get< NumberType >();
		
		if( Ranges< NumberType >( element->_rangeExpr ).isInRanges( value ) )
			status = eStatusValid;
		
		element->_mapValue = Map< NumberType >( element->_map ).getLabel( value );
		
		return status;
	}
}

Checker::Checker()
	: _exprParser( new ExpressionParser )
{
}

void Checker::check( const ShPtrElement element )
{
	if( element->_size == 0 && element->_countExpr.empty() )
	{
		LOG_WARNING( "[checker] " << element->_id << ": Null data size !" );
	}
	
	element->_dispValue = Translator( element ).get( element->_displayType );

	ShPtrElement parent   = element->getParent();
	ShPtrElement previous = element->getPrevious();

	if( parent != nullptr && previous != nullptr && element->getSpecNode()->next() == nullptr && ! parent->_groupSizeExpr.empty() )
	{
		parent->_groupSize = _exprParser->getExpressionResult< size_t >( parent->_groupSizeExpr );
		// LOG_TRACE( "Set " << element->_id << "'s parent (" << parent->_id << ") groupSize (" << parent->_groupSizeExpr << "): " << parent->_groupSize );
	}

	// if nothing to compare
	if( element->_values.empty() && element->_rangeExpr.empty() && element->_map.empty() )
	{
		element->_status = eStatusPassOver;
		_exprParser->addElementToContext( element );
		return;
	}

	EStatus status = eStatusInvalid;

	switch( element->_type )
	{
		case eTypeUnknown      : LOG_ERROR( "[checker] Unknown element type, cannot check it" ); break;
		
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
		case eTypeHexa  :
		{
			status = eStatusInvalid;
			std::string orig = Translator( element ).get();
			std::string lowCase = orig;
			std::string upCase  = orig;
			std::transform( lowCase.begin(), lowCase.end(), lowCase.begin(), ::tolower );
			std::transform(  upCase.begin(),  upCase.end(),  upCase.begin(), ::toupper );

			for( std::string value : element->_values )
				if( value == orig || value == lowCase || value == upCase )
					status = eStatusValid;
			
			if( status == eStatusInvalid )
				element->_error += "Invalid value - ";
			break;
		}

		case eTypeRaw   :
		{
			status = eStatusPassOver;
			break;
		}

		case eTypeExif :
		{
			break;
		}
		case eTypeKlv  :
		{
			break;
		}
	}

	if( ! isRequirementValid( element ) )
	{
		LOG_INFO( "[checker] " << element->_id << " : requirement not valid -> skipped" );
		element->_status = eStatusSkip;
		_exprParser->addElementToContext( element );
		return;
	}

	if( element->_isOptional && status == eStatusInvalid && element->_iteration == 1 )
	{
		LOG_INFO( "[checker] " << element->_id << " : invalid but optional -> invalid but skipped" );
		element->_status = eStatusInvalidButOptional;
		_exprParser->addElementToContext( element );
		return;
	}

	if( parent != nullptr && ! parent->_isOrdered && status == eStatusInvalid )
	{
		LOG_INFO( "[checker] " << element->_id << " : unordered group -> invalid but skipped" );
		element->_status = eStatusInvalidButSkip;
		if( element->getSpecNode()->next() == nullptr )
			checkLastUnorderedElement( element );
		return;
	}

	element->_status = status;

	// if element invalid and repeated : checked if iterations valid
	if( status == eStatusInvalid && ! element->_repetExpr.empty() )
	{
		LOG_TRACE( "[checker] check Repetitions" );
		element->_status = eStatusInvalidButSkip;
		std::string errorMessage;
		ShPtrElement previous = element->getPrevious();
		if( ! isIterationValid( previous, errorMessage ) )
		{
			LOG_ERROR( "[checker] " << element->_id << ": " << errorMessage );
			element->_error += errorMessage;
			_exprParser->addElementToContext( element );
			element->_status = eStatusInvalidForIteration;
		}
		LOG_TRACE( "[checker] repetitions " << previous );
		return;
	}

	LOG_INFO( "[checker] " << element->_id << " : return status = " << status );
	_exprParser->addElementToContext( element );

}

size_t Checker::getSize( const ShPtrElement element )
{
	if( ! element->_countExpr.empty() && element->_size == 0 )
	{
		element->_size = _exprParser->getExpressionResult< size_t >( element->_countExpr );
		//LOG_TRACE( "COUNT: " << element->_id << "'s size: " << element->_size );
	}
	if( element->_type == eTypeHexa )
		return element->_size >> 1;
	return element->_size;
}

bool Checker::isIterationValid( const ShPtrElement element, std::string& errorMessage )
{
	if( element->_repetExpr.empty() )
		return true;

	std::stringstream error;
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
	errorMessage = "[checker] Out of repetition range (" + error.str() + ") - ";
	return false;
}

bool Checker::isRequirementValid( const ShPtrElement element )
{
	if( element->_requiredExpr.empty() )
		return true;

	bool requirement = _exprParser->getExpressionResult< bool >( element->_requiredExpr );
	return requirement;
}

void Checker::checkLastUnorderedElement( const ShPtrElement element )
{
	//LOG_TRACE( "[checker] " << element->_id << ": Last element " << &*element << " <-" << &*element->getPrevious() );
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
					prev->_error += errorMessage;
					_exprParser->addElementToContext( prev );
					prev->getParent()->_status = eStatusInvalidGroupForIteration;
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
		parent->_status = eStatusInvalidForUnordered;
	}
	else
	{
		LOG_TRACE( "[checker] " << element->_id << ": End of unordered group, remaining children: " << childIds.size() );
	}
}

}

