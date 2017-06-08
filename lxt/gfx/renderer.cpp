/*
 *  renderer.cpp
 *  Luxatron
 *
 *  Created by Justin on 29/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "renderer.h"

#include "gles/gl_headers.h"
#include "index_buffer.h"
#include "vertex_buffer.h"
#include "vertex_declaration.h"
#include "texture.h"
#include "math/math.h"
#include "mesh.h"
#include "state_cache.h"

using namespace Lxt;

LXT_ZONE_REGISTER(RendererDrawIndexed);
LXT_ZONE_REGISTER(RendererSetMaterial);

namespace // local 
{
    LXT_ZONE_REGISTER(RendererDebugDrawLines);
    
	// This must remain in same order as Lxt::PrimitiveType	
	GLuint LXT_to_GL_PrimitiveMap[] =
	{
		GL_POINTS,
		GL_LINES,
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP
	};
	
	// For statistics, how many indicies per primitive.
	size_t Indices_Per_Primitive[] =
	{
		1,
		2,
		3,
		1,	// not absolutely accurate, but close enough.
	};

	// Draw stuff with lines
	class DebugLines
	{
#pragma pack(push,2)
        struct ShortPos
        {
            ShortPos(uint16_t x,
                     uint16_t y,
                     uint16_t z)
            : m_x(x), m_y(y), m_z(z), m_w(1)
            {
            }
            int16_t m_x, m_y, m_z, m_w;
        };
        
		struct LineVertex
		{
			LineVertex( Vec3 p, Colour c )
			: m_position(p.m_x, p.m_y, p.m_z), m_colour(c)
			{
            }
			
			ShortPos m_position;
			Colour	 m_colour;	
		};
#pragma pack(pop)
		
	public:		
		DebugLines()
		{
			m_lineVertices.reserve(256);
			m_lineIndices.reserve(512);
		}
		
		void AddLine(const Vec3& a_start, const Vec3& a_end, Colour a_colour)
		{
			m_lineVertices.push_back( LineVertex(a_start, a_colour) );
			m_lineVertices.push_back( LineVertex(a_end, a_colour) );
			
			size_t i = m_lineVertices.size();
			m_lineIndices.push_back( i - 2 );
			m_lineIndices.push_back( i - 1 );
		}
		
		void Render( Renderer& a_renderer )
		{
            LXT_ZONE_AUTO(RendererDebugDrawLines);
            
			if (m_lineIndices.size() == 0) return;
			
			Mesh lineMesh(1);
			
			// vertex declaration
			lineMesh.m_primitiveType = LXT_PT_LINE;
			lineMesh.m_vd.Add( VET_SHORT4, VEU_POSITION );
			lineMesh.m_vd.Add( VET_UBYTE4, VEU_COLOUR );
            const size_t str = lineMesh.m_vd.Stride() ;
            const size_t siz = sizeof(LineVertex);
            LXT_ASSERT(str == siz);
			
			// indicies and vertices
			lineMesh.m_vertices.Set((void*)&m_lineVertices[0], 
                                m_lineVertices.size() * sizeof(LineVertex));
			
			lineMesh.m_indices.Set((void*)&m_lineIndices[0], 
									  m_lineIndices.size() *
									  sizeof(IndexBuffer::IndexType));
			
			// material
			LXT_ASSERT( lineMesh.m_batches.size() == 1 );
			lineMesh.m_batches[0].m_indexStart	= 0;
			lineMesh.m_batches[0].m_indexCount	= m_lineIndices.size();
			lineMesh.m_batches[0].m_material.Set( Vec4(1.f,1.f,1.f,1.f),
												 Vec4(0.f,0.f,0.f,0.f),
												 0.f,
												 NULL,
												 MBT_ALPHA,
												 false );
			lineMesh.Upload();
			
			// Draw
			a_renderer.Render( lineMesh );
			
			// forget lines
			m_lineVertices.clear();
			m_lineIndices.clear();
		}
		
	private:
		std::vector<LineVertex> m_lineVertices;
		std::vector<uint16_t>	m_lineIndices;
	};
	
	DebugLines s_debugLines;
}

Renderer::Renderer()
{
	m_stats.m_drawCalls = 0;
	m_stats.m_primitives= 0;
	m_viewWidth			= 0;
	m_viewHeight		= 0;
}

Renderer::~Renderer()
{
}

bool Renderer::Create( size_t a_viewWidth, size_t a_viewHeight )
{
	const GLfloat	lightAmbient[] = {0.0, 0.0, 0.0, 1.0};
	const GLfloat	lightDiffuse[] = {1.0, 1.0, 1.0, 1.0};
	const GLfloat	lightSpecular[] = {1.0, 1.0, 1.0, 1.0};
	const GLfloat	lightPosition[] = {1000.0, 1000.0, 0.1, 0.0}; 
	
	// Configure OpenGL lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular );
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); 

	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	
	glViewport(0, 0, a_viewWidth, a_viewHeight );
	
	m_viewWidth	= a_viewWidth;
	m_viewHeight= a_viewHeight; 

	return true;
}

// View ------------------------------------------------------------------------
void Renderer::GetView( Vec2& a_dimensions )
{
	a_dimensions.m_x = m_viewWidth;
	a_dimensions.m_y = m_viewHeight;
}

void Renderer::SetModelView( const Mat4& a_modelView )
{
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( (const float*)(&a_modelView) );
}

void Renderer::GetModelView( Mat4& a_modelView )
{
	glGetFloatv( GL_MODELVIEW_MATRIX, (float*)(&a_modelView) );
}

void Renderer::SetProjection( const Mat4& a_projection )
{
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( (const float*)(&a_projection) );
	
	// iphone retardation - rotate projection for landscape view
	glRotatef( -90.0f, 0.0f, 0.0f, 1.0f );
}

void Renderer::GetProjection( Mat4& a_projection )
{
	glGetFloatv( GL_PROJECTION_MATRIX, (float*)(&a_projection) );
}

void Renderer::ApplyModelView( const Mat4& a_modelView )
{
	glMatrixMode( GL_MODELVIEW );
	glMultMatrixf( (const float*)(&a_modelView) );	
}

bool Renderer::Render( Mesh& a_mesh )
{
	bool status = false;
	
	// draw each batch
	for ( size_t i = 0; i < a_mesh.m_batches.size(); i++ )
	{
		const Batch& batch = a_mesh.m_batches[i];
		
		status = 
		DrawIndexed(a_mesh.m_vd,
					a_mesh.m_vertices,
					a_mesh.m_indices,
					batch.m_material,
					a_mesh.m_primitiveType, 
					batch.m_indexStart, 
					batch.m_indexCount );
		LXT_ASSERT( status );
	}
	
	return status;
}

void Renderer::GetStats( RenderStats& a_stats )
{
	a_stats = m_stats;
	
	m_stats.m_drawCalls = 0;
	m_stats.m_primitives = 0;
}


void Renderer::AddDebugLine(const Vec3& a_start, 
							const Vec3& a_end, 
							Colour		a_colour)
{
	s_debugLines.AddLine(a_start, a_end, a_colour);
}

void Renderer::DrawDebugLines()
{
	s_debugLines.Render(*this);
}

bool Renderer::DrawIndexed(const VertexDeclaration& a_vd,
						   const VertexBuffer&		a_vb,
						   const IndexBuffer&		a_ib,
						   const Material&			a_material,
						   PrimitiveType			a_primitiveType,
						   uint16_t					a_startIndex,
						   uint16_t					a_numIndices )
{
    LXT_ZONE_AUTO(RendererDrawIndexed);
    
	bool status = true;
	
	// bind vertices first, or vertex pointer binding below doesn't work.
	status = a_vb.Bind();
	
	if ( status )
	{
		// bind declaration
		status = a_vd.Set();
	}
	
	if ( status )
	{
		// bind indicies
		status	&= a_ib.Bind(); 
	}
	
	if ( status )
	{
		// set material, must be done after VertexDeclaration, because that
		// will reset GL_COLOR_MATERIAL state.
		status &= SetMaterial( a_material );
	}
	
	// Draw
	if ( status )
	{
		const size_t offset = sizeof(uint16_t) * a_startIndex;
		
		glDrawElements( LXT_to_GL_PrimitiveMap[ a_primitiveType ], 
						a_numIndices, 
						GL_UNSIGNED_SHORT, 
						(const GLvoid*) offset );
		
		// Update statistics
		m_stats.m_drawCalls++;
		m_stats.m_primitives += 
					a_numIndices/Indices_Per_Primitive[ a_primitiveType ];
		
		status &= glGetError() == GL_NO_ERROR;
	}
	
	return status;
}

bool Renderer::SetMaterial( const Material& a_material ) 
{
    LXT_ZONE_AUTO(RendererSetMaterial);
    
	// Light state
	if ( a_material.m_lit )
	{
		glEnable( GL_LIGHTING );

		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,   a_material.m_diffuse.m_v );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,  a_material.m_specular.m_v );
		glMaterialf(  GL_FRONT_AND_BACK, GL_SHININESS, a_material.m_shininess );		
	}
	else 
	{
		glDisable( GL_LIGHTING );
	}

    // store old depth mask to avoid redundant state calls (todo use a generic state manager)
    static GLint oldDepthMask = -1;
	GLint depthMask = GL_TRUE;
	
	if ( a_material.m_blendType == MBT_OPAQUE )
	{
		// Opaque rendering
		glDisable( GL_BLEND );
	}
	else
	{
		// Translucent rendering
		glEnable( GL_BLEND );
		
		GLint src, dst;
		
		switch ( a_material.m_blendType )
		{
			case MBT_ADD:
				depthMask = GL_FALSE;
				src = GL_SRC_ALPHA;
				dst = GL_ONE;
				break;
			case MBT_ALPHA:
				src = GL_SRC_ALPHA;
				dst = GL_ONE_MINUS_SRC_ALPHA;
				break;
			default:
				LXT_ASSERT( "Unsupported blend type" );
		}
		
		glBlendFunc( src, dst );
	}
    
    if (depthMask != oldDepthMask)
    {
        glDepthMask(depthMask);
        oldDepthMask = depthMask;
    }
	
	// Texture State
	if ( a_material.m_texture == NULL )
	{
		glBindTexture( GL_TEXTURE_2D, 0 ); // unbind current
		glDisable( GL_TEXTURE_2D );
	}
	else
	{
		glEnable( GL_TEXTURE_2D );
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		a_material.m_texture->Bind();
	}
	
	return glGetError() == GL_NO_ERROR;
}