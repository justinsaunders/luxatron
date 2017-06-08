/*
 *  texture.h
 *  Luxatron
 *
 *  Created by Justin on 4/09/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_GFX_TEXTURE_H__
#define __LXT_GFX_TEXTURE_H__

#include "core/types.h"

namespace Lxt 
{
	class Texture : private Noncopyable
	{
	public:
		Texture();
		~Texture();
		
		// Create a texture with given dimensions, and starting at mip level
		// through to another mip level, using given OpenGL formats and type.
		bool Create( uint16_t	a_width,		uint16_t a_height, 
					 uint8_t	a_levelStart,	uint8_t a_levelEnd,
					 uint32_t	a_format,		uint32_t a_type,
					 const void* a_data );
		
		bool Bind();
		bool Unbind();
	
		bool Free();
		
		uint16_t GetWidth() const { return m_width; }
		uint16_t GetHeight() const { return m_height; }
				
	public:
		uint32_t m_id;
		uint16_t m_width;
		uint16_t m_height;
	};
	
	// Utility function to load a texture (uses Core Graphics internally).
	bool TextureLoad( const Path& a_fileName, Lxt::Texture& a_texture );
}

#endif // __LXT_GFX_TEXTURE_H__
