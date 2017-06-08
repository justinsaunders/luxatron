/*
 *  test_rect.cpp
 *  test_runner
 *
 *  Created by Justin on 14/06/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#include <UnitTest++.h>
#include "../rect.h"

namespace Lxt 
{
	TEST( Rect_Construction )
	{
		Rect r( 10.f, 20.f, 100.f, 200.f );
		
		CHECK_EQUAL( 10.f, r.m_x );
		CHECK_EQUAL( 20.f, r.m_y );
		CHECK_EQUAL( 100.f, r.m_width );
		CHECK_EQUAL( 200.f, r.m_height );
	}

	TEST( Rect_Coordinates )
	{
		Rect r( 10.f, 20.f, 100.f, 200.f );
	
		Vec2 a = r.A();
		Vec2 b = r.B();
		Vec2 c = r.Centre();
		
		CHECK_EQUAL( 10.f, a.m_x );
		CHECK_EQUAL( 20.f, a.m_y );
	
		CHECK_EQUAL( 110.f, b.m_x );
		CHECK_EQUAL( 220.f, b.m_y );
		
		CHECK_EQUAL( 60.0f, c.m_x );
		CHECK_EQUAL( 120.0f, c.m_y );
	}

	TEST( Rect_SetCentre )
	{
		Rect r( 0.f, 0.f, 100.f, 200.f );

		// old centre is 50, 100
		r.SetCentre( 75.f, 125.f );

		CHECK_EQUAL( 25.f, r.m_x );
		CHECK_EQUAL( 25.f, r.m_y );
		
		// Dimensions should be the same
		CHECK_EQUAL( 100.f, r.m_width );
		CHECK_EQUAL( 200.f, r.m_height );
	}
	
	TEST( Rect_Scale )
	{
		Rect r( 50.f, 50.f, 10.f, 20.f );
		
		// make 5 times bigger
		r.Scale( 5.f );
		
		// Center should be the same
		Vec2 c = r.Centre();
		
		CHECK_EQUAL( 55.f, c.m_x );
		CHECK_EQUAL( 60.f, c.m_y );
		
		// Dimensions should be 5x bigger
		CHECK_EQUAL( 50.f, r.m_width );
		CHECK_EQUAL( 100.0f, r.m_height );
	};
}