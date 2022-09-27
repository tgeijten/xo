#pragma once

#include "xo/xo_types.h"
#include "xo/system/assert.h"
#include "xo/container/label_vector.h"
#include "xo/numerical/math.h"
#include "xo/string/string_tools.h"
#include "xo/serialization/char_stream.h"

#include <vector>
#include <string>
#include <ostream>
#include <iostream>

namespace xo
{
	/// Simple storage class for storing frames with channels of data
	// #todo: use iterators for frames
	// #todo: use deque for storage, profile results
	template< typename T, typename L = std::string >
	class storage
	{
	public:
		using container_type = std::vector< T >;
		using value_type = T;
		using label_type = L;

		struct const_frame {
			const_frame( const storage< T, L >& s, index_t f ) : sto_( s ), ofs_( f * s.channel_size() ) {}
			const T& operator[]( index_t i ) const { return sto_.data()[ ofs_ + i ]; }
			const T& operator[]( const L& l ) const { return sto_.data()[ ofs_ + sto_.find_channel( l ) ]; }
			const storage< T, L >& sto_;
			index_t ofs_;
		};

		struct frame {
			frame( storage< T, L >& s, index_t f ) : sto_( s ), fidx_( f ) {}
			T& operator[]( index_t i ) const { return sto_( fidx_, i ); }
			T& operator[]( const L& l ) const { return sto_( fidx_, sto_.find_or_add_channel( l ) ); }
			storage< T, L >& sto_;
			index_t fidx_;
		};

		storage( size_t frames = 0, size_t channels = 0, T value = T() ) : frame_size_( frames ), labels_( channels ), data_( channels * frames, value ) {}
		virtual ~storage() {}

		/// add a channel and resize buffer if needed
		index_t add_channel( L label, const T& value = T() ) {
			resize( frame_size(), channel_size() + 1, value );
			return labels_.set( channel_size() - 1, label );
		}

		/// add a channel with data, resize buffer if needed
		index_t add_channel( L label, const std::vector< T >& data ) {
			resize( max( frame_size(), std::size( data ) ), channel_size() + 1 );
			labels_.set( channel_size() - 1, label );
			auto cidx = channel_size() - 1;
			for ( index_t fidx = 0; fidx < std::size( data ); ++fidx ) ( *this )( fidx, cidx ) = data[ fidx ];
			return cidx;
		}

		/// find index of a label
		index_t find_channel( const L& label ) const { return labels_.find_or_throw( label ); }

		/// find index of a label
		index_t try_find_channel( const L& label ) const { return labels_.find( label ); }

		/// find or add a channel
		index_t find_or_add_channel( const L& label, const T& value = T() ) {
			auto idx = labels_.find( label );
			return idx == no_index ? add_channel( label, value ) : idx;
		}

		/// set channel label
		void set_label( index_t channel, L label ) { labels_.set( channel, label ); }

		/// get channel label
		const L& get_label( index_t channel ) const { return labels_[ channel ]; }

		/// add frame to storage
		frame add_frame( T value = T( 0 ) ) { data_.resize( data_.size() + channel_size(), value ); ++frame_size_; return back(); }

		/// add frame to storage
		frame add_frame( const std::vector< T >& data ) {
			xo_assert( std::size( data ) == channel_size() );
			data_.resize( data_.size() + channel_size() );
			++frame_size_;
			std::copy( std::begin( data ), std::end( data ), data_.end() - channel_size() );
			return back();
		}

		/// number of channels
		size_t channel_size() const { return labels_.size(); }

		/// number of frames in storage
		size_t frame_size() const { return frame_size_; }

		/// check if there is any data
		bool empty() const { return data_.empty(); }

		/// clear the storage
		void clear() { frame_size_ = 0; labels_.clear(); data_.clear(); }

		/// access value (no bounds checking)
		const T& operator()( index_t frame, index_t channel ) const { return data_[ frame * channel_size() + channel ]; }
		T& operator()( index_t frame, index_t channel ) { return data_[ frame * channel_size() + channel ]; }

		/// access frame
		const_frame operator[]( index_t f ) const { return const_frame( *this, f ); }
		frame operator[]( index_t f ) { return frame( *this, f ); }
		const_frame front() const { return const_frame( *this, 0 ); }
		frame front() { return frame( *this, 0 ); }
		const_frame back() const { return const_frame( *this, frame_size() - 1 ); }
		frame back() { return frame( *this, frame_size() - 1 ); }

		/// get the interpolated value of a specific frame / channel
		T get_interpolated_value( T frame_idx, index_t channel ) {
			auto frame_f = floor( frame_idx );
			auto frame_w = frame_idx - frame_f;
			index_t frame0 = static_cast<index_t>( frame_f );
			xo_assert( frame0 + 1 < frame_size() );
			index_t ofs = frame0 * this->channel_size() + channel;
			return ( T( 1 ) - frame_w ) * data_[ ofs ] + frame_w * data_[ ofs + this->channel_size() ];
		}

		void resize( size_t nframes, size_t nchannels, T value = T() ) {
			xo_error_if( nframes < frame_size() || nchannels < channel_size(), "Cannot shrink storage" );
			if ( nchannels > channel_size() ) {
				if ( frame_size() > 1 ) {
					// reorganize existing data
					container_type new_data( nchannels * nframes, value );
					for ( index_t fi = 0; fi < frame_size(); ++fi )
						for ( index_t ci = 0; ci < channel_size(); ++ci )
							new_data[ nchannels * fi + ci ] = data_[ channel_size() * fi + ci ];
					data_ = std::move( new_data );
				}
				else data_.resize( nframes * nchannels, value ); // just resize

				labels_.resize( nchannels );
				frame_size_ = nframes;
			}
			else {
				// just resize data
				data_.resize( nframes * nchannels, value );
				frame_size_ = nframes;
			}
		}

		std::vector<T> get_channel( index_t channel ) const {
			std::vector<T> vec( frame_size() );
			for ( index_t fi = 0; fi < frame_size(); ++fi )
				vec[ fi ] = ( *this )( fi, channel );
			return vec;
		}
		std::vector<T> get_channel( const L& label ) const { return get_channel( find_channel( label ) ); }

		const label_vector< L >& labels() const { return labels_; }
		const container_type& data() const { return data_; }
		container_type& data() { return data_; }

	private:
		size_t frame_size_;
		label_vector< L > labels_;
		container_type data_;
	};

	template< typename T, typename L > std::ostream& operator<<( std::ostream& str, const storage< T, L >& buf ) {
		for ( index_t ci = 0; ci < buf.channel_size(); ++ci ) {
			str << buf.get_label( ci );
			if ( ci == buf.channel_size() - 1 ) str << std::endl; else str << '\t';
		}
		for ( index_t fi = 0; fi < buf.frame_size(); ++fi ) {
			for ( index_t ci = 0; ci < buf.channel_size(); ++ci ) {
				str << buf( fi, ci );
				if ( ci == buf.channel_size() - 1 ) str << std::endl; else str << '\t';
			}
		}
		return str;
	}

	template< typename T, typename L > std::istream& operator>>( std::istream& str, storage< T, L >& sto ) {
		string line;
		if ( std::getline( str, line ) )
		{
			for ( const auto& l : xo::split_str( line, "\t " ) )
				sto.add_channel( l );
			while ( std::getline( str, line ) ) {
				auto f = sto.add_frame();
				xo::char_stream cstr( line.c_str() );
				for ( index_t ci = 0; cstr.good() && ci < sto.channel_size(); ++ci )
					cstr >> f[ ci ];
			}
		}

		return str;
	}
}
