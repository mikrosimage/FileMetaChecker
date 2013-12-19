%module specReader

%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
%include "std_iostream.i"

%{
#include <SpecReader/SpecChecker.hpp>
#include <SpecReader/SpecNode.hpp>
#include <SpecReader/Specification.hpp>
%}

%include <SpecReader/SpecChecker.hpp>
%include <SpecReader/SpecNode.hpp>
%include <SpecReader/Specification.hpp>
