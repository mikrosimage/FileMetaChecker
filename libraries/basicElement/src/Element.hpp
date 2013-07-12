#ifndef _BASIC_ELEMENT_ELEMENT_HPP_
#define _BASIC_ELEMENT_ELEMENT_HPP_

#include <string>
#include <common/global.hpp>

namespace basic_element
{

class Element
{
public:
	enum EType
	{
		eTypeUnknown = 0,
		eTypeNumber,
		eTypeAscii,
		eTypeHexa,
		eTypeExif,
		eTypeData,
		eTypeKlv
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

	std::string getId()     { return _id; }
	std::string getLabel()  { return _label; }
	EType       getType()   { return _type; }
	EStatus     getStatus() { return _status; }

	void setData( const char* data, const size_t& size );
	virtual EStatus checkData() = 0;


protected:
	void setStatus( const EStatus status );

private:
	std::string _id;
	std::string _label;
	EType       _type;
	EStatus     _status;
};

}

#endif
