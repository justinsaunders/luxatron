/*
 *  buffer.cpp
 *  test_runner
 *
 *  Created by Justin on 27/01/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "buffer.h"
#include "debug.h"

namespace Lxt 
{
	void Buffer::Clear()
	{
		std::vector<uint8_t> empty;
		m_data.swap( empty );
	}
	
	uint32_t Buffer::Append( const Buffer& a_buffer )
	{
		// append length
		const uint32_t length = a_buffer.Size();
		uint32_t total		  = Append( length );
		
		// append contents
		total += Append( a_buffer.Data(), a_buffer.Size() );
		
		return total;
	}
	
	uint32_t Buffer::Read(	Buffer&		a_destination,
							uint32_t	a_offset )
	{
		// read length
		uint32_t length = 0;
		uint32_t total  = 0;
		uint32_t read   = Read( length, a_offset );
		
		if ( read == sizeof( length ) )
		{
			a_offset += read;
			total	 += read;
			
			LXT_ASSERT( a_offset + length <= Size() );
			total += a_destination.Append( a_offset + Data(),
										  length );
		}
		
		return total;
	}
	
}