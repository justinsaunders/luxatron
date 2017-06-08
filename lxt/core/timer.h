/*
 *  timer.h
 *  test_runner
 *
 *  Created by Justin on 28/02/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_TIMER_H__
#define __LXT_TIMER_H__

#include <mach/mach_time.h>

namespace Lxt
{
	// Get the number of system ticks since beginning of system time.
	inline uint64_t GetSystemTickCount()
	{
		return mach_absolute_time();
	}
	
	// Get the period of a system tick in seconds.
	inline double GetSystemTickPeriod()
	{
		// Use timebase info to account for differences between architecture's
		// tick time.
		mach_timebase_info_data_t info;
		mach_timebase_info(&info);
		
		return double(info.numer) / ( double(info.denom) * 1e9 );
	}
}


#endif // __LXT_TIMER_H__
