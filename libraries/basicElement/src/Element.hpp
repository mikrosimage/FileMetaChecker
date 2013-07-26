#ifndef _BASIC_ELEMENT_ELEMENT_HPP_
#define _BASIC_ELEMENT_ELEMENT_HPP_

#include <string>
#include "common.hpp"

namespace basic_element
{

class Element
{
public:
	enum EType
	{
		eTypeUnknown = 0,
		eTypeNumber,
		eTypeExif,
		eTypeData,
		eTypeKlv
	};

	enum ENumberType
	{
		eNumberTypeUnknown = 0,
		eNumberTypeInt8,
		eNumberTypeUInt8,
		eNumberTypeInt16,
		eNumberTypeUInt16,
		eNumberTypeInt32,
		eNumberTypeUInt32,
		eNumberTypeInt64,
		eNumberTypeUInt64,
		eNumberTypeFloat,
		eNumberTypeDouble,
		eNumberTypeIeeeExtended
	};

	enum EDataType
	{
		eDataTypeUnknown = 0,
		eDataTypeAscii,
		eDataTypeHexa,
		eDataTypeRaw
	};

	enum EStatus
	{
		eStatusUnknown = 0,
		eStatusValid,
		eStatusInvalid,
		eStatusPassOver
	};

	Element( EType type );

	~Element()
	{
	}

	void setId   ( const std::string& id );
	void setLabel( const std::string& label );

	std::string getId()       { return _id; }
	std::string getLabel()    { return _label; }
	EType       getType()     { return _type; }
	
	ENumberType getNumberSubType();
	EDataType   getDataSubType();

	EStatus     getStatus()   { return _status; }
	size_t      getUniqueId() { return _uniqueId; }

	void setBigEndianness( bool isBigEndian );
	bool getBigEndianness() const;

	void getEndianOrderedData( char* buffer, const char* data ) const;
	void reverseEndianness( char* buffer, const char* data ) const;
	
	virtual void setData( const char* data, const size_t& size ) = 0;
	virtual void getData( char* buffer ) const = 0;
	virtual EStatus checkData() = 0;

	// static EType getType( const std::string& type );		// @todo

protected:
	void setStatus( const EStatus status );

private:
	std::string   _id;
	std::string   _label;
	EType         _type;
	EStatus       _status;
	size_t        _uniqueId;
	static size_t _lastUniqueId;

protected:
	int    _subType;
	size_t _size;
	bool   _isBigEndian;
};

}

#endif
