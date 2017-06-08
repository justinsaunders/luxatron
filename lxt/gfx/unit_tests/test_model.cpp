/*
 *  test_model.cpp
 *  test_runner
 *
 *  Created by Justin on 16/04/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include <UnitTest++.h>
#include "gfx/gfx.h"
#include "core/archive.h"

namespace 
{	
	// DummyTexturePool just inserts a dummy texture, so it never has to be
	// "loaded".
	class DummyTexturePool : public Lxt::TexturePool
	{
	public:
		DummyTexturePool()
		{
			m_textures.insert( std::make_pair( "dummy.png", (Lxt::Texture*)NULL ) );
		}
	};
	
	struct Fixture
	{
		Fixture()
		{			
			Lxt::Model::Node* node0 = new Lxt::Model::Node();
			Lxt::Model::Node* node1 = new Lxt::Model::Node();
			Lxt::Model::Node* node2 = new Lxt::Model::Node();
			
			node1->m_children.push_back( node2 );
			node0->m_children.push_back( node1 );
			
			m_root = node0;
		}
		
		~Fixture()
		{
			// don't delete meshes, they are owned by model.
		}
		
		Lxt::Model::Node*	m_root;
		DummyTexturePool	m_tp;
	};
}

namespace Lxt 
{	
	TEST_FIXTURE( Fixture, Model_NodesStoreExtract )
	{
		// Store
		Archive a;
		size_t written = Model::Node::Store( m_root, m_tp, a );
		
		// Extract
		Model::Node*	n		= NULL;
		size_t			offset	= 0;
		
		// Check
		Model::Node::Extract( n, m_tp, a, offset );
		
		CHECK_EQUAL( written, offset );
		CHECK( m_root->m_transform.Equal( n->m_transform ) );
		CHECK_EQUAL( size_t(1), n->m_children.size() );
					
		Model::Node* oldN = n;
		n = n->m_children[0];
		CHECK_EQUAL( size_t(1), n->m_children.size() );
		
		n = n->m_children[0];
		CHECK_EQUAL( size_t(0), n->m_children.size() );
		
		// stop leaks
		delete oldN;
		delete m_root;
	}
	
	TEST_FIXTURE( Fixture, Model_SimpleStoreExtract )
	{		
		Model m;
		m.SetRoot( m_root );
		
		Archive a;
		size_t written = Model::Store( m, m_tp, a );
		
		Model m2;
		size_t offset = 0; 
		Model::Extract( m2, m_tp, a, offset);
		
		CHECK_EQUAL( written, offset );
	}
}

