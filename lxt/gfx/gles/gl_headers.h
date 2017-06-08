/*
 *  gl_headers.h
 *  Luxatron™
 *
 *  Created by Justin on 1/02/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_GFX_GL_HEADERS_H_
#define __LXT_GFX_GL_HEADERS_H_

#include "core/platforms.h"

#if LXT_PLATFORM_CURRENT == LXT_PLATFORM_IPHONE
	#include <OpenGLES/ES1/gl.h>
#elif LXT_PLATFORM_CURRENT == LXT_PLATFORM_MAC
	#include <OpenGL/OpenGL.h>
#else
	#error "Undefined platform, can't include OpenGL.h"
#endif // LXT_PLATFORM_

#endif // __LXT_GFX_GL_HEADERS_H_