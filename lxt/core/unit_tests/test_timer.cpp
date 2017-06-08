/*
 *  test_timer.cpp
 *  mac_test_runner
 *
 *  Created by Justin on 4/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include <UnitTest++.h>
#include "timer.h"
#include "math/util.h"

namespace 
{
	struct Fixture
	{
		double Seconds_Per_Tick[3];
		
		Fixture()
		{
			// Times are 166.67, 1, 30 nanoseconds per clock tick
			Seconds_Per_Tick[ LXT_ARCHITECTURE_ARM	] = ( 1000.0/6.0 ) / 1e9;
			Seconds_Per_Tick[ LXT_ARCHITECTURE_INTEL] = 1000.0/1e9;
			Seconds_Per_Tick[ LXT_ARCHITECTURE_PPC	] = 30.0/1e9;
		}
	};
}

namespace Lxt 
{
	TEST_FIXTURE( Fixture, Timer_Preconditions )
	{
		double expected = Seconds_Per_Tick[ LXT_ARCHITECTURE_CURRENT ];
		CHECK( Equal( expected, GetSystemTickPeriod() ) );
	}
}