/*
 *  zone_stack.cpp
 *  
 *
 *  Created by Justin on 28/02/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "zone_stack.h"

namespace Lxt 
{
	// static initializers - default zone is always first
	ZoneId		ZoneRegistry::s_zoneCount	= 1;
	const char*	ZoneRegistry::s_zoneRegistry[ ZR_MAX_ZONE_COUNT ] 
											= { "Default Zone" };
	
	ZoneId ZoneRegistry::DeclareZone( const char* a_zoneName )
	{
		LXT_ASSERT( s_zoneCount < ZR_MAX_ZONE_COUNT );
		s_zoneRegistry[ s_zoneCount ] = a_zoneName;
		return s_zoneCount++;
	}
	
}