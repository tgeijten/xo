#pragma once

#include "xo/container/flat_map.h"
#include "xo/utility/types.h"
#include "xo/container/sorted_vector.h"
#include <map>

namespace xo
{
	string random_string( size_t n )
	{
		string s;
		for ( auto i = 0; i < n; ++i )
			s += char( rand_uni_int( int( 'A' ), int( 'Z' ) ) );
		return s;
	}

	void container_test()
	{
		struct sortable
		{
			int e;
			std::string s;
			bool operator<( const sortable& other ) const { if ( e == other.e ) return s < other.s; else return e < other.e; }
		};

		sorted_vector< sortable > sv;
		sv.insert( sortable{ 3, "peer" } );
		sv.insert( sortable{ 1, "appel" } );
		sv.insert( sortable{ 2, "banaan" } );
		sv.insert( sortable{ 3, "banaan" } );
		sv.insert( sortable{ 1, "banaan" } );

		//for ( auto& e : sv )
		//	cout << e.e << e.s << endl;
		flat_map< string, int > vm;
		vm[ "Appel" ] = 1;
		vm[ "Peer" ] = 2;
		vm[ "Banaan" ] = 3;
		vm[ "Peer" ] = 4;
		XO_TEST( vm[ "Appel" ] == 1 );
		XO_TEST( vm[ "Peer" ] == 4 );
		XO_TEST( vm[ "Banaan" ] == 3 );

		// compare flat map to map
		flat_map< string, int > map1;
		std::map< string, int > map2;
		for ( int i = 0; i < 1000; ++i )
		{
			// fill with random data
			string key = random_string( i / 100 + 1 );
			map1[ key ] = i;
			map2[ key ] = i;

			if ( i % 10 == 0 )
			{
				// erase random element
				int idx = rand_uni_int( 0, (int)map1.size() - 1 );
				map1.erase( map1.begin() + idx );
				auto map2it = map2.begin();
				for ( int i = 0; i < idx; ++i )
					++map2it;
				map2.erase( map2it );
			}
		}

		auto it1 = map1.begin();
		auto it2 = map2.begin();
		bool map_equals_flat_map = true;
		for ( ; it1 != map1.end() && it2 != map2.end(); ++it1, ++it2 )
			map_equals_flat_map &= it1->first == it2->first;

		XO_TEST( map1.size() == map2.size() );
		XO_TEST( map_equals_flat_map );
	}
}
