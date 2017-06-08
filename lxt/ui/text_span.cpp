/*
 *  text_span.cpp
 *  starkiller
 *
 *  Created by Justin on 6/09/10.
 *  Copyright 2010 Monkey Style Games. All rights reserved.
 *
 */

#include "text_span.h"
#include "renderer.h"
#include "mesh.h"
#include "camera.h"
#include "font.h"

#include "gfx/gles/gl_headers.h"

namespace // local
{
	struct FontVertex
	{
		Lxt::Vec2	m_position;
		Lxt::Vec2	m_tex;
		Lxt::Colour	m_colour;
	};
} // namespace local

namespace Lxt 
{
	
TextSpan::TextSpan( Font& a_font, Colour a_colour, const std::string& a_text )
 : m_font( a_font ), m_colour( a_colour ), m_position( 0.f, 0.f ),
	m_mesh( NULL )
{
	m_mesh = CreateMesh( m_font, m_colour, a_text.c_str() );
}

TextSpan::~TextSpan()
{
	delete m_mesh; m_mesh = NULL;
}

void TextSpan::SetText( const std::string& a_text )
{
	delete m_mesh;
	m_mesh = TextSpan::CreateMesh( m_font, m_colour, a_text.c_str() );
}

void TextSpan::SetPosition( const Vec2& a_position )
{
	m_position = a_position;
}

void TextSpan::Render( Renderer& a_renderer )
{	
	glLoadIdentity();
	glTranslatef( m_position.m_x, m_position.m_y, 0.0f );
	
	a_renderer.Render( *m_mesh );
}

Mesh* TextSpan::CreateMesh( Font&		a_font, 
							Colour	    a_colour,
							const char* a_string )
{
	size_t length = strlen( a_string );
	
	// create vertex buffer
	std::vector< FontVertex > fv;
	fv.reserve( length * 4 );
	
	// create index buffer
	std::vector< uint16_t > fi;
	fi.reserve( length * 6 );
	
	size_t x = 0;
	
	for ( size_t i = 0; i < length; i++ )
	{
		// get glyph info this char
		size_t c = a_string[i] - a_font.GetInfo()->m_startIndex; // index
		const Lxt::GlyphInfo& gi = a_font.GetInfo()->m_glyphs[ c ];
		
		FontVertex	v1 = 
		{
			// vert 1
			Vec2( float(gi.m_xOffset) + x + 0.0f, gi.m_yOffset ),
			Vec2( gi.m_start.m_x, gi.m_start.m_y ),
			a_colour,
		};
		
		FontVertex v2 = 
		{
			// vert 2
			Vec2( float(gi.m_xOffset) + x + gi.m_width,  gi.m_yOffset ),
			Vec2( gi.m_end.m_x, gi.m_start.m_y ),
			a_colour,
		};
		
		FontVertex v3 = 
		{
			// vert 3
			Vec2( float(gi.m_xOffset) + x + 0.0f,  gi.m_yOffset + gi.m_height ),
			Vec2( gi.m_start.m_x, gi.m_end.m_y ),
			a_colour,
		};
		
		FontVertex v4 = 
		{
			// vert 3
			Vec2( float(gi.m_xOffset) + x + gi.m_width,  gi.m_yOffset + gi.m_height ),
			Vec2( gi.m_end.m_x, gi.m_end.m_y ),
			a_colour,
		};			
		
		fv.push_back( v1 );
		fv.push_back( v2 );
		fv.push_back( v3 );
		fv.push_back( v4 );
		
		const size_t istep = i * 4;
		fi.push_back( 0 + istep );
		fi.push_back( 1 + istep );
		fi.push_back( 2 + istep );
		fi.push_back( 1 + istep );
		fi.push_back( 3 + istep );
		fi.push_back( 2 + istep );
		
		// move cursor
		x += gi.m_xAdvance;
	}
	
	// Allocate a new mesh with one batch, caller will own mesh
	Mesh* newMesh = new Mesh(1);
	
	// vertex declaration
	newMesh->m_primitiveType = LXT_PT_TRIANGLE;
	newMesh->m_vd.Add( VET_FLOAT2, VEU_POSITION );
	newMesh->m_vd.Add( VET_FLOAT2, VEU_TEXCOORD );
	newMesh->m_vd.Add( VET_UBYTE4, VEU_COLOUR );
	LXT_ASSERT( newMesh->m_vd.Stride() == sizeof(FontVertex) );
	
	// indicies and vertices
	newMesh->m_vertices.Set( (void*) &fv[0], fv.size() * sizeof(fv[0]) );
	newMesh->m_indices.Set( (void*) &fi[0], fi.size() * sizeof(fi[0]) );
	
	// material
	newMesh->m_batches[0].m_indexStart	= 0;
	newMesh->m_batches[0].m_indexCount	= fi.size();
	newMesh->m_batches[0].m_material.Set( Vec4(1.f,1.f,1.f,1.f ),
										 Vec4(0.f,0.f,0.f,0.f ),
										 0.f,
										 a_font.GetTexture(),
										 MBT_ALPHA,
										 false );
	newMesh->Upload();

	return newMesh;
}

} // namespace Lxt