#pragma once

#include "xo/system/platform.h"
#include "vec3_type.h"
#include "bounding_box.h"
#include "transform.h"

namespace xo
{
	class XO_API shape
	{
	public:
		enum type { undefined, sphere, box, capsule, cylinder, cone };

		shape( type t = undefined, float x_radius = 0, float y_height = 0, float z = 0 );
		shape( const prop_node& pn );

		float radius() const;
		float height() const;
		float half_height() const;
		vec3f dim() const;
		vec3f half_dim() const;
		float volume() const;
		vec3f corner( index_t idx ) const;
		bounding_boxf bounding_box( const transformf& t ) const;
		
		void scale( float factor );
		void scale( const vec3f& factor );

		type type_;
		float x_, y_, z_;
	};
}
