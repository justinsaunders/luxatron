/*
 *  lxt_splash.h
 *  LXT splash section that must be included in all LXT applications
 *
 *  Created by Justin on 13/06/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_SPLASH_H__
#define	__LXT_SPLASH_H__

#include "game/section.h"
#include "gfx.h"
#include "math/math.h"

#include "../ui/fx/focus.h"

namespace Lxt 
{
	class Camera;

	class LxtSplash : public Lxt::Section
	{
	public:
		LxtSplash(SectionController& a_sectionController);
		virtual ~LxtSplash();
		
		virtual void Prepare( Lxt::ProgressInfo* a_pi );
		virtual bool Play();
		virtual void Draw(); 
		
	private:
		// Resources
		Lxt::Camera		m_2dCamera;
		Lxt::Texture	m_lxtTexture;
		Lxt::Handle		m_lxtSound;
		
		// Effects
		Focus			m_focusEffect;
		float			m_alpha;
		float			m_size;
		
		// Play
		const float		m_duration;				// how long section will last
		Lxt::Rect		m_lxtRect;				// tile dimensions
	};
}

#endif // __LXT_SPLASH_H__