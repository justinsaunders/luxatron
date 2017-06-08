/*
 *  vertex_buffer.cpp
 *  Luxatron
 *
 *  Created by Justin on 25/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "vertex_buffer.h"

#include "gles/gl_headers.h"
#include "core/debug.h"

namespace Lxt
{
	bool VertexBuffer::Upload()
	{
        if (m_id != 0) return false; // already created.
		
		// create hardware buffer
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, m_data.Size(), m_data.Data(), 
                     GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		if (glGetError() == GL_NO_ERROR)
        {
            // unload from CPU side - don't do this if this is a non-static
            // array
            m_data.Clear();
            return true;
        }
        
        return false;
	}
		
	void VertexBuffer::Free()
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
	
	bool VertexBuffer::Bind() const
	{
		// this must happen before format is set
        LXT_ASSERT(m_id != 0);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		
        return glGetError() == GL_NO_ERROR;
	}
	
	/*static*/ bool VertexBuffer::Unbind()
	{
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		return glGetError() == GL_NO_ERROR;
	}
	
} // namespace Lxt