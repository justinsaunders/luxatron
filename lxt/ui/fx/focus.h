/*
 *  focus.h
 *  starkiller
 *
 *  Created by Justin on 13/06/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_UI_FX_FOCUS_H__
#define __LXT_UI_FX_FOCUS_H__

#include "math/util.h"

namespace Lxt 
{
	// Apply a "focus" effect, increasing alpha and decreasing size to "focus",
	// and decreasing alpha and increasing size to "defocus". Just a silly UI effect.
	class Focus
	{
	public:
		Focus(	float a_startAlpha, float a_endAlpha, 
				float a_startSize, float a_endSize,
				float a_period )
			:	
				m_alpha( a_startAlpha ),
				m_size( a_startSize),
				m_startAlpha( a_startAlpha ),
				m_endAlpha( a_endAlpha ),
				m_startSize( a_startSize ),
				m_endSize( a_endSize ),
				m_period( a_period )
		{
		}

		void Update( float a_secondsElapsed )
		{
			if ( a_secondsElapsed > m_period ) a_secondsElapsed = m_period;
			
			const float t = a_secondsElapsed/m_period;
			
			m_alpha = Lxt::Lerp( m_startAlpha, m_endAlpha, t );
			m_size	= Lxt::Lerp( m_startSize, m_endSize, t );
		}
		
		float Alpha() const { return m_alpha; };
		float Size() const	{ return m_size; };
		
	private:
		float m_alpha, m_size;
		
		const float m_period;
		const float m_startAlpha, m_endAlpha;
		const float m_startSize, m_endSize;
	};
}

#endif // __LXT_UI_FX_FOCUS_H__