/*
 *  util.h
 *  model
 *
 *  Created by Justin on 3/05/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_GFX_UTIL_H___
#define __LXT_GFX_UTIL_H___

#include "gfx/colour.h"
#include "math/vec3.h"

namespace Lxt
{
	class Texture;
	class Renderer;
	
	// Draw a 2d rectangle with a given given color and texture.
	// If both colour and texture are supplied they will be blended.
	void DrawRect(	Renderer&	a_renderer,
					float a_x,	float a_y,
					float a_x2, float a_y2, 
					Colour		a_colour,
					Texture*	a_texture = NULL );
	
	inline void DrawXZGrid(Renderer& a_renderer);	
}

#endif // __LXT_GFX_UTIL_H___
