/*
 *  vertex_buffer.h
 *  Luxatron
 *
 *  Created by Justin on 25/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_GFX_VERTEX_BUFFER_H__
#define __LXT_GFX_VERTEX_BUFFER_H__

#include "core/types.h"

namespace Lxt
{
    // A simple static VBO wrapper
	class VertexBuffer : private Noncopyable
	{
	public:
		VertexBuffer()
		: m_id(0)
		{
		}
		
		~VertexBuffer()
		{
			Free();
		}
		
        void Set(const void* a_data, size_t size) 
        { 
            m_data.Clear(); 
            m_data.Append(a_data, size); 
        }
        
        const Buffer& Get() const { return m_data; }
        
		bool Upload();
		void Free();
		
		bool Bind() const;
		static bool Unbind();
		
	private:
		uint32_t m_id;
        Buffer   m_data;
	};
} // namespace Lxt

#endif // __LXT_GFX_VERTEX_BUFFER_H__
