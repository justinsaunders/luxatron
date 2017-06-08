/*
 *  array.h
 *  
 *	A fixed length array type.
 *
 *  Created by Justin on 3/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_CORE_ARRAY_H__
#define __LXT_CORE_ARRAY_H__

#include "core/types.h"

namespace Lxt
{
	template< typename T, size_t N >
	class Array
	{
	public:
		typedef T ITEM_TYPE;
		
		const ITEM_TYPE& operator[]( size_t a_index ) const
		{
			LXT_ASSERT( a_index < N );
			return m_data[ a_index ];
		}
		
		ITEM_TYPE& operator[]( size_t a_index )
		{
			LXT_ASSERT( a_index < N );
			return m_data[ a_index ];
		}
		
		size_t Size() const
		{
			return N;
		}
		
	private:
		ITEM_TYPE m_data[ N ];
	};

	// Array length for standard C arrays - defines must include namespace prefix
	#define LXT_ARRAY_LEN( x ) sizeof( x ) / sizeof( x[0] )
} // namespace Lxt

#endif // __LXT_CORE_ARRAY_H__

