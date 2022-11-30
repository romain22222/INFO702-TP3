// file Image2D.hpp
#ifndef _IMAGE2D_HPP_
#define _IMAGE2D_HPP_
#include <vector>

/// Classe générique pour représenter des images 2D.
template <typename TValue>
class Image2D {
public:
	typedef Image2D<TValue>    Self;      // le type de *this
	typedef TValue             Value;     // le type pour la valeur des pixels
	typedef std::vector<Value> Container; // le type pour stocker les valeurs des pixels de l'image.

	struct Iterator : public Container::iterator {
		Iterator( Self & image, int x, int y )
				: Container::iterator( image.m_data.begin() + image.index( x, y ) )
		{}
	};

	/// @return un itérateur pointant sur le début de l'image
	Iterator begin() { return start( 0, 0 ); }
	/// @return un itérateur pointant après la fin de l'image
	Iterator end()   { return start( 0, h() ); }
	/// @return un itérateur pointant sur le pixel (x,y).
	Iterator start( int x, int y ) { return Iterator( *this, x, y ); }

	struct ConstIterator : public Container::const_iterator {
		ConstIterator( Self & image, int x, int y )
				: Container::const_iterator( image.m_data.begin() + image.index( x, y ) )
		{}
	};

	/// @return un itérateur pointant sur le début de l'image
	ConstIterator cbegin() const { return cstart( 0, 0 ); }
	/// @return un itérateur pointant après la fin de l'image
	ConstIterator cend() const { return cstart( 0, h() ); }
	/// @return un itérateur pointant sur le pixel (x,y).
	ConstIterator cstart( int x, int y ) const { return ConstIterator( *this, x, y ); }


	// Constructeur par défaut
	Image2D()
			: m_width(0), m_height(0), m_data(Container()) {}
	// Constructeur avec taille w x h. Remplit tout avec la valeur g
	// (par défaut celle donnée par le constructeur par défaut).
	Image2D( int w, int h, Value g = Value() )
			: m_width(w), m_height(h), m_data(Container(w*h, g)) {}

	// Remplit l'image avec la valeur \a g.
	void fill( Value g ) {
		m_data = Container(m_width*m_height, g);
	}

	/// @return la largeur de l'image.
	int w() const {
		return m_width;
	}
	/// @return la hauteur de l'image.
	int h() const {
		return m_height;
	}

	/// Accesseur read-only à la valeur d'un pixel.
	/// @return la valeur du pixel(i,j)
	Value at( int i, int j ) const {
		return m_data.at(index(i,j));
	}

	/// Accesseur read-write à la valeur d'un pixel.
	/// @return une référence à la valeur du pixel(i,j)
	Value& at( int i, int j ) {
		return m_data.at(index(i,j));
	}

	template <typename TAccessor>
	struct GenericConstIterator : public Container::const_iterator {
		typedef TAccessor Accessor;
		typedef typename Accessor::Argument  ImageValue; // Color ou unsigned char
		typedef typename Accessor::Value     Value;      // unsigned char (pour ColorGreenAccessor)
		typedef typename Accessor::Reference Reference;  // ColorGreenReference (pour ColorGreenAccessor)

		GenericConstIterator( const Image2D<ImageValue>& image, int x, int y )
				: Container::const_iterator( image.m_data.begin() + image.index( x, y ) ) {};

		// Accès en lecture (rvalue)
		Value operator*() const
		{ return Accessor::access( Container::const_iterator::operator*() ); } //< Appel de op* de l'térateur de vector

	};
	template <typename Accessor>
	GenericConstIterator< Accessor > cstart( int x = 0, int y = 0 ) const
	{ return GenericConstIterator< Accessor >( *this, x, y ); }
	template <typename Accessor>
	GenericConstIterator< Accessor > cbegin() const
	{ return cstart<Accessor>(); }
	template <typename Accessor>
	GenericConstIterator< Accessor > cend() const
	{ return cstart<Accessor>( 0, h()); }

	template <typename TAccessor>
	struct GenericIterator : public Container::iterator {
		typedef TAccessor Accessor;
		typedef typename Accessor::Argument  ImageValue; // Color ou unsigned char
		typedef typename Accessor::Value     Value;      // unsigned char (pour ColorGreenAccessor)
		typedef typename Accessor::Reference Reference;  // ColorGreenReference (pour ColorGreenAccessor)

		GenericIterator( Image2D<ImageValue>& image, int x, int y )
				: Container::iterator( image.m_data.begin() + image.index( x, y ) ) {};

		// Accès en écriture (lvalue)
		Reference operator*()
		{ return Accessor::access( Container::iterator::operator*() ); }

	};
	template <typename Accessor>
	GenericIterator< Accessor > start( int x = 0, int y = 0 )
	{ return GenericIterator< Accessor >( *this, x, y ); }
	template <typename Accessor>
	GenericIterator< Accessor > begin()
	{ return start<Accessor>(); }
	template <typename Accessor>
	GenericIterator< Accessor > end()
	{ return start<Accessor>( 0, h()); }
private:
	Container m_data; // mes données; évitera de faire les allocations dynamiques
	int m_width; // ma largeur
	int m_height; // ma hauteur

	/// @return l'index du pixel (x,y) dans le tableau \red m_data.
	int index( int x, int y ) const {
		return y*m_width+x;
	}
};

class Histogram {
public:
	template <typename InputIterator>
	void init( InputIterator it, InputIterator itE ) {
		_histo = std::vector<double>(256,0);
		for ( InputIterator ih = it; ih != itE; ih++ )
		{
			_histo[(double)(*ih)]++;
		}
		for (auto& v : _histo) {
			v/=(itE-it);
		}
		_cumhisto = std::vector<double>(_histo);
		for (auto it = _cumhisto.begin()+1; it != _cumhisto.end() ; it++) {
			*it += *(it-1);
		}
	}
	int egalisation(int j) const {
		return 255 * _cumhisto[j];
	}
	std::vector<double> _histo;
	std::vector<double> _cumhisto;
};

#endif // _IMAGE2D_HPP_