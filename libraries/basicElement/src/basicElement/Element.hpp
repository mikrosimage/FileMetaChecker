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

	void setLabel      ( const std::string& label );
	void setDisplayType( const std::string& displayType );
	
	void setBigEndianness( const bool isBigEndian );
	void setIsOptional   ( const bool isOptional );
	void setIsOrdered    ( const bool isOrdered );
	void setHasGroup     ( const bool hasGroup );

	void setCount      ( const std::string& countExpr );
	void setGroupSize  ( const std::string& groupSizeExpr );
	void setRequirement( const std::string& requiredExpr );
	void setRepetitions( const std::vector< std::pair< std::string, std::string > >& repetitions );
	
	void setRepetitionId( const size_t& iteration );
	
	void addErrorLabel  ( const std::string& error );
	void addWarningLabel( const std::string& warning );

protected:
	void setType   ( const std::string& type );
	void setSubType( const std::string& subType );
	
	void setStatus ( const EStatus status );

public:
	std::string getId()       const { return _id; }
	std::string getLabel()    const { return _label; }
	EType       getType()     const { return _type; }
	EStatus     getStatus()   const { return _status; }
	size_t      getSize()     const { return _size; }
	size_t      getUniqueId() const { return _uniqueId; }
	
	bool isBigEndian() const { return _bigEndianData; }
	bool isOptional()  const { return _isOptional; }
	bool isOrdered()   const { return _isOrdered; }
	bool hasGroup()    const { return _hasGroup; }

	std::string getCount()       const { return _countExpr; }
	std::string getGroupSize()   const { return _groupSizeExpr; }
	std::string getRequirement() const { return _requiredExpr; }
	std::vector< std::pair< std::string, std::string > > getRepetitions() const { return _repetExpr; }

	std::string getErrorLabel()   const { return _error; }
	std::string getWarningLabel() const { return _warning; }

	size_t getRepetitionId() const { return _repetitionId; }

	template< typename OutputType = ESubType >
	OutputType getSubType() const;

	void getEndianOrderedData( char* buffer, const char* data ) const;
	void reverseEndianness   ( char* buffer, const char* data ) const;

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
	size_t        _repetitionId;
	
	std::string   _countExpr;
	std::string   _groupSizeExpr;
	std::string   _requiredExpr;
	
	std::vector< std::pair< std::string, std::string > > _repetExpr;

	EType         _type;
	ESubType      _subType;
	EDisplayType  _displayType;
	
	EStatus       _status;
	
	char*         _data;
	
	bool          _bigEndianData;
	bool          _isOptional;
	bool          _isOrdered;
	bool          _hasGroup;
};

}
#include "Element.tcc"

#endif
