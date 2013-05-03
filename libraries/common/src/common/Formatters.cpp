#include "Formatters.hpp"

namespace common {
namespace formatters {

boost::shared_ptr<Formatter> Formatter::get()
{
	if( !formatter )
	{
		formatter = new Formatter();
	}
	return boost::shared_ptr<Formatter>( formatter );
}

Formatter* Formatter::formatter = NULL;

}
}
