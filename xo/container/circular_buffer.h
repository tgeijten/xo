#pragma once

#include "vector_type.h"

namespace xo
{
	template< typename T >
	class circular_buffer
	{
	public:
		using value_type = T;

		circular_buffer( size_t frames = 0, size_t channels = 0 ) :	in_ofs_( 0 ), out_ofs_( 0 )
		{
			resize( frames, channels );
		}

		void set( index_t channel, const T& value ) { data_[ in_ofs_ + channel ] = value; }
		const T& get( index_t channel ) const { return data_[ out_ofs_ + channel ]; }

		void resize( size_t frames, size_t channels ) {
			xo_error_if( in_ofs_ != out_ofs_, "Cannot resize buffer after advance()" );
			frames_ = frames;
			channels_ = channels;
			data_.resize( frames_ * channels_ );
		}
		index_t add_channel() { resize( frames_, channels_ + 1 ); return channels_ - 1; }
		index_t add_frame() { resize( frames_ + 1, channels_ ); return frames_ - 1; }

		void advance() {
			in_ofs_ = ( in_ofs_ + channels_ ) % ( frames_ * channels_ );
			if ( out_ofs_ == in_ofs_ )
				out_ofs_ = ( out_ofs_ + channels_ ) % ( frames_ * channels_ );
		}

	private:
		size_t frames_;
		size_t channels_;
		index_t in_ofs_;
		index_t out_ofs_;
		vector<T> data_;
	};
}
