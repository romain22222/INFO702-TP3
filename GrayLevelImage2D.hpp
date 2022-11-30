#ifndef _GRAYLEVELIMAGE2D_HPP_
#define _GRAYLEVELIMAGE2D_HPP_

#include <iostream>
#include <vector>
#include <exception>
#include <fstream>
#include <sstream>
#include <string>


class GrayLevelImage2D {

public:
  typedef unsigned char          GrayLevel;// le type pour les niveaux de gris.
  typedef std::vector<GrayLevel> Container;// le type pour stocker les niveaux de gris de l'image.

  /**
     Représente un itérateur sur toutes les valeurs d'une image.

     Model of DefaultConstructible, CopyConstructible, Assignable,
     RandomAccessIterator. */
  struct Iterator : public Container::iterator {
    Iterator( GrayLevelImage2D& Image, int x, int y );
  };


public:
  GrayLevelImage2D()
  : m_width(0), m_height(0), m_data(Container()) {}
  GrayLevelImage2D( int w, int h, GrayLevel g = 0 )
  : m_width(w), m_height(h), m_data(Container(w*h, g)) {}
  void fill( GrayLevel g ) {
	  m_data = Container(m_width*m_height, g);
  }

  //! [gli2d-sec3]
  /// @return la largeur de l'image.
  int w() const {
	  return m_width;
  }
  /// @return la hauteur de l'image.
  int h() const {
	  return m_height;
  }

  /**
     Accesseur read-only à la valeur d'un pixel.
     @return la valeur du pixel(i,j)
  */
  GrayLevel at( int i, int j ) const {
	  return m_data.at(index(i,j));
  }

  /**
     Accesseur read-write à la valeur d'un pixel.
     @return une référence à la valeur du pixel(i,j)
  */
  GrayLevel& at( int i, int j ) {
	  return m_data.at(index(i,j));
  }
  //! [gli2d-sec3]
  Iterator begin();
  Iterator end();
  Iterator start( int x, int y );

  std::pair<int,int> position( Iterator it ) const;

  bool importPGM( std::istream & input );
  bool exportPGM( std::ostream & output, bool ascii = true );

  void filtrageMedian(int k);

  GrayLevelImage2D histogramShown();

  void egalise();

  void rendreNet(double alpha);

  void netlifyAt(GrayLevelImage2D& copy, int i, int j, double alpha);

private:
  // Calcule l'indice dans m_data du pixel (x,y).
  int index( int x, int y ) const {
	  return y*m_width+x;
  }
  // Le tableau contenant les valeurs des pixels.
  Container m_data;
  // la largeur
  int       m_width;
  // la hauteur
  int       m_height;
};

class Histogram {
public:
	void init (GrayLevelImage2D & img);
	int egalisation(int j) const;
	std::vector<double> _histo;
	std::vector<double> _cumhisto;
};


#endif // #ifndef _GRAYLEVELIMAGE2D_HPP_
