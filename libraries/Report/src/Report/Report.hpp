#ifndef _REPORT_REPORT_HPP_
#define _REPORT_REPORT_HPP_

#include <iomanip>
#include <memory>
#include <vector>
#include <Common/common.hpp>

namespace basic_element
{
	class Element;
}

namespace rapid_parser
{
	class Serializer;
}

namespace report_generator
{

/**
 * Report display levels definition
 */
enum EReportDisplayLevel
{
	eReportDisplayNone = 0,
	eReportDisplayID,
	eReportDisplayType,
	eReportDisplayValue,
	eReportDisplayStatus,
	eReportDisplaySize,
	eReportDisplayIteration,
	eReportDisplayComments,
	eReportDisplayOffset,
};

/**
 *  Report generator, to print or write the comparison results.
 */
class Report
{
public:
	typedef std::shared_ptr< basic_element::Element > ShPtrElement;

	enum EExportType
	{
		eExportTypeJson = 0,
		eExportTypeXml
	};

	/**
	 * Report's constructor.
	 * @param elementList Vector of Element references. Default is an empty vector.
	 */
	Report( const std::vector< ShPtrElement >& elementList = std::vector< ShPtrElement >() );
	
	/**
	 * Set the display log report level.
	 * @param level Display log level.
	 */
	void setPrintVerbosity( const EReportDisplayLevel level );

	/**
	 * Add an Element to the list.
	 * @param element Element reference to add.
	 */
	void add( const ShPtrElement& element );

	/**
	 * Add several Elements to the list.
	 * @param elementList Vector of Element references to add.
	 */
	void add( const std::vector< ShPtrElement >& elementList );

	/**
	 * Get vector of Elements.
	 * @return Elements present in Report.
	 */
	std::vector< ShPtrElement > get( );

	/**
	 * Get an Element from the list.
	 * @param elementId Element's ID.
	 * @param iteration Element's iteration. Default is 1.
	 * @return          Corresponding Element reference.
	 */
	ShPtrElement get( const std::string& elementId, const size_t iteration = 1 ) const;
	
	/**
	 * Get the Element list size.
	 * @return  Size of the list of Elements.
	 */
	size_t getElementListSize() const;

	/**
	 * Get the Element list.
	 * @return  Vector of Elements.
	 */
	std::vector< ShPtrElement > getElementList() const;

	/**
	 * Replace an Element from the list by another with the same unique ID.
	 * @param newElement Element reference to update.
	 */
	void update( const ShPtrElement& newElement );

	/**
	 * Serialize Elements from the list.
	 * @param exportType Formatting type of export.
	 */
	std::string serialize( EExportType exportType = eExportTypeJson, bool indent = false );
	
protected:
	size_t getDisplayOffset( const ShPtrElement& element );
	std::string tabulation( size_t tabNum );
	std::string tabulation( size_t tabNum, const std::string& str );
	bool isPrintable( const ShPtrElement element );

	void serialize( rapid_parser::Serializer* serializer, const std::vector< ShPtrElement >& elements, std::vector<char*>& path );

private:
	std::vector< ShPtrElement > _elementList;
	EReportDisplayLevel _verbosity;

};

}

#endif
