/*
 *  zone_stack.h
 *  Zone registry for all registered zones, and a class to represent current
 *	zone stack.
 *
 *  Created by Justin on 28/02/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_ZONE_STACK_H__
#define __LXT_ZONE_STACK_H__

#include "debug.h"

namespace Lxt 
{	
	typedef	uint16_t ZoneId;

	// This class is a true global singleton as it is valid for all execution 
	// threads. It represents all the registered zones in the programm
	enum { ZR_MAX_ZONE_COUNT	= 1024 };
	enum { ZR_DEFAULT_ZONE_ID	= 0 };

	class ZoneRegistry
	{
	public:
		static ZoneId		DeclareZone( const char* a_zoneName );
		static const char*	GetZoneName( ZoneId a_zi );
		static ZoneId		GetZoneCount();
	
	private:
		// All registered zones, filled during static initialization and indexed
		// by ZoneId.
		static const char*	s_zoneRegistry[ ZR_MAX_ZONE_COUNT ];
		static ZoneId		s_zoneCount;
	};
	
	inline const char* ZoneRegistry::GetZoneName( ZoneId a_zi )
	{
		return s_zoneRegistry[ a_zi ];
	}
	
	inline ZoneId ZoneRegistry::GetZoneCount()
	{
		return s_zoneCount;
	}

	// There should be one zone stack per thread of execution, and it needs
	// the global zone registry. However, Lxt currently only supports one thread.
	enum { ZS_MAX_STACK_SIZE	= 256 };
	class ZoneStack
	{
	public:
		ZoneStack() 
			:	m_top( 0 )
		{
			EnterZone( ZR_DEFAULT_ZONE_ID );
		}
		
		~ZoneStack()
		{
			LeaveZone( ZR_DEFAULT_ZONE_ID );
		}
		
		inline ZoneId	CurrentZone();	
		inline void		EnterZone( ZoneId a_zi );
		inline void		LeaveZone( ZoneId a_zi );
		
	private:
		// After static init. this is the stack
		ZoneId		m_stack[ ZS_MAX_STACK_SIZE ];
		uint32_t	m_top;
	};
	
	inline ZoneId ZoneStack::CurrentZone() 
	{ 
		return m_stack[m_top-1]; 
	}	
	
	inline void ZoneStack::EnterZone( ZoneId a_zi )
	{
		LXT_ASSERT( m_top < ZS_MAX_STACK_SIZE + 1 );
		m_stack[ m_top++ ] = a_zi;
	}
	
	inline void	ZoneStack::LeaveZone( ZoneId a_zi )
	{
		LXT_ASSERT( a_zi == CurrentZone() );
		LXT_ASSERT( m_top > 0 );
		--m_top;
	}
		
}

#endif // __LXT_ZONE_STACK_H__
