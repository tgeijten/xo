#include <map>
#include "xo/xo_types.h"
#include "xo/container/flat_map.h"
#include "xo/container/sorted_vector.h"
#include "xo/container/table.h"
#include "xo/numerical/random.h"
#include "xo/system/test_case.h"
#include "xo/container/container_algorithms.h"
#include "xo/string/string_tools.h"
#include "xo/container/flat_set.h"
#include "xo/container/indexed_map.h"

namespace xo
{
	string random_string( size_t n )
	{
		string s;
		for ( auto i = 0; i < n; ++i )
			s += char( rand_uni_int( int( 'A' ), int( 'Z' ) ) );
		return s;
	}

	XO_TEST_CASE( xo_sorted_vector )
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

		XO_CHECK( sv.front().e == 1 );
		XO_CHECK( sv.front().s == "appel" );
		XO_CHECK( sv[ 1 ].e == 1 );
		XO_CHECK( sv[ 1 ].s == "banaan" );
		XO_CHECK( sv.back().s == "peer" );
		XO_CHECK( sv.size() == 5 );
	}

	XO_TEST_CASE( xo_flat_map )
	{
		flat_map< string, int > vm;
		vm[ "Appel" ] = 1;
		vm[ "Peer" ] = 2;
		vm[ "Banaan" ] = 3;
		vm[ "Peer" ] = 4;
		XO_CHECK( vm[ "Appel" ] == 1 );
		XO_CHECK( vm[ "Peer" ] == 4 );
		XO_CHECK( vm[ "Banaan" ] == 3 );

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

		XO_CHECK( map1.size() == map2.size() );
		XO_CHECK( map_equals_flat_map );
	}

	XO_TEST_CASE( xo_flat_set )
	{
		flat_set< string > fs;
		XO_CHECK( fs.empty() );
		fs.insert( "appel" );
		fs.insert( "appel" );
		XO_CHECK( fs.size() == 1 );
		fs.insert( "peer" );
		fs.insert( "banaan" );
		fs.insert( "peer" );
		fs.insert( "Appel" );
		fs.insert( "banaan" );
		XO_CHECK( !fs.empty() );
		XO_CHECK( fs.size() == 4 );
		XO_CHECK( fs.front() == "Appel" );
		XO_CHECK( fs.back() == "peer" );
		XO_CHECK( fs.count( "appel" ) == 1 );
		XO_CHECK( fs.contains( "Appel" ) );
		XO_CHECK( fs.find( "Appel" ) == fs.begin() );

		const flat_set<int> fs2{ 4, 1, -5, 3 };
		XO_CHECK( fs2.front() == -5 );
		XO_CHECK( fs2.back() == 4 );
		XO_CHECK( fs2.find( -5 ) == fs2.begin() );
		XO_CHECK( fs2.find( 2 ) == fs2.end() );
	}

	XO_TEST_CASE( xo_container_algorithms )
	{
		std::vector<double> values{ 7.0, 4.0, 2.0, 3.0 };
		XO_CHECK( average( values ) == 4.0 );
		XO_CHECK( median( values ) == 3.5 );
	}

	XO_TEST_CASE( xo_table )
	{
		table< double > t;
		for ( int i = 0; i < 10; ++i )
		{
			t.add_column( stringf( "C%d", i ) );
			t.add_row( stringf( "R%d", i ) );
			t( stringf( "R%d", i ), stringf( "C%d", i ) ) = i;
		}

		XO_CHECK( t( "R1", "C1" ) == 1.0 );
	}

	XO_TEST_CASE( indexed_map )
	{
		indexed_map< string, string > im;
		XO_CHECK( im.empty() );
		im.emplace_back( "appel", "nothing" );
		im.emplace_back( "appel", "second" );
		im[ "banana" ] = "third";
		XO_CHECK( im.size() == 3 );
		XO_CHECK( im[ 0 ].second == "nothing" );
		im[ "appel" ] = "first";
		XO_CHECK( im[ 0 ].second == "first" );
		XO_CHECK( im.size() == 3 );
		XO_CHECK( im.front().first == "appel" );
		XO_CHECK( im.count( "appel" ) == 2 );
		XO_CHECK( im.contains( "banana" ) );
		XO_CHECK( im.find( "appel" )->second == "first" );
		XO_CHECK( im.find( "appel", ++im.find( "appel" ) )->second == "second" );
		XO_CHECK( im.find( "banana" )->second == "third" );
		XO_CHECK( im.find( "banana", ++im.find( "banana" ) ) == im.end() );
	}
}
