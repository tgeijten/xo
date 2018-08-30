#pragma once

#include "storage.h"
#include "xo/filesystem/path.h"
#include "xo/geometry/vec3_type.h"
#include "xo/geometry/quat_type.h"

namespace xo
{
	template< typename T, typename L = std::string >
	class file_storage : public storage< T, L >
	{
	public:
		file_storage( const path& filename ) : storage(), filename_( filename ) {}
		virtual ~file_storage() {
			if ( !empty() )
				std::ofstream( filename_.str() ) << *this;
		}
	private:
		path filename_;
	};

	template< typename T, typename L  > index_t	find_frame_index( const storage<T, L>& sto, const T& val, index_t ch = 0 )
	{
		if ( sto.empty() )
			return no_index;

		index_t first = 0;
		index_t count = sto.frame_size();

		// lower_bound algorithm
		while ( count > 0 ) {
			index_t step = count / 2;
			index_t idx = first + step;
			if ( sto( idx, ch ) < val ) {
				first = idx + 1;
				count -= step + 1;
			}
			else count = step;
		}

		first = xo::min( first, sto.frame_size() - 1 );

		// check if value is closer to lower_bound frame or to prev
		if ( first > 1 && ( val - sto( first - 1, ch ) < sto( first, ch ) - val ) )
			return first - 1;
		else return first;
	}

	template< typename T, typename L  > void write( storage<T, L>& sto, const string& str, const vec3_<T>& v ) {
		auto f = sto.back();
		f[ str + ".x" ] = v.x;
		f[ str + ".y" ] = v.y;
		f[ str + ".z" ] = v.z;
	}

	template< typename T, typename L  > void write( storage<T, L>& sto, const string& str, const quat_<T>& q ) {
		auto f = sto.back();
		f[ str + ".w" ] = q.w;
		f[ str + ".x" ] = q.x;
		f[ str + ".y" ] = q.y;
		f[ str + ".z" ] = q.z;
	}

	template< typename T > void read( typename const storage<T>::const_frame& f, const string& str, vec3_<T>& v ) {
		v.x = f[ str + ".x" ];
		v.y = f[ str + ".y" ];
		v.z = f[ str + ".z" ];
	}

	template< typename T > void read( typename const storage<T>::const_frame& f, const string& str, quat_<T>& q ) {
		q.w = f[ str + ".w" ];
		q.x = f[ str + ".x" ];
		q.y = f[ str + ".y" ];
		q.z = f[ str + ".z" ];
	}
}
