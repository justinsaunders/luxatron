/*
 *  vec3.h
 *  Luxatron
 *
 *  Created by Justin on 18/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_MATH_VEC3_H__
#define __LXT_MATH_VEC3_H__

namespace Lxt 
{
	// A vector of three floats. This is for storage only - if you require
	// math, then use Vec4 with W set to 0 or 1 as appropriate. 
	// This type is packed by default - if you require 16 byte alignment 
	// then use Vec4.
	struct Vec3
	{
		Vec3()
		{
			// Unititialized by default
		}
		
		Vec3( float a_x, float a_y, float a_z )
		:  m_x( a_x ), m_y( a_y ), m_z( a_z )
		{}
		
		float m_x, m_y, m_z;
	};

} // namespace Lxt

#endif // __LXT_MATH_VEC3_H__
