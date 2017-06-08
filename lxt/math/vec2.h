/*
 *  vec3.h
 *  Luxatron
 *
 *  Created by Justin on 18/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_MATH_VEC2_H__
#define __LXT_MATH_VEC2_H__

namespace Lxt 
{
	// A vector of two floats.
	struct Vec2
	{
		Vec2()
		{
		}
		
		Vec2( float a_x, float a_y ) : m_x( a_x ), m_y( a_y )
		{
		}
		
		float m_x, m_y;
	};
} // namespace Lxt

#endif // __LXT_MATH_VEC3_H__