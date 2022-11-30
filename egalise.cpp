#include <iostream>
#include <fstream>
#include "Image2D.hpp"
#include "Image2DWriter.hpp"
#include "Accessor.hpp"

using namespace std;

int main( int argc, char** argv )
{
	typedef Image2D<Color> ColorImage2D;
	typedef Image2D<unsigned char> GrayLevelImage2D;
	typedef Image2DWriter<Color> ColorImage2DWriter;
	typedef ColorImage2D::Iterator ColorIterator;
	typedef Image2D<Color>::GenericIterator<ColorValueAccessor> HUEIterator;
	if ( argc < 2 )
	{
		std::cerr << "Usage: histogram <input.ppm>" << std::endl;
		return 0;
	}
	std::string fn = argv[1];
	std::string line = fn;
	std::string delim = ".ppm";
	std::string n = line.substr(0, line.find(delim));
	std::ifstream input( fn );
	ColorImage2D img = ColorImage2DWriter::read( input );
	input.close();

	Histogram h;
	h.init(img.begin<ColorValueAccessor>(), img.end<ColorValueAccessor>());

	for (HUEIterator it = img.begin<ColorValueAccessor>(), itE = img.end<ColorValueAccessor>(); it != itE; it++) {
		*it = h.egalisation(*it);
	}

	ofstream output( n + "_egalise.ppm");
	ColorImage2DWriter::write( img, output, false );
	output.close();
	return 0;
}
