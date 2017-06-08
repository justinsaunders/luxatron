/*
 *  memory.h
 *  test_runner
 *
 *  Created by Justin on 5/03/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_CORE_MEMORY_H__
#define __LXT_CORE_MEMORY_H__

#include <stdlib.h>

namespace Lxt
{
	// Expose total allocations for debugging
	extern size_t g_totalAllocations;
	
	// Provide implementations for standard memory functions
	inline void* Malloc( size_t a_bytes )
	{
		++g_totalAllocations;
		return malloc( a_bytes );
	}
	
	inline void* Calloc( size_t a_numElements, size_t a_elementSize )
	{
		++g_totalAllocations;
		return calloc( a_numElements, a_elementSize );
	}
	
	inline void* Realloc( void* a_block, size_t a_newSize )
	{
		return realloc( a_block, a_newSize );
	}
	
	inline void Free( void* a_block )
	{
		free( a_block );
		--g_totalAllocations;
	}
}

// Override global new and delete in global namespace
inline void operator delete( void* a_block )
{
	Lxt::Free( a_block );
}

inline void operator delete[]( void* a_block )
{
	Lxt::Free( a_block );
}

inline void * operator new( size_t a_bytes )
{
	return Lxt::Malloc( a_bytes );
}

inline void * operator new[]( size_t a_bytes )
{
	return Lxt::Malloc( a_bytes );
}

// Override malloc, calloc, realloc, free
#define malloc Lxt::Malloc
#define calloc Lxt::Calloc
#define realloc Lxt::Realloc
#define free Lxt::Free


#endif // __LXT_CORE_MEMORY_H__