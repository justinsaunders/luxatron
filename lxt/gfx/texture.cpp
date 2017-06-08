/*
 *  texture.cpp
 *  Luxatron
 *
 *  Created by Justin on 4/09/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "texture.h"

#include "gles/gl_headers.h"
#include "math/util.h"

namespace Lxt 
{
	Texture::Texture()
	:	m_id( 0 ),
		m_width( 0 ),
		m_height( 0 )
	{
	}
	
	Texture::~Texture()
	{
		Free();
	}

	bool Texture::Create(uint16_t	a_width,		uint16_t a_height, 
						 uint8_t	a_levelStart,	uint8_t a_levelEnd,
						 uint32_t	a_format,		uint32_t a_type,
						 const void* a_data )
	{
		if ( m_id != 0 )
		{
			// texture already created
			return false;
		}
		
		if ( a_width > 1024 || a_height > 1024 )
		{
			LXT_LOG_ERROR("Texture is too large (%dx%d)\n",
						  a_width, a_height );
			return false;
		}
		
		if ( !IsPow2( a_width ) || !IsPow2( a_height ) )
		{
			LXT_LOG_ERROR("Texture does not have power of two dimensions (%dx%d)\n",
						  a_width, a_height );
			return false;
		}
		
		// set width and height
		m_width = a_width;
		m_height = a_height;
			
		// generate texture id, bind
		glGenTextures( 1, &m_id );
		glBindTexture( GL_TEXTURE_2D, m_id );
		
		// specify linear filtering
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		// wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		// upload
		glTexImage2D( GL_TEXTURE_2D, 0, a_format, a_width, a_height, 0, 
						a_format, a_type, a_data );
		
		return glGetError() == GL_NO_ERROR;
	}
	
	bool Texture::Bind()
	{
		glBindTexture( GL_TEXTURE_2D, m_id );
		return false;		
	}
	
	bool Texture::Unbind()
	{
		glBindTexture( GL_TEXTURE_2D, 0 );
		return false;		
	}
	
	bool Texture::Free()
	{
		if ( m_id != 0 )
		{
			glDeleteTextures( 1, &m_id );
			m_id = 0;
			
			return true;
		}
		
		// tried to free already freed texture
		return false;		
	}
	
} // namespace Lxt