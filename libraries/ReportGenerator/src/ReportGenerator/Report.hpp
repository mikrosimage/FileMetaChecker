#ifndef _REPORT_GENERATOR_REPORT_HPP_
#define _REPORT_GENERATOR_REPORT_HPP_

#include <iomanip>
#include <memory>
#include <vector>
#include <Common/common.hpp>

namespace basic_element
{
	class Element;
}

namespace report_generator
{

/**
 *  Report generator, to print or write the comparison results.
 */
class Report
{
	typedef std::shared_ptr< basic_element::Element > ShPtrElement;
	
public:

	/**
	 * Report's constructor.
	 * @param elementList Vector of Element references. Default is an empty vector.
	 */
	Report( const std::vector< ShPtrElement >& elementList = std::vector< ShPtrElement >() );
	
	/**
	 * Add an Element to the list.
	 * @param element Element reference to add.
	 */
	void add( const ShPtrElement element );

	/**
	 * Add several Elements to the list.
	 * @param elementList Vector of Element references to add.
	 */
	void add( const std::vector< ShPtrElement >& elementList );

	/**
	 * Get an Element from the list.
	 * @param elementId Element's ID.
	 * @param iteration Element's iteration. Default is 1.
	 * @return          Corresponding Element reference.
	 */
	ShPtrElement get( const std::string& elementId, const size_t& iteration = 1 );

	/**
	 * Replace an Element from the list by another with the same unique ID.
	 * @param newElement Element reference to update.
	 */
	void update( const ShPtrElement newElement );
	
	/**
	 * Print the report legend.
	 */
	void printHelper();

	/**
	 * Print every printable Elements of the list.
	 */
	void print();

	/**
	 * Print one Element of the list.
	 * @param element   Element reference to print.
	 */
	void print( const ShPtrElement element );

	/**
	 * Write the Elements from the list in a XML file.
	 * @param filename Output file name.
	 */
	void writeXml( const std::string& filename );
	
protected:
	size_t getDisplayOffset( const ShPtrElement element );
	std::string tabulation( size_t tabNum );
	std::string tabulation( size_t tabNum, const std::string& str );
	bool isPrintable( const ShPtrElement element );


private:
	std::vector< ShPtrElement > _elementList;

};

}

#endif
