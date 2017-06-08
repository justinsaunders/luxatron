/*
 *  test_mesh.cpp
 *  Luxatron
 *
 *  Created by Justin on 16/01/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#include <UnitTest++.h>
#include "gfx/gfx.h"
#include "core/archive.h"

namespace Lxt 
{
	struct Fixture
	{
		Mesh m_fixture;
		
		Fixture() : m_fixture(0)
		{
			// Create simple mesh - small origin marker with three lines on XYZ
			
			// Create vertex declaration
			struct triVert
			{
				Lxt::Vec3	pos;
				uint32_t	abgr;
			};
			
			m_fixture.m_vd.Add( Lxt::VET_FLOAT3, Lxt::VEU_POSITION	);
			m_fixture.m_vd.Add( Lxt::VET_UBYTE4, Lxt::VEU_COLOUR	);
			
			// ensure declaration size matches input
			LXT_ASSERT( m_fixture.m_vd.Stride() == sizeof( triVert ) );
			
			// Create batch list
			Batch b1;
			b1.m_indexStart = 0;
			b1.m_indexCount = 6;
			
			m_fixture.m_batches.push_back( b1 );
			
			// Create vertices
			triVert vertices[] = 
			{
				{
					// vert 1 - origin 
					Vec3(0.0f, 0.0f, 0.0f),
					0xFFFFFFFF,
				},
				{
					// vert 2 - x major
					Vec3(1.0f, 0.0f, 0.0f),
					0xFF0000FF
				},
				{
					// vert 3 - y major 
					Vec3(0.0f, 1.0f, 0.0f),
					0xFF00FF00
				},
				{
					// vert 3 - z major 
					Vec3(0.0f, 0.0f, 1.0f),
					0xFFFF0000
				}
			};
			
			// Create Indices
			uint16_t indices[] = 
			{	
				0,1,	// x
				0,2,	// y
				0,3		// z
			};
			
			m_fixture.m_vertices.Set(vertices, sizeof(vertices));
			m_fixture.m_indices.Set(indices, sizeof(indices));
			
		}
	};
	
	TEST_FIXTURE( Fixture, Mesh_SimpleEncodeDecode )
	{
		Archive		archive;
		TexturePool dummyPool;
		
		Mesh::Store( m_fixture, dummyPool, archive );
		
		Mesh	thawed(0);
		size_t	offset = 0;
		Mesh::Extract( thawed, dummyPool, archive, offset );
		
		// Vertex Declaration
		CHECK_EQUAL( m_fixture.m_vd.m_decl.size(),
					thawed.m_vd.m_decl.size() );
		CHECK_EQUAL( m_fixture.m_vd.m_stride, thawed.m_vd.m_stride );
		
		// Buffers
		// Size
		CHECK_EQUAL( m_fixture.m_vertices.Get().Size(), 
                    thawed.m_vertices.Get().Size() );
		CHECK_EQUAL( m_fixture.m_indices.Get().Size(), thawed.m_indices.Get().Size()	);
		
		// First word
		CHECK_EQUAL( *((uint32_t*)(m_fixture.m_vertices.Get().Data())),
					*((uint32_t*)(thawed.m_vertices.Get().Data())) );
		CHECK_EQUAL( *((uint32_t*)m_fixture.m_indices.Get().Data()),
					*((uint32_t*)thawed.m_indices.Get().Data()) );
					
		// Batches		
		CHECK_EQUAL( m_fixture.m_batches[0].m_indexStart,
					thawed.m_batches[0].m_indexStart );
		
		CHECK_EQUAL( m_fixture.m_batches[0].m_indexCount,
					thawed.m_batches[0].m_indexCount );
		
		// Type
		CHECK_EQUAL( m_fixture.m_primitiveType, thawed.m_primitiveType );
	}
}

