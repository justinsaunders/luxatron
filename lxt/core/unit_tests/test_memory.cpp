/*
 *  test_memory.cpp
 *  test_runner
 *
 *  Created by Justin on 5/03/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include <UnitTest++.h>
#include "core/memory.h"

namespace
{
	struct Fixture
	{
		Fixture()
		{
			s_instanceCount++;
		}
		~Fixture()
		{
			s_instanceCount--;
		}
		
		static size_t s_instanceCount;
	};
	
	size_t Fixture::s_instanceCount = 0;
}

namespace Lxt
{
	TEST( Memory_Count_C_Allocations )
	{
		const size_t startAllocs = g_totalAllocations;
		
		void* p1 = malloc( 100 );
		CHECK_EQUAL( startAllocs + 1, g_totalAllocations );
		
		void* p2 = calloc( 10, 10 );
		CHECK_EQUAL( startAllocs + 2, g_totalAllocations );
		
		p1 = realloc( p1, 120 );
		CHECK_EQUAL( startAllocs + 2, g_totalAllocations );
		
		free( p1 );
		CHECK_EQUAL( startAllocs + 1, g_totalAllocations );
		
		free( p2 );
		CHECK_EQUAL( startAllocs, g_totalAllocations );
	}
	
	TEST( Memory_Count_CPP_Allocations )
	{
		const size_t startAllocs = g_totalAllocations;
		
		int* pi = new int;
		CHECK_EQUAL( startAllocs + 1, g_totalAllocations );
		
		int* pai = new int[10];
		CHECK_EQUAL( startAllocs + 2, g_totalAllocations );
		
		delete pi;
		CHECK_EQUAL( startAllocs + 1, g_totalAllocations );
		
		delete[] pai;
		CHECK_EQUAL( startAllocs, g_totalAllocations );
	}
	
	TEST( Memory_Delete_Array )
	{
		// ensure that delete[] does its job.
		CHECK_EQUAL( size_t(0), Fixture::s_instanceCount );
		
		Fixture* fa = new Fixture[10];
		
		CHECK_EQUAL( size_t(10), Fixture::s_instanceCount );
		
		delete[] fa;
		
		CHECK_EQUAL( size_t(0), Fixture::s_instanceCount );
	}
}