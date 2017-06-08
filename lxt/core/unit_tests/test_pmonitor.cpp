/*
 *  test_pmonitor.cpp
 *  test_runner
 *
 *  Created by Justin on 1/03/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include <UnitTest++.h>
#include "pmonitor.h"
#include "math/util.h"

LXT_ZONE_REGISTER(test_pzone1);
LXT_ZONE_REGISTER(test_pzone2);
LXT_ZONE_REGISTER(test_pzone3);

namespace 
{
	struct Fixture
	{
		Lxt::ZoneStack zs;
		Lxt::PMonitor  pmonitor;
	
		// When fixture is created, profiling begins.
		Fixture()
			: pmonitor( zs, 32, NULL )
		{
		}
	};
}

namespace Lxt 
{	
	TEST_FIXTURE( Fixture, PMonitor_Simple )
	{
		// delay for at least one second
		sleep( 1 );
	
		// next frame.
		pmonitor.Update();
		
		// Get time spent in unallocated - should be around 1 second.
		uint64_t tc = pmonitor.GetSample( 0, ZR_DEFAULT_ZONE_ID );
		CHECK( tc > 0 );
		
		float s = (float) tc * GetSystemTickPeriod();
		// amount elapsed should be within 25% of this
		// (allowing for severe slowdown from background tasks).
		CHECK( Equal( s, 1.0f, 0.25f ) );
		
		// Everything else in last frame should be zero, and everything in this
		// frame (#1) should be zero
		for ( size_t i = 0; i < ZoneRegistry::GetZoneCount(); i++ )
		{
			if ( i != ZR_DEFAULT_ZONE_ID )
			{
				// check last frame
				CHECK_EQUAL( uint64_t(0), pmonitor.GetSample( 0, i ) );
			}
			
			// check this frame
			CHECK_EQUAL( uint64_t(0), pmonitor.GetSample( 1, i ) );
		}
	}

	TEST_FIXTURE( Fixture, PMonitor_Stack )
	{
        for (size_t i = 0; i < 100; i++)
        {
            pmonitor.EnterZone(LxtZone_test_pzone1);
            usleep(10000);
                pmonitor.EnterZone(LxtZone_test_pzone2 );
                usleep(10000);
                    pmonitor.EnterZone(LxtZone_test_pzone3);
                    usleep(10000);
                    pmonitor.LeaveZone(LxtZone_test_pzone3);
                usleep(10000);
                pmonitor.LeaveZone(LxtZone_test_pzone2);
            usleep(10000);
            pmonitor.LeaveZone(LxtZone_test_pzone1);
            
            size_t    f = i % pmonitor.GetNumFrames(); // frame number
            uint64_t dz = pmonitor.GetSample(f, ZR_DEFAULT_ZONE_ID);
            uint64_t z1 = pmonitor.GetSample(f, LxtZone_test_pzone1);
            uint64_t z2 = pmonitor.GetSample(f, LxtZone_test_pzone2);
            uint64_t z3 = pmonitor.GetSample(f, LxtZone_test_pzone3);
            uint64_t t  = pmonitor.GetFrameTotal(f);
            double	 p  = GetSystemTickPeriod();
            
            CHECK(Equal(dz * p, 0, 0.01f));
            CHECK(Equal(z1 * p, 0.02f, 0.01f));
            CHECK(Equal(z2 * p, 0.02f, 0.01f));
            CHECK(Equal(z3 * p, 0.01f, 0.01f));
            CHECK(Equal(t  * p, 0.05f, 0.01f));
            
            pmonitor.Update();
        }
	}
}
