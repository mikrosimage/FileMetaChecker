#ifndef _REPORT_REPORT_HPP_
#define _REPORT_REPORT_HPP_

#include <boost/property_tree/ptree.hpp>

#include <Element.hpp>
#include <Data/Data.hpp>
#include <Number/Number.hpp>

namespace bpt = boost::property_tree;
namespace be  = basic_element;
namespace bed = be::data_element;
namespace ben = be::number_element;

namespace report_generator
{

class Report
{
public:
	Report();
	~Report();

	template< typename ElementType >
	void addBasicElement( std::shared_ptr< ElementType > element );

	std::shared_ptr< be::Element > getBasicElement( const size_t uniqueId );
	
	// void add( const std::string& entry, const std::string& value );
	
	// void add(const bpt::ptree &tree, const std::string& rootpath );

	// void add( const std::vector< bpt::ptree >& tree, const std::string& rootpath );
	
	// void exportReport( const std::string& filename );
	
private:
	std::map< size_t, std::shared_ptr< be::Element > > _elementMap;
	std::map< size_t, std::shared_ptr< ben::Number > > _numberMap;	// !!!!!! Number< NumberType > !!!!!
	std::map< size_t, std::shared_ptr< bed::Data   > > _dataMap;

	bpt::basic_ptree< std::string, size_t, std::less< std::string > > _basicElementTree;
};

}

#include "Report.tcc"

#endif
