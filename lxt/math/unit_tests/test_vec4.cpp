/*
 *  test_vec4.cpp
 *  Luxatron
 *
 *  Created by Justin on 28/09/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include <UnitTest++.h>

#include "math/vec4.h"

namespace Lxt 
{
	TEST( Vec4_Set )
	{
		Vec4 v;
		v.Set( 1.0f, 2.0f, 3.0f, 4.0f );
		
		CHECK( v.m_x == 1.0f );
		CHECK( v.m_y == 2.0f );
		CHECK( v.m_z == 3.0f );
		CHECK( v.m_w == 4.0f );		
	}
	
	TEST( Vec4_Create_Components )
	{
		Vec4 v( 1.0f, 2.0f, 3.0f, 4.0f );
		CHECK( v.m_x == 1.0f );
		CHECK( v.m_y == 2.0f );
		CHECK( v.m_z == 3.0f );
		CHECK( v.m_w == 4.0f );		
	}

	TEST( Vec4_Create_Copy )
	{
		Vec4 v1( 1.0f, 2.0f, 3.0f, 4.0f );

		Vec4 v2( v1 );
		CHECK( v2.m_x == 1.0f );
		CHECK( v2.m_y == 2.0f );
		CHECK( v2.m_z == 3.0f );
		CHECK( v2.m_w == 4.0f );
	}
	
	TEST( Vec4_Equals )
	{
		Vec4 v1( 1.0f, 2.0f, 3.0f, 4.0f );

		Vec4 v2( 1.0f, 2.0f, 3.0f, 4.0f );
		
		Vec4 v3( 1.0f, 2.0f, 3.0f, 4.1f );
		
		// Equal
		CHECK( v1.Equal( v2 ) );
		CHECK( v2.Equal( v1 ) );
		
		// Not equal
		CHECK( ! ( v1.Equal( v3 ) ) );
		CHECK( ! ( v3.Equal( v1 ) ) );
	}

	TEST( Vec4_Add )
	{
		Vec4 v1( 1.0f, 2.0f, 3.0f, 4.0f );
		Vec4 v2( -1.0f, -2.0f, -3.0f, -4.0f );		
		Vec4 v3( 0.0f, 0.0f, 0.0f, 0.0f );
		
		v1.Add( v2 );
		
		CHECK( v1.Equal( v3 ) );
	}
	
	TEST( Vec4_Scale )
	{
		Vec4 v1( 1.0f, 2.0f, 3.0f, 4.0f );
		v1.Scale( 2.0f );
		
		CHECK( v1.Equal( Vec4( 2.0f, 4.0f, 6.0f, 8.0f ) ) );
	}
	
	TEST( Vec4_Dot )
	{
		Vec4 v1( 1.0f, 2.0f, 3.0f, 4.0f );
		Vec4 v2( 1.0f, 2.0f, 3.0f, 4.0f );
				
		CHECK_EQUAL( 1.0f + 4.0f + 9.0f + 16.0f, v1.Dot( v2 ) );
		CHECK_EQUAL( 1.0f + 4.0f + 9.0f + 16.0f, v2.Dot( v1 ) );
	}
		
	TEST( Vec4_Cross )
	{
		Vec4 v1( 1.0f, 0.0f, 0.0f, 0.0f );
		Vec4 v2( 0.0f, 1.0f, 0.0f, 0.0f );
		
		Vec4 r1;
		v1.Cross3( v2, r1 );
		
		Vec4 r2;
		v2.Cross3( v1, r2 );
		
		CHECK( r1.Equal( Vec4( 0.0f, 0.0f, 1.0f, 0.0f ) ) );
		CHECK( r2.Equal( Vec4( 0.0f, 0.0f, -1.0f, 0.0f ) ) );
	}
	
	TEST( Vec4_LengthSquared )
	{
		Vec4 v1( 0.5f, 1.0f, -2.0f, 4.0f );
		
		CHECK_EQUAL( 0.25f + 1.0f + 4.0f + 16.0f, v1.LengthSquared() );		
	}
	
	TEST( Vec4_Length )
	{
		Vec4 v1( 0.5f, 1.0f, -2.0f, 4.0f );

		CHECK_EQUAL( sqrtf( 0.25f + 1.0f + 4.0f + 16.0f ), v1.Length() );		
	}

	TEST( Vec4_Normalise )
	{
		Vec4 v1( -1.0f, 2.0f, -3.0f, 4.0f );
		Vec4 v2( v1 );
		
		v1.Scale( 1.0f/v1.Length() );
		v2.Normalise();
		
		CHECK( v1.Equal( v2 ) ); 
	}
	
	TEST( Vec4_AssignmentOperator )
	{
		Vec4 v1( 1.0f, 2.0f, 3.0f, 4.0f );
		Vec4 v2;
		Vec4 v3 = v2 = v1;
		
		CHECK( v2.Equal( v1 ) );
		CHECK( v3.Equal( v1 ) );		
	}
}






