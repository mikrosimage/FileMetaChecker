%module spec

%include "std_string.i"
%include "std_map.i"

%{
#include <Common/common.hpp>
#include <Common/log.hpp>
%}

namespace std {
%template(map_string_etype) map<string, EType>;
%template(map_string_edisplaytype) map<string, EDisplayType>;

%template(map_string_estatus) map<EStatus, string>;
%template(map_string_etypestring) map<EType, string>;
%template(map_string_edisplaytypestring) map<EDisplayType, string>;
}

%include <Common/common.hpp>
%include <Common/log.hpp>