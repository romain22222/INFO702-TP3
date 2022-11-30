#include <iostream>
#include <fstream>
#include "Image2D.hpp"
#include "Color.hpp"

using namespace std;

int main( int argc, char** argv )
{
	typedef unsigned char GrayLevel;
	typedef Image2D<Color> ColorImage2D;
	ColorImage2D img( 8, 8, Color( 255, 0, 255 ) ); // imagette 8x8 remplie de magenta
	for ( int y = 0; y < img.h(); ++y )
	{
		for ( int x = 0; x < img.w(); ++x )
			std::cout << " " << img.at( x, y ).r() << "-" << img.at( x, y ).g() << "-" << img.at( x, y ).b();
		std::cout << std::endl;
	}
	return 0;
}