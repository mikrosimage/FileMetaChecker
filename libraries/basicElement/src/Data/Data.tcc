#include "Data.hpp"

#include <Number/Number.hpp>

#include <iostream>

namespace basic_element
{
namespace data_element
{

Data::Data( const std::string& id, const ESubType& subType, const EDisplayType& dispType )
	: Element ( id, eTypeData, subType, dispType )
	, _data   ( NULL )
{
}

Data::~Data()
{
	if( _data != NULL )
	{
		delete[] _data;
		_data = NULL;
	}
}

void Data::setData( const char* data, const size_t& size )
{
	_data = new char [size];
	_size = size;
	
	getEndianOrderedData( _data, data );
}

size_t Data::getSize() const
{
	return _size;
}

template< EDisplayType DisplayType, typename OutputType >
OutputType Data::get() const
{
	if( _data == NULL )
		throw std::runtime_error( "Undefined data" );
	return _data;
}

template< >
std::string Data::get< eDisplayTypeRaw, std::string >() const
{
	if( _data == NULL )
		return "";
	
	std::string data;
	for( size_t i = 0; i < _size; i++ )
		data.push_back( _data[i] );
	return data;
}

template< >
std::string Data::get< eDisplayTypeAscii, std::string >() const
{
	if( _data == NULL )
		return "";

	std::stringstream sstr;
	for (size_t i = 0; i < _size; ++i)
		sstr << _data[i];
	//BE_LOG_TRACE( " Data: \tTO STRING (Ascii): " << sstr.str() );
	return sstr.str();
}

template< >
std::string Data::get< eDisplayTypeHexa, std::string >() const
{
	if( _data == NULL )
		return "";

	std::stringstream sstr;
	char* buffer = new char [ _size ];
	std::memcpy( buffer, _data, _size );
	for( size_t i = 0; i < _size; ++i )
	{
		sstr << std::hex << std::setfill('0') << std::setw(2) << (int)(unsigned char)buffer[i];
	}
	delete[] buffer;
	//BE_LOG_TRACE( " Data: \tTO STRING (Hexa): " << sstr.str() );
	return sstr.str();
}

template< typename NumberType >
std::vector< NumberType > Data::convertToVector() const
{
	std::vector< NumberType > vector;
	for( size_t i = 0; i < _size; i += sizeof( NumberType ) )
	{
		number_element::Number<NumberType> tmpNumber( "" );
		tmpNumber.setData( &_data[i], sizeof( NumberType ) );
		vector.push_back( tmpNumber.getValue() );
		//BE_LOG_TRACE( " Data: \tTO INT VECTOR : " << vector.at( i ) );
	}
	return vector;
}

template< >
std::vector< number_element::uint8 > Data::get< eDisplayTypeNumbers, std::vector< number_element::uint8 > >() const
{
	return convertToVector< number_element::uint8 >();
}

template< >
std::vector< number_element::int8 > Data::get< eDisplayTypeNumbers, std::vector< number_element::int8 > >() const
{
	return convertToVector< number_element::int8 >();
}

template< >
std::vector< number_element::uint16 > Data::get< eDisplayTypeNumbers, std::vector< number_element::uint16 > >() const
{
	return convertToVector< number_element::uint16 >();
}

template< >
std::vector< number_element::int16 > Data::get< eDisplayTypeNumbers, std::vector< number_element::int16 > >() const
{
	return convertToVector< number_element::int16 >();
}

template< >
std::vector< number_element::uint32 > Data::get< eDisplayTypeNumbers, std::vector< number_element::uint32 > >() const
{
	return convertToVector< number_element::uint32 >();
}

template< >
std::vector< number_element::int32 > Data::get< eDisplayTypeNumbers, std::vector< number_element::int32 > >() const
{
	return convertToVector< number_element::int32 >();
}

template< >
std::vector< number_element::uint64 > Data::get< eDisplayTypeNumbers, std::vector< number_element::uint64 > >() const
{
	return convertToVector< number_element::uint64 >();
}

template< >
std::vector< number_element::int64 > Data::get< eDisplayTypeNumbers, std::vector< number_element::int64 > >() const
{
	return convertToVector< number_element::int64 >();
}

template< >
std::vector< float > Data::get< eDisplayTypeNumbers, std::vector< float > >() const
{
	return convertToVector< float >();
}

template< >
std::vector< double > Data::get< eDisplayTypeNumbers, std::vector< double > >() const
{
	return convertToVector< double >();
}

template< >
std::vector< number_element::ieeeExtended > Data::get< eDisplayTypeNumbers, std::vector< number_element::ieeeExtended > >() const
{
	return convertToVector< number_element::ieeeExtended >();
}

void Data::setSpecData( const std::string& specValue )
{
	_specValue = specValue;
	_size = ( _subType != eSubTypeHexa ) ? _specValue.size() : 0.5 * _specValue.size() ;
}

void Data::setSpecData( const std::vector< std::string >& specValues )
{
	_specValues = specValues;
	size_t size = 0;
	for( std::string value : _specValues )
	{
		if( size != 0 && size != value.size() )
			throw std::runtime_error( "Specification Error: Multiple values must have the same size" );
		size = value.size();
	}
	_size = ( _subType != eSubTypeHexa ) ? size : 0.5 * size;
}

Element::EStatus Data::checkData()
{
	if( _size == 0 )
		LOG_WARNING( getId() << ": Null data size !" );

	if( _specValue.empty() && _specValues.empty() )
	{
		setStatus( eStatusPassOver );
		return eStatusPassOver;
	}

	_status = eStatusInvalid;
	switch( _subType )
	{
		case eSubTypeAscii :
		{
			if( ! _specValue.empty() && _specValue == get<eDisplayTypeAscii, std::string>() )
				_status = eStatusValid;
			if( ! _specValues.empty() )
			{
				for( std::string value : _specValues )
					if( value == get<eDisplayTypeAscii, std::string>() )
						_status = eStatusValid;
			}
			return _status;
		}

		case eSubTypeHexa :
		{
			if( ! _specValue.empty() && _specValue == get<eDisplayTypeHexa,  std::string>()  )
				_status = eStatusValid;
			if( ! _specValues.empty() )
			{
				for( std::string value : _specValues )
					if( value == get<eDisplayTypeHexa,  std::string>() )
						_status = eStatusValid;
			}
			return _status;
		}

		case eSubTypeRaw :
		{
			_status = eStatusPassOver;
			return _status;
		}
		
		case eSubTypeUnknown :
		default:
		{
			_status = eStatusUnknown;
			return _status;
		}
	}
	return _status;
}

std::vector< std::pair< std::string, std::string > > Data::getElementInfo()
{
	std::vector< std::pair< std::string, std::string > > elemInfo;
	std::vector< std::pair< std::string, std::string > > commonInfo = getCommonElementInfo();
	
	elemInfo.insert( elemInfo.end(), commonInfo.begin(), commonInfo.end() );

	elemInfo.push_back( std::make_pair( kType, getSubType<std::string>() ) );
	switch( _displayType )
	{
		case eDisplayTypeDefault : elemInfo.push_back( std::make_pair( kData, get<eDisplayTypeRaw,     std::string>() ) ); break;
		case eDisplayTypeAscii   : elemInfo.push_back( std::make_pair( kData, get<eDisplayTypeAscii,   std::string>() ) ); break;
		case eDisplayTypeHexa    : elemInfo.push_back( std::make_pair( kData, get<eDisplayTypeHexa,    std::string>() ) ); break;
		case eDisplayTypeRaw     : elemInfo.push_back( std::make_pair( kData, get<eDisplayTypeRaw,     std::string>() ) ); break;
		case eDisplayTypeNumbers :
		{
			std::stringstream out;
			
			std::vector<number_element::uint8> numbers = get< eDisplayTypeNumbers, std::vector<number_element::uint8> >();

			for( auto number : numbers )
			{
				out << number << ", ";
			}
			char toDel[2];
			out >> toDel;
					
			//copy( numbers.begin(), numbers.end(), std::ostream_iterator< std::string >( out, ", " ) );
			elemInfo.push_back( std::make_pair( kData, out.str() ) );
			break;
		}
	}
	return elemInfo;
}

Data& Data::operator=( const Data& other )
{
	if( this != &other )
	{
		this->setData( other.get<eDisplayTypeRaw, char*>(), other.getSize() );
	}
	return *this;
}

}
}
