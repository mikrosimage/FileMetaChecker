#ifndef _EXTRACTOR_SPECS_SPECIFICATION_HPP_
#define _EXTRACTOR_SPECS_SPECIFICATION_HPP_

#include <boost/property_tree/ptree.hpp>

#include <vector>

namespace bpt = boost::property_tree;

class Specification
{
public:
    Specification();

private:
    std::vector<bpt::ptree> specList;
};

#endif
