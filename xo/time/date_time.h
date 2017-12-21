#pragma once
#include "xo/numerical/bounded.h"

#include <cstdint>

namespace xo
{
	struct date_time
	{
		int year;
		enum month_t { jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec } month;
		bounded< int, 1, 31 > day;
		enum day_of_week_t { mon, tue, wed, thu, fri, sat, sun } day_of_week;
		bounded< int, 0, 23 > hour;
		bounded< int, 0, 59 > minute;
		bounded< int, 0, 59 > second;
		bounded< int, 0, 999 > millisecond;

		std::int64_t timestamp() const;
	};

	bool operator<( const date_time& a, const date_time& b ) {
		return a.timestamp() < b.timestamp();
	}
}
