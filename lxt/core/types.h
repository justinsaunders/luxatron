/*
 *  types.h
 *  Luxatron
 *
 *  Created by Justin on 24/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_CORE_TYPES_H__
#define __LXT_CORE_TYPES_H__

// Define standard types for integers such as uint8_t, int16_t etc.
#include <stdint.h>
#include <sys/types.h>

namespace Lxt 
{
	// Non-copyable base class, from 
	// http://www.boost.org/doc/libs/1_36_0/boost/noncopyable.hpp
	// To use, privately inherit from this class
	class Noncopyable
	{
	protected:
		Noncopyable() {}
		~Noncopyable() {}
		
	private:  // emphasize the following members are private
		Noncopyable( const Noncopyable& );
		const Noncopyable& operator=( const Noncopyable& );
	};

	// A subclass of a pointer container type that deletes all elements on
	// destruction
	template<typename Container> 
	class AutoContainer : public Container
	{
	public:
		typedef typename Container::iterator iterator;
		
		~AutoContainer()
		{
			for ( iterator it = Container::begin(); 
				 it != Container::end(); it++ )
			{
				delete *it;
			}
		}
	};
	
	// A generic handle type - useful for resources like textures and sounds, 
	// where it may be an index.
	typedef uint16_t Handle;
	const uint16_t INVALID_HANDLE = (Lxt::Handle)-1;
    
    // Swap an unsigned integer
    inline uint32_t ByteSwap(uint32_t ui32Long)
    {
        return ((ui32Long&0x000000FF)<<24) + 
                ((ui32Long&0x0000FF00)<<8) + 
                ((ui32Long&0x00FF0000)>>8) + ((ui32Long&0xFF000000) >> 24);
    }
    
}

#endif // __LXT_CORE_TYPES_H__
