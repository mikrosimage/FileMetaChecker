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

typedef std::map< std::string, std::shared_ptr< basic_element::Element > > ElementMap;

template< typename Type >
struct Vector
{
	typedef std::vector< std::pair< Type, Type > > Pair;
};

struct ElementIter
{
	size_t iter;
	std::string parentId;
	std::vector< std::string > childrenId;
};

struct CurrentElement
{
	std::string            nodeId;
	size_t                 nodeIter;
	size_t                 repetNumber;
	size_t                 initPos;
	bool                   sentToReport;
	bool                   inRangeCheck;
};

class Comparator
{
public:
	Comparator( filereader::FileReader* file, const spec_reader::SpecList& specs );
	~Comparator();

	void compare( const std::string& specId, report_generator::Report& report );

protected:
	template< typename ElementType >
	std::shared_ptr< ElementType > getElement( const spec_reader::SpecNode& node );

	void checkNode( const spec_reader::SpecNode& node, report_generator::ReportNode& reportNode, const bool& isFirstChild = false );
	size_t iterateNode( const spec_reader::SpecNode& node );
	void resetIteration( const std::string& nodeId );

	void checkOptional( const spec_reader::SpecNode&                    node,
                        const std::shared_ptr< basic_element::Element > element,
                        spec_reader::SpecNode&                          nextNode,
                        bool&                                           sentToReport );

	void checkRepetition( const spec_reader::SpecNode&                    node,
                          const std::shared_ptr< basic_element::Element > element,
                          spec_reader::SpecNode&                          nextNode,
                          report_generator::ReportNode&                   reportNode,
                          CurrentElement&                                 currentNode );

	void checkGroupSize( const std::string&            nodeGroupSize,
                         const size_t&                 filePosition,
                         const size_t&                 initPosition,
                         const size_t&                 elementSize,
                         report_generator::ReportNode* nextReportNode );

	std::shared_ptr< be::Element > getElementFromNode( const spec_reader::SpecNode& node );
	
	void extractRepetition( size_t& repetNumber, Vector< size_t >::Pair& repetRange, const Vector< std::string >::Pair& nodeRepetitions );
	size_t extractGroupSize( const std::string& groupSizeExpr );

private:
	filereader::FileReader*              _file;
	spec_reader::SpecList                _specs;
	report_generator::Report*            _report;
	ElementMap                           _elementList;
	std::map< std::string, ElementIter > _elementIter;
};

}

#include "Comparator.tcc"

#endif
