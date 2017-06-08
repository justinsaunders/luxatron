/*
 *  index_buffer.cpp
 *  Luxatron
 *
 *  Created by Justin on 25/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "index_buffer.h"

#include "gles/gl_headers.h"
namespace Lxt
{
	bool IndexBuffer::Upload()
	{
		if (m_id != 0) return false; // already created.
		
		// Copy input data directly to buffer
		glGenBuffers( 1, &m_id);
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_id );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_data.Size(), m_data.Data(), 
                     GL_STATIC_DRAW );
		
		if (glGetError() == GL_NO_ERROR)
        {
            m_data.Clear();
            return true;
        }
        
        return false;
	}
	
	bool IndexBuffer::Bind() const
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_id );
		return glGetError() == GL_NO_ERROR;
	}
	
	bool IndexBuffer::Unbind()
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		return glGetError() == GL_NO_ERROR;
	}
	
	void IndexBuffer::Free()
	{
		if ( m_id != 0 )
		{
			glDeleteBuffers( 1, &m_id );
			m_id = 0;
        }
        else
        {
            m_data.Clear();
        }
	}

} // namespace Lxt