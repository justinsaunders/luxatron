/*
 *  test_state_buffer.cpp
 *  mac_test_runner
 *
 *  Created by Justin Saunders on 13/10/09.
 *  Copyright 2009 Net Ventures. All rights reserved.
 *
 */

#include "gfx/gles/state_buffer.h"
#include "gfx/state_description.h"

#include <UnitTest++.h>

using namespace Lxt;

namespace 
{
	struct RSFixture
	{
		RasterStateDesc f_desc, f_desc2;
		
		RSFixture()
		{
			f_desc.m_cullMode	= CM_NONE;
			f_desc2.m_cullMode	= CM_FRONT;
		}
	};
	
	struct DSFixture
	{
		DepthStateDesc f_desc, f_desc2;
		
		DSFixture()
		{
			f_desc.m_depthTestEnabled	= false;
			f_desc.m_depthWriteEnabled	= true;
			f_desc.m_depthFunc			= DF_ALWAYS;
			
			f_desc2.m_depthTestEnabled	= false;
			f_desc2.m_depthWriteEnabled	= true;
			f_desc2.m_depthFunc			= DF_NEVER;			
		}
	};
	
	struct BSFixture
	{
		BlendStateDesc f_desc, f_desc2;
		
		BSFixture()
		{
			f_desc.m_blendEnabled	= true;
			f_desc.m_srcBlend		= BF_ZERO;
			f_desc.m_dstBlend		= BF_SRC_ALPHA_SATURATE;
			
			f_desc2.m_blendEnabled	= false;
			f_desc2.m_srcBlend		= BF_ZERO;
			f_desc2.m_dstBlend		= BF_SRC_ALPHA_SATURATE;
		}
	};
}

TEST( Test_GLFlag )
{
	GLFlag lightOn; lightOn.Set( GL_LIGHTING, true ); // lighting on
	GLFlag lightOff; lightOff.Set( GL_LIGHTING, false ); // lighting off

	// Check lighting gets enabled
	glDisable( GL_LIGHTING );
	lightOn.m_setState( lightOn.m_flag );
	CHECK_EQUAL( true,  (bool)glIsEnabled( GL_LIGHTING ) );
	
	// Check lighting gets disabled
	lightOff.m_setState( lightOff.m_flag );
	CHECK_EQUAL( false, (bool)glIsEnabled( GL_LIGHTING ) );
}

TEST_FIXTURE( RSFixture, Test_RasterStateBuffer )
{
	// Check compilation
	RasterStateBuffer buf;
	CHECK( buf.Compile( f_desc ) );
	CHECK_EQUAL( (unsigned int)GL_FRONT_AND_BACK, buf.m_cullMode );
	
	// Equality
	RasterStateBuffer buf2;
	CHECK( buf2.Compile( f_desc2 ) );
	CHECK( !buf.Equal( buf2 ) );
	CHECK( buf.Equal( buf ) );
	
	// Execution
	buf.Bind();
	CHECK_EQUAL( 1, glIsEnabled( GL_CULL_FACE ) );
	
	buf2.Bind();
	CHECK_EQUAL( 0, glIsEnabled( GL_CULL_FACE ) );
}

TEST_FIXTURE( DSFixture, Test_DepthStateBuffer )
{
	// Check compilation
	DepthStateBuffer buf;
	CHECK( buf.Compile( f_desc ) );
	CHECK_EQUAL( (unsigned int)GL_DEPTH_TEST, buf.m_depthTestFlag.m_flag );
	CHECK_EQUAL( true, buf.m_depthWriteEnabled );
	CHECK_EQUAL( (unsigned int)GL_ALWAYS, buf.m_depthFunc );
	
	// Equality
	DepthStateBuffer buf2;
	CHECK( buf2.Compile( f_desc2 ) );
	CHECK( !buf.Equal( buf2 ) );
	CHECK( buf.Equal( buf ) );
}

TEST_FIXTURE( BSFixture, Test_BlendStateBuffer )
{
	// Check compilation
	BlendStateBuffer buf;
	CHECK( buf.Compile( f_desc ) );
	CHECK_EQUAL( (unsigned int)GL_BLEND, buf.m_blendFlag.m_flag );
	CHECK_EQUAL( (unsigned int)GL_ZERO, buf.m_srcBlend );
	CHECK_EQUAL( (unsigned int)GL_SRC_ALPHA_SATURATE, buf.m_dstBlend );
	
	// Equality
	BlendStateBuffer buf2;
	CHECK( buf2.Compile( f_desc2 ) );
	CHECK( !buf.Equal( buf2 ) );
	CHECK( buf.Equal( buf ) );}