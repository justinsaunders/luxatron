/*
 *  util.cpp
 *  model
 *
 *  Created by Justin on 3/05/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "util.h"

#include "gfx/gles/gl_headers.h"
#include "gfx/renderer.h"
#include "gfx/mesh.h"
#include "math/vec2.h"

namespace  
{
	struct QuadVertex
	{
		Lxt::Vec2	m_position;
		Lxt::Vec2	m_tex;
		Lxt::Colour	m_colour;
	};
	
	QuadVertex					quadVertices[4];
	Lxt::IndexBuffer::IndexType	quadIndices[6] = { 0, 1, 2, 1, 3, 2 };
}

namespace Lxt 
{
	void DrawRect(	Renderer&	a_renderer,
					float a_x, float a_y,
					float a_x2, float a_y2, 
					Colour		a_colour,
					Texture*	a_texture )
	{
		// TODO - reuse this object each draw
		Mesh quadMesh(1);
		
		// vertex declaration
		quadMesh.m_primitiveType = LXT_PT_TRIANGLE;
		quadMesh.m_vd.Add( VET_FLOAT2, VEU_POSITION );
		quadMesh.m_vd.Add( VET_FLOAT2, VEU_TEXCOORD );
		quadMesh.m_vd.Add( VET_UBYTE4, VEU_COLOUR );
		LXT_ASSERT( quadMesh.m_vd.Stride() == sizeof( QuadVertex ) );
		
		// Set vertices
		quadVertices[0].m_position.m_x	= a_x;
		quadVertices[0].m_position.m_y	= a_y;
		quadVertices[0].m_tex.m_x		= 0;
		quadVertices[0].m_tex.m_y		= 0;
		quadVertices[0].m_colour		= a_colour; 

		quadVertices[1].m_position.m_x	= a_x2;
		quadVertices[1].m_position.m_y	= a_y;
		quadVertices[1].m_tex.m_x		= 1;
		quadVertices[1].m_tex.m_y		= 0;
		quadVertices[1].m_colour		= a_colour; 
		
		quadVertices[2].m_position.m_x	= a_x;
		quadVertices[2].m_position.m_y	= a_y2;
		quadVertices[2].m_tex.m_x		= 0;
		quadVertices[2].m_tex.m_y		= 1;
		quadVertices[2].m_colour		= a_colour; 
		
		quadVertices[3].m_position.m_x	= a_x2;
		quadVertices[3].m_position.m_y	= a_y2;
		quadVertices[3].m_tex.m_x		= 1;
		quadVertices[3].m_tex.m_y		= 1;
		quadVertices[3].m_colour		= a_colour; 
		
		// indicies and vertices
		quadMesh.m_vertices.Set((void*)&quadVertices[0], sizeof(quadVertices));
		
		quadMesh.m_indices.Set((void*)&quadIndices[0], sizeof(quadIndices));
		
		// material
		LXT_ASSERT(quadMesh.m_batches.size() == 1);
		quadMesh.m_batches[0].m_indexStart	= 0;
		quadMesh.m_batches[0].m_indexCount	= LXT_ARRAY_LEN(quadIndices);
		quadMesh.m_batches[0].m_material.Set(Vec4(1.f,1.f,1.f,1.f),
											 Vec4(0.f,0.f,0.f,0.f),
											 0.f,
											 a_texture,
											 MBT_ALPHA,
											 false);
		quadMesh.Upload();
		
		// Draw
		glLoadIdentity();
		a_renderer.Render(quadMesh);
	}
	
	inline void DrawXZGrid(Renderer& a_renderer)
	{
		for (size_t i = 0; i < 10; i++ )
		{
			Vec3 s((i * 500.f) - 2500.f, 0, 2500.f);
			Vec3 e((i * 500.f) - 2500.f, 0, -2500.f);
			
			a_renderer.AddDebugLine(s, e, Colour(0x0000FF80));
			
			std::swap(s.m_x, s.m_z);
			std::swap(e.m_x, e.m_z);
			
			a_renderer.AddDebugLine(s, e, Colour(0x00FF0088));
		}
	}
}
