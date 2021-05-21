#pragma once

#include "xo/container/pair_type.h"

template <typename T, typename U>
xo::pair<T, U> operator+( const xo::pair<T, U>& l, const xo::pair<T, U>& r ) {
	return { l.first + r.first, l.second + r.second };
}
template <typename T, typename U>
xo::pair<T, U>& operator+=( xo::pair<T, U>& l, const xo::pair<T, U>& r ) {
	l.first += r.first; l.second += r.second; return l;
}
template <typename T, typename U>
xo::pair<T, U> operator-( const xo::pair<T, U>& l, const xo::pair<T, U>& r ) {
	return { l.first - r.first, l.second - r.second };
}
template <typename T, typename U>
xo::pair<T, U>& operator-=( xo::pair<T, U>& l, const xo::pair<T, U>& r ) {
	l.first -= r.first; l.second -= r.second; return l;
}
