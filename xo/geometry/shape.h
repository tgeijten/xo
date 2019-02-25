#pragma once

#include "xo/system/platform.h"
#include "vec3_type.h"
#include "aabb.h"
#include "transform.h"
#include <variant>
#include <array>

#if defined(_MSC_VER)
#	pragma warning( push )
#	pragma warning( disable: 4251 )
#endif

namespace xo
{
	struct XO_API sphere
	{
		float radius_;

		float volume() const { return ( 4.0f / 3.0f ) * constantsf::pi() * cubed( radius_ ); }
		vec3f dim() const { return vec3f( radius_ ); }
	};

	struct XO_API box
	{
		vec3f half_dim_;

		float volume() const { return 8.0f * half_dim_.x * half_dim_.y * half_dim_.z; }
		vec3f dim() const { return 2.0f * half_dim_; }
		std::array< vec3f, 8 > corners() const;
	};

	struct XO_API cylinder
	{
		float radius_;
		float height_;

		float volume() const { return pi_f * squared( radius_ ) * height_; }
		vec3f dim() const { return vec3f( radius_, radius_, height_ ); }
	};

	struct XO_API capsule
	{
		float radius_;
		float height_;

		float volume() const { return pi_f * squared( radius_ ) * ( ( 4.0f / 3.0f ) * ( radius_ + height_ ) ); }
		vec3f dim() const { return vec3f( radius_, radius_, height_ ); }
	};

	struct XO_API cone
	{
		float radius_;
		float height_;

		float volume() const { return pi_f * squared( radius_ ) * height_ / 3.0f; }
		vec3f dim() const { return vec3f( radius_, radius_, height_ ); }
	};

	struct XO_API plane
	{
		vec3f normal_;

		float volume() const { return 0.0; }
		vec3f dim() const { XO_NOT_IMPLEMENTED; }
	};

	enum class shape_type : size_t { sphere, box, cylinder, capsule, cone, plane };
	using shape = std::variant< sphere, box, cylinder, capsule, cone, plane >;

	XO_API shape make_shape( const prop_node& pn );

	XO_API float volume( const shape& s );

	inline void scale( sphere& s, float f ) { s.radius_ *= f; }
	inline void scale( box& s, float f ) { s.half_dim_ *= f; }
	inline void scale( cylinder& s, float f ) { s.radius_ *= f; s.height_ *= f; }
	inline void scale( capsule& s, float f ) { s.radius_ *= f; s.height_ *= f; }
	inline void scale( cone& s, float f ) { s.radius_ *= f; s.height_ *= f; }
	inline void scale( plane& s, float f ) { }
	XO_API void scale( shape& s, float f );

	XO_API vec3f dim( const shape& s );

	XO_API aabbf aabb( const sphere& s, const transformf& t );
	XO_API aabbf aabb( const box& s, const transformf& t );
	XO_API aabbf aabb( const cylinder& s, const transformf& t );
	XO_API aabbf aabb( const capsule& s, const transformf& t );
	XO_API aabbf aabb( const cone& s, const transformf& t );
	XO_API aabbf aabb( const plane& s, const transformf& t );
	XO_API aabbf aabb( const shape& s, const transformf& t );

	XO_API vec3f inertia( const sphere& s, float density );
	XO_API vec3f inertia( const box& s, float density );
	XO_API vec3f inertia( const cylinder& s, float density );
	XO_API vec3f inertia( const capsule& s, float density );
	XO_API vec3f inertia( const cone& s, float density );
	XO_API vec3f inertia( const plane& s, float density );
	XO_API vec3f inertia( const shape& s, float density );
}

#if defined(_MSC_VER)
#	pragma warning( pop )
#endif
