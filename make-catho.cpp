#include <cmath>
#include <iostream>
#include <fstream>
#include "Image2D.hpp"
#include "Image2DWriter.hpp"
#include "Accessor.hpp"

int main( int argc, char** argv )
{
	typedef Image2D<Color> ColorImage2D;
	typedef Image2DWriter<Color> ColorImage2DWriter;
	typedef ColorImage2D::Iterator ColorIterator;
	if ( argc < 2 )
	{
		std::cerr << "Usage: save-channels <input.ppm>" << std::endl;
		return 0;
	}
	std::string fn = argv[1];
	std::string line = fn;
	std::string delim = ".ppm";
	std::string n = line.substr(0, line.find(delim));
	std::ifstream input( fn );
	ColorImage2D img = ColorImage2DWriter::read( input );
	input.close();
	typedef ColorImage2D::GenericIterator< ColorRedAccessor >   ColorRedIterator;
	typedef ColorImage2D::GenericIterator< ColorGreenAccessor > ColorGreenIterator;
	typedef ColorImage2D::GenericIterator< ColorBlueAccessor >  ColorBlueIterator;
	ColorRedIterator   itRed   = img.begin< ColorRedAccessor >();
	ColorGreenIterator itGreen = img.begin< ColorGreenAccessor >();
	ColorBlueIterator  itBlue  = img.begin< ColorBlueAccessor >();
// On écrit la composante verte dans l'image en niveaux de gris.
	int x = 0;
	for ( ColorIterator it = img.begin(), itE = img.end();
		  it != itE; ++it )
	{
		switch ( x % 3 ) {
			case 0: *itGreen = *itBlue  = 0; break;
			case 1: *itRed   = *itBlue  = 0; break;
			case 2: *itRed   = *itGreen = 0; break;
		}
		++itRed; ++itGreen; ++itBlue;
		x = ( x+1 ) % img.w();
	}
	std::ofstream output( n + "_catho.ppm" );
	ColorImage2DWriter::write( img, output, false );
	output.close();
	return 0;
}