#pragma once

#include "vector_type.h"
#include "xo/system/assert.h"

namespace xo
{
	template< typename T >
	class circular_buffer
	{
	public:
		using value_type = T;

		circular_buffer( size_t frames = 0, size_t channels = 0 ) : in_ofs_( 0 ), out_ofs_( 0 )
		{
			resize( frames, channels );
		}

		void set( index_t channel, const T& value ) { data_[in_ofs_ + channel] = value; }
		const T& get( index_t channel ) const { return data_[out_ofs_ + channel]; }

		const T& front( index_t channel ) const { return data_[out_ofs_ + channel]; }
		const T& back( index_t channel ) const { return data_[in_ofs_ + channel]; }
		T& back( index_t channel ) { return data_[in_ofs_ + channel]; }

		size_t capacity() const { return frames_; }
		bool empty() const { return in_ofs_ == out_ofs_; }

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

		void clear_back( const T& clear_value = T( 0 ) ) {
			for ( index_t channel = 0; channel < channels_; ++channel )
				data_[in_ofs_ + channel] = clear_value;
		}

		void reset() {
			in_ofs_ = out_ofs_ = 0;
			std::fill( data_.begin(), data_.end(), T( 0 ) );
		}

	private:
		size_t frames_;
		size_t channels_;
		index_t in_ofs_;
		index_t out_ofs_;
		vector<T> data_;
	};
}
