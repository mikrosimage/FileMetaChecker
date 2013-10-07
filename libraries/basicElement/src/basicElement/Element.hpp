#ifndef _BASIC_ELEMENT_ELEMENT_HPP_
#define _BASIC_ELEMENT_ELEMENT_HPP_

#include <common/global.hpp>

#include <string>
#include <specDefinition.hpp>
#include <SpecNode/SpecNode.hpp>

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
	Element( const spec_reader::SpecNode& node );

	~Element();

	void setLabel( const std::string& label );
	void setDisplayType( const std::string& displayType );
	void setBigEndianness( bool isBigEndian );

protected:
	void setType( const std::string& type );
	void setSubType( const std::string& subType );
	void setStatus( const EStatus status );

public:
	std::string getId()       const { return _id; }
	std::string getLabel()    const { return _label; }
	EType       getType()     const { return _type; }
	size_t      getSize()     const { return _size; }
	bool        isBigEndian() const { return _bigEndianData; }
	EStatus     getStatus()   const { return _status; }
	size_t      getUniqueId() const { return _uniqueId; }
	
	template< typename OutputType = ESubType >
	OutputType getSubType() const;

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
	virtual void set( const char* data, const size_t& size ) = 0;
	virtual std::vector< std::pair< std::string, std::string > > getElementInfo() = 0;
	virtual EStatus checkData() = 0;

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
	
	char*         _data;
	bool          _bigEndianData;
};

}
#include "Element.tcc"

#endif
