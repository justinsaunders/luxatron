/*
 *  test_util.cpp
 *  test_runner
 *
 *  Created by Justin on 7/10/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include <UnitTest++.h>
#include "math/util.h"
#include "math/vec4.h"
#include "math/mat4.h"
#include "math/quat.h"
#include "math/vec3.h"

namespace Lxt
{
	TEST( Math_Util_Equal )
	{
		float a = 1.0f;
		float b = 1.1f;
		float c = 2.0f;
		
		// Equal
		CHECK( Equal( a, 1.0f ) );
		CHECK( !Equal( a, 1.1f ) );
		
		CHECK( Equal( b, 1.0f, 0.1001f ) );
		CHECK( !Equal( b, 1.0f ) );
		
		CHECK( Equal( b, c, 1.0f ) );
		CHECK( Equal( a, c, 1.00001f ) ); 
		
		// Tolerance zero
		CHECK( Equal( 0.0f, 0.0f, 1.0f ) );
		CHECK( !Equal( 1.0f, 1.1f, 0.0f ) );
	}
	
	TEST( Math_Util_Clamp )
	{
		float a = 0.0f, b = 1.0f, c;
		
		c = Clamp( a, b, -1.0f );
		CHECK_EQUAL( 0.0f, c );
		
		c = Clamp( a, b, 2.0f );
		CHECK_EQUAL( 1.0f, c );
		
		c = Clamp( a, b, 0.5f );
		CHECK_EQUAL( 0.5f, c );
	}	
	
	TEST( Math_Util_Lerp )
	{
		float a = 0.0f, b = 1.0f, c;
		
		c = Lerp( a, b, 0.0f );
		CHECK_EQUAL( 0.0f, c );
		
		c = Lerp( a, b, 1.0f );
		CHECK_EQUAL( 1.0f, c );
		
		c = Lerp( -1.0f, 1.0f, 0.5f );
		CHECK_EQUAL( 0.0f, c );
	}
	
	TEST( Math_Util_EaseInEaseOut )
	{
		float a = 10.0f, b = 20.0f, c;
		
		c = EaseInEaseOut( a, b, 0.0f );
		CHECK_EQUAL( a, c );
		
		c = EaseInEaseOut( a, b, 1.0f );
		CHECK_EQUAL( b, c );
		
		c = EaseInEaseOut( -1.0f, 1.0f, 0.5f );
		CHECK( Equal(0.0f, c ) );		
	}
	
	TEST( Math_Util_IsPow2 )
	{
		CHECK( !IsPow2(0) );
		CHECK( IsPow2(1) );
		CHECK( IsPow2(2)  );
		CHECK( !IsPow2(3) );
		CHECK( IsPow2(4)  );
		CHECK( !IsPow2(127));
		CHECK( IsPow2(256));
	}

	TEST( Math_Util_RandomInRange )
	{
		float r = RandomInRange( 0.0f, 0.0f );
		CHECK_EQUAL( r, 0.0f );
	}
	
	TEST( Math_Util_DegToRad )
	{
		CHECK( Equal( M_PI, DegToRad( 180.0f ) ) );
	}
	
	TEST( Math_Util_MakeRotationMatrix )
	{
		Vec4 x( 1.0f, 0.0f, 0.0f, 0.0f );
		Vec4 nx = x; nx.Scale( -1.0f );
		Vec4 y( 0.0f, 1.0f, 0.0f, 0.0f );
		Vec4 ny = y; ny.Scale( -1.0f );
		Vec4 z( 0.0f, 0.0f, 1.0f, 0.0f );
		Vec4 nz = z; nz.Scale( -1.0f );
		
		Mat4 x90;
		MakeRotationMatrix( 90, 1.0f, 0.0f, 0.0f, x90 );
		Mat4 y90;
		MakeRotationMatrix( 90, 0.0f, 1.0f, 0.0f, y90 );
		Mat4 z90;
		MakeRotationMatrix( 90, 0.0f, 0.0f, 1.0f, z90 );
		
		// Test rotation around same axis, should be no change
		Vec4 xt, yt, zt;
		
		Mat4::Multiply( x90, x, xt );
		CHECK( xt.Equal( x ) );
		
		Mat4::Multiply( y90, y, yt );
		CHECK( yt.Equal( y ) );
		
		Mat4::Multiply( z90, z, zt );
		CHECK( zt.Equal( z ) );
		
		// Rotate around axis to swap axes
		Mat4::Multiply( y90, x, xt );
		CHECK( xt.Equal( nz ) );
		
		Mat4::Multiply( x90, y, yt );
		CHECK( yt.Equal( z ) );
		
		Mat4::Multiply( z90, x, xt );
		CHECK( xt.Equal( y ) );
	}
	
	TEST( Math_Util_MakeRotationAroundPointMatrix )
	{
		// some normals
		Vec4 x( 1.0f, 0.0f, 0.0f, 0.0f );
		Vec4 nx = x; nx.Scale( -1.0f );
		Vec4 y( 0.0f, 1.0f, 0.0f, 0.0f );
		Vec4 ny = y; ny.Scale( -1.0f );
		Vec4 z( 0.0f, 0.0f, 1.0f, 0.0f );
		Vec4 nz = z; nz.Scale( -1.0f );
		
		// results
		Vec4 xt;
		
		// test points 
		Vec3 origin(0.f, 0.f, 0.f);
		Vec3 point(100.f, 0.0f, 0.0f);
		Vec3 axis(0.0f, 1.0f, 0.0f);
		
		// Rotate x normal around origin on y axis
		Mat4 m; 
		MakeRotationAroundLineMatrix( 90.f,
									 origin.m_x,
									 origin.m_y,
									 origin.m_z,
									 axis.m_x,
									 axis.m_y,
									 axis.m_z, 
									 m );
		Mat4::Multiply( m, x, xt );
		
		CHECK( xt.Equal( nz ) );
		
		// Rotate x normal around point on y axis ( position should not count )
		MakeRotationAroundLineMatrix( 90.f,
									 point.m_x,
									 point.m_y,
									 point.m_z,
									 axis.m_x,
									 axis.m_y,
									 axis.m_z, 
									 m );
		
		Mat4::Multiply( m, x, xt );
		
		CHECK( xt.Equal( nz ) );
		
		// Rotate point around another point on y axis
		Vec4 xp( 101.f, 0.f, 0.f, 1.f );
		Mat4::Multiply( m, xp, xt );
		
		CHECK( Equal( 100.f, xt.m_x ) );
		CHECK_EQUAL( 0.f,	xt.m_y );
		CHECK_EQUAL( -1.f,	xt.m_z );
		CHECK_EQUAL( 1.0f,	xt.m_w );
	}
}
