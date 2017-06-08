/*
 *  state_buffer.h
 *  starkiller
 *
 *  Created by Justin Saunders on 13/10/09.
 *  Copyright 2009 Net Ventures. All rights reserved.
 *
 */

#ifndef __LXT_GFX_STATE_BUFFER_H__
#define __LXT_GFX_STATE_BUFFER_H__

#include "gl_headers.h"

namespace Lxt
{
	struct RasterStateDesc;
	struct DepthStateDesc;
	struct BlendStateDesc;
	
	// Pointer to a void function that takes a GLEnum
	typedef void (*GLFunc)(GLenum);
	
	struct GLFlag
	{
		GLuint			m_flag;			// store the GL flag eg GL_LIGHTING
		GLFunc			m_setState;		// function to set state for this flag
		
		void Set( GLuint a_flag, bool a_enabled );
		bool Equal( const GLFlag& a_other ) const
		{
			return m_flag == a_other.m_flag && m_setState == a_other.m_setState;
		}
	};
	
	struct RasterStateBuffer 
	{
		GLuint	m_cullMode;
		GLFlag	m_cullFlag;
		
		bool Compile( const RasterStateDesc& a_desc );
		bool Bind() const;
		bool Equal( const RasterStateBuffer& a_other ) const;
	};
	
	struct DepthStateBuffer
	{
		GLFlag	m_depthTestFlag;
		bool	m_depthWriteEnabled;
		GLuint	m_depthFunc;
		
		bool Compile( const DepthStateDesc& a_desc );
		bool Bind() const;
		bool Equal( const DepthStateBuffer& a_other ) const;
	};
	
	struct BlendStateBuffer
	{
		GLFlag m_blendFlag;
		GLuint m_srcBlend;
		GLuint m_dstBlend;
		
		bool Compile( const BlendStateDesc& a_desc );
		bool Bind() const;
		bool Equal( const BlendStateBuffer& a_other ) const;
	};	
}

#endif // __LXT_GFX_STATE_BUFFER_H__
