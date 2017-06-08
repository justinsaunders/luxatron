/*
 *  quat.h
 *  Quaternion class
 *
 *  Created by Justin on 30/10/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_MATH_QUAT_H__
#define __LXT_MATH_QUAT_H__

#include "vec4.h"

namespace Lxt 
{
	struct Mat4;
	
	struct Quat : public Vec4
	{
	public:
		// Idenity/Zero Quaternion
		const static Quat Identity;
		const static Quat Zero;
		
		// creation
		inline Quat();
		inline Quat( const Quat& );
		inline Quat( float a_x, float a_y, float a_z, float a_w );
		inline ~Quat() {};
		
		// Conversion
		void FromRotationMatrix( const Mat4& a_rotation );
		void ToRotationMatrix( Mat4& a_rotation ) const;
		void FromAngleAxis( float a_degrees, float a_x, float a_y, float a_z );
		
		// Interpolation
		void Slerp( const Quat& a_q0, const Quat& a_q1, float a_t );
	};
	
	inline Quat::Quat()
	{
	}
	
	inline Quat::Quat( const Quat& a_other )
	{
		Copy( a_other );
	}
	
	inline Quat::Quat( float a_x, float a_y, float a_z, float a_w )
	{
		Set( a_x, a_y, a_z, a_w );
	}	
}

#endif // __LXT_MATH_QUAT_H__
