/*
 *  test_texture_pool.cpp
 *  mac_test_runner
 *
 *  Created by Justin on 5/04/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#include <UnitTest++.h>
#include "texture_pool.h"

namespace 
{
	struct Fixture
	{
		Fixture()
		{
			m_simpleInputs.push_back( std::make_pair( "a.png", 10.0 ) );
			m_simpleInputs.push_back( std::make_pair( "b.png", 15.0 ) );
			m_simpleInputs.push_back( std::make_pair( "c.png", 20.0 ) );
		}
		
		std::vector< Lxt::PathDate >	m_simpleInputs;
		std::vector< Lxt::PathDate >	m_emptyInputs;
		Lxt::TexturePool				m_tp;
	};
}

namespace Lxt 
{
	// Skip these tests, this class is poo and should be removed ASAP.
	
	/*
	TEST_FIXTURE( Fixture, TexturePool_Empty )
	{
		bool updated = TexturePool::Link( m_emptyInputs, 0, false, m_tp );
		
		CHECK_EQUAL( false, updated );
		CHECK_EQUAL( 0, m_tp.GetTextures().size() );
	}
	
	TEST_FIXTURE( Fixture, TexturePool_Simple )
	{
		// Add three new items to empty pool
		bool updated = TexturePool::Link( m_simpleInputs, 0, false, m_tp );
		
		CHECK_EQUAL( true, updated );
		CHECK_EQUAL( 3, m_tp.GetTextures().size() );
	}
	
	TEST_FIXTURE( Fixture, TexturePool_AddEmpty )
	{
		// Add three new items to empty pool
		TexturePool::Link( m_simpleInputs, 0, false, m_tp );
		
		// Add empty pool, should remove entries
		bool updated = TexturePool::Link( m_emptyInputs, 0, false, m_tp );
		
		CHECK_EQUAL( updated,	true );
		CHECK_EQUAL( 0,			m_tp.GetTextures().size() );
	}
	
	TEST_FIXTURE( Fixture, TexturePool_AddSame )
	{
		// Add three new items to empty pool
		TexturePool::Link( m_simpleInputs, 0, false, m_tp );
	
		// Add same items with pool at a new date, should be no change
		bool updated = TexturePool::Link( m_simpleInputs, 30, false, m_tp );
		
		CHECK_EQUAL( updated,	false );
		CHECK_EQUAL( 3,			m_tp.GetTextures().size() );
	}
	
	TEST_FIXTURE( Fixture, TexturePool_AddSameButOneNewer )
	{
		// Add three new items to empty pool
		TexturePool::Link( m_simpleInputs, 0, false, m_tp );

		m_simpleInputs.erase( m_simpleInputs.begin() );
		m_simpleInputs.push_back( std::make_pair( "a.png", 30 ) );
		
		// Add newer
		bool updated = TexturePool::Link( m_simpleInputs, 20, false, m_tp );
		CHECK_EQUAL( updated,	true );
		CHECK_EQUAL( 3,			m_tp.GetTextures().size() );
	}
	
	TEST_FIXTURE( Fixture, TexturePool_Append )
	{
		// Add three new items to empty pool
		TexturePool::Link( m_simpleInputs, 0, false, m_tp );
		
		std::vector< Lxt::PathDate > appendage;
		appendage.push_back( std::make_pair( "d.png", 30 ) );
		
		// Add newer
		bool updated = TexturePool::Link( appendage, 20, true, m_tp );
		CHECK_EQUAL( updated,	true );
		CHECK_EQUAL( 4,			m_tp.GetTextures().size() );
	}
	*/
}