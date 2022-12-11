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
	if ( argc < 2 )
	{
		std::cerr << "Usage: egaliseurTriColor <input.ppm>" << std::endl;
		return 0;
	}
	std::string fn = argv[1];
	std::string line = fn;
	std::string delim = ".ppm";
	std::string n = line.substr(0, line.find(delim));
	std::ifstream input( fn );
	ColorImage2D img = ColorImage2DWriter::read( input );
	input.close();

	Histogram hR;
	hR.init(img.begin<ColorRedAccessor>(), img.end<ColorRedAccessor>());
	Histogram hG;
	hG.init(img.begin<ColorGreenAccessor>(), img.end<ColorGreenAccessor>());
	Histogram hB;
	hB.init(img.begin<ColorBlueAccessor>(), img.end<ColorBlueAccessor>());

	for (ColorIterator it = img.begin(), itE = img.end(); it != itE; it++) {
		*it = Color(hR.egalisation((int)(*it).red), hG.egalisation((int)(*it).green), hB.egalisation((int)(*it).blue));
	}

	ofstream output( n + "_egaliseTriColor.ppm");
	ColorImage2DWriter::write( img, output, false );
	output.close();
	return 0;
}
