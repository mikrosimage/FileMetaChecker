#ifndef _REPORT_TRANSFORM_HPP_
#define _REPORT_TRANSFORM_HPP_

#include "Report/Report.hpp"
#include <Tools/ElementTranslator.hpp>

namespace bpt = boost::property_tree;
namespace be  = basic_element;
namespace ben = be::number_element;
namespace bed = be::data_element;

namespace report_generator
{

class Transform
{
public:
	enum EReportType
	{
		eReportTypeXml = 0,
		eReportTypeJson
	};

	Transform();
	Transform( const Report& report );
	~Transform();

	void setBasicElementReport( const Report& report );
	void transformTree( const EReportType& type );

protected:
	bpt::ptree translate( ReportTree::value_type& rootNode );
	bpt::ptree extractElement( std::shared_ptr< be::Element > element );

	bpt::ptree toXml( std::vector< std::pair< std::string, std::string > > elementInfo );

	template < typename ElementType >
	std::shared_ptr< ElementType > translateElement( std::shared_ptr< be::Element > element );

private:
	ReportTree  _basicElementTree;
	bpt::ptree  _report;
	EReportType _type;
};

}

#include "Transform.tcc"

#endif