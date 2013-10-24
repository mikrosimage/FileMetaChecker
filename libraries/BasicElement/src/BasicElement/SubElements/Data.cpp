#include "Data.hpp"
#include <SpecReader/SpecNode.hpp>
#include <Common/log.hpp>

namespace basic_element
{

Data::Data( const std::shared_ptr< spec_reader::SpecNode > node, const std::shared_ptr< Element > previous )
 : Element( node, previous )
{
}

}
