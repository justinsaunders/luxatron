/*
 *  test_buffer.cpp
 *  test_runner
 *
 *  Created by Justin on 27/01/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include <UnitTest++.h>
#include "core/buffer.h"

namespace 
{
	struct Fixture
	{
		std::vector<int>	f_v;
		Lxt::Buffer			f_buffer;
		Lxt::Buffer			f_b2;
		
		Fixture()
		{
			f_v.push_back( -1 );
			f_v.push_back( 0 );
			f_v.push_back( 65530 );
			
			f_b2.Append( f_v );		
		}
	};
};

namespace Lxt 
{
	TEST( Buffer_Construct )
	{
		Buffer empty;
		CHECK_EQUAL( size_t(0), empty.Size() );
		CHECK_EQUAL( size_t(0), empty.Capacity() );
		
		Buffer ten_buffer( 10 );
		CHECK_EQUAL( size_t(0), ten_buffer.Size() );
		CHECK_EQUAL( size_t(10),ten_buffer.Capacity() );
		
	}
	
	TEST( Buffer_AppendRaw )
	{
		Buffer		intBuffer;
		uint32_t	uintValue = 5;
		
		intBuffer.Append( (void*)&uintValue, sizeof(uintValue) );
		
		CHECK_EQUAL( intBuffer.Size(), sizeof(uintValue) );
		CHECK_EQUAL( uintValue, *((uint32_t*)intBuffer.Data()) );
	}
	
	TEST( Buffer_ReadRaw )
	{
		Buffer		intBuffer;
		uint32_t	uintValue = 5;
		uint32_t	outValue = 5;
		
		// append an int into the buffer, then read back
		intBuffer.Append( (void*)&uintValue, sizeof(uintValue) );
		size_t r = intBuffer.Read( &outValue, sizeof(uintValue), 0 );

		CHECK_EQUAL( sizeof(uintValue), r );
		CHECK_EQUAL( uintValue, outValue );
	}
	
	TEST( Buffer_Clear )
	{
		Buffer		bigBuffer;
		int			value		= 5;
		
		// fill before
		for ( uint32_t i = 0; i < 1000; i++ )
			bigBuffer.Append((void*)&value, sizeof(value) );
		
		// now clear it - so it is completely empty (no reserved capacty either).
		bigBuffer.Clear();
		
		CHECK_EQUAL( size_t(0), bigBuffer.Size() );
		CHECK_EQUAL( size_t(0), bigBuffer.Capacity() );
	}
	
	TEST_FIXTURE( Fixture, Buffer_Append )
	{
		uint32_t		  uintValue = 5;
		
		CHECK_EQUAL( sizeof(uintValue), f_buffer.Append( uintValue ) );
		CHECK_EQUAL( sizeof(uintValue), f_buffer.Size() );
		CHECK_EQUAL( uintValue, *((uint32_t*) f_buffer.Data()) );
	}
	
	TEST_FIXTURE( Fixture, Buffer_Append_Vector )
	{
		// Length should be size_t (length) plus each component.
		const size_t expectedLength = sizeof( size_t ) + f_v.size() * sizeof(int);
		CHECK_EQUAL( expectedLength, f_buffer.Append( f_v ) );
		CHECK_EQUAL( expectedLength, f_buffer.Size() );
		
		// First item should be length = 3
		const uint8_t *p = f_buffer.Data();
		CHECK_EQUAL( f_v.size(), *((size_t*)p) );
		p += sizeof(size_t);
		
		// Next three should be the values
		for ( uint32_t i = 0; i < f_v.size(); i++ )
		{
			CHECK_EQUAL( f_v[i], *((int*)p) );
			p +=sizeof(int);
		}
	}
	
	TEST_FIXTURE( Fixture, Buffer_Append_Buffer )
	{
		// Append a buffer onto an existing buffer.
		
		// length should be size_t plus each component
		const size_t expectedLength = sizeof( size_t ) + f_b2.Size();
		CHECK_EQUAL( expectedLength, f_buffer.Append( f_b2 ) );
		CHECK_EQUAL( expectedLength, f_buffer.Size() );
		
		// First item should be buffer length = 3
		const uint8_t *p = f_buffer.Data();
		CHECK_EQUAL( f_b2.Size(), *((size_t*)p) );
		p += sizeof(size_t);
		
		// Second item should be vector length = 3
		CHECK_EQUAL( f_v.size(), *((size_t*)p) );
		p += sizeof(size_t);
		
		// Next three should be the values
		for ( uint32_t i = 0; i < f_v.size(); i++ )
		{
			CHECK_EQUAL( f_v[i], *((int*)p) );
			p +=sizeof(int);
		}
	}
	
	TEST_FIXTURE( Fixture, Buffer_Extract )
	{
		int16_t				value		= 5;
		int16_t				extracted	= 0;
		
		f_buffer.Append( value );
		CHECK_EQUAL( sizeof( value ), f_buffer.Read( extracted,(uint32_t) 0 ) );
		CHECK_EQUAL( value, extracted );
	}
	
	TEST_FIXTURE( Fixture, Buffer_Extract_Vector )
	{
		std::vector<int>	extracted;
		
		f_buffer.Append( f_v );
		
		// Length should be size_t (length) plus each component.
		const size_t expectedLength = sizeof( size_t ) + f_v.size() * sizeof(int);
		
		CHECK_EQUAL( expectedLength, f_buffer.Read( extracted, 0 ) );
		
		// check contents
		CHECK_EQUAL( f_v.size(), extracted.size() );
		
		for ( uint32_t i = 0; i < f_v.size(); i++ )
		{
			CHECK_EQUAL( f_v[i], extracted[i] );
		}
	}
	
	TEST_FIXTURE( Fixture, Buffer_Extract_Buffer )
	{	
		// Length should be size_t (length) plus each component.
		const size_t expectedLength = f_buffer.Append( f_b2 );
		
		Buffer	extracted;
		CHECK_EQUAL( expectedLength, f_buffer.Read( extracted, 0 ) );
	}
	
	TEST( Buffer_Std_String )
	{
		Buffer b;
		std::string input = "Hello, world!";
		std::string output;
		
		b.Append( input );
		CHECK_EQUAL( sizeof(size_t) + input.length(), b.Read( output, 0 ) );
		CHECK_EQUAL( input, output );		
	}
	
	TEST( Buffer_C_String )
	{
		Buffer b;
		const char* input = "Hello, world!";
		std::string output;
		
		b.Append( input );
		CHECK_EQUAL( sizeof(size_t) + strlen(input), b.Read( output, 0 ) );
		CHECK( strcmp( input, output.c_str() ) == 0 );		
	}
}