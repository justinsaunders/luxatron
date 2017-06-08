/*
 *  vertex_declaration.h
 *  Luxatron
 *
 *  Created by Justin on 25/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_GFX_VERTEX_DECLARATION_H__
#define __LXT_GFX_VERTEX_DECLARATION_H__

#include <vector>
#include "core/types.h"

namespace Lxt
{
	enum PrimitiveType
	{
		LXT_PT_POINT			= 0,
		LXT_PT_LINE				= 1,
		LXT_PT_TRIANGLE			= 2,
		LXT_PT_TRIANGLE_STRIP	= 3,
		LXT_PT_MAX
	};
	
	// Element data type and length - mapped internally to render API types
    // If this changes all meshes must be rebuild. Maps to 
	enum VertexElementType
	{
		// Unsigned bytes (8 bit)
		VET_UBYTE2 = 0,
		VET_UBYTE4,
		
		// Signed shorts (16 bit)
		VET_SHORT2,
        VET_SHORT3,
		VET_SHORT4,
		
		// Floats (32 bit)
		VET_FLOAT1,
		VET_FLOAT2,
		VET_FLOAT3,
		VET_FLOAT4,
		VET_MAX
	};
	
	// Element usage
	enum VertexElementUsage
	{
		VEU_POSITION = 0,
		VEU_NORMAL,
		VEU_COLOUR,   
		VEU_TEXCOORD,
		VEU_PARTICLE_SIZE,
		VEU_MAX
	};
	
	// Describes the element type 
	struct VertexElement
	{
		uint32_t			m_offset;	// offset into buffer of element
		VertexElementType   m_type;		// type
		VertexElementUsage  m_usage;	// usage
	};
	
	// Usage
	// VertexDeclaration vd(3).Add( VET_FLOAT3, VEU_POSITION )
	//                     .Add( VET_FLOAT3, VEU_NORMAL )
	//                     .Add( VET_FLOAT2, VEU_TEXCOORD );
	//
	// size_t stride     = vd.Stride();
	//
	class VertexDeclaration
	{
	public:  
		VertexDeclaration( size_t a_sizeHint = 0 );
		~VertexDeclaration();
		
		VertexDeclaration&		Add( VertexElementType	a_vt, 
									 VertexElementUsage a_vu );
		
		size_t					Stride() const	{ return m_stride; }
		size_t					Length() const	{ return m_decl.size(); }
		
		// Prepare hardware for using this vertex declartion. 
		bool					Set() const;
		
	public:
		std::vector<VertexElement>	m_decl;
		uint32_t					m_stride;
	};
	
} // namespace Lxt

#endif // __LXT_GFX_VERTEX_DECLARATION_H__