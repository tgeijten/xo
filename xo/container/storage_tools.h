#pragma once

#include "storage.h"

namespace xo
{
	template< typename T, typename L = std::string >
	class file_storage : public storage< T, L >
	{
	public:
		file_storage( const path& filename ) : storage(), filename_( filename )  {}
		virtual ~file_storage() { if ( !storage_.empty() ) std::ofstream( filename_.str() ) << storage_; }
	private:
		path filename_;
	};


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

	template< typename T > void read( typename storage<T>::const_frame& f, const string& str, vec3_<T>& v ) {
		v.x = f[ str + ".x" ];
		v.y = f[ str + ".y" ];
		v.z = f[ str + ".z" ];
	}

	template< typename T > void read( typename storage<T>::const_frame& f, const string& str, quat_<T>& q ) {
		q.w = f[ str + ".w" ];
		q.x = f[ str + ".x" ];
		q.y = f[ str + ".y" ];
		q.z = f[ str + ".z" ];
	}
}
