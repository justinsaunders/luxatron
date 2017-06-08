/*
 *  mesh.h
 *  A mesh represents the atomic unit of drawing in LXT. It can represent
 *	triangles, lines or points. Batches divide the mesh into sections by 
 *  material.
 *
 *  Created by Justin on 24/10/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_GFX_MESH_H__
#define __LXT_GFX_MESH_H__

#include <memory>

#include "material.h"
#include "vertex_declaration.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "core/buffer.h"

namespace Lxt
{
	class Archive;
	class Renderer;
	
	struct Batch
	{
		Material m_material;		
		uint16_t m_indexStart;
		uint16_t m_indexCount;
	};
	
	struct Mesh
	{
		// Create mesh
		Mesh( size_t batches = 0 );
		
		// Is mesh ready to be drawn or is it empty?
		bool IsEmpty() { return m_primitiveType == LXT_PT_MAX; };
		
		// Upload mesh to GPU, emptying main memory buffers
		void Upload();
		
		// Output to console, for debugging.
		void Dump();
		
		// Decode a mesh from an archive - will return a fully constructed
		// mesh, or false if something goes wrong. Mesh will still require
		// uploading.
		static bool Extract(Mesh&			a_mesh, 
							TexturePool&	a_tp, 
							Archive&		a_source, 
							size_t&			a_offset );
		
		// Encode a mesh into the given memory buffer.
		static size_t Store(const Mesh&			a_mesh, 
							const TexturePool&	a_tp, 
							Archive&			a_destination );
		
		VertexDeclaration	m_vd;
		VertexBuffer	    m_vertices;
		IndexBuffer			m_indices;
		std::vector<Batch>	m_batches;
		PrimitiveType		m_primitiveType;
	};
}

#endif
