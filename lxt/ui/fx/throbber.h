/*
 *  throbber.h
 *  Cycle between two values given an initial period of a whole cycle.
 *	Crappy UI level code.
 *
 *  Created by Justin on 13/06/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_UI_FX_THROBBER_H__
#define __LXT_UI_FX_THROBBER_H__

#include "gfx/colour.h"

namespace Lxt 
{
	class Throbber
	{
	public:
		typedef Lxt::Colour T; // todo make generic type
		
		Throbber( T a_value1, T a_value2, float a_period )
		{
			m_value1 = a_value1;
			m_value2 = a_value2;
			m_period = a_period;		
		}
		
		T GetValue( float a_time )
		{
			const float hp= m_period/2.0f;				// half period
			float		t = fmodf( a_time, m_period );
			
			T a, b;
			
			if ( t > hp )
			{
				t -= hp;
				b = m_value1;
				a = m_value2;
			}
			else
			{
				a = m_value1;
				b = m_value2;
			}
			
			// Scale t to 0->1 == half period
			t = t / hp;
			T value = Lerp( a, b, t );
			return value;
		}
		
	private:
		T		m_value1;
		T		m_value2;
		float	m_period;
	};
}

#endif // __LXT_UI_FX_THROBBER_H__