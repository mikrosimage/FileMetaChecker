#ifndef _COMPARATOR_COMPARATOR_HPP_
#define _COMPARATOR_COMPARATOR_HPP_

#include <memory>

namespace basic_element
{
	class Element;
}

namespace spec_reader
{
	class Specification;
	class SpecNode;
}

namespace file_reader
{
	class FileReader;
}

namespace report_generator
{
	class Report;
}

namespace comparator
{

/**
 *  Comparator, check orchestrator.
 */
class Comparator
{
	typedef std::shared_ptr< basic_element::Element > ShPtrElement;
	typedef std::shared_ptr< spec_reader::SpecNode  > ShPtrSpecNode;
	
public:
	/**
	 * Comparator's constructor.
	 */
	Comparator();

	/**
	 * Compare an input file to a specification, and write a report.
	 * @param spec   Specification used as the comparison reference.
	 * @param reader FileReader, to extract raw data from the file checked.
	 * @param report Report to fill, and print or write.
	 */
	void check( spec_reader::Specification& spec, file_reader::FileReader& reader, report_generator::Report& report );

private:
	bool         isInUnorderedGroup( const ShPtrElement element );
	size_t       getWordLength     ( const ShPtrElement element, file_reader::FileReader& file );
	void         updateParentSize  ( const ShPtrElement element );
	ShPtrElement getNextParent     ( const ShPtrElement element, const ShPtrSpecNode node );
	void         checkGroupSize    ( const ShPtrElement element, file_reader::FileReader& file );
};

}

#endif
