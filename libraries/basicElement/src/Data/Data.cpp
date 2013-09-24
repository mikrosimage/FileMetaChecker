#include "Data.hpp"

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

void Data::getData( char* buffer ) const
{
	//BE_LOG_TRACE( " Data: \tGET DATA from @ " << &_data << " to @ " << &buffer );
	std::memcpy( buffer, _data, _size );
}

size_t Data::getSize() const
{
	return _size;
}

std::string Data::getAscii() const
{
	if( _data == NULL )
		return "";

	std::stringstream sstr;
	for (size_t i = 0; i < _size; ++i)
		sstr << _data[i];
	//BE_LOG_TRACE( " Data: \tTO STRING (Ascii): " << sstr.str() );
	return sstr.str();
}

std::string Data::getHexa() const
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

std::vector< unsigned int > Data::toIntVector()
{
	std::vector< unsigned int > vector;
	for( size_t i = 0; i < _size; ++i )
	{
		vector.push_back( ( unsigned char ) _data[i] );
		//BE_LOG_TRACE( " Data: \tTO INT VECTOR : " << vector.at( i ) );
	}
	return vector;
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
			if( ! _specValue.empty() && _specValue == getAscii() )
				_status = eStatusValid;
			if( ! _specValues.empty() )
			{
				for( std::string value : _specValues )
					if( value == getAscii() )
						_status = eStatusValid;
			}
			return _status;
		}

		case eSubTypeHexa :
		{
			if( ! _specValue.empty() && _specValue == getHexa()  )
				_status = eStatusValid;
			if( ! _specValues.empty() )
			{
				for( std::string value : _specValues )
					if( value == getHexa() )
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

	elemInfo.push_back( std::make_pair( kType, getStringFromType() ) );
	switch( _displayType )
	{
		case eDisplayTypeDefault : elemInfo.push_back( std::make_pair( kData, getRawDataString() ) ); break;	// @todo _data from 0 to size
		case eDisplayTypeAscii   : elemInfo.push_back( std::make_pair( kData, getAscii()         ) ); break;
		case eDisplayTypeHexa    : elemInfo.push_back( std::make_pair( kData, getHexa()          ) ); break;
	}
	return elemInfo;
}

Data& Data::operator=( const Data& other )
{
	if( this != &other )
	{
		this->setData( other.getDataPtr(), other.getSize() );
	}
	return *this;
}

std::string Data::getStringFromType()
{
	switch( _subType )
	{
		case eSubTypeAscii   : return kAscii;
		case eSubTypeHexa    : return kHexa;
		case eSubTypeRaw     : return kRaw;
		case eSubTypeUnknown :
		default :
			return kUnknown;
	}
	return kUnknown;
}

std::string Data::getStringFromData()
{
	switch( _subType )
	{
		case eSubTypeAscii   : return getAscii();
		case eSubTypeHexa    : return getHexa();
		case eSubTypeRaw     : return getRawDataString();
		case eSubTypeUnknown :
		default :
			return "";
	}
	return "";
}

std::string Data::getRawDataString()
{
	std::string data;
	for( size_t i = 0; i < _size; i++ )
		data.push_back( _data[i] );
	return data;
}

char* Data::getDataPtr( ) const
{
	return _data;
}

}
}
