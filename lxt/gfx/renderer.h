/*
 *  renderer.h
 *  Luxatron
 *
 *  Created by Justin on 29/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_GFX_RENDERER_H__
#define __LXT_GFX_RENDERER_H__

#include <vector>

#include "vertex_declaration.h"
#include "state_description.h"
#include "colour.h"

namespace Lxt 
{
	class StateCache;
	class VertexBuffer;
	class IndexBuffer;
	class Texture;
	class Mat4;
	class Mesh;
	class Material;
	class Vec2;
	
	// Render statistics
	struct RenderStats
	{
		size_t	m_drawCalls;
		size_t	m_primitives;
	};
	
	class Renderer 
	{
	public:
		// Creation
		Renderer();
		bool Create( size_t a_viewWidth, size_t a_viewHeight );
		
		// Destruction
		~Renderer();

		// Get framebuffer dimensions
		void GetView( Vec2&	a_dimensions );
		
		// Set & get current modelview matrix
		void SetModelView( const Mat4&	a_modelView );
		void GetModelView( Mat4&		a_modelView );
		
		// Post-multiply current modelview matrix by given matrix and set
		void ApplyModelView( const Mat4& a_modelView );
		
		// Set & get current projection matrix
		void SetProjection( const Mat4& a_projection );
		void GetProjection( Mat4&		a_projection );
		
		// Render a mesh, returns true if no errors were reported by hardware.
		bool Render( Mesh& a_mesh );
		
		// Statistics
		void GetStats( RenderStats& a_stats );
		
		// Debugging
		void AddDebugLine(const Vec3& a_start, 
						  const Vec3& a_end, 
						  Colour a_colour);
		void DrawDebugLines();
		
	private:
		// Sent current rendering material
		bool SetMaterial( const Material& a_material );
		
		// Render currently bound vertex information
		bool DrawIndexed(const VertexDeclaration&	a_vd,
						 const VertexBuffer&		a_vb,
						 const IndexBuffer&			a_ib,
						 const Material&			a_material,
						 PrimitiveType				a_primitiveType,
						 uint16_t					a_startIndex,
						 uint16_t					a_numIndices );
		
		// Member data
		RenderStats	m_stats;
		size_t		m_viewWidth;
		size_t		m_viewHeight;
	};
	
} // namespace Lxt


#endif // __LXT_GFX_RENDERER_H__
