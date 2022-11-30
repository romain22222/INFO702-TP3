#include <iostream>
#include <fstream>
#include "Image2D.hpp"
#include "Color.hpp"
#include "Image2DWriter.hpp"

int main(int argc, char** argv )
{
	typedef Image2D<Color> ColorImage2D;
	typedef ColorImage2D::Iterator Iterator;
	if ( argc < 2 )
	{
		std::cerr << "Usage: invert-red-blue <input.ppm>" << std::endl;
		return 0;
	}
	std::string fn = argv[1];
	std::ifstream input( fn );
	ColorImage2D img = Image2DWriter<Color>::read(input);
	input.close();

	for (auto p:img) {
		p = Color(p.b(), p.g(), p.r());
	}

	std::ofstream output("rbinverted-" + fn);
	Image2DWriter<Color>::write(img, output, false);
	return 0;
}
