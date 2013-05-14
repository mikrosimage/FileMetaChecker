#ifndef _EXTRACTOR_SPECS_COMMON_HPP_
#define _EXTRACTOR_SPECS_COMMON_HPP_

#include <boost/property_tree/ptree.hpp>

#include <map>

namespace bpt = boost::property_tree;

typedef bpt::ptree Spec;
typedef bpt::ptree::value_type Node;
typedef bpt::ptree::const_assoc_iterator SpecIt;
typedef const bpt::ptree::value_type SubSpec;

typedef std::map<std::string, unsigned int> ElementsMap;
typedef std::pair<std::string, unsigned int> ElementsPair;


const std::string kId     = "id";
const std::string kLabel  = "label";

const std::string kAscii  = "ascii";
const std::string kHexa   = "hexa";
const std::string kType   = "type";
const std::string kCount  = "count";
const std::string kMap    = "map";
const std::string kRange  = "range";
const std::string kMin    = "min";
const std::string kMax    = "max";

const std::string kGroup  = "group";
const std::string kGroupSize    = "groupSize";

const std::string kEndian        = "endian";
const std::string kEndianBig     = "big";
const std::string kEndianLittle  = "little";

const std::string kOptional      = "optional";
const std::string kOptionalTrue  = "true";
const std::string kOptionalFalse = "false";

const std::string kOrdered       = "ordered";
const std::string kOrderedTrue   = "true";
const std::string kOrderedFalse  = "false";

#endif
