/*
 *  text_span.h
 *  starkiller
 *
 *  Created by Justin on 6/09/10.
 *  Copyright 2010 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_GFX_TEXT_SPAN_H__
#define	__LXT_GFX_TEXT_SPAN_H__

#include <string>
#include "colour.h"
#include "math/Vec2.h"

namespace Lxt 
{
	class Renderer;
	class Mesh;
	class Camera;
	class Font;
	
	class TextSpan
	{
	public:
		TextSpan( Font& a_font, Colour a_colour, const std::string& a_text );
		~TextSpan();
		
		void SetText( const std::string& a_text );
		void SetPosition( const Vec2& a_position );
		
		void Render( Renderer& a_renderer );	
		
		static Mesh* CreateMesh( Font& a_font, Colour a_colour,
								const char* a_string );
	private:
		Font&	m_font;
		Colour	m_colour;
		Vec2	m_position;
		Mesh*	m_mesh;
	};
}

#endif // __LXT_GFX_TEXT_SPAN_H__