/*
 *  lig_tests.cpp
 *  mac_test_runner
 *
 *  Created by Justin on 24/08/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include <UnitTest++.h>

#include <dae.h>
#include <dom/domCOLLADA.h>

#include "../dae_to_lig.h"
#include "../dae_geometry_to_lig.h"
#include "../lig_scene.h"
#include "../lig_mesh.h"

namespace Lxt
{
	struct GeometryFixture
	{
		DAE				dae;
		domGeometry*	dg;
		
		GeometryFixture( char* a_dae_path, char* a_geometry_id )
		{
			daeElement* root = dae.open( a_dae_path );
			LXT_ASSERT( root != NULL );
			
			// Get the first geometry node
			std::vector<daeElement*>	elements= dae.getDatabase()->idLookup( a_geometry_id );
			LXT_ASSERT( 1 == elements.size() );
			
			dg	=(domGeometry*) elements[0];
			LXT_ASSERT( NULL != dg );
		}
	};
	
	struct TriangleFixture : public GeometryFixture
	{
		TriangleFixture() :
		GeometryFixture( "/Users/justin/Projects/msg/art/test/unit_tested/triangle.dae",
						"box-lib" )
		{
		}
	};
	
	struct MultiVerticesFixture : public GeometryFixture
	{
		MultiVerticesFixture() :
		GeometryFixture( "/Users/justin/Projects/msg/art/test/unit_tested/triangle_multi_verts.dae",
						"box-lib" )
		{
		}
	};
		
	// TODO remove absolute paths
	TEST_FIXTURE( TriangleFixture, DAE_Simple_to_DGM )
	{
		Lxt::DomGeometryMesh dgm;
		bool status = Lxt::ConvertDomGeometryToDGM( dg, dgm );
		CHECK( status );
		
		// Check sources
		CHECK_EQUAL( 3, dgm.m_sources.size() );
		
		// Check triangles
		CHECK_EQUAL( 2, dgm.m_triangles.size() );
		
		// Check inputs
		CHECK_EQUAL( 3, dgm.m_triangles[0].m_inputs.size() );
		CHECK_EQUAL( 3, dgm.m_triangles[1].m_inputs.size() );
		
		DomInputGroup& dig = dgm.m_triangles[0].m_inputs;
		
		CHECK_EQUAL( dig[0].m_inputOffset, 0 );
		CHECK_EQUAL( dig[0].m_semantic, "POSITION" );
		CHECK_EQUAL( dig[0].m_source, dgm.m_sources[0] );
		
		CHECK_EQUAL( dig[1].m_inputOffset, 1 );
		CHECK_EQUAL( dig[1].m_semantic, "NORMAL" );
		CHECK_EQUAL( dig[1].m_source, dgm.m_sources[1] );
		
		CHECK_EQUAL( dig[2].m_inputOffset, 2 );
		CHECK_EQUAL( dig[2].m_semantic, "COLOR" );
		CHECK_EQUAL( dig[2].m_source, dgm.m_sources[2] );
		
		// check indices for triangle 1
		CHECK_EQUAL( 0, dig[0].m_indices.m_elements[0] );
		CHECK_EQUAL( 1, dig[0].m_indices.m_elements[1] );
		CHECK_EQUAL( 2, dig[0].m_indices.m_elements[2] );
		
		CHECK_EQUAL( 0, dig[1].m_indices.m_elements[0] );
		CHECK_EQUAL( 0, dig[1].m_indices.m_elements[1] );
		CHECK_EQUAL( 0, dig[1].m_indices.m_elements[2] );
		
		CHECK_EQUAL( 0, dig[2].m_indices.m_elements[0] );
		CHECK_EQUAL( 1, dig[2].m_indices.m_elements[1] );
		CHECK_EQUAL( 2, dig[2].m_indices.m_elements[2] );
	
		// check indices for triangle 2
		DomInputGroup& dig2 = dgm.m_triangles[1].m_inputs;
		
		CHECK_EQUAL( 2, dig2[0].m_indices.m_elements[0] );
		CHECK_EQUAL( 1, dig2[0].m_indices.m_elements[1] );
		CHECK_EQUAL( 0, dig2[0].m_indices.m_elements[2] );
		
		CHECK_EQUAL( 1, dig2[1].m_indices.m_elements[0] );
		CHECK_EQUAL( 1, dig2[1].m_indices.m_elements[1] );
		CHECK_EQUAL( 1, dig2[1].m_indices.m_elements[2] );
		
		CHECK_EQUAL( 2, dig2[2].m_indices.m_elements[0] );
		CHECK_EQUAL( 1, dig2[2].m_indices.m_elements[1] );
		CHECK_EQUAL( 0, dig2[2].m_indices.m_elements[2] );
	}
	
	TEST_FIXTURE( TriangleFixture, DAE_Simple_to_LIG )
	{
		Lxt::DomGeometryMesh dgm;
		bool status = Lxt::ConvertDomGeometryToDGM( dg, dgm );
		CHECK( status );
	
		LigMaterialMap			dummy_materials;
		std::vector<LigMesh*>	meshes;
		status = Lxt::ConvertDGMToLigMeshes( dgm, dummy_materials, meshes );
		
		// Check output
		CHECK( status );
		CHECK_EQUAL( 1, meshes.size() );
		
		LigMesh* m = meshes[0];
		
		// Check primitive type
		CHECK_EQUAL( LXT_PT_TRIANGLE, m->m_primitiveType );
		
		// Check vertex streams
		CHECK_EQUAL( 3, m->m_vertexStreams.size() );
		
		// Check index streams
		CHECK_EQUAL( 6, m->m_indexStreams.size() );
		
		// triangle 1
		CHECK_EQUAL( 0, m->m_indexStreams[0].m_elements[0] );
		CHECK_EQUAL( 1, m->m_indexStreams[0].m_elements[1] );
		CHECK_EQUAL( 2, m->m_indexStreams[0].m_elements[2] );
		
		CHECK_EQUAL( 0, m->m_indexStreams[1].m_elements[0] );
		CHECK_EQUAL( 0, m->m_indexStreams[1].m_elements[1] );
		CHECK_EQUAL( 0, m->m_indexStreams[1].m_elements[2] );
		
		CHECK_EQUAL( 0, m->m_indexStreams[2].m_elements[0] );
		CHECK_EQUAL( 1, m->m_indexStreams[2].m_elements[1] );
		CHECK_EQUAL( 2, m->m_indexStreams[2].m_elements[2] );
		
		// triangle 2
		CHECK_EQUAL( 2, m->m_indexStreams[3].m_elements[0] );
		CHECK_EQUAL( 1, m->m_indexStreams[3].m_elements[1] );
		CHECK_EQUAL( 0, m->m_indexStreams[3].m_elements[2] );
		
		CHECK_EQUAL( 1, m->m_indexStreams[4].m_elements[0] );
		CHECK_EQUAL( 1, m->m_indexStreams[4].m_elements[1] );
		CHECK_EQUAL( 1, m->m_indexStreams[4].m_elements[2] );
		
		CHECK_EQUAL( 2, m->m_indexStreams[5].m_elements[0] );
		CHECK_EQUAL( 1, m->m_indexStreams[5].m_elements[1] );
		CHECK_EQUAL( 0, m->m_indexStreams[5].m_elements[2] );
		
		// Check batches
		CHECK_EQUAL( 2, m->m_batches.size() );
		
		// batch 1
		const LigBatch& b1 = m->m_batches[0];
		CHECK_EQUAL( "BlueSG",	b1.m_materialName );
		CHECK_EQUAL( 3,			b1.m_semantics.size() );
		
		CHECK_EQUAL( "POSITION",b1.m_semantics[0].m_name );
		CHECK_EQUAL( 0,			b1.m_semantics[0].m_istreamOffset );
		CHECK_EQUAL( 0,			b1.m_semantics[0].m_vstreamOffset );

		CHECK_EQUAL( "NORMAL",	b1.m_semantics[1].m_name );
		CHECK_EQUAL( 1,			b1.m_semantics[1].m_istreamOffset );
		CHECK_EQUAL( 1,			b1.m_semantics[1].m_vstreamOffset );
		
		CHECK_EQUAL( "COLOR",	b1.m_semantics[2].m_name );
		CHECK_EQUAL( 2,			b1.m_semantics[2].m_istreamOffset );
		CHECK_EQUAL( 2,			b1.m_semantics[2].m_vstreamOffset );
		
		// batch 2
		const LigBatch& b2= m->m_batches[1];
		CHECK_EQUAL( "BlueSG",	b2.m_materialName );
		CHECK_EQUAL( 3,			b2.m_semantics.size() );
		
		CHECK_EQUAL( "POSITION",b2.m_semantics[0].m_name );
		CHECK_EQUAL( 3,			b2.m_semantics[0].m_istreamOffset );
		CHECK_EQUAL( 0,			b2.m_semantics[0].m_vstreamOffset );
		
		CHECK_EQUAL( "NORMAL",	b2.m_semantics[1].m_name );
		CHECK_EQUAL( 4,			b2.m_semantics[1].m_istreamOffset );
		CHECK_EQUAL( 1,			b2.m_semantics[1].m_vstreamOffset );
		
		CHECK_EQUAL( "COLOR",	b2.m_semantics[2].m_name );
		CHECK_EQUAL( 5,			b2.m_semantics[2].m_istreamOffset );
		CHECK_EQUAL( 2,			b2.m_semantics[2].m_vstreamOffset );
	}
	
	TEST_FIXTURE( MultiVerticesFixture, DAE_Multi_Vertices_to_DGM )
	{
		Lxt::DomGeometryMesh dgm;
		bool status = Lxt::ConvertDomGeometryToDGM( dg, dgm );
		CHECK( status );
		
		// Check sources
		CHECK_EQUAL( 3, dgm.m_sources.size() );
		
		// Check triangles
		CHECK_EQUAL( 2, dgm.m_triangles.size() );
		
		// Check inputs
		DomInputGroup& dig = dgm.m_triangles[0].m_inputs;
		
		CHECK_EQUAL( 3, dig.size() );
		
		CHECK_EQUAL( dig[0].m_inputOffset, 0 );
		CHECK_EQUAL( dig[0].m_semantic, "POSITION" );
		CHECK_EQUAL( dig[0].m_source, dgm.m_sources[0] );
		
		CHECK_EQUAL( dig[1].m_inputOffset, 0 );
		CHECK_EQUAL( dig[1].m_semantic, "COLOR" );
		CHECK_EQUAL( dig[1].m_source, dgm.m_sources[2] );
		
		CHECK_EQUAL( dig[2].m_inputOffset, 1 );
		CHECK_EQUAL( dig[2].m_semantic, "NORMAL" );
		CHECK_EQUAL( dig[2].m_source, dgm.m_sources[1] );
		
		// check indices for triangle 1
		CHECK_EQUAL( 0, dig[0].m_indices.m_elements[0] );
		CHECK_EQUAL( 1, dig[0].m_indices.m_elements[1] );
		CHECK_EQUAL( 2, dig[0].m_indices.m_elements[2] );

		CHECK_EQUAL( 0, dig[1].m_indices.m_elements[0] );
		CHECK_EQUAL( 1, dig[1].m_indices.m_elements[1] );
		CHECK_EQUAL( 2, dig[1].m_indices.m_elements[2] );		
		
		CHECK_EQUAL( 0, dig[2].m_indices.m_elements[0] );
		CHECK_EQUAL( 0, dig[2].m_indices.m_elements[1] );
		CHECK_EQUAL( 0, dig[2].m_indices.m_elements[2] );
		
		// check indices for triangle 2
		DomInputGroup& dig2 = dgm.m_triangles[1].m_inputs;
		
		CHECK_EQUAL( 2, dig2[0].m_indices.m_elements[0] );
		CHECK_EQUAL( 1, dig2[0].m_indices.m_elements[1] );
		CHECK_EQUAL( 0, dig2[0].m_indices.m_elements[2] );

		CHECK_EQUAL( 2, dig2[1].m_indices.m_elements[0] );
		CHECK_EQUAL( 1, dig2[1].m_indices.m_elements[1] );
		CHECK_EQUAL( 0, dig2[1].m_indices.m_elements[2] );		
				
		CHECK_EQUAL( 1, dig2[2].m_indices.m_elements[0] );
		CHECK_EQUAL( 1, dig2[2].m_indices.m_elements[1] );
		CHECK_EQUAL( 1, dig2[2].m_indices.m_elements[2] );
	}
	
	TEST_FIXTURE( MultiVerticesFixture, DAE_Multi_Vertices_to_LIG )
	{
		Lxt::DomGeometryMesh dgm;
		bool status = Lxt::ConvertDomGeometryToDGM( dg, dgm );
		CHECK( status );
		
		LigMaterialMap			dummy_materials;
		std::vector<LigMesh*>	meshes;
		status = Lxt::ConvertDGMToLigMeshes( dgm, dummy_materials, meshes );
		
		// Check output
		CHECK( status );
		CHECK_EQUAL( 1, meshes.size() );
		
		LigMesh* m = meshes[0];
		
		// Check primitive type
		CHECK_EQUAL( LXT_PT_TRIANGLE, m->m_primitiveType );
		
		// Check vertex streams
		CHECK_EQUAL( 3, m->m_vertexStreams.size() );
		
		// Check index streams
		CHECK_EQUAL( 6, m->m_indexStreams.size() );
		
		// triangle 1
		CHECK_EQUAL( 0, m->m_indexStreams[0].m_elements[0] );
		CHECK_EQUAL( 1, m->m_indexStreams[0].m_elements[1] );
		CHECK_EQUAL( 2, m->m_indexStreams[0].m_elements[2] );
		
		CHECK_EQUAL( 0, m->m_indexStreams[1].m_elements[0] );
		CHECK_EQUAL( 1, m->m_indexStreams[1].m_elements[1] );
		CHECK_EQUAL( 2, m->m_indexStreams[1].m_elements[2] );	
		
		CHECK_EQUAL( 0, m->m_indexStreams[2].m_elements[0] );
		CHECK_EQUAL( 0, m->m_indexStreams[2].m_elements[1] );
		CHECK_EQUAL( 0, m->m_indexStreams[2].m_elements[2] );
		
		// triangle 2
		CHECK_EQUAL( 2, m->m_indexStreams[3].m_elements[0] );
		CHECK_EQUAL( 1, m->m_indexStreams[3].m_elements[1] );
		CHECK_EQUAL( 0, m->m_indexStreams[3].m_elements[2] );
		
		CHECK_EQUAL( 2, m->m_indexStreams[4].m_elements[0] );
		CHECK_EQUAL( 1, m->m_indexStreams[4].m_elements[1] );
		CHECK_EQUAL( 0, m->m_indexStreams[4].m_elements[2] );
		
		CHECK_EQUAL( 1, m->m_indexStreams[5].m_elements[0] );
		CHECK_EQUAL( 1, m->m_indexStreams[5].m_elements[1] );
		CHECK_EQUAL( 1, m->m_indexStreams[5].m_elements[2] );
		
		// Check batches
		CHECK_EQUAL( 2, m->m_batches.size() );
		
		// batch 1
		const LigBatch& b1 = m->m_batches[0];
		CHECK_EQUAL( "BlueSG",	b1.m_materialName );
		CHECK_EQUAL( 3,			b1.m_semantics.size() );
		
		CHECK_EQUAL( "POSITION",b1.m_semantics[0].m_name );
		CHECK_EQUAL( 0,			b1.m_semantics[0].m_istreamOffset );
		CHECK_EQUAL( 0,			b1.m_semantics[0].m_vstreamOffset );
		
		CHECK_EQUAL( "COLOR",	b1.m_semantics[1].m_name );
		CHECK_EQUAL( 1,			b1.m_semantics[1].m_istreamOffset );
		CHECK_EQUAL( 2,			b1.m_semantics[1].m_vstreamOffset );
		
		CHECK_EQUAL( "NORMAL",	b1.m_semantics[2].m_name );
		CHECK_EQUAL( 2,			b1.m_semantics[2].m_istreamOffset );
		CHECK_EQUAL( 1,			b1.m_semantics[2].m_vstreamOffset );
		
		// batch 2
		const LigBatch& b2= m->m_batches[1];
		CHECK_EQUAL( "BlueSG",	b2.m_materialName );
		CHECK_EQUAL( 3,			b2.m_semantics.size() );
		
		CHECK_EQUAL( "POSITION",b2.m_semantics[0].m_name );
		CHECK_EQUAL( 3,			b2.m_semantics[0].m_istreamOffset );
		CHECK_EQUAL( 0,			b2.m_semantics[0].m_vstreamOffset );
		
		CHECK_EQUAL( "COLOR",	b2.m_semantics[1].m_name );
		CHECK_EQUAL( 4,			b2.m_semantics[1].m_istreamOffset );
		CHECK_EQUAL( 2,			b2.m_semantics[1].m_vstreamOffset );
		
		CHECK_EQUAL( "NORMAL",	b2.m_semantics[2].m_name );
		CHECK_EQUAL( 5,			b2.m_semantics[2].m_istreamOffset );
		CHECK_EQUAL( 1,			b2.m_semantics[2].m_vstreamOffset );
	}
}