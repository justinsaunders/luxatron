/*
 *  test_zone_stack.cpp
 *  test_runner
 *
 *  Created by Justin on 28/02/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include <UnitTest++.h>
#include "zone_stack.h"

namespace 
{
	struct Fixture
	{
		Lxt::ZoneStack zs;
	};
	
	Lxt::ZoneId alphaZoneId =	Lxt::ZoneRegistry::DeclareZone( "alpha" );
	Lxt::ZoneId betaZoneId =	Lxt::ZoneRegistry::DeclareZone( "beta" );
	Lxt::ZoneId gammaZoneId =	Lxt::ZoneRegistry::DeclareZone( "gamma" );
}

namespace Lxt 
{
	TEST_FIXTURE( Fixture, ZoneStack_Default )
	{
		// This test cheats by "knowing" what the default zone id is
		CHECK_EQUAL( ZoneId(ZR_DEFAULT_ZONE_ID), zs.CurrentZone() );
	}
	
	TEST( ZoneStack_Declared )
	{
		CHECK( alphaZoneId	!= 0 );
		CHECK( alphaZoneId	!= betaZoneId );
		CHECK( betaZoneId	!= gammaZoneId );
	}
	
	TEST( ZoneStack_ZoneNames )
	{
		CHECK_EQUAL( "Default Zone" , ZoneRegistry::GetZoneName( 0 ) );
		CHECK_EQUAL( "alpha"		, ZoneRegistry::GetZoneName( alphaZoneId ) );
		CHECK_EQUAL( "beta"			, ZoneRegistry::GetZoneName( betaZoneId ) );
		CHECK_EQUAL( "gamma"		, ZoneRegistry::GetZoneName( gammaZoneId ) );
	}
	
	TEST_FIXTURE( Fixture, ZoneStack_BeginEnd )
	{
		zs.EnterZone( alphaZoneId );
		CHECK_EQUAL( alphaZoneId, zs.CurrentZone() );
		
		zs.EnterZone( betaZoneId );
		CHECK_EQUAL( betaZoneId, zs.CurrentZone() );
		
		zs.EnterZone( gammaZoneId );

		CHECK_EQUAL( gammaZoneId, zs.CurrentZone() );
		
		zs.LeaveZone( gammaZoneId );
		CHECK_EQUAL( betaZoneId, zs.CurrentZone() );
		
		zs.LeaveZone( betaZoneId );
		CHECK_EQUAL( alphaZoneId, zs.CurrentZone() );
		
		zs.LeaveZone( alphaZoneId );
		CHECK_EQUAL( 0, zs.CurrentZone() );
	}
}