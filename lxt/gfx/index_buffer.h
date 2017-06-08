/*
 *  index_buffer.h
 *  Luxatron
 *
 *  Created by Justin on 25/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_GFX_INDEX_BUFFER_H__
#define __LXT_GFX_INDEX_BUFFER_H__

#include "core/types.h"

namespace Lxt
{
	// Class to represent a 16 bit index buffer object.
	class IndexBuffer : private Noncopyable
	{
	public:
		typedef uint16_t IndexType;
			
		IndexBuffer()
		:	m_id( 0 )
		{}
		
		~IndexBuffer()
		{
			Free();
		}
		
		void Set(const void* a_data, size_t a_size)
        {
            m_data.Clear();
            m_data.Append(a_data, a_size);
        }
        
        const Buffer& Get() const { return m_data; }

        bool Upload();
		void Free();
        
		// Return number of indicies in buffer
		//size_t Count() const { return m_length; }
		
		bool Bind() const;
		bool Unbind();
		
		
	private:
		uint32_t m_id;
		Buffer   m_data;   
	};
	
} // namespace Lxt

#endif // __LXT_GFX_INDEX_BUFFER_H__
