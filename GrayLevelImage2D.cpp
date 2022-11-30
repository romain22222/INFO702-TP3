#include "GrayLevelImage2D.hpp"
#include <list>
#include <algorithm>

GrayLevelImage2D::Iterator::Iterator( GrayLevelImage2D& image, int x, int y )
: Container::iterator( image.m_data.begin() + image.index( x, y ) ) {}

GrayLevelImage2D::Iterator GrayLevelImage2D::start(int x, int y) {
	return Iterator(*this, x, y);
}

GrayLevelImage2D::Iterator GrayLevelImage2D::begin() {
	return start(0,0);
}

GrayLevelImage2D::Iterator GrayLevelImage2D::end() {
	return start(m_width, m_height-1);
}

std::string readline(std::istream &input) {
	std::string str;
	do {
		std::getline( input, str );
	} while (str != ""  && str[0]=='#');
	return str;
}

bool GrayLevelImage2D::importPGM(std::istream &input) {
	if ( ! input.good() ) return false;
	std::string format = readline(input);
	std::string line = readline(input);
	std::string delim = " ";
	m_width = std::stoi(line.substr(0, line.find(delim)));
	line.erase(0, line.find(delim) + delim.length());
	m_height = std::stoi(line);
	fill(0);
	readline(input);
	if (format == "P5") {
		input >> std::noskipws;
		unsigned char v;
		for ( Iterator it = begin(), itE = end(); it != itE; ++it ) {
			input >> v;
			*it = v;
		}
	} else {
		input >> std::skipws;
		int v;
		for ( Iterator it = begin(), itE = end(); it != itE; ++it ) {
			input >> v;
			*it = v;
		}
	}
	return true;
}

bool GrayLevelImage2D::exportPGM(std::ostream &output, bool ascii) {
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

void GrayLevelImage2D::filtrageMedian(int k) {
	GrayLevelImage2D copy(*this);
	for (int i = 0; i < m_height; ++i) {
		for (int j = 0; j < m_width; ++j) {
			std::list<int> table;
			int nbE = 0;
			for (int x = 0; x < 2*k+1; ++x) {
				for (int y = 0; y < 2*k+1; ++y) {
					if (0 <= j+x-k && j+x-k < m_width && 0 <= i+y-k && i+y-k < m_height) {
						table.push_front(at(j+x-k,i+y-k));
						nbE++;
					}
				}
			}
			table.sort();
			auto it = table.begin();
			for (int l = 0; l < nbE/2; ++l) {
				it++;
			}
			copy.at(j,i) = *it;
		}
	}
	this->m_data = copy.m_data;
}

void Histogram::init(GrayLevelImage2D &img) {
	_histo = std::vector<double>(256,0);
	for ( auto p : img )
	{
		_histo[(double)(p)]++;
	}
	for (auto& v : _histo) {
		v/=img.h()*img.w();
	}
	_cumhisto = std::vector<double>(_histo);
	for (auto it = _cumhisto.begin()+1; it != _cumhisto.end() ; it++) {
		*it += *(it-1);
	}
}

int Histogram::egalisation(int j) const {
	return 255 * _cumhisto[j];
}

GrayLevelImage2D GrayLevelImage2D::histogramShown() {
	Histogram h;
	h.init(*this);
	auto copy = std::vector<double>(h._histo);
	std::sort(copy.begin(), copy.end());
	double max = *(copy.end()-1);
	GrayLevelImage2D shownHisto(512,256,255);
	for (int j = 0; j < 256; ++j) {
		for (int i = 0; i < 256; ++i) {
			if (h._histo[i]*256/max>=256-j) {
				shownHisto.at(i,j) = 0;
			}
		}
	}
	max = *(h._cumhisto.end()-1);
	for (int j = 0; j < 256; ++j) {
		for (int i = 0; i < 256; ++i) {
			if (h._cumhisto[i]*256/max>=256-j) {
				shownHisto.at(i+256,j) = 0;
			}
		}
	}
	return shownHisto;
}

void GrayLevelImage2D::egalise() {
	Histogram h;
	h.init(*this);
	for (auto& p : *this) {
		p = h.egalisation(p);
	}
}

void GrayLevelImage2D::rendreNet(double alpha) {
	GrayLevelImage2D copy(*this);
	for (int j = 0; j < m_width; ++j) {
		for (int i = 0; i < m_height; ++i) {
			netlifyAt(copy, i, j, alpha);
		}
	}
	*this = copy;
}

void GrayLevelImage2D::netlifyAt(GrayLevelImage2D &copy, int i, int j, double alpha) {
	double newVal = at(i,j) * (1 + alpha);
	if (i > 0) newVal -= at(i-1,j) * (alpha/4);
	if (i < m_width - 1) newVal -= at(i+1,j) * (alpha/4);
	if (j > 0) newVal -= at(i,j-1) * (alpha/4);
	if (j < m_height - 1) newVal -= at(i,j+1) * (alpha/4);
	copy.at(i,j) = newVal;
}