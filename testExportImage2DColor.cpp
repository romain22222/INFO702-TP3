#include <iostream>
#include <fstream>
#include "Image2D.hpp"
#include "Color.hpp"
#include "Image2DWriter.hpp"

int main()
{
	typedef Image2D<Color> ColorImage2D;
	typedef ColorImage2D::Iterator Iterator;
	ColorImage2D img( 256, 256, Color( 0, 0, 0 ) );
	Iterator it = img.begin();
	for ( int y = 0; y < 256; ++y )
		for ( int x = 0; x < 256; ++x )
		{
			*it++ = Color( y, x, (2*x+2*y) % 256 );
		}
	std::ofstream output( "colors.ppm" ); // ios::binary for Windows system
	bool ok2 = Image2DWriter<Color>::write( img, output, false );
	if ( !ok2 ) {
		std::cerr << "Error writing output file." << std::endl;
		return 1;
	}
	output.close();
}
