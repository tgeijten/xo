#include "plane.h"

namespace xo
{
	vec3f intersection( const linef& r, const plane& s, const transformf& t )
	{
		vec3f delta_pos = r.pos_ - t.p;
		vec3f plane_normal = t.q * s.normal_;
		auto p1 = dot_product( delta_pos, plane_normal );
		auto p2 = dot_product( r.dir_, plane_normal );
		return r.pos_ - ( p1 / p2 ) * r.dir_;
	}
}
