#pragma once

namespace xo
{
	template< typename It > struct iterator_istream {
		using value_type = typename It::value_type;
		iterator_istream( It pos ) : pos_( pos ) {}
		iterator_istream& operator>>( value_type& v ) { v = *pos_++; return *this; }
		It pos_;
	};

	template< typename It > struct iterator_ostream {
		using value_type = typename It::value_type;
		iterator_ostream( It pos ) : pos_( pos ) {}
		iterator_ostream& operator<<( const value_type& v ) { *pos_++ = v; return *this; }
		iterator_ostream& operator<<( value_type&& v ) { *pos_++ = std::move( v ); return *this; }
		It pos_;
	};

	template< typename C > struct back_inserter_stream {
		using value_type = typename C::value_type;
		back_inserter_stream( C& cont ) : cont_( cont ) {}
		back_inserter_stream& operator<<( const value_type& v ) { cont_.push_back( v ); return *this; }
		back_inserter_stream& operator<<( value_type&& v ) { cont_.push_back( std::move( v ) ); return *this; }
		C& cont_;
	};
}
