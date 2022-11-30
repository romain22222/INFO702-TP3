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
};

/// Specialization for gray-level images.
template <>
class Image2DWriter<unsigned char> {
public:
	typedef unsigned char Value;
	typedef Image2D<Value> Image;

	static bool write( Image & img, std::ostream & output, bool ascii )
	{
		output << (ascii ? "P2" : "P5") << std::endl;
		output << m_width << ' ' << m_height << std::endl;
		output << "255" << std::endl;
		if (ascii) {
			for ( Iterator it = begin(), itE = end(); it != itE; ++it ) {
				output << (int)*it << ' ';
			}
		} else {
			output << std::noskipws;
			for ( Iterator it = begin(), itE = end(); it != itE; ++it ) {
				output << *it;
			}
		}
		return true;
	}
};

/// Specialization for color images.
template <>
class Image2DWriter<Color> {
public:
	typedef Color Value;
	typedef Image2D<Value> Image;

	static bool write( Image & img, std::ostream & output, bool ascii )
	{
		output << "P6" << std::endl;
		output << img.w() << " " << img.h() << std::endl;
		output << "255" << std::endl;
		for ( Iterator it = img.begin(), itE = img.end(); it != itE; ++it )
		{
			Color c = *it;
			output << c.red << c.green << c.blue;
		}
		output.close();
		return true;
	}
};

#endif // _IMAGE2DWRITER_HPP_