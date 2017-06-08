/*
 *  lig_lig_to_lxtm_tests.cpp
 *  convert_dae
 *
 *  Created by Justin on 18/09/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include <UnitTest++.h>
#include <dae.h>
#include <dom/domCOLLADA.h>

#include "../dae_to_lig.h"
#include "../lig_scene.h"
#
#include "lig_mesh.h"
#include "lig_scene.h"
#include "lig_to_lxtm.h"

namespace 
{
	struct SimpleFixture
	{
		Lxt::LigMesh		f_onePointLigMesh;
		Lxt::TexturePool	f_texturePool;
		Lxt::LigMaterial	f_dummyMaterial;
		Lxt::LigMaterialMap	f_materialMap;
		
		SimpleFixture()
		{
			f_materialMap.insert( 
						std::make_pair( "dummy material", f_dummyMaterial ) );
			
			f_onePointLigMesh.m_primitiveType = Lxt::LXT_PT_POINT;

			// stream 1 - point
			{
				Lxt::LigVertexStream ls;
				ls.m_stride = 3;
				ls.m_elements.push_back( 1.f );
				ls.m_elements.push_back( 2.f );
				ls.m_elements.push_back( 3.f );
				
				f_onePointLigMesh.m_vertexStreams.push_back( ls );
			}
			
			// stream 2 - normal
			{
				Lxt::LigVertexStream ls;
				ls.m_stride = 3;
				ls.m_elements.push_back( 4.f );
				ls.m_elements.push_back( 5.f );
				ls.m_elements.push_back( 6.f );
				
				f_onePointLigMesh.m_vertexStreams.push_back( ls );
			}
			
			// stream 3 - UV
			{
				Lxt::LigVertexStream ls;
				ls.m_stride = 2;
				ls.m_elements.push_back( 0.5f );
				ls.m_elements.push_back( 0.6f );
				
				f_onePointLigMesh.m_vertexStreams.push_back( ls );
			}
			
			// stream 4 - colour
			{
				Lxt::LigVertexStream ls;
				ls.m_stride = 4;
				ls.m_elements.push_back( 0.0f );
				ls.m_elements.push_back( 0.25f );
				ls.m_elements.push_back( 0.5f );
				ls.m_elements.push_back( 1.0f );
				
				f_onePointLigMesh.m_vertexStreams.push_back( ls );
			}
			
			Lxt::LigBatch b;
			b.m_materialName = "dummy material";
			
			{
				Lxt::LigIndexStream is;
				is.m_elements.push_back( 0 );
			
				f_onePointLigMesh.m_indexStreams.push_back( is );
				
				Lxt::LigSemantic	sem;
				sem.m_name			= "POSITION";
				sem.m_istreamOffset	= 0;
				sem.m_vstreamOffset = 0;
				
				b.m_semantics.push_back( sem );
			}
			
			{
				Lxt::LigIndexStream is;
				is.m_elements.push_back( 0 );
				
				f_onePointLigMesh.m_indexStreams.push_back( is );
				
				Lxt::LigSemantic	sem;
				sem.m_name			= "NORMAL";
				sem.m_istreamOffset	= 1;
				sem.m_vstreamOffset = 1;
				
				b.m_semantics.push_back( sem );
			}
			
			{
				Lxt::LigIndexStream is;
				is.m_elements.push_back( 0 );
				
				f_onePointLigMesh.m_indexStreams.push_back( is );
				
				Lxt::LigSemantic	sem;
				sem.m_name			= "TEXCOORD";
				sem.m_istreamOffset	= 2;
				sem.m_vstreamOffset = 2;
				
				b.m_semantics.push_back( sem );
			}
			
			{
				Lxt::LigIndexStream is;
				is.m_elements.push_back( 0 );
				
				f_onePointLigMesh.m_indexStreams.push_back( is );
				
				Lxt::LigSemantic	sem;
				sem.m_name			= "COLOR";
				sem.m_istreamOffset	= 3;
				sem.m_vstreamOffset = 3;
				
				b.m_semantics.push_back( sem );
			}
			
			f_onePointLigMesh.m_batches.push_back( b );
		}
	};
	
	struct TriangleFixture
	{
		Lxt::LigScene		f_triangleLigScene;
		Lxt::TexturePool	f_texturePool;
		
		TriangleFixture()
		{
			// load scene
			DAE dae;
			daeElement* root = dae.open( 
				"/Users/Justin/Projects/msg/art/test/unit_tested/triangle.dae" );
			LXT_ASSERT( root != NULL );
			
			// convert to LIG
			bool status = Lxt::ConvertDAEtoLIG( root, f_triangleLigScene );
			LXT_ASSERT( status );
		}
	};
}

namespace Lxt 
{
	TEST_FIXTURE( SimpleFixture, SimpleLigToLxtm )
	{
		Lxt::Mesh lm;
		ConvertLIGMeshToLXTMesh( f_onePointLigMesh, f_materialMap, 
								f_texturePool, lm );
		
		// vd
		CHECK_EQUAL( 4, lm.m_vd.Length() );
		CHECK_EQUAL( 3 * sizeof(float) + // position
					 3 * sizeof(float) + // normal
					 2 * sizeof(float) + // texcoord
					 4 * sizeof(uint8_t),// colour 
					lm.m_vd.Stride() );
		
		// batch
		CHECK_EQUAL( 1, lm.m_batches.size() );
		CHECK_EQUAL( 0, lm.m_batches[0].m_indexStart );
		CHECK_EQUAL( 1, lm.m_batches[0].m_indexCount );
		
		// others
		CHECK_EQUAL( lm.m_vd.Stride(),	lm.m_vertices.Size() );	// 1 point
		CHECK_EQUAL( sizeof(uint16_t),	lm.m_indices.Size() );	// 1 index
		CHECK_EQUAL( LXT_PT_POINT,		lm.m_primitiveType );	// point
		
		// verify single vertex.
		const uint8_t*	p = lm.m_vertices.Data();
		const float*	pf = (float*)p;
		
		// check position
		CHECK_EQUAL( 1.f, *pf++ );
		CHECK_EQUAL( 2.f, *pf++ );
		CHECK_EQUAL( 3.f, *pf++ );
		
		// check normal
		CHECK_EQUAL( 4.f, *pf++ );
		CHECK_EQUAL( 5.f, *pf++ );
		CHECK_EQUAL( 6.f, *pf++ );
		
		// check normal
		CHECK_EQUAL( 0.5f, *pf++ );
		CHECK_EQUAL( 0.6f, *pf++ );
		
		// check colour
		p = (uint8*)pf;
		CHECK_EQUAL( 0x00, *p++ );
		CHECK_EQUAL( 0x40, *p++ );
		CHECK_EQUAL( 0x80, *p++ );
		CHECK_EQUAL( 0xFF, *p++ );
	}
	
	TEST_FIXTURE( TriangleFixture, TriangleLigToLxtm )
	{
		// load model
		Lxt::Model model;
		bool status = ConvertLIGScenetoLXTModel(f_triangleLigScene,
												f_texturePool, 
												model );
		CHECK( status );
		
		// get mesh
		Lxt::Mesh& mesh = model.GetRoot()->m_children[0]->m_mesh;		
		CHECK( !mesh.IsEmpty() );
		
		// vd
		CHECK_EQUAL( 3, mesh.m_vd.Length() );
		CHECK_EQUAL( 3 * sizeof( float )+	// position
					 3 * sizeof( float )+	// normal
					 4 * sizeof( uint8_t ), // colour
						mesh.m_vd.Stride() );
		
		// batches and totals
		CHECK_EQUAL( 2, mesh.m_batches.size() ); // two sides of triangle
		CHECK_EQUAL( mesh.m_vd.Stride() * 6, mesh.m_vertices.Size() );	// 6 points
		CHECK_EQUAL( sizeof(uint16_t) * 6,	mesh.m_indices.Size() );	// 6 indices
		CHECK_EQUAL( LXT_PT_TRIANGLE, mesh.m_primitiveType );	// triangles
		
		// batch 1
		CHECK_EQUAL( 0, mesh.m_batches[0].m_indexStart );
		CHECK_EQUAL( 3, mesh.m_batches[0].m_indexCount );
		
		// batch 2
		CHECK_EQUAL( 3, mesh.m_batches[1].m_indexStart );
		CHECK_EQUAL( 3, mesh.m_batches[1].m_indexCount );		
	}				
}
