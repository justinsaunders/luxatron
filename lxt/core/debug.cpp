/*
 *  debug.cpp
 *  test_runner
 *
 *  Created by Justin on 29/09/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "debug.h"
#include "zone_stack.h"

namespace Lxt
{	
	ScopedLeakChecker::ScopedLeakChecker( ZoneStack* a_zs, size_t a_numKnownLeaks ) 
		:	m_base( g_totalAllocations ),
			m_zoneStack( a_zs ),
			m_numKnownLeaks( a_numKnownLeaks )
	{ 
	}
	
	ScopedLeakChecker::~ScopedLeakChecker() 
	{
		if ( (m_base + m_numKnownLeaks) != g_totalAllocations )
		{
			const char* curZone = m_zoneStack ? 
						ZoneRegistry::GetZoneName( m_zoneStack->CurrentZone() )
						: "<no zone stack>";
			
			LXT_LOG_SPAM("Memory alloc mismatch in zone '%s':\n" 
						 " Baseline allocations = %lu (%lu + %lu known leaks).\n"
						 " Actual allocations = %lu.\n"
						 " Mismatch = %lu.\n",
						 curZone, 
						 m_base + m_numKnownLeaks, m_base, m_numKnownLeaks,
						 g_totalAllocations,
						 m_base + m_numKnownLeaks - g_totalAllocations );
		}
	}
}