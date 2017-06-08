/*
 *  dae_geometry_to_lig.h
 *  convert_dae
 *
 *  Created by Justin on 18/08/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_DAE_GEOMETRY_TO_LIG_H__
#define __LXT_DAE_GEOMETRY_TO_LIG_H__

#include "lig_scene.h"
#include "lig_stream.h"

class domGeometry;
class domSource;
class domTriangles;

namespace Lxt
{
	class LigMesh;
	
	// The input for a triangles element
	struct DomInput
	{
		size_t					m_inputOffset;		// offset of input in p indices
		std::string				m_semantic;			// input semantic
		LigIndexStream			m_indices;			// source index data
		domSource*				m_source;			// source vertex data
	};
	
	typedef std::vector<DomInput> DomInputGroup;
	
	// The triangles element, with its inputs
	struct DomTriangles
	{
		domTriangles*	m_triangles;
		DomInputGroup	m_inputs;
	};
	
	typedef std::vector<DomTriangles> DomTrianglesGroup;
	
	typedef std::vector<domSource*> DomSourceGroup;
	
	// All the triangles, sources and inputs for a single geometry->mesh element.
	// All elements appear in order of definition in file, and all elements are
	// owned by the dae document.
	struct DomGeometryMesh
	{
		DomTrianglesGroup	m_triangles;
		DomSourceGroup		m_sources;
	};
		
	// Convert a COLLADA geometry to lig meshes - this is the entry point 
	// for this module. Other functions are exposed for testing.
	bool ConvertDomGeometryToLigMeshes(	domGeometry*			a_geometry, 
										const LigMaterialMap&	a_materials,
										std::vector<LigMesh*>&	a_outMeshes );
	
	// Convert from COLLADA to simplified intermediate structure
	bool ConvertDomGeometryToDGM(	domGeometry*			a_geometry,
									Lxt::DomGeometryMesh&	a_dgm );

	// Convert from intermediate structure to LigMesh
	bool ConvertDGMToLigMeshes( const DomGeometryMesh&	a_dgm, 
								const LigMaterialMap&	a_materials,
							    std::vector<LigMesh*>&	a_outMeshes	);
	
};

#endif // __LXT_DAE_GEOMETRY_TO_LIG_H__