/*
 *  test_state_cache.cpp
 *  test_runner
 *
 *  Created by Justin on 26/10/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#include <UnitTest++.h>
#include "state_cache.h"
#include "state_description.h"

namespace  
{
	struct Fixture
	{
		Lxt::StateCache		f_cache;
		
		Lxt::RasterStateDesc f_cullOff;
		Lxt::RasterStateDesc f_cullOn;
		
		Fixture() : f_cache( 8 )
		{
			f_cullOff.m_cullMode = Lxt::CM_NONE;
			f_cullOn.m_cullMode	 = Lxt::CM_FRONT;
		}
	};
}

namespace Lxt
{
	TEST_FIXTURE( Fixture, Test_StateCache_Raster )
	{
		// Add off
		Handle h_off = f_cache.CreateRasterState( f_cullOff );
		CHECK( h_off == 0 );
		
		// Add on - should be a unique handle
		Handle h_on = f_cache.CreateRasterState( f_cullOn );
		CHECK( h_on == 1 );
		CHECK( ! f_cache.GetRasterState( h_on )->Equal( *f_cache.GetRasterState( h_off ) ) );
		
		// Add off again - should be same
		Handle h_off2 = f_cache.CreateRasterState( f_cullOff );
		CHECK( h_off2 == h_off );
		
		// Add on again - should be same
		Handle h_on2 = f_cache.CreateRasterState( f_cullOn );
		CHECK( h_on2 == h_on );
	}
	
	TEST_FIXTURE( Fixture, Test_StateCache_Full )
	{
		StateCache smallCache(1); // only room for one of each
		
		// Fill Cache
		Handle h = smallCache.CreateRasterState( f_cullOn );
		CHECK( Lxt::INVALID_HANDLE != h);
		
		// Overflow, should return invalid handle
		CHECK( Lxt::INVALID_HANDLE == 
					smallCache.CreateRasterState( f_cullOff ) );
		
		// Adding same, should be valid
		CHECK( h == smallCache.CreateRasterState( f_cullOn ) );
	}
}