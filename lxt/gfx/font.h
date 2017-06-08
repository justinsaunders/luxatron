/*
 *  font.h
 *  Luxatron™
 *
 *  Created by Justin on 13/09/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_GFX_FONT_H__
#define __LXT_GFX_FONT_H__

#include <vector>
#include <string>
#include "texture.h"
#include "math.h"

namespace Lxt 
{
	// Forward definitions
	class Renderer;
	struct FontInfo;
	struct GlyphInfo;
	struct Vec3;
	struct Colour;
	
	// A class that holds a single font and can render it.
	class Font
	{
	public:
		Font();
		~Font();
		
		// Create a font, returns true if succesful. Font instance can't be
		// recreated, so subsequent calls to Create() will fail.
		bool Create( const char* a_fontFile );

		// Get information about font, returns NULL if font not created.
		const FontInfo* GetInfo() const { return m_info; }
		
		// Get the texture that this font is based on
		Texture*		GetTexture() { return &m_texture; };
		
		// Get the width of a given string of text, but don't draw it
		uint32_t CalculateWidth( const char* a_string );
			
	private:
		FontInfo*			m_info;
		Texture				m_texture;
	}; // class Font
	
	// This holds a record of useful information about the font.
	struct FontInfo
	{
		// Name of the font eg Arial, Georgia
		std::string m_face;
		
		// Filename of texture for this font.
		std::string	m_texturePageFile;
		
		// Size of font (line height) and base height (?) 
		uint16_t	m_size;
		uint16_t	m_base;
		
		// Start and end indices of the font. Currently font must represent a
		// single contiguous range.
		uint16_t	m_startIndex;
		uint16_t	m_endIndex;
		
		// Info about each glyph
		std::vector<GlyphInfo> m_glyphs;
	}; // struct FontInfo
	
	
	// This holds a record of useful information about a glyph.
	struct GlyphInfo
	{
		// UVs are scaled to texture dimensions. 
		Vec2	m_start;
		Vec2	m_end;
		
		uint8_t m_width;
		uint8_t m_height;
		int8_t m_xOffset;
		int8_t m_yOffset;
		uint8_t m_xAdvance;
	};
	
} // namespace Lxt

#endif // __LXT_GFX_FONT_H__
