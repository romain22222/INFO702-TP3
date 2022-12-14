#ifndef _COLOR_HPP_
#define _COLOR_HPP_

/**
   Représente une couleur avec un codage RGB. Ce codage utilise 3
   octets, le premier octet code l'intensité du rouge, le deuxième
   l'intensité du vert, le troisième l'intensité du bleu.
*/
struct Color {
	typedef unsigned char Byte;

	/// Code les 3 canaux RGB sur 3 octets.
	Byte red, green, blue;

	Color() {}
	/// Crée la couleur spécifiée par (_red,_green,_blue).
	Color( Byte _red, Byte _green, Byte _blue )
			: red( _red ), green( _green ), blue( _blue ) {}

	/// @return l'intensité de rouge (entre 0.0 et 1.0)
	float r() const { return ( (float) red ) / 255.0; }
	/// @return l'intensité de vert (entre 0.0 et 1.0)
	float g() const { return ( (float) green ) / 255.0; }
	/// @return l'intensité de bleu (entre 0.0 et 1.0)
	float b() const { return ( (float) blue ) / 255.0; }

	/// Sert à désigner un canal.
	enum Channel { Red, Green, Blue };
	/// @return le canal le plus intense.
	Channel argmax() const
	{
		if ( red >= green ) return red >= blue ? Red : Blue;
		else                return green >= blue ? Green : Blue;
	}
	/// @return l'intensité maximale des canaux
	float max() const { return std::max( std::max( r(), g() ), b() ); }
	/// @return l'intensité minimale des canaux
	float min() const { return std::min( std::min( r(), g() ), b() ); }
	/**
	   Convertit la couleur RGB en le modèle HSV (TSV en français).
	   @param h la teinte de la couleur (entre 0 et 359), hue en anglais.
	   @param s la saturation de la couleur (entre 0.0 et 1.0)
	   @param v la valeur ou brillance de la couleur (entre 0.0 et 1.0).
	*/
	void getHSV( int & h, float & s, float & v ) const
	{
		// Taking care of hue
		if ( max() == min() ) h = 0;
		else {
			switch ( argmax() ) {
				case Red:   h = ( (int) ( 60.0 * ( g() - b() ) / ( max() - min() ) + 360.0 ) ) % 360;
					break;
				case Green: h = ( (int) ( 60.0 * ( b() - r() ) / ( max() - min() ) + 120.0 ) );
					break;
				case Blue:  h = ( (int) ( 60.0 * ( r() - g() ) / ( max() - min() ) + 240.0 ) );
					break;
			}
		}
		// Taking care of saturation
		s = max() == 0.0 ? 0.0 : 1.0 - min() / max();
		// Taking care of value
		v = max();
	}

	void setHSV( int h, float s, float v )
	{
		int ti = h/60;
		float f = (float)h/60. - ti;
		float l = v * (1 - s);
		float m = v * (1 - f * s);
		float n = v * (1 - (1 - f) * s);
		switch (ti) {
			case 0: setColor(v, n, l); break;
			case 1: setColor(m, v, l); break;
			case 2: setColor(l, v, n); break;
			case 3: setColor(l, m, v); break;
			case 4: setColor(n, l, v); break;
			case 5: setColor(v, l, m); break;
		}
	}

	void setColor(float r, float g, float b) {
		red = r*255;
		green = g*255;
		blue = b*255;
	}
};

#endif //_COLOR_HPP_