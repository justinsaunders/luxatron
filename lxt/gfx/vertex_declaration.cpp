/*
 *  vertex_declaration.cpp
 *  Luxatron
 *
 *  Created by Justin on 25/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "vertex_declaration.h"
#include "gles/gl_headers.h"

namespace // local
{	
	// This maps VertexElementType and VertexElementUsage to OpenGL types and
	// sizes.
	struct TypeInfoEntry
	{
		GLenum		m_type;	// GL type - GL_BYTE, GL_FLOAT etc
		uint16_t	m_size; // sizeof GL type
		uint16_t	m_count;// number of each
	};
	
	static const TypeInfoEntry VET_to_GL_map[ Lxt::VET_MAX ] = 
	{
	/*VET_UBYTE2*/{ GL_UNSIGNED_BYTE, 1, 2 },		
	/*VET_UBYTE4*/{ GL_UNSIGNED_BYTE, 1, 4 },			
	/*VET_SHORT2*/{ GL_SHORT, 2, 2 },
    /*VET_SHORT3*/{ GL_SHORT, 2, 3 },
	/*VET_SHORT4*/{ GL_SHORT, 2, 4 },			
	/*VET_FLOAT1*/{ GL_FLOAT, 4, 1 },	
	/*VET_FLOAT2*/{ GL_FLOAT, 4, 2 },	
	/*VET_FLOAT3*/{ GL_FLOAT, 4, 3 },	
	/*VET_FLOAT4*/{ GL_FLOAT, 4, 4 },	
	};
} // namespace local

namespace Lxt 
{
	VertexDeclaration::VertexDeclaration( size_t sizeHint ) 
		: m_stride( 0 )
	{
		m_decl.reserve( sizeHint );
	}
	
	VertexDeclaration::~VertexDeclaration()
	{
	}
	
	VertexDeclaration& VertexDeclaration::Add(	VertexElementType	a_vt,
												VertexElementUsage	a_vu )
	{
		// Create a new element with current stride as the offset
		VertexElement ve;
		ve.m_offset	= m_stride;
		ve.m_type	= a_vt;
		ve.m_usage	= a_vu;
		
		// add new element to list
		m_decl.push_back( ve );
		
		// offset stride by size of this element
		const TypeInfoEntry& entry = VET_to_GL_map[ a_vt ];
		m_stride += entry.m_count * entry.m_size;
		
		return *this;
	}
	
	bool VertexDeclaration::Set() const
	{
		const size_t n		= m_decl.size();	// number of elements
		
		// switch everything off
		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_COLOR_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		glDisable( GL_COLOR_MATERIAL );
		
		// loop through declaration and enable state and pointer
		for ( size_t i = 0; i < n; i++ )
		{
			// Get element and its offset in buffer
			const VertexElement&	element = m_decl[ i ];
			const GLvoid*			offset	= (const GLvoid*) element.m_offset;
			
			// Get GL type and count of element
			const TypeInfoEntry&	entry	= VET_to_GL_map[ element.m_type ];
			const GLenum			type	= entry.m_type;
			const GLint				size	= entry.m_count;
			
			switch ( element.m_usage )
			{
				case VEU_POSITION:
					glEnableClientState( GL_VERTEX_ARRAY );
					glVertexPointer( size, type, m_stride, offset );
					break;
				case VEU_NORMAL:
					glEnableClientState( GL_NORMAL_ARRAY );
					glNormalPointer( type, m_stride, offset );
					break;
				case VEU_COLOUR:
					glEnable( GL_COLOR_MATERIAL );
					glEnableClientState( GL_COLOR_ARRAY );
					glColorPointer( size, type, m_stride, offset );
					// Note iPhone doesn't support non-size-4 colours of any
					// type - glColorPointer will report an error and glGetError
					// should pick it up.
					break;
				case VEU_TEXCOORD:
					glEnableClientState( GL_TEXTURE_COORD_ARRAY );
					glTexCoordPointer( size, type, m_stride, offset );
					break;
				default:
					// error unhandled vertex usage
					return false;
			}
		}
		
		return glGetError() == GL_NO_ERROR;		
	}
} // namespace Lxt