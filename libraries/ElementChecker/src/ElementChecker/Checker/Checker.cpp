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

	// if nothing to compare
	if( element->_values.empty() && element->_rangeExpr.empty() && element->_map.empty() )
	{
		element->_status = eStatusPassOver;
		_elementList.push_back( element );
		return;
	}

	// element check :
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

	// optional case : do nothing
	if( element->_isOptional && status == eStatusInvalid && element->_iteration == 1 )
	{
		LOG_ERROR( "CHECKER: " << element->_id << ": is Optional" );
		element->_status = status;
		return;
	}

	std::shared_ptr< basic_element::Element > parent = element->getParent();

	// end of unordered group : check iterations
	if( parent != nullptr && ! parent->_isOrdered && status == eStatusInvalid )
	{
		LOG_ERROR( "CHECKER: " << element->_id << ": Unordered group" );
		element->_status = eStatusInvalidButSkip;

		if( element->getSpecNode()->next() == nullptr )
		{
			LOG_ERROR( "CHECKER: " << element->_id << ": Last element" );
			// create a list with the children IDs
			std::set< std::string > childIds = parent->getSpecNode()->getChildrenNodes();
			// if the previous element exists (the current element is not the first)
			if( element->getPrevious() == nullptr )
				throw std::runtime_error( "Checker: Invalid tree" );
			// creates a pointer to the previous element
			std::shared_ptr< basic_element::Element > prev = element->getPrevious();
			// while the previous element is not the parent
			while( prev->_id != parent->_id )
			{
				// for each ID in the children ID list
				for( auto id : childIds )				
				{
					// if the previous element's ID is equal to one ID of the list
					if( prev->_id == id && prev->_status == eStatusValid )
					{
						LOG_ERROR( "CHECKER: >>> childIds: " << id );
						// erase this ID from the list if repetitions ok !
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
				// go to the previous element of the previous, etc..
				prev = prev->getPrevious();
			}
			LOG_ERROR( "CHECKER: " << element->_id << ": End of unordered group, remaining children: " << childIds.size() );
			// if it remains some IDs in the list
			if( childIds.size() != 0 )
			{
				LOG_ERROR( "CHECKER: " << parent->_id );
				LOG_ERROR( "CHECKER: " << &*parent );
				// every nodes haven't been checked : the parent is not valid
				parent->_status = eStatusInvalidForUnordered;
			}
		}
		LOG_ERROR( "CHECKER: " << element->_id << "'s status: " << element->_status );
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
		LOG_ERROR( "CHECKER: " << element->_id << "'s status: eStatusInvalid" );
		return;
	}

	LOG_ERROR( "CHECKER: " << element->_id << "'s status: " << status );
	_elementList.push_back( element );

	// check GroupSize
	if( parent != nullptr && element->getPrevious() != nullptr && element->getSpecNode()->next() == nullptr && ! parent->_groupSizeExpr.empty() )
	{
		std::shared_ptr< basic_element::Element > prev = element->getPrevious();
		size_t groupSize = element->_size;
		while( prev != nullptr || prev->_id != parent->_id )
		{
			groupSize += prev->_size;
			// LOG_FATAL( "ELEMENT: prev: " << prev->_id << " size: " << prev->_size << " | " << groupSize );
			if( prev->_id == parent->_id )
				break;
			prev = prev->getPrevious();
		}
		ExpressionParser groupSizeParser( _elementList );
		size_t parentGroupSize = groupSizeParser.getExpressionResult< size_t >( parent->_groupSizeExpr );
		// LOG_FATAL( "parentGroupSize: " << parentGroupSize );

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

size_t Checker::getSize( const std::shared_ptr< basic_element::Element > element )
{
	if( ! element->_countExpr.empty() )
	{
		ExpressionParser sizeParser( _elementList );
		element->_size = sizeParser.getExpressionResult< size_t >( element->_countExpr );
		LOG_ERROR( "COUNT: " << element->_id << "'s size: " << element->_size );
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


}

