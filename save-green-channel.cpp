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
		std::cerr << "Usage: save-green-channel <input.ppm>" << std::endl;
		return 0;
	}
	std::string fn = argv[1];
	std::ifstream input( fn ); // récupère le 1er argument.
	ColorImage2D img = ColorImage2DWriter::read( input );
	input.close();
	typedef Image2D<unsigned char> GrayLevelImage2D;
	typedef Image2DWriter<unsigned char> GrayLevelImage2DWriter;
	typedef GrayLevelImage2D::Iterator GrayLevelIterator;
	GrayLevelImage2D img2( img.w(), img.h() );

	//-----------------------------------------------------------------------------
	// vvvvvvvvv Toute la transformation couleur -> canal vert est ici vvvvvvvvvvvv
	//
	// Servira à parcourir la composante verte de l'image couleur.
	typedef ColorImage2D::GenericConstIterator< ColorGreenAccessor > ColorGreenConstIterator;
	// Notez comment on appelle la méthode \b générique `begin` de `Image2D`.
	ColorGreenConstIterator itGreen = img.cbegin< ColorGreenAccessor >();
	// On écrit la composante verte dans l'image en niveaux de gris.
	for ( GrayLevelIterator it = img2.begin(), itE = img2.begin();
		  it != itE; ++it )
	{
		*it = *itGreen;
		++itGreen;
		// NB: si on veut faire *itGreen++, il faut redéfinir GenericConstIterator<T>::operator++(int).
	}
	//-----------------------------------------------------------------------------

	std::ofstream output( "greenChan-" + fn );
	bool ok2 = GrayLevelImage2DWriter::write( img2, output, false );
	if ( !ok2 )
	{
		std::cerr << "Error writing output file." << std::endl;
		return 1;
	}
	output.close();
	return 0;
}