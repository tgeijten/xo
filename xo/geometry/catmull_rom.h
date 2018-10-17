#pragma once

namespace xo
{
	template< typename P >
	struct catmull_rom_segment
	{
		catmull_rom_segment( const P& p0, const P& p1, const P& p2, const P& p3 ) :
		a_( 2 * p1 ),
		b_( -p0 + p2 ),
		c_( 2 * p0 - 5 * p1 + 4 * p2 - p3 ),
		d_( -p0 + 3 * p1 - 3 * p2 + p3 )
		{}

		template< typename T > P operator()( T t ) const { return T( 0.5 ) * ( a_ + b_ * t + c_ * t * t + d_ * t * t * t ); }

	private:
		P a_, b_, c_, d_;
	};

	template< typename P >
	struct catmull_rom_segment_alpha
	{
		catmull_rom_segment_alpha( const P& p0, const P& p1, const P& p2, const P& p3, float alpha ) :
		p0_( p0 ), p1_( p1 ), p2_( p2 ), p3_( p3 ) {
			t0 = 0.0f;
			t1 = pow( distance( p0, p1 ), alpha );
			t2 = t1 + pow( distance( p1, p2 ), alpha );
			t3 = t2 + pow( distance( p2, p3 ), alpha );
		}

		P operator()( float t ) const {
			// TODO: make this implementation more efficient
			t = t1 + t * ( t2 - t1 );
			auto A1 = ( t1 - t ) / ( t1 - t0 ) * p0_ + ( t - t0 ) / ( t1 - t0 ) * p1_;
			auto A2 = ( t2 - t ) / ( t2 - t1 ) * p1_ + ( t - t1 ) / ( t2 - t1 ) * p2_;
			auto A3 = ( t3 - t ) / ( t3 - t2 ) * p2_ + ( t - t2 ) / ( t3 - t2 ) * p3_;
			auto B1 = ( t2 - t ) / ( t2 - t0 ) * A1 + ( t - t0 ) / ( t2 - t0 ) * A2;
			auto B2 = ( t3 - t ) / ( t3 - t1 ) * A2 + ( t - t1 ) / ( t3 - t1 ) * A3;
			return ( t2 - t ) / ( t2 - t1 ) * B1 + ( t - t1 ) / ( t2 - t1 ) * B2;
		}

	private:
		float t0, t1, t2, t3;
		P p0_, p1_, p2_, p3_;
	};

	template< typename S > double compute_spline_length( const S& spline, float t0, float t1, int steps = 100 )
	{
		double dist = 0.0;
		auto pos = spline( t0 );
		for ( int i = 1; i <= steps; ++i )
		{
			auto new_pos = spline( t0 + ( t1 - t0 ) * i / steps );
			dist += distance( pos, new_pos );
			pos = new_pos;
		}
		return dist;
	}

	template< typename P, typename T >
	class catmull_rom
	{
	public:
		catmull_rom( const std::vector< P >& points, const std::vector< T >& times, T alpha = T( 0.5 ) ) :
		times_( times )
		{
			xo_assert( points.size() >= 4 && times.size() == ( points.size() - 2 ) );
			segments_.reserve( points.size() - 4 );
			for ( size_t i = 0; i <= points.size() - 4 )
				segments_.emplace_back( points[ i ], points[ i + 1 ], points[ i + 2 ], points[ i + 3 ] );
		}

		P operator()( T t ) {
			auto it = std::lower_bound( times_.begin(), times_.end(), t );
			if ( it == times_.end() ) it = times_.begin();
			if ( it == times_.end() - 1 ) it = times_.end() - 2;
			size_t index = it - times_.begin();
			return segments_[ index ]( ( t - *it ) / ( *( it + 1 ) - *it ) );
		}

	private:
		std::vector< catmull_rom_segment< P > > segments_;
		std::vector< T > times_;
	};
}
