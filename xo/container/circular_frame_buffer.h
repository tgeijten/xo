#pragma once

#include "label_vector.h"
#include "xo/system/assert.h"

namespace xo
{
	template< typename T, typename L = void >
	class circular_frame_buffer
	{
	public:
		circular_frame_buffer( size_t channels = 0, size_t frames = 0 ) : data_( frames * channels ), frame_buf_size_( frames ), frame_size_( 0 ), labels_( channels ) {}
		~circular_frame_buffer() {}

		/// add a channel
		index_t add_channel( L label ) { resize_buffer( frame_size(), channel_size() + 1 ); return labels_.set( channel_size() - 1, label ); }
		void set_label( index_t channel, L label ) { labels_.set( channel, label ); }
		const L& get_label( index_t channel ) const { return labels_[ channel ]; }
		index_t find_channel( const L& label ) const { return labels_.find( label ); }
		index_t find_or_add_channel( const L& label ) { auto idx = labels_.find( label ); return idx == no_index ? add_channel( label ) : idx; }

		index_t add_frame() { return ++frame_size_; }

		/// number of channels
		size_t channel_size() const { return labels_.size(); }

		/// number of frames in storage
		size_t frame_size() const { return frame_size_; }

		/// check if there is any data
		bool empty() const { return data_.empty(); }

		/// get the interpolated value of a specific frame / channel
		T get_interpolated_value( index_t frame0, index_t channel, T pos ) {
			xo_assert( frame0 >= frame_count() - frame_buf_size_ && frame0 < frame_count() && channel < this->channel_count() );
			index_t ofs0 = ( frame0 % frame_buf_size_ ) * this->channel_count() + channel;
			index_t ofs1 = ( ( frame0 + 1 ) % frame_buf_size_ ) * this->channel_count() + channel;
			return ( T(1) - pos ) * data_[ ofs0 ] + pos * data_[ ofs1 ];
		}

		/// access value (no bounds checking)
		T& operator()( index_t frame, index_t channel ) { return data( frame % frame_buf_size_, channel ); }
		const T& operator()( index_t frame, index_t channel ) const { data( frame % frame_buf_size_, channel ); }

		/// access value of most recent frame
		T& operator[]( index_t channel ) { return data( ( frame_size_ - 1 ) % frame_buf_size_, channel ); }
		const T& operator[]( index_t channel ) const { return data( ( frame_size_ - 1 ) % frame_buf_size_, channel ); }

		/// access value frames back
		T& reverse( index_t frame, index_t channel ) { return data( ( frame_size_ - frame - 1 ) % frame_buf_size_, channel ); }
		const T& reverse( index_t frame, index_t channel ) const { return data( ( frame_size_ - frame - 1 ) % frame_buf_size_, channel ); }

		/// access value with bounds checking
		T& at( index_t frame, index_t channel ) {
			xo_assert( frame >= frame_size() - frame_buf_size_ && frame < frame_size() && channel < channel_size() );
			return data( frame % frame_buf_size_, channel );
		}
		const T& at( index_t frame, index_t channel ) const {
			xo_assert( frame >= frame_size() - frame_buf_size_ && frame < frame_size() && channel < channel_size() );
			return data( frame % frame_buf_size_, channel );
		}

		/// resize buffer, resets contents
		void resize_buffer( size_t nframes, size_t nchannels, const T& value = T() ) {
			data_.assign( nframes * nchannels, value );
			frame_buf_size_ = nframes;
			labels_.resize( nchannels );
		}

		void resize_buffer( size_t nframes, const T& value = T() ) {
			data_.assign( nframes * channel_size(), value );
			frame_buf_size_ = nframes;
		}

	private:
		T& data( index_t frame, index_t channel ) { return data_[ frame * channel_size() + channel ]; }
		const T& data( index_t frame, index_t channel ) const { return data_[ frame * channel_size() + channel ]; }

		label_vector< L > labels_;
        std::vector< T > data_;
		size_t frame_buf_size_;
		index_t frame_size_;
	};
}
