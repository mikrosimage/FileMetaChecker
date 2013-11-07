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

Checker::Checker()
{
}

void Checker::check( const std::shared_ptr< basic_element::Element > element )
{
	if( element->_size == 0 && element->_countExpr.empty() )
		LOG_WARNING( element->_id << ": Null data size !" );

	element->_dispValue = Translator( element ).get( element->_displayType );

	// if nothing to compare
	if( element->_values.empty() && element->_rangeExpr.empty() && element->_map.empty() )
	{
		element->_status = eStatusPassOver;
		_elementList.push_back( element );
		return;
	}

	EStatus status = eStatusPassOver;
	if( ! element->_rangeExpr.empty() )
		status = eStatusInvalid;

	switch( element->_type )
	{
		case eTypeUnknown : LOG_ERROR( "Unknown element type, cannot check it" ); break;
		case eTypeInt8 :
		{
			be::int8 value = Translator( element ).get< be::int8 >();
			if( Ranges< be::int8 >( element->_rangeExpr ).isInRanges( value ) )
				status = eStatusValid;
			element->_mapValue = Map< be::int8 >( element->_map ).getLabel( value );
			break;
		}
		case eTypeUInt8 :
		{
			be::uint8 value = Translator( element ).get< be::uint8 >();
			if( Ranges< be::uint8 >( element->_rangeExpr ).isInRanges( value ) )
				status = eStatusValid;
			element->_mapValue = Map< be::uint8 >( element->_map ).getLabel( value );
			break;
		}
		case eTypeInt16 :
		{
			be::int16 value = Translator( element ).get< be::int16 >();
			if( Ranges< be::int16 >( element->_rangeExpr ).isInRanges( value ) )
				status = eStatusValid;
			element->_mapValue = Map< be::int16 >( element->_map ).getLabel( value );
			break;
		}
		case eTypeUInt16 :
		{
			be::uint16 value = Translator( element ).get< be::uint16 >();
			if( Ranges< be::uint16 >( element->_rangeExpr ).isInRanges( value ) )
				status = eStatusValid;
			element->_mapValue = Map< be::uint16 >( element->_map ).getLabel( value );
			break;
		}
		case eTypeInt32 :
		{
			be::int32 value = Translator( element ).get< be::int32 >();
			if( Ranges< be::int32 >( element->_rangeExpr ).isInRanges( value ) )
				status = eStatusValid;
			element->_mapValue = Map< be::int32 >( element->_map ).getLabel( value );
			break;
		}
		case eTypeUInt32 :
		{
			be::uint32 value = Translator( element ).get< be::uint32 >();
			if( Ranges< be::uint32 >( element->_rangeExpr ).isInRanges( value ) )
				status = eStatusValid;
			element->_mapValue = Map< be::uint32 >( element->_map ).getLabel( value );
			break;
		}
		case eTypeInt64 :
		{
			be::int64 value = Translator( element ).get< be::int64 >();
			if( Ranges< be::int64 >( element->_rangeExpr ).isInRanges( value ) )
				status = eStatusValid;
			element->_mapValue = Map< be::int64 >( element->_map ).getLabel( value );
			break;
		}
		case eTypeUInt64 :
		{
			be::uint64 value = Translator( element ).get< be::uint64 >();
			if( Ranges< be::uint64 >( element->_rangeExpr ).isInRanges( value ) )
				status = eStatusValid;
			element->_mapValue = Map< be::uint64 >( element->_map ).getLabel( value );
			break;
		}
		case eTypeFloat :
		{
			float value = Translator( element ).get< float >();
			if( Ranges< float >( element->_rangeExpr ).isInRanges( value ) )
				status = eStatusValid;
			element->_mapValue = Map< float >( element->_map ).getLabel( value );
			break;
		}
		case eTypeDouble :
		{
			double value = Translator( element ).get< double >();
			if( Ranges< double >( element->_rangeExpr ).isInRanges( value ) )
				status = eStatusValid;
			element->_mapValue = Map< double >( element->_map ).getLabel( value );
			break;
		}
		case eTypeIeeeExtended :
		{
			be::ieeeExtended value = Translator( element ).get< be::ieeeExtended >();
			if( Ranges< be::ieeeExtended >( element->_rangeExpr ).isInRanges( value ) )
				status = eStatusValid;
			element->_mapValue = Map< be::ieeeExtended >( element->_map ).getLabel( value );
			break;
		}

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
		LOG_TRACE( "CHECKER: (" << element->_id << ") requirement not valid, element skipped" );
		element->_isOptional = true;
		element->_status = eStatusSkip;
		return;
	}

	if( element->_isOptional && status == eStatusInvalid && element->_iteration == 1 )
	{
		LOG_TRACE( "CHECKER: " << element->_id << ": is Optional" );
		element->_status = status;
		return;
	}

	std::shared_ptr< basic_element::Element > parent = element->getParent();

	if( parent != nullptr && ! parent->_isOrdered && status == eStatusInvalid )
	{
		LOG_TRACE( "CHECKER: " << element->_id << ": Unordered group" );
		element->_status = eStatusInvalidButSkip;
		if( element->getSpecNode()->next() == nullptr )
			checkLastUnorderedElement( element );
		return;
	}

	element->_status = status;

	// if element invalid and repeated : checked if iterations valid
	if( status == eStatusInvalid && ! element->_repetExpr.empty() )
	{
		std::string errorMessage;
		if( ! isIterationValid( element->getPrevious(), errorMessage ) )
		{
			LOG_ERROR( element->_id << ": " << errorMessage );
			element->_error += errorMessage;
			_elementList.push_back( element );
		}
		return;
	}

	LOG_TRACE( "CHECKER: " << element->_id << "'s status: " << status );
	_elementList.push_back( element );

	if( parent != nullptr && element->getPrevious() != nullptr && element->getSpecNode()->next() == nullptr && ! parent->_groupSizeExpr.empty() )
		checkGroupSize( element );
}

size_t Checker::getSize( const std::shared_ptr< basic_element::Element > element )
{
	if( ! element->_countExpr.empty() && element->_size == 0 )
	{
		ExpressionParser sizeParser( _elementList );
		element->_size = sizeParser.getExpressionResult< size_t >( element->_countExpr );
		LOG_TRACE( "COUNT: " << element->_id << "'s size: " << element->_size );
	}
	return element->_size;
}

bool Checker::isIterationValid( const std::shared_ptr< basic_element::Element > element, std::string& errorMessage )
{
	if( element->_repetExpr.empty() )
		return true;

	std::stringstream error;
	for( std::pair< std::string, std::string > repetPair : element->_repetExpr )
	{
		if( repetPair.first == repetPair.second )
		{
			ExpressionParser repetParser( _elementList );
			size_t repetNumber = repetParser.getExpressionResult< size_t >( repetPair.first );
			LOG_ERROR( "CHECKER: repetition : " << element->_iteration << " / " << repetNumber );
			if( element->_iteration == repetNumber )
				return true;
			error << element->_iteration << " / " << repetNumber;
		}
		else
		{
			ExpressionParser repetParser( _elementList );
			size_t repetMin = 0;
			size_t repetMax = 0;

			if( ! repetPair.first.empty() )
				repetMin = repetParser.getExpressionResult< size_t >( repetPair.first );
			if( ! repetPair.second.empty() )
				repetMax = repetParser.getExpressionResult< size_t >( repetPair.second );

			LOG_ERROR( "CHECKER: repetitions : " << element->_iteration << " / [" << repetMin << ", " << repetMax << "]" );
			if( repetMin <= element->_iteration )
				if( repetMax == 0 || repetMax >= element->_iteration )
					return true;
			error << element->_iteration << " / [" << repetMin << ", " << repetMax << "]";
		}
	}
	errorMessage = "Out of repetition range (" + error.str() + ") - ";
	return false;
}

bool Checker::isRequirementValid( const std::shared_ptr< basic_element::Element > element )
{
	if( element->_requiredExpr.empty() )
		return true;

	ExpressionParser conditionParser( _elementList );
	bool requirement = conditionParser.getExpressionResult< bool >( element->_requiredExpr );
	return requirement;
}

void Checker::checkLastUnorderedElement( const std::shared_ptr< basic_element::Element > element )
{
	LOG_TRACE( "CHECKER: " << element->_id << ": Last element" );
	if( element->getPrevious() == nullptr )
		throw std::runtime_error( "Checker: Invalid tree" );

	std::shared_ptr< basic_element::Element > parent = element->getParent();
	std::shared_ptr< basic_element::Element > prev   = element->getPrevious();
	std::set< std::string > childIds = parent->getSpecNode()->getChildrenNodes();

	while( prev->_id != parent->_id )
	{
		for( std::string id : childIds )				
		{
			if( prev->_id == id && prev->_status == eStatusValid )
			{
				LOG_TRACE( "CHECKER: >>> childIds: " << id );

				std::string errorMessage;
				if( ! prev->_repetExpr.empty() && ! isIterationValid( prev, errorMessage ) )
				{
					LOG_ERROR( "(" << prev->_id << ") " << errorMessage );
					prev->_error += errorMessage;
					_elementList.push_back( prev );
					prev->getParent()->_status = eStatusInvalidGroupForIteration;
				}

				childIds.erase( id );
			}
		}
		prev = prev->getPrevious();
	}

	LOG_TRACE( "CHECKER: " << element->_id << ": End of unordered group, remaining children: " << childIds.size() );

	if( childIds.size() != 0 )
		parent->_status = eStatusInvalidForUnordered;
}

void Checker::checkGroupSize( const std::shared_ptr< basic_element::Element > element )
{
	std::shared_ptr< basic_element::Element > parent = element->getParent();
	std::shared_ptr< basic_element::Element > prev   = element->getPrevious();
	size_t groupSize = element->_size;

	while( prev != nullptr || prev->_id != parent->_id )
	{
		groupSize += prev->_size;
		if( prev->_id == parent->_id )
			break;
		prev = prev->getPrevious();
	}

	ExpressionParser groupSizeParser( _elementList );
	size_t parentGroupSize = groupSizeParser.getExpressionResult< size_t >( parent->_groupSizeExpr );

	int sizeDiff = parentGroupSize - groupSize;
	if( sizeDiff != 0 )
	{
		std::stringstream warningMessage;
		if( sizeDiff > 0 )
			warningMessage << "Group size difference: " << sizeDiff << " missing bytes - ";
		if( sizeDiff < 0 )
			warningMessage << "Group size difference: " << abs( sizeDiff ) << " unexpected bytes - ";
		LOG_WARNING( warningMessage.str() );
		parent->_warning += warningMessage.str();
	}
}

}

