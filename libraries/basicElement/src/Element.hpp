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

	Element( const std::string& id, EType type, ESubType subType = eSubTypeUnknown, EDisplayType dispType = eDisplayTypeDefault );

	~Element()
	{
	}

	void setId   ( const std::string& id );
	void setLabel( const std::string& label );
protected:
	void setStatus( const EStatus status );
	
public:
	std::string getId()      const { return _id; }
	std::string getLabel()   const { return _label; }
	EType       getType()    const { return _type; }
	ESubType    getSubType() const { return _subType; }
	
	void setDisplayType( const std::string& displayType );
	
	EStatus     getStatus()   { return _status; }
	size_t      getUniqueId() { return _uniqueId; }

	void setBigEndianness( bool isBigEndian );
	bool getBigEndianness() const;

	void getEndianOrderedData( char* buffer, const char* data ) const;
	void reverseEndianness   ( char* buffer, const char* data ) const;

	void addErrorLabel( const std::string& error );
	void addWarningLabel( const std::string& warning );
	std::string getErrorLabel();
	std::string getWarningLabel();

protected:
	std::vector< std::pair< std::string, std::string > > getCommonElementInfo();
	std::string getStatusString();


public:
	
	virtual void setData( const char* data, const size_t& size ) = 0;
	virtual void getData( char* buffer ) const = 0;
	virtual EStatus checkData() = 0;
	virtual std::vector< std::pair< std::string, std::string > > getElementInfo() = 0;
	virtual size_t getSize() const = 0;

protected:
	static size_t _lastUniqueId;
	
	std::string   _id;
	std::string   _label;
	std::string   _error;
	std::string   _warning;
	
	size_t        _uniqueId;
	size_t        _size;
	
	EType         _type;
	ESubType      _subType;
	EDisplayType  _displayType;
	
	EStatus       _status;
	
	bool          _bigEndianData;
};

}

#endif
