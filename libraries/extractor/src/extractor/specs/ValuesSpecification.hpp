#ifndef _EXTRACTOR_SPECS_VALUE_SPECIFICATION_HPP_
#define _EXTRACTOR_SPECS_VALUE_SPECIFICATION_HPP_

#include <common/global.hpp>
#include <extractor/expressionParser/ExpressionParser.hpp>

#include <boost/foreach.hpp>

template<typename NumberType>
NumberType getValueFromString( const std::string& string )
{
	return atoi( string.c_str() );
}

template< >
float getValueFromString<float>( const std::string& string )
{
	return atof( string.c_str() );
}

template< >
double getValueFromString<double>( const std::string& string )
{
	return atof( string.c_str() );
}

template<typename NumberType>
inline bool isEqual( const NumberType& val1, const NumberType& val2 )
{
	return val1 == val2;
}

template< >
inline bool isEqual<float>( const float& val1, const float& val2 )
{
	return val1 == val2 || ( std::isnan( val1 ) && std::isnan( val2 ) );
}

template< >
inline bool isEqual<double>( const double& val1, const double& val2 )
{
	return val1 == val2 || ( std::isnan( val1 ) && std::isnan( val2 ) );
}

template< typename Type >
std::string getMap( SubSpec& subSpec, std::map< Type, std::string >& map, const size_t size, const bool isBigEndian, const Type value )
{
	std::string description = "";
	if( boost::optional< const Spec& > mapNode = subSpec.second.get_child_optional( kMap ) )
	{
		BOOST_FOREACH( SubSpec& m, mapNode.get() )
		{
			std::string index = m.second.ordered_begin()->first;
			try
			{
				Type i = getValueFromString< Type >( index );
				if( isEqual( value, i ) )
				{
					description = m.second.get< std::string >( index );
				}
			}
			catch(...)
			{
				LOG_WARNING( "Can not convert index of map: " << index );
			}
		}
	}
	return description;
}

template< typename Type >
std::vector< Type > getMultipleValues( SubSpec& subSpec, const std::string& nodeName )
{
	std::vector< Type > vector;
	if( boost::optional< const Spec& > valuesNode = subSpec.second.get_child_optional( nodeName ) )
	{
		std::string stringValue = subSpec.second.get< Type >( nodeName );

		if( stringValue.length() == 0 )
		{
			BOOST_FOREACH( SubSpec& value, valuesNode.get( ) )
			{
				vector.push_back( value.second.get_value< Type >() );
			}
		}
		else
		{
			vector.push_back( stringValue );
		}
	}
	return vector;
}

std::vector< size_t > getRepetition( SubSpec& subSpec, ElementsMap& _headerElements )
{
	std::vector< size_t > nodeRepetition;
	if( boost::optional< const Spec& > repetitionNode = subSpec.second.get_child_optional( kRepetition ) )
	{	
		boost::optional< size_t > repeated = subSpec.second.get_optional< size_t >( kRepetition );
		// LOG_INFO( " >>> repeated : " << repeated );
		if( repeated == NULL )
		{
			boost::optional< double > repeatedeDouble = subSpec.second.get_optional< double >( kRepetition );
			if( repeatedeDouble != NULL )
			{
				if( repeatedeDouble < 0 )
					throw std::runtime_error( "Value Error : 'repeated' value must be strictly positive ( " + subSpec.second.get< std::string > ("id") + " )." );
				throw std::runtime_error( "Value Error : 'repeated' value must be a positive integer ( " + subSpec.second.get< std::string > ("id") + " )." );
			}

			boost::optional< std::string > repeatedeExpr = subSpec.second.get_optional< std::string >( kRepetition );
			// LOG_INFO( " >>> repeatedeExpr :" << repeatedeExpr );
			if( repeatedeExpr != NULL )
			{
				std::string repeatedeExpr = subSpec.second.get< std::string >( kRepetition );
				if( !repeatedeExpr.empty() )
				{
					// LOG_INFO( " --- CASE EXPRESSION --- " );
					ExpressionParser repetition = ExpressionParser();
					repetition.setVariables( _headerElements );
					size_t repetitionValue = repetition.parseExpression<size_t>( repeatedeExpr );
					nodeRepetition.push_back( repetitionValue );
					nodeRepetition.push_back( repetitionValue );
				}
				else
				{
					// LOG_INFO( " --- CASE MULTIPLE --- " );
					BOOST_FOREACH( SubSpec& m, repetitionNode.get() )
					{
						nodeRepetition.clear();
						if( !m.second.get_child_optional( kMin ) && !m.second.get_child_optional( kMax ) )
							throw std::runtime_error( "in 'repeated': range must contain at least 1 value : 'min' or/and 'max' ( " + subSpec.second.get< std::string > ("id") + " )." );

						size_t min;
						size_t max;
						if( m.second.get_child_optional( kMin ) )
						{
							if( m.second.get< long >( kMin ) < 1 )
								throw std::runtime_error( "Value Error in 'repeated': the 'min' value must be strictly positive ( " + subSpec.second.get< std::string > ("id") + " )." );
							min = m.second.get< size_t >( kMin );
						}
						else
						{
							min = 1;
						}

						if( m.second.get_child_optional( kMax ) )
						{
							if( m.second.get< long >( kMax ) < 1 )
								throw std::runtime_error( "Value Error in 'repeated': the 'max' value must be strictly positive ( " + subSpec.second.get< std::string > ("id") + " )." );
							max = m.second.get< size_t >( kMax );
						}
						else
						{
							max = std::numeric_limits<size_t>::max();
						}

						if( min > max )
							throw std::runtime_error( "Value Error in 'repeated': the 'max' value must be superior to the 'min' one ( " + subSpec.second.get< std::string > ("id") + " )." );

						nodeRepetition.push_back( min );
						nodeRepetition.push_back( max );
					}
				}
			}
		}
		else
		{
			// LOG_INFO( " --- CASE VALUE --- " );
			nodeRepetition.clear();
			if( subSpec.second.get< long >( kRepetition ) < 1 )
				throw std::runtime_error( "Value Error : 'repeated' value must be strictly positive ( " + subSpec.second.get< std::string > ("id") + " )." );

			size_t repeatedValue = subSpec.second.get< size_t >( kRepetition );
			nodeRepetition.push_back( repeatedValue );
			nodeRepetition.push_back( repeatedValue );
		}
	}
	else
	{
		// LOG_INFO( " --- DEFAULT CASE --- " );
		nodeRepetition.push_back( 1 );
		nodeRepetition.push_back( 1 );
	}	
	// BOOST_FOREACH( size_t value, nodeRepetition )
	// {
	// 	LOG_INFO( "value : " << value );
	// }
	if( nodeRepetition.size() != 2 )
		throw std::runtime_error( "'repeated' must contain 1 integer value or a 2 integer values range ('min' or/and 'max') ( " + subSpec.second.get< std::string > ("id") + " )." );

	return nodeRepetition;
}

#endif
