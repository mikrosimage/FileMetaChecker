#ifndef _BASIC_ELEMENT_ELEMENT_HPP_
#define _BASIC_ELEMENT_ELEMENT_HPP_

#include <string>
#include "common.hpp"
#include <specDefinition.hpp>

namespace basic_element
{

class Element
{
public:
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
protected:
	void setStatus( const EStatus status );
	
public:
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
	void reverseEndianness   ( char* buffer, const char* data ) const;

protected:
	std::vector< std::pair< std::string, std::string > > getCommonElementInfo();
	std::string getStatusString();

public:
	
	virtual void setData( const char* data, const size_t& size ) = 0;
	virtual void getData( char* buffer ) const = 0;
	virtual EStatus checkData() = 0;
	virtual std::vector< std::pair< std::string, std::string > > getElementInfo() = 0;

private:
	std::string   _id;
	std::string   _label;
	EType         _type;
	size_t        _uniqueId;
	static size_t _lastUniqueId;

protected:
	EStatus       _status;
	int           _subType;
	size_t        _size;
	bool          _bigEndianData;
};

}

#endif
