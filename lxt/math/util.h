/*
 *  util.h
 *  Luxatron
 *
 *  Created by Justin on 14/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_MATH_UTIL_H__
#define __LXT_MATH_UTIL_H__

#include <stdlib.h>

namespace Lxt
{
	struct Mat4;
	struct Quat;
	struct Vec4;
	struct Vec3;
	
	// Error tolerance for floating point comparison
	#define LXT_EPSILON 0.0001f
	
	inline bool Equal( float a_a, float a_b, float a_error = LXT_EPSILON )
	{
		float delta = fabsf( a_b - a_a );
		return delta <= a_error;
	}
	
	inline float Clamp( float a_low, float a_high, float a_value )
	{
		return std::min( std::max( a_value, a_low ), a_high );
	}
	
	inline float Lerp( float a_a, float a_b, float a_t )
	{
		return a_a + ( ( a_b - a_a ) * a_t );
	}
	
	// Return value on cosine curve where a_a => 0 and a_b => pi 
	float EaseInEaseOut(float a_a, float a_b, float a_t);
	
	inline float RandomInRange(float a_min, float a_max)
	{
		float r = float(rand()) / float(RAND_MAX);
	
		return Lerp(a_min, a_max, r);
	}
	
	inline int Random(int a_max)
	{
		return rand()%(a_max+1);
	}
	
	inline bool IsPow2( uint32_t a_i )
	{
		if ( a_i == 0 ) return false;
		
		return ((a_i - 1) & a_i) == 0;
	}
	
	inline float DegToRad( float degrees )
	{
		return degrees/180.0f * float(M_PI);
	}
	
	// Make a perspective projection matrix
	void MakePerspectiveMatrix(	float a_FOV, 
								float a_aspect, 
								float a_near, 
								float a_far,
								Mat4& a_output );
	
	// Make a parallel projection matrix
	void MakeOrthographicMatrix( float a_left,
								 float a_right,
								 float a_bottom,
								 float a_top,
								 float a_near,
								 float a_far,
								 Mat4& a_output );
	
	// Make a rotation matrix around a vector (x,y,z)
	void MakeRotationMatrix( float a_degrees,
							 float a_x,
							 float a_y,
							 float a_z,
							 Mat4& a_output );
	
	
	// Make a matrix to rotate a around a point (abc) with a given axis (uvw)
	void MakeRotationAroundLineMatrix(const float	a_degrees, 
									  const float	a_a,
									  const float	a_b,
									  const float	a_c,
									  const float	a_u,
									  const float	a_v,
									  const float	a_w,
									  Mat4&			a_matrix );
	
	// Make a LookAt matrix given a position, target and unit up vector.
	void MakeLookAtMatrix(const Vec4&	a_pos, 
						  const Vec4&	a_target, 
						  const Vec4&	a_up,
						  Mat4&			a_mat );
} // namespace Lxt

#endif // __LXT_MATH_UTIL_H__