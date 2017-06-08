/*
 *  state_buffer.cpp
 *  starkiller
 *
 *  Created by Justin Saunders on 13/10/09.
 *  Copyright 2009 Net Ventures. All rights reserved.
 *
 */

#include "state_buffer.h"
#include "state_description.h"

using namespace Lxt;

// GLFlag ----------------------------------------------------------------------
static GLFunc s_stateFuncs[] = { glDisable, glEnable };

void GLFlag::Set( GLuint a_flag, bool a_enabled )
{
	m_flag		= a_flag;
	m_setState	= s_stateFuncs[ size_t(a_enabled) ];
}

// Raster ----------------------------------------------------------------------
bool RasterStateBuffer::Compile( const RasterStateDesc& a_desc )
{	
	// Translate to GL
	GLuint gl_cull = GL_FRONT_AND_BACK;
	switch ( a_desc.m_cullMode) 
	{
		case CM_BACK:
			gl_cull = GL_BACK;
			break;
		case CM_FRONT:
			gl_cull = GL_FRONT;
		default:
			break;
	}
	
	m_cullMode			 = gl_cull;
	m_cullFlag.Set( GL_CULL_FACE, a_desc.m_cullMode == CM_NONE );
	
	return true;
}

bool RasterStateBuffer::Bind() const
{
	// Set internal GL states
	glCullFace( m_cullMode );
	m_cullFlag.m_setState( m_cullFlag.m_flag );
	
	return true;
}

bool RasterStateBuffer::Equal( const RasterStateBuffer& a_other ) const
{
	return m_cullMode == a_other.m_cullMode && 
				m_cullFlag.Equal( a_other.m_cullFlag );
}

// Depth -----------------------------------------------------------------------
bool DepthStateBuffer::Compile( const DepthStateDesc& a_desc )
{
	// Translate to GL
	GLuint gl_df = GL_ALWAYS;
	switch ( a_desc.m_depthFunc ) 
	{
		case DF_NEVER:
			gl_df = GL_NEVER;
			break;
		case DF_LESS:
			gl_df = GL_LESS;
			break;
		case DF_EQUAL:
			gl_df = GL_EQUAL;
			break;
		case DF_GREATER:
			gl_df = GL_GREATER;
			break;
		default:
			break;
	}
	
	m_depthFunc					= gl_df;
	m_depthTestFlag.Set( GL_DEPTH_TEST, a_desc.m_depthTestEnabled );
	m_depthWriteEnabled			= a_desc.m_depthWriteEnabled;
	
	return true;
}

bool DepthStateBuffer::Bind() const
{
	// Set internal GL states
	m_depthTestFlag.m_setState( m_depthTestFlag.m_flag );	
	glDepthMask( m_depthWriteEnabled );
	glDepthFunc( m_depthFunc );
	
	return true;
}

bool DepthStateBuffer::Equal( const DepthStateBuffer& a_other ) const
{
	return m_depthTestFlag.Equal( a_other.m_depthTestFlag ) &&
		m_depthWriteEnabled == a_other.m_depthWriteEnabled &&
		m_depthFunc == a_other.m_depthFunc;
}

// Blend -----------------------------------------------------------------------
namespace  
{
	GLuint GetGLBlendFromLXTBlend( Lxt::BlendFact a_bf )
	{
		GLuint blend = GL_SRC_ALPHA_SATURATE;
		
		switch ( a_bf )
		{
			case BF_ZERO:
				blend = GL_ZERO; break;
			case BF_ONE:
				blend = GL_ONE; break;
			case BF_SRC_COLOR:
				blend = GL_SRC_COLOR; break;
			case BF_ONE_MINUS_SRC_COLOR:
				blend = GL_ONE_MINUS_SRC_COLOR; break;
			case BF_DST_COLOR:
				blend = GL_DST_COLOR; break;
			case BF_ONE_MINUS_DST_COLOR:
				blend = GL_ONE_MINUS_DST_COLOR; break;
			case BF_SRC_ALPHA:
				blend = GL_SRC_ALPHA; break;
			case BF_ONE_MINUS_SRC_ALPHA:
				blend = GL_ONE_MINUS_SRC_ALPHA; break;
			case BF_DST_ALPHA:
				blend = GL_DST_ALPHA; break;
			case BF_ONE_MINUS_DST_ALPHA:
				blend = GL_ONE_MINUS_DST_ALPHA; break;
			default: break;
		}
		
		return blend;
	}
}

bool BlendStateBuffer::Compile( const BlendStateDesc& a_desc )
{
	// Translate to GL
	m_blendFlag.Set( GL_BLEND, a_desc.m_blendEnabled );
	
	m_srcBlend = GetGLBlendFromLXTBlend( a_desc.m_srcBlend );
	m_dstBlend = GetGLBlendFromLXTBlend( a_desc.m_dstBlend );
	
	return true;
}

bool BlendStateBuffer::Bind() const
{
	// Set internal GL states
	m_blendFlag.m_setState( m_blendFlag.m_flag );
	glBlendFunc( m_srcBlend, m_dstBlend );
	
	return true;
}

bool BlendStateBuffer::Equal( const BlendStateBuffer& a_other ) const
{
	return m_blendFlag.Equal( a_other.m_blendFlag ) &&
		m_srcBlend == a_other.m_srcBlend &&
		m_dstBlend == a_other.m_dstBlend;
}

