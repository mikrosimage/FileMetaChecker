%module report

%include "std_string.i"
%include "std_vector.i"
%include "std_shared_ptr.i"

%{
#include <Report/Report.hpp>
%}

%inline %{
basic_element::Element* getElem( std::shared_ptr< basic_element::Element > *e )
{
	return e->get();
}
%}

namespace std {

%template(ElementPtr)  shared_ptr< basic_element::Element >;
%shared_ptr( basic_element::Element );
%template(ElementList) vector< shared_ptr< basic_element::Element > >;
}

%include <Report/Report.hpp>
