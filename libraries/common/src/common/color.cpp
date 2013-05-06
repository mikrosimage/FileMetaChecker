#include "color.hpp"

namespace common {

boost::shared_ptr<Color> Color::get()
{
	return color;
}

boost::shared_ptr<Color> Color::color(new Color);

}
