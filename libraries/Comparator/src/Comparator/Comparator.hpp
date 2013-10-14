#ifndef _COMPARATOR_COMPARATOR_HPP_
#define _COMPARATOR_COMPARATOR_HPP_

namespace basic_element
{
	class Element; 
}
namespace spec_reader      { class Specification; }
namespace spec_reader      { class SpecNode; }
namespace file_reader      { class FileReader; }
namespace report_generator { class Report; }

class Comparator
{
public:
	Comparator();
	void check( spec_reader::Specification& spec, file_reader::FileReader& reader, report_generator::Report& report );
	
private:
	basic_element::Element* getElement( const spec_reader::SpecNode* node, const basic_element::Element* previous );
	
};

#endif
