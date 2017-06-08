/*
 *  colour.h
 *  Luxatron
 *
 *  Created by Justin on 24/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_GFX_COLOUR_H__
#define __LXT_GFX_COLOUR_H__

#include "math/vec4.h"

namespace Lxt 
{
	struct Colour
	{
        enum 
        {
            //        AABBGGRR
            ZERO  = 0x00000000,
            WHITE = 0xFFFFFFFF,
            BLACK = 0xFF000000,
            RED   = 0xFF0000FF,
            GREEN = 0xFF00FF00,
            BLUE  = 0xFFFF0000,
            PINK  = 0xFFFF00FF
        };
        
        union 
        {
            uint32_t m_abgr;
            
            struct
            {
                uint8_t m_r;
                uint8_t m_g;
                uint8_t m_b;
                uint8_t m_a;
            };
        };
        
        Colour()
        {
        }
        
        Colour(uint32_t a_rgba)
        {
            Set(a_rgba);
        }
        
        Colour(const Vec4& a_colour)
        {
            Set(a_colour);
        }
        
		// Create a colour struct from an unsigned int with A,B,G,R components
		// in each byte.
        void Set(uint32_t a_rgba)
		{
            m_abgr = ByteSwap(a_rgba);
		};
		
		// Create a colour struct from four floats representing R,G,B,A 
		// components.
		void Set( const Vec4& a_colour )
		{		 
			m_r = uint8_t(((a_colour.m_r * 255.0f) + 0.5f));
			m_g = uint8_t(((a_colour.m_g * 255.0f) + 0.5f));
			m_b = uint8_t(((a_colour.m_b * 255.0f) + 0.5f));
			m_a = uint8_t(((a_colour.m_a * 255.0f) + 0.5f));
		};
		
		// Convert into four floats
		static inline Vec4 GetRGBA( const Colour& a_colour )
		{
			Vec4 c;
			
			c.m_r = float(a_colour.m_r);
			c.m_g = float(a_colour.m_g);
			c.m_b = float(a_colour.m_b);
			c.m_a = float(a_colour.m_a);
			
			c.Scale( 1.0f / 255.0f );
			
			return c;
		};
	};
       
	// Global lerp function
	inline Colour Lerp( Colour a_a, Colour a_b, float a_t )
	{
		// Pretty slow, converting to Vec4, lerping, then back.
		Vec4 a = Colour::GetRGBA( a_a );
		Vec4 b = Colour::GetRGBA( a_b );
		Vec4 c;
		
		c.m_r = Lerp( a.m_r, b.m_r, a_t );
		c.m_g = Lerp( a.m_g, b.m_g, a_t );
		c.m_b = Lerp( a.m_b, b.m_b, a_t );
		c.m_a = Lerp( a.m_a, b.m_a, a_t );
		
		Colour rv; rv.Set( c );
        return rv;
	}
}


#endif // __LXT_GFX_COLOUR_H__