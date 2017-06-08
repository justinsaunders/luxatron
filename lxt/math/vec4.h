/*
 *  vec4.h
 *  Luxatron™
 *
 *  Created by Justin on 27/09/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_MATH_VEC4__
#define __LXT_MATH_VEC4__

#include "vec3.h"
#include "util.h"

namespace Lxt 
{	
	// A single precision floating point vector with four components. A Vec4 is
	// un-initialised by default.
	struct Vec4
	{
		// Idenity/Zero Matrix
		const static Vec4 Zero;
		
		// operations
		inline void		Set( float a_x, float a_y, float a_z, float a_w );
		inline void		Copy( const Vec4& a_rhs );
		inline void		Add( const Vec4& a_rhs );
		inline void		Subtract( const Vec4& a_rhs );
		inline void		Scale( float a_scale );
		inline float	Dot( const Vec4& a_rhs ) const;
		inline void		Cross3( const Vec4& a_rhs, Vec4& a_result ) const;
		inline float	Length() const;
		inline float	LengthSquared() const;
		inline void		Normalise();
		
		inline Vec3		Xyz() const;
		
		// comparison
		inline bool		Equal( const Vec4& a_rhs, float a_error = LXT_EPSILON ) const;
		
		// overloaded operators
		inline Vec4&	operator=( const Vec4& a_rhs );
		
		// creation
		inline Vec4();
		inline Vec4( const Vec4& );
		inline Vec4( float a_x, float a_y, float a_z, float a_w );
		inline ~Vec4() {};
		
	public:
		union
		{
			// Array access
			float m_v[4];

			// named access - dimensions
			struct
			{
				float m_x, m_y, m_z, m_w;
			};
			
			// named access - colour components
			struct
			{
				float m_r, m_g, m_b, m_a;
			};
			
			// TODO - use gcc vector extension
			// typedef float m_v4sf __attribute__ ((vector_size (16)));		
		};
	};

// Include regular cpp implementation
#include "vec4_reference_impl.h"

} // namespace Lxt

#endif // __LXT_MATH_VEC4__
