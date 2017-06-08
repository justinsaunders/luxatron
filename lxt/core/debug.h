/*
 *  debug.h
 *  test_runner
 *
 *  Created by Justin on 29/09/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_CORE_DEBUG_H__
#define __LXT_CORE_DEBUG_H__

#include "assert.h"
#include "log.h"
#include "timer.h"

// A simple scoped leak checker - not defined in memory.h, because debug.h
// is higher level (ie memory tracking overrides debugging).
namespace Lxt 
{
	class ZoneStack;
	
	class ScopedLeakChecker
	{
	public:
		ScopedLeakChecker( ZoneStack* a_zs = NULL, size_t a_numKnownLeaks = 0);
		~ScopedLeakChecker();

	private:
		struct KnownLeaks 
		{
			const char* m_description;
			size_t		m_count;
		};
		
		size_t		m_base;
		ZoneStack*	m_zoneStack;
		
		size_t		m_numKnownLeaks;
	};
	
	class ScopedSpamTimer
	{
	public:
		ScopedSpamTimer( const char* a_areaName )
		:	m_start( GetSystemTickCount() ),
		m_name( a_areaName )
		{
		}
		
		~ScopedSpamTimer()
		{
			uint64_t	end = GetSystemTickCount();
			double		s	= (end - m_start) * GetSystemTickPeriod();
			
			LXT_LOG_SPAM("ScopedSpamTimer(): %2.2f seconds spend in '%s'\n", 
						 s, m_name );
		}
		
	private:
		uint64_t	m_start;
		const char* m_name;
	};
				
} // Lxt
#endif