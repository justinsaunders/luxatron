/*
 *  buffer.h
 *  Luxatron
 *
 *  Created by Justin on 27/01/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_CORE_BUFFER_H__
#define __LXT_CORE_BUFFER_H__

#include "core/types.h"
#include <vector>
#include <string>

namespace Lxt
{
	// A simple class representing a contiguous piece of memory that allows easy
	// resizing and appending.
	class Buffer : private Noncopyable
	{
	public:
		Buffer( size_t	a_capacity = 0 )	{ m_data.reserve( a_capacity ); }
		~Buffer()							{};
		
		// Append raw data, returning amount of bytes actually written. May be
		// less than a_length if something went wrong (eg allocation failed).
		uint32_t inline Append( const void*	a_source,
								uint32_t	a_length );
		
		// Read raw data, returning bytes actually read. Caller must have 
		// allocated enough storage for destination.
		uint32_t inline Read(	void*		a_destination, 
								uint32_t	a_length, 
								uint32_t	a_offset );	
		
		// POD types
		inline uint32_t Append(	uint8_t		a_value );
		inline uint32_t Read(	uint8_t&	a_value, uint32_t a_offset );
		inline uint32_t Append(	int8_t		a_value );
		inline uint32_t Read(	int8_t&		a_value, uint32_t a_offset );
		inline uint32_t Append(	uint16_t	a_value );
		inline uint32_t Read(	uint16_t&	a_value, uint32_t a_offset );
		inline uint32_t Append(	int16_t		a_value );
		inline uint32_t Read(	int16_t&	a_value, uint32_t a_offset );
		inline uint32_t Append(	uint32_t	a_value );
		inline uint32_t Read(	uint32_t&	a_value, uint32_t a_offset );
		inline uint32_t Append(	int32_t		a_value );
		inline uint32_t Read(	int32_t&	a_value, uint32_t a_offset );
		inline uint32_t Append(	float		a_value );
		inline uint32_t Read(	float&		a_value, uint32_t a_offset );
		
		// Append a vector of items.
		template< typename T >
		uint32_t inline Append( const std::vector<T>& a_source );
		// Read a vector into a given destination. Note this will resize the
		// given vector.
		template< typename T >
		uint32_t inline Read( std::vector<T>& a_destination, uint32_t a_offset );
		
		// Append a memory buffer. Note this is not compatible with an stored
		// vector, as the length here is the number of bytes, not item count.
		uint32_t Append( const Buffer& a_buffer );
				
		// Extract a memory buffer. See note in "Store".
		uint32_t Read( Buffer& a_destination, uint32_t a_offset );
		
		// Append a string
		uint32_t inline Append( const std::string&	a_source );
		// Extract a string
		uint32_t inline Read( std::string& a_destination, uint32_t a_offset );
		
		// Resize buffer to given length, contents are preserved.
		void	inline	Resize( size_t a_newSize );
		
		// Get direct access to the data
		const uint8_t*	Data() const			{ return &m_data[0]; }
		uint8_t*		NonConstData()			{ return &m_data[0]; }
		
		// Information about underlying storage
		size_t			Size() const			{ return m_data.size(); }
		size_t			Capacity() const		{ return m_data.capacity(); }
		
		// Destroy all data, and reduce buffer heap storage to 0.
		void			Clear();
		
	private:
		// This should be raw pointers, to avoid silly swap trick when
		// clearing.
		std::vector< uint8_t > m_data;
	};
	
	uint32_t inline Buffer::Append( const void*	a_source,
									uint32_t	a_length )
	{
		uint32_t total = 0;
		
		for ( size_t i = 0; i < a_length; i++ )
		{
			uint8_t* p = ((uint8_t*)a_source) + i;
			m_data.push_back( *p );
			total++;
		}
		
		return total;
	}
	
	uint32_t inline Buffer::Read( void* a_destination, uint32_t a_length,
								 uint32_t a_offset )
	{
		uint32_t total = 0;
		
		if ( a_offset + a_length > Size() )
		{
			// Fail, not enough room
			return 0;
		}
		
		uint8_t*		dst = ((uint8_t*)a_destination);
		const uint8_t*	src = Data() + a_offset;
		
		for ( uint32_t i = 0; i < a_length; i++ )
		{
			*dst++ = *src++;
			total++;
		}
		
		return total;
	}
	
	// POD types
	inline uint32_t Buffer::Append(	uint8_t	a_value )
	{
		return Append( (const void*)&a_value, sizeof( a_value ) );
	}
	inline uint32_t Buffer::Read(	uint8_t& a_value, uint32_t a_offset )
	{
		return Read( (void*)&a_value, sizeof( a_value ), a_offset );
	}
	
	inline uint32_t Buffer::Append(	int8_t a_value )
	{
		return Append( (const void*)&a_value, sizeof( a_value ) );
	}
	inline uint32_t Buffer::Read(	int8_t&	a_value, uint32_t a_offset )
	{
		return Read( (void*)&a_value, sizeof( a_value ), a_offset );
	}
	
	inline uint32_t Buffer::Append(	uint16_t a_value )
	{
		return Append( (const void*)&a_value, sizeof( a_value ) );
	}
	inline uint32_t Buffer::Read(	uint16_t& a_value, uint32_t a_offset )
	{
		return Read( (void*)&a_value, sizeof( a_value ), a_offset );
	}
	
	inline uint32_t Buffer::Append(	int16_t	a_value )
	{
		return Append( (const void*)&a_value, sizeof( a_value ) );
	}
	inline uint32_t Buffer::Read(	int16_t&	a_value, uint32_t a_offset )
	{
		return Read( (void*)&a_value, sizeof( a_value ), a_offset );
	}
	
	inline uint32_t Buffer::Append(	uint32_t a_value )
	{
		return Append( (const void*)&a_value, sizeof( a_value ) );
	}
	inline uint32_t Buffer::Read(	uint32_t&	a_value, uint32_t a_offset )
	{
		return Read( (void*)&a_value, sizeof( a_value ), a_offset );
	}
	
	inline uint32_t Buffer::Append(	int32_t	a_value )
	{
		return Append( (const void*)&a_value, sizeof( a_value ) );
	}
	inline uint32_t Buffer::Read(	int32_t&	a_value, uint32_t a_offset )
	{
		return Read( (void*)&a_value, sizeof( a_value ), a_offset );
	}
	
	inline uint32_t Buffer::Append(	float a_value )
	{
		return Append( (const void*)&a_value, sizeof( a_value ) );
	}
	inline uint32_t Buffer::Read( float& a_value, uint32_t a_offset )
	{
		return Read( (void*)&a_value, sizeof( a_value ), a_offset );
	}
	
	
	template< typename T>
	uint32_t inline Buffer::Append( const std::vector<T>& a_source )
	{
		// append length
		const uint32_t length	= a_source.size();
		uint32_t total			= Append( length );
		
		// append contents
		for (uint32_t i = 0; i < length; i++ )
			total += Append( a_source[i] );
		
		return total;
	}
	
	// Append a string
	uint32_t inline Buffer::Append( const std::string& a_source )
	{
		// append length
		const uint32_t	length	= a_source.size();
		uint32_t		total	= Append( length );
		
		// append contents
		for (size_t i = 0; i < length; i++ )
			total += Append( (uint8_t)a_source[i] );
		
		return total;		
	}
	
	template< typename T >
	uint32_t inline Buffer::Read(	std::vector<T>& a_destination, 
									uint32_t		a_offset )						
	{
		// read length
		uint32_t length = 0;
		uint32_t total  = 0;
		uint32_t read   = Read( length, a_offset );
		
		if ( read == sizeof( length ) )
		{
			a_offset += read;
			total	 += read;
			
			// reserve length & resize - done this way to try to avoid 
			// std::vector over allocation.
			a_destination.reserve( length );
			a_destination.resize( length );
			
			// add components
			for ( size_t i = 0; i < length; i++ )
			{
				// create item in place
				read = Read( a_destination[i], a_offset );
				
				if ( read != sizeof(T) )
				{
					// fail reading a component
					break;
				}
				
				a_offset += read;
				total	 += read;
			}
		}
		
		return total;
	}	
	
	uint32_t inline Buffer::Read( std::string& a_destination, uint32_t a_offset )
	{
		// read length
		uint32_t length = 0;
		uint32_t total  = 0;
		uint32_t read   = Read( length, a_offset );
		
		if ( read == sizeof( length ) )
		{
			a_offset += read;
			total	 += read;
			
			// reserve length & resize - done this way to try to avoid 
			// std::vector over allocation.
			a_destination.reserve( length );
			a_destination.resize( length );
			
			// add components
			for ( size_t i = 0; i < length; i++ )
			{
				// create item in place
				read = Read( (uint8_t&) a_destination[i], a_offset );
				
				if ( read != sizeof(std::string::value_type) )
				{
					// fail reading a component
					break;
				}
				
				a_offset += read;
				total	 += read;
			}
		}
		
		return total;
	}
	
	inline void Buffer::Resize( size_t a_length )
	{
		m_data.resize( a_length );
	}
}

#endif // __LXT_CORE_BUFFER_H__


					 
		
		
		