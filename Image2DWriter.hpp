#ifndef _IMAGE2DWRITER_HPP_
#define _IMAGE2DWRITER_HPP_

#include <iostream>
#include <string>
#include "Color.hpp"
#include "Image2D.hpp"

template <typename TValue>
class Image2DWriter {
public:
	typedef TValue Value;
	typedef Image2D<Value> Image;

	static bool write( Image & img, std::ostream & output, bool ascii )
	{
		std::cerr << "[Image2DWriter<TValue>::write] NOT IMPLEMENTED." << std::endl;
		return false;
	}

	static Image read(std::ifstream & input)
	{
		std::cerr << "[Image2DWriter<TValue>::read] NOT IMPLEMENTED." << std::endl;
		return false;
	}
};

std::string readline(std::ifstream &input) {
	std::string str;
	do {
		std::getline( input, str );
	} while (str != ""  && str[0]=='#');
	return str;
}

/// Specialization for gray-level images.
template <>
class Image2DWriter<unsigned char> {
public:
	typedef unsigned char Value;
	typedef Image2D<Value> Image;

	static bool write( Image & img, std::ostream & output, bool ascii )
	{
		output << (ascii ? "P2" : "P5") << std::endl;
		output << img.w() << ' ' << img.h() << std::endl;
		output << "255" << std::endl;
		if (ascii) {
			for ( Image::Iterator it = img.begin(), itE = img.end(); it != itE; ++it ) {
				output << (int)*it << ' ';
			}
		} else {
			output << std::noskipws;
			for ( Image::Iterator it = img.begin(), itE = img.end(); it != itE; ++it ) {
				output << *it;
			}
		}
		return true;
	}

	static Image read( std::ifstream & input)
	{
		if ( ! input.good() ) return Image();
		std::string format = readline(input);
		std::string line = readline(input);
		std::string delim = " ";
		int width = std::stoi(line.substr(0, line.find(delim)));
		line.erase(0, line.find(delim) + delim.length());
		int height = std::stoi(line);
		Image img(width, height, 0);
		readline(input);
		if (format == "P5") {
			input >> std::noskipws;
			unsigned char v;
			for ( Image::Iterator it = img.begin(), itE = img.end(); it != itE; ++it ) {
				input >> v;
				*it = v;
			}
		} else {
			input >> std::skipws;
			int v;
			for ( Image::Iterator it = img.begin(), itE = img.end(); it != itE; ++it ) {
				input >> v;
				*it = v;
			}
		}
		return img;
	}
};

/// Specialization for color images.
template <>
class Image2DWriter<Color> {
public:
	typedef Color Value;
	typedef Image2D<Value> Image;

	static bool write( Image & img, std::ostream & output, bool ascii)
	{
		output << "P6" << std::endl;
		output << img.w() << " " << img.h() << std::endl;
		output << "255" << std::endl;
		for ( Image::Iterator it = img.begin(), itE = img.end(); it != itE; ++it )
		{
			Color c = *it;
			output << c.red << c.green << c.blue;
		}
		return true;
	}

	static Image read(std::ifstream & input) {
		readline(input);
		std::string line = readline(input);
		std::string delim = " ";
		int width = std::stoi(line.substr(0, line.find(delim)));
		line.erase(0, line.find(delim) + delim.length());
		int height = std::stoi(line);
		Image img(width, height, Color());
		readline(input);
		Color::Byte r;
		Color::Byte g;
		Color::Byte b;
		input >> std::noskipws;
		for ( Image::Iterator it = img.begin(), itE = img.end(); it != itE; ++it )
		{
			input >> r;
			input >> g;
			input >> b;
			*it = Color(r,g,b);
		}
		return img;
	}
};

#endif // _IMAGE2DWRITER_HPP_