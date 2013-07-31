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
	Transform();
	Transform( const Report& report );
	~Transform();

	void setBasicElementReport( const Report& report );

	void transform();

protected:
	bpt::ptree translate( ReportTree::value_type& rootNode );
	void extractElement( std::shared_ptr< be::Element > element );
	
	template < typename ElementType >
	std::shared_ptr< ElementType > translateElement( std::shared_ptr< be::Element > element );

private:
	ReportTree _basicElementTree;
	bpt::ptree _report;
};

}

#include "Transform.tcc"

#endif