/*
 *  mesh.cpp
 *  model
 *
 *  Created by Justin on 24/10/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "mesh.h"
#include "core/archive.h"
#include "core/log.h"
#include "texture.h"

namespace Lxt 
{
	Mesh::Mesh( size_t batches )
				:	m_batches( batches ),
					m_primitiveType( LXT_PT_MAX )
	{
	}
	
	void Mesh::Upload()
	{
        LXT_ASSERT( m_indices.Get().Size() > 0 );
		LXT_ASSERT( m_vd.m_stride > 0 );
		
		m_vertices.Upload();
        m_indices.Upload();
	}
	
	void Mesh::Dump()
	{
		const size_t maxElements = 16; // only show this many elements.
		
		LXT_LOG_SPAM("Vertex Declaration ( %d byte stride ):\n",
						m_vd.m_stride );
		
		for ( uint32_t i = 0; i < m_vd.m_decl.size(); i++ )
		{
			LXT_LOG_SPAM( " %d VET=%d VEU=%d offset=%d\n",
							i,
							m_vd.m_decl[i].m_type,
							m_vd.m_decl[i].m_usage,
							m_vd.m_decl[i].m_offset );
		}
		
		float*			vp			= (float*)m_vertices.Get().Data();
		const size_t	numVertices	= m_vertices.Get().Size()/m_vd.m_stride;
		const size_t	nv			= std::min( numVertices, maxElements );
		
		LXT_LOG_SPAM("Vertices: (%lu)\n", numVertices );
		for ( uint32_t j = 0; j < nv; j++ )
		{
			LXT_LOG_SPAM( " %u:", j );
			
			for ( uint32_t i = 0; i < (m_vd.m_stride/sizeof(float) ); i++ )
			{
				LXT_LOG_SPAM( "%2.2f ", *vp++ );
			}
			LXT_LOG_SPAM( "\n" );
		}
		if ( numVertices > nv )
			LXT_LOG_SPAM( "... (%lu more truncated.)\n", numVertices - maxElements);
		
		uint16_t*		ip			= (uint16_t*)m_indices.Get().Data();
		const size_t	numIndices	= m_indices.Get().Size()/sizeof(IndexBuffer::IndexType);
		const size_t	ni			= std::min(numIndices, maxElements);
		
		LXT_LOG_SPAM( "Indices: (%lu)\n", numIndices );
		for ( uint32_t i = 0; i < ni; i++ )
		{
			LXT_LOG_SPAM( "%d ", *ip++ );
		}
		LXT_LOG_SPAM( "\n" );
		if ( numIndices > ni )
			LXT_LOG_SPAM( "... (%lu more truncated.)\n", numIndices - maxElements);
		
		LXT_LOG_SPAM("Batches:\n");
		for ( size_t i = 0; i < m_batches.size(); i++ )
		{
			LXT_LOG_SPAM( " %lu: Range=%d -> %d\n",
						 i,
						 m_batches[i].m_indexStart,
						 m_batches[i].m_indexCount );
			
			m_batches[i].m_material.Dump();
		}
		
		LXT_LOG_SPAM("PrimitiveType: %d\n", m_primitiveType );
						 
	}
	
	bool Mesh::Extract(Mesh&		a_mesh,
					   TexturePool&	a_texPool,
					   Archive&		a_source,
					   size_t&		a_offset )
	{
		Buffer& src = a_source.Storage();
		
		// VertexDeclaration and elements 
		uint32_t numElements = 0;
		a_offset+= src.Read( numElements, a_offset );
		
		a_mesh.m_vd.m_decl.resize( numElements );
		for ( size_t i = 0; i < numElements; i++ )
		{
			VertexElement& ve = a_mesh.m_vd.m_decl[i];
			a_offset += src.Read( ve.m_offset,	a_offset );
			a_offset += src.Read( (uint32_t&) ve.m_type,	a_offset );
			a_offset += src.Read( (uint32_t&) ve.m_usage,	a_offset );
		}
							
		a_offset+= src.Read(a_mesh.m_vd.m_stride, a_offset);
        
        // Vertices
        {
            Buffer vb;
            a_offset+= src.Read(vb, a_offset);
            a_mesh.m_vertices.Set((void*)(vb.Data()), vb.Size()); 
        }
        
        // Indices
        {
            Buffer ib;
            a_offset+= src.Read(ib, a_offset );
            a_mesh.m_indices.Set((void*)(ib.Data()), ib.Size());
        }
		
		// handle batches
		uint32_t numBatches = 0;
		a_offset+= src.Read( numBatches, a_offset );
		
		a_mesh.m_batches.resize( numBatches );
		for ( size_t i = 0; i < numBatches; i++ )
		{
			Material::Extract( a_mesh.m_batches[i].m_material, 
								a_texPool, a_source, a_offset );
			a_offset += src.Read( a_mesh.m_batches[i].m_indexStart, a_offset );
			a_offset += src.Read( a_mesh.m_batches[i].m_indexCount, a_offset );
		}
		
		a_offset+= src.Read( (uint32_t&) a_mesh.m_primitiveType, a_offset );

		return true;
	}
	
	size_t Mesh::Store(const Mesh&		a_mesh,
					 const TexturePool&	a_texPool,
					 Archive&			a_destination )
	{
		Buffer& dst		= a_destination.Storage();
		size_t	written = 0;
		
		// VertexDeclaration and elements
		written += dst.Append( (uint32_t) a_mesh.m_vd.m_decl.size() );
		for ( size_t i = 0; i < a_mesh.m_vd.m_decl.size(); i++ )
		{
			const VertexElement& ve = a_mesh.m_vd.m_decl[i];
			written += dst.Append( ve.m_offset );
			written += dst.Append( (uint32_t) ve.m_type	);
			written += dst.Append( (uint32_t) ve.m_usage	);
		}
		written += dst.Append( a_mesh.m_vd.m_stride );
		
		written += dst.Append( a_mesh.m_vertices.Get() );
		written += dst.Append( a_mesh.m_indices.Get() );
		
		// handle batches
		written += dst.Append( (uint32_t) a_mesh.m_batches.size() ); 
		for ( size_t i = 0; i < a_mesh.m_batches.size(); i++ )
		{
			written += Material::Store( a_mesh.m_batches[i].m_material, 
									    a_texPool,
									    a_destination );
			written += dst.Append( a_mesh.m_batches[i].m_indexStart );
			written += dst.Append( a_mesh.m_batches[i].m_indexCount );
		}
		
		written += dst.Append( (uint32_t) a_mesh.m_primitiveType );
		
		return written;
	}
}
