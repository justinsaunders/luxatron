/*
 *  test_material.cpp
 *  test_runner
 *
 *  Created by Justin on 12/03/09.
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
		Lxt::Material		m_fixture;
		DummyTexturePool	m_dummyTexPool;
		
		Fixture()
		{
			m_fixture.Set( Lxt::Vec4( 1.0f, 2.0f, 3.0f, 4.0f ),
						   Lxt::Vec4( 5.0f, 6.0f, 7.0f, 8.0f ), 
						  9.0f, NULL, Lxt::MBT_MAX, true );
		}
	};
}

namespace Lxt 
{
	TEST_FIXTURE( Fixture, Material_EncodeDecode )
	{
		Fixture f;
		Archive a;
		size_t written = Material::Store( f.m_fixture,  m_dummyTexPool, a );
		
		Material output;
		size_t offset = 0;
		Material::Extract( output, m_dummyTexPool, a, offset );
		
		CHECK_EQUAL( written, offset );

		/*
		CHECK( f.m_fixture.m_diffuse.Equal( output.m_diffuse ) );
		CHECK( f.m_fixture.m_specular.Equal( output.m_specular ) );
		CHECK_EQUAL( f.m_fixture.m_shininess, output.m_shininess );
		CHECK_EQUAL( (Texture*)NULL, output.m_texture );
		CHECK_EQUAL( f.m_fixture.m_blendType, output.m_blendType );
		CHECK_EQUAL( f.m_fixture.m_lit, output.m_lit );
		 */
	}
	
}