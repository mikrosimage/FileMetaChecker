#ifndef _COMPARATOR_COMPARATOR_HPP_
#define _COMPARATOR_COMPARATOR_HPP_

#include <SpecList/SpecList.hpp>
#include <Report/Report.hpp>

namespace filereader
{
	class FileReader;
}


namespace comparator
{

typedef std::map< std::string, std::shared_ptr< be::Element > > ElementMap;

class Comparator
{
public:
	Comparator( filereader::FileReader* file, const spec_reader::SpecList& specs );
	~Comparator();

	void compare( const std::string& specId, report_generator::Report& report );

protected:
	template < typename SubType >
	SubType getNodeSubType( const std::string& nodeType );

	template< typename ElementType >
	std::shared_ptr< ElementType > getElement( const spec_reader::SpecNode& node );

	void checkNode( const spec_reader::SpecNode& node, report_generator::ReportNode& reportNode, const bool& isFirstChild = false );
	std::shared_ptr< be::Element > getElementFromNode( const spec_reader::SpecNode& node );
		
private:
	filereader::FileReader*   _file;
	spec_reader::SpecList     _specs;
	report_generator::Report* _report;
	ElementMap                _elementList;
};

}

#include "Comparator.tcc"

#endif
