#include <iostream>
#include <fstream>
#include "Image2D.hpp"
#include "Image2DWriter.hpp"
#include "Accessor.hpp"

using namespace std;

typedef Image2D<Color> ColorImage2D;
typedef Image2D<unsigned char> ColorChannelImage2D;
typedef Image2DWriter<Color> ColorImage2DWriter;
typedef ColorImage2D::Iterator ColorIterator;
typedef ColorChannelImage2D::Iterator ChannelIterator;
typedef Image2D<Color>::GenericIterator<ColorRedAccessor> RedIterator;
typedef Image2D<Color>::GenericIterator<ColorGreenAccessor> GreenIterator;
typedef Image2D<Color>::GenericIterator<ColorBlueAccessor> BlueIterator;

void floute(ColorChannelImage2D& c, ChannelIterator& it, int m) {
	int v = 0;
	int pos = it - c.begin();
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < m; ++j) {
			v += c.at(pos+i+j*c.w(),0);
		}
	}
	*it = v/(m*m);
}

int main( int argc, char** argv )
{


	if ( argc < 3 )
	{
		std::cerr << "Usage: upscale <input.ppm> <multiplier>" << std::endl;
		return 0;
	}
	std::string fn = argv[1];
	std::string line = fn;
	std::string delim = ".ppm";
	std::string n = line.substr(0, line.find(delim));
	std::ifstream input( fn );
	ColorImage2D img = ColorImage2DWriter::read( input );
	input.close();

	int multiplier = stoi(argv[2]);
	int newW = img.w()*multiplier;
	int newH = img.h()*multiplier;

	// 1 - Calculer sur 3 channel différents l'upscale rgb

	ColorChannelImage2D redC(newW, newH, 0);
	ColorChannelImage2D greenC(newW, newH, 0);
	ColorChannelImage2D blueC(newW, newH, 0);
	RedIterator itR = img.begin<ColorRedAccessor>();
	GreenIterator itG = img.begin<ColorGreenAccessor>();
	BlueIterator itB = img.begin<ColorBlueAccessor>();
	int oldPos, oldPosX, oldPosY;
	for (;itR != img.end<ColorRedAccessor>();)
	{
		oldPos = itR-img.begin<ColorRedAccessor>();
		oldPosX = (oldPos*multiplier)%newW;
		oldPosY = (oldPos*multiplier)/newW*multiplier;
		for (int i = 0; i < multiplier; ++i) {
			for (int j = 0; j < multiplier; ++j) {
				redC.at(oldPosX + i, oldPosY + j) = *itR;
				greenC.at(oldPosX + i, oldPosY + j) = *itG;
				blueC.at(oldPosX + i, oldPosY + j) = *itB;
			}
		}
		++itR;
		++itG;
		++itB;
	}

	// 2 - Flouter les cases upscaled pour créer la matière entre
	ChannelIterator itCR = redC.begin();
	ChannelIterator itCG = greenC.begin();
	ChannelIterator itCB = blueC.begin();

	for (;itCR != redC.start(0, newH-multiplier+1);)
	{
		if (newW - (itCR-redC.begin())%newW < multiplier)
		{} else {
			floute(redC, itCR, multiplier);
			floute(greenC, itCG, multiplier);
			floute(blueC, itCB, multiplier);
		}
		++itCR;
		++itCG;
		++itCB;
	}

	// 3 - Remplir cette image en parcourant les 3 channels précédents
	ColorImage2D upscaledImg(newW, newH, Color(0,0,0));
	itCR = redC.begin();
	itCG = greenC.begin();
	itCB = blueC.begin();
	for (ColorIterator it = upscaledImg.begin();it != upscaledImg.end(); it++)
	{
		*it = Color(*itCR, *itCG, *itCB);
		++itCR;
		++itCG;
		++itCB;
	}

	// 4 - Enregistrer l'image ainsi obtenue
	ofstream output( n + "_upscaled_times_" + std::to_string(multiplier) + ".ppm");
	ColorImage2DWriter::write( upscaledImg, output, false );
	output.close();
	return 0;
}
