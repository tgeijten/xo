#pragma once

#include "xo/system/platform.h"
#include "vec3_type.h"
#include "bounding_box.h"
#include "transform.h"
#include "xo/string/dictionary.h"

#if defined(_MSC_VER)
#	pragma warning( push )
#	pragma warning( disable: 4251 )
#endif

namespace xo
{
	class XO_API shape
	{
	public:
		enum shape_type { undefined, sphere, box, capsule, cylinder, cone };
		static dictionary< shape_type > shape_dict;

		shape( shape_type t = undefined, float x_radius = 0, float y_height = 0, float z = 0 );
		shape( const prop_node& pn );

		// shape properties
		shape_type type() const { return type_; }
		const string& name() const;
		float radius() const;
		float height() const;
		float half_height() const;
		vec3f dim() const;
		vec3f half_dim() const;
		float volume() const;
		vec3f corner( index_t idx ) const;
		bounding_boxf bounding_box( const transformf& t ) const;
		float x() const { return x_; }
		float y() const { return y_; }
		float z() const { return z_; }

		// physical properties
		float compute_mass( float density ) const;
		vec3f compute_inertia( float density ) const;

		// modify shape
		void scale( float factor );
		void scale( const vec3f& factor );

	private:
		shape_type type_;
		float x_, y_, z_;
	};
}

#if defined(_MSC_VER)
#	pragma warning( pop )
#endif
