#include "color.hpp"

namespace common {

boost::shared_ptr<Color> Color::get()
{
	if( !color )
	{
		color = new Color();
	}
	return boost::shared_ptr<Color>( color );
}

Color* Color::color = NULL;

}
