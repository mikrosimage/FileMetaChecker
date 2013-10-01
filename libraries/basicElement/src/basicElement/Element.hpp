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

	~Element()
	{
	}

	void setLabel( const std::string& label );
protected:
	void setStatus( const EStatus status );
	
public:
	std::string getId()       const { return _id; }
	std::string getLabel()    const { return _label; }
	EType       getType()     const { return _type; }
	size_t      getSize()     const { return _size; }
	bool        isBigEndian() const { return _bigEndianData; }
	
	template< typename OutputType = ESubType >
	OutputType getSubType() const;
	
	void setDisplayType( const std::string& displayType );
	
	EStatus     getStatus()   { return _status; }
	size_t      getUniqueId() { return _uniqueId; }

	void setBigEndianness( bool isBigEndian );

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
	virtual EStatus checkData() = 0;
	virtual std::vector< std::pair< std::string, std::string > > getElementInfo() = 0;
	// virtual void setFromNode( const SpecNode& node ) = 0;	// @todo 

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
#include "Element.tcc"

#endif
