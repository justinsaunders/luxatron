/*
 *  rect.h
 *  test_runner
 *
 *  Created by Justin on 14/06/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_MATH_RECT_H__
#define __LXT_MATH_RECT_H__

#include "vec2.h"

namespace Lxt 
{
	class Rect
	{
	public:
		Rect()
			: m_x( 0.f ), m_y( 0.f ), m_width( 0.f ), m_height( 0.f )
		{
		}
		
		Rect( float a_x, float a_y, float a_width, float a_height )
			: m_x( a_x ), m_y( a_y ), m_width( a_width ), m_height( a_height )
		{
		}

		// Bounds
		Vec2 A() const { return Vec2( m_x, m_y ); }
		Vec2 B() const { return Vec2( m_x + m_width, m_y + m_height ); }
		
		bool IsWithin( float a_x, float a_y );
		
		// Centre
		Vec2 Centre() const { return Vec2( m_x + m_width/2.0f, m_y + m_height/2.0f ); }
		void SetCentre( float a_x, float a_y );
	
		// Scale
		void Scale( float a_scale );
		
	// Data members
		float m_x, m_y;				// Origin
		float m_width,	m_height;	// Dimensions
	};
	
	inline bool Rect::IsWithin( float a_x, float a_y )
	{
		if ( a_x >= m_x && a_x <= (m_x + m_width) &&
			 a_y >= m_y && a_y <= (m_y + m_height) )
			return true;
		
		return false;
	}
	
	inline void Rect::SetCentre( float a_x, float a_y )
	{
		Vec2 c = Centre();
		Vec2 d;
		
		d.m_x = a_x - c.m_x;
		d.m_y = a_y - c.m_y;
		
		m_x += d.m_x;
		m_y += d.m_y;
	}
	
	inline void Rect::Scale( float a_scale )
	{
		Vec2 c = Centre();
		
		m_width *= a_scale;
		m_height*= a_scale;
		
		SetCentre( c.m_x, c.m_y );
	}
}

#endif // __LXT_MATH_RECT_H__

