/*
 *  lig_mesh.h
 *  convert_dae
 *
 *  Created by Justin on 8/07/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_LIG_MESH_H__
#define __LXT_LIG_MESH_H__

#include "lig_material.h"
#include "lig_stream.h"

#include "gfx/vertex_declaration.h"

namespace Lxt 
{	
	// A semantic maps indices that point to vertices, both are owned by LigMesh
	struct LigSemantic
	{
		std::string	m_name;
		size_t		m_istreamOffset; // offset of index stream in mesh 
		size_t		m_vstreamOffset; // offset of vertex stream in mesh
	};
	
	// A batch is a collection of indices into source streams for one material.
	// Eg it is a submesh defined by a single material. The material is stored
	// in the scene that owns the mesh.
	//
	// LigMeshes have multiple index streams, but later conversion may remove 
	// this if hardware does not support it.
	struct LigBatch
	{
		std::string					m_materialName;		
		std::vector< LigSemantic >	m_semantics;
	};
	
	// A mesh is made from one or more batches (submeshes) each with a unique
	// material.
	struct LigMesh
	{
		PrimitiveType					m_primitiveType;
		std::vector< LigVertexStream >	m_vertexStreams; // all vertices used
		std::vector< LigIndexStream >	m_indexStreams;	 // all indices used
		std::vector< LigBatch >			m_batches;
	};
}

#endif // __LXT_LIG_NODE_H__
