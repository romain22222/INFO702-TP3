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
	typedef Image2D<unsigned char> GrayLevelImage2D;
	typedef Image2DWriter<unsigned char> GrayLevelImage2DWriter;
	typedef GrayLevelImage2D::Iterator GrayLevelIterator;
	GrayLevelImage2D imgR( img.w(), img.h() );
	GrayLevelImage2D imgB( img.w(), img.h() );
	GrayLevelImage2D imgG( img.w(), img.h() );

	//-----------------------------------------------------------------------------
	// vvvvvvvvv Toute la transformation couleur -> canal vert est ici vvvvvvvvvvvv
	//
	// Servira à parcourir la composante verte de l'image couleur.
	typedef ColorImage2D::GenericConstIterator< ColorGreenAccessor > ColorGreenConstIterator;
	typedef ColorImage2D::GenericConstIterator< ColorRedAccessor > ColorRedConstIterator;
	typedef ColorImage2D::GenericConstIterator< ColorBlueAccessor > ColorBlueConstIterator;
	// Notez comment on appelle la méthode \b générique `begin` de `Image2D`.
	ColorGreenConstIterator itGreen = img.begin< ColorGreenAccessor >();
	ColorRedConstIterator itRed = img.begin< ColorRedAccessor >();
	ColorBlueConstIterator itBlue = img.begin< ColorBlueAccessor >();
	GrayLevelIterator itB = imgB.begin();
	GrayLevelIterator itG = imgG.begin();
	for ( GrayLevelIterator itR = imgR.begin(), itE = imgR.end();
		  itR != itE; ++itR )
	{
		*itR = *itRed;
		*itB = *itBlue;
		*itG = *itGreen;
		++itGreen;
		++itBlue;
		++itRed;
		++itB;
		++itG;
	}
	//-----------------------------------------------------------------------------

	std::ofstream outputR( n + "_red.pgm" );
	GrayLevelImage2DWriter::write( imgR, outputR, false );
	outputR.close();
	std::ofstream outputG( n + "_green.pgm" );
	GrayLevelImage2DWriter::write( imgG, outputG, false );
	outputG.close();
	std::ofstream outputB( n + "_blue.pgm" );
	GrayLevelImage2DWriter::write( imgB, outputB, false );
	outputB.close();
	return 0;
}