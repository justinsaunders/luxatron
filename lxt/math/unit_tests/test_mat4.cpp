/*
 *  test_mat4.cpp
 *  test_runner
 *
 *  Created by Justin on 29/09/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */


#include <UnitTest++.h>

#include "math/mat4.h"
#include "math/vec4.h"

namespace // private
{
	struct Fixture
	{
		Fixture()
		: 
		m_mat4(   Lxt::Vec4( 0.0f,   1.0f,  2.0f,  3.0f ),
				  Lxt::Vec4( 4.0f,   5.0f,  6.0f,  7.0f ),
			      Lxt::Vec4( 8.0f,   9.0f, 10.0f, 11.0f ),
				  Lxt::Vec4( 12.0f, 13.0f, 14.0f, 15.0f ) ),
		m_rotate( Lxt::Vec4(0.707f, 0.707f, 0.0f, 0.0f ),
				  Lxt::Vec4(-.707f, 0.707f, 0.0f, 0.0f ),
				  Lxt::Vec4(0.0f,     0.0f, 1.0f, 0.0f ),
				  Lxt::Vec4(0.0f,     0.0f, 0.0f, 1.0f ) ),
		m_translate
				(Lxt::Vec4(1.0f, 0.0f, 0.0f, 0.0f),
				 Lxt::Vec4(0.0f, 1.0f, 0.0f, 0.0f),
				 Lxt::Vec4(0.0f, 0.0f, 1.0f, 0.0f),
				 Lxt::Vec4(1.0f, 2.0f, 3.0f, 1.0f) )
		{
		}
		
		Lxt::Mat4 m_mat4;
		Lxt::Mat4 m_rotate;
		Lxt::Mat4 m_translate;
	}; // Fixture
}

namespace Lxt 
{		
	TEST( Mat4_Identity )
	{
		for ( size_t i = 0; i < 4; i++ )
		{
			for ( size_t j = 0; j < 4; j++ )
			{
				if ( i == j ) 
					CHECK_EQUAL( 1.0f, Mat4::Identity( i, j ) );
				else
					CHECK_EQUAL( 0.0f, Mat4::Identity( i, j ) );
			}
		}
	}
	
	TEST_FIXTURE( Fixture, Mat4_SetColumn )
	{
		m_mat4.SetColumn( 0, Vec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
		m_mat4.SetColumn( 1, Vec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
		m_mat4.SetColumn( 2, Vec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
		m_mat4.SetColumn( 3, Vec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
		
		CHECK( m_mat4.Equal( Mat4::Zero) );
	}
	
	TEST_FIXTURE( Fixture, Mat4_SetRow )
	{
		m_mat4.SetRow( 0, Vec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
		m_mat4.SetRow( 1, Vec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
		m_mat4.SetRow( 2, Vec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
		m_mat4.SetRow( 3, Vec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
		
		CHECK( m_mat4.Equal( Mat4::Zero ) );
	}
	
	TEST_FIXTURE( Fixture, Mat4_GetColumn )
	{
		Vec4 c[4];
		
		for ( size_t i = 0; i < 4; i++ )
		{
			m_mat4.GetColumn( i, c[i] );
		}
		
		CHECK( Vec4( 0.0f, 1.0f, 2.0f, 3.0f ).Equal( c[0] ) );
		CHECK( Vec4( 4.0f, 5.0f, 6.0f, 7.0f ).Equal( c[1] ) );
		CHECK( Vec4( 8.0f, 9.0f, 10.0f, 11.0f ).Equal( c[2] ) );
		CHECK( Vec4( 12.0f, 13.0f, 14.0f, 15.0f ).Equal( c[3] ) );			  
	}
	
	TEST_FIXTURE( Fixture, Mat4_GetRow )
	{
		Vec4 r[4];
			  
		for ( size_t i = 0; i < 4; i++ )
		{
			m_mat4.GetRow( i, r[i] );
		}
			  
		CHECK( Vec4( 0.0f, 4.0f, 8.0f, 12.0f ).Equal( r[0] ) );
		CHECK( Vec4( 1.0f, 5.0f, 9.0f, 13.0f ).Equal( r[1] ) );
		CHECK( Vec4( 2.0f, 6.0f, 10.0f, 14.0f).Equal( r[2] ) );
		CHECK( Vec4( 3.0f, 7.0f, 11.0f, 15.0f).Equal( r[3] ) );			  
	}
	

	TEST_FIXTURE( Fixture, Mat4_Equal )
	{
		CHECK( Mat4::Identity.Equal( Mat4::Identity ) );
		CHECK( !Mat4::Identity.Equal( m_mat4 ) );
		
		// Check epsilon comparison
		Mat4 c = m_mat4;
		c(3,3) += 1.0f;
		
		CHECK( c.Equal( m_mat4, 1.1f ) );
		CHECK( !c.Equal( m_mat4, 0.1f ) );
	}
	
	TEST_FIXTURE( Fixture, Mat4_AssignmentOperator )
	{
		Mat4 test;
		test = m_mat4;
		
		CHECK( m_mat4.Equal( test ) );	  		
	}
				 
	TEST_FIXTURE( Fixture, Mat4_MultiplyVec4 )
	{
		Vec4 v( 1.0f, 2.0f, 3.0f, 1.0f );
		Vec4 r;
		
		Mat4::Multiply( Mat4::Identity, v, r );
		
		CHECK( r.Equal( v ) );
		
		Mat4::Multiply( m_translate, v, r );
		
		CHECK( r.Equal( Vec4( 2.0f, 4.0f, 6.0f, 1.0f ) ) );
	}
	
	TEST_FIXTURE( Fixture, Mat4_MultiplyMat4 )
	{
		Mat4 r0;
		Mat4::Multiply( Mat4::Identity, Mat4::Identity, r0 );
		CHECK( r0.Equal( Mat4::Identity )  );
		
		Mat4 r1;
		Mat4::Multiply( Mat4::Identity, m_mat4, r1 );
		CHECK( r1.Equal( m_mat4 ) );
		
		Mat4::Multiply( m_mat4, Mat4::Identity, r1 );
		CHECK( r1.Equal( m_mat4 ) );
		
		// trans X rot should be same as rotation then translation
		Mat4 r2;
		Mat4::Multiply( m_translate, m_rotate, r2 );
		const Vec4 v( 1.0f, 1.0f, 1.0f, 1.0f );
		Vec4 v2;
		Mat4::Multiply( r2, v, v2 );
		
		Vec4 v3;
		Mat4::Multiply( m_rotate, v, v3 );
		Vec4 v4;
		Mat4::Multiply( m_translate, v3, v4 );
		
		CHECK( v2.Equal( v4 ) );
	}
	
		
	TEST_FIXTURE( Fixture, Mat4_IndexOperatorConst )
	{
		CHECK_EQUAL( 0.0f, m_mat4(0,0) );
		CHECK_EQUAL( 1.0f, m_mat4(0,1) );
		CHECK_EQUAL( 2.0f, m_mat4(0,2) );
		CHECK_EQUAL( 3.0f, m_mat4(0,3) );
		
		CHECK_EQUAL( 4.0f, m_mat4(1,0) );
		CHECK_EQUAL( 5.0f, m_mat4(1,1) );
		CHECK_EQUAL( 6.0f, m_mat4(1,2) );
		CHECK_EQUAL( 7.0f, m_mat4(1,3) );
		
		CHECK_EQUAL( 8.0f, m_mat4(2,0) );
		CHECK_EQUAL( 9.0f, m_mat4(2,1) );
		CHECK_EQUAL( 10.0f, m_mat4(2,2) );
		CHECK_EQUAL( 11.0f, m_mat4(2,3) );
		
		CHECK_EQUAL( 12.0f, m_mat4(3,0) );
		CHECK_EQUAL( 13.0f, m_mat4(3,1) );
		CHECK_EQUAL( 14.0f, m_mat4(3,2) );
		CHECK_EQUAL( 15.0f, m_mat4(3,3) );
	}
	
	TEST( Mat4_IndexOperator )
	{
		Mat4 m;
		
		for ( size_t i = 0; i < 4; i++ )
		{
			for ( size_t j = 0; j < 4; j++ )
			{
				m( i, j ) = ( i == j ) ? 1.0f : 0.0f;
			}
		}
		
		CHECK_EQUAL( 1.0f, m(0,0) );
		CHECK_EQUAL( 0.0f, m(0,1) );
		CHECK_EQUAL( 0.0f, m(0,2) );
		CHECK_EQUAL( 0.0f, m(0,3) );
		
		CHECK_EQUAL( 0.0f, m(1,0) );
		CHECK_EQUAL( 1.0f, m(1,1) );
		CHECK_EQUAL( 0.0f, m(1,2) );
		CHECK_EQUAL( 0.0f, m(1,3) );
		
		CHECK_EQUAL( 0.0f, m(2,0) );
		CHECK_EQUAL( 0.0f, m(2,1) );
		CHECK_EQUAL( 1.0f, m(2,2) );
		CHECK_EQUAL( 0.0f, m(2,3) );
		
		CHECK_EQUAL( 0.0f, m(3,0) );
		CHECK_EQUAL( 0.0f, m(3,1) );
		CHECK_EQUAL( 0.0f, m(3,2) );
		CHECK_EQUAL( 1.0f, m(3,3) );		
	}
	
	TEST_FIXTURE( Fixture, Mat4_IsOrthonormal )
	{
		CHECK_EQUAL( false, m_mat4.IsOrthonormal() );
		CHECK_EQUAL( true,	Mat4::Identity.IsOrthonormal() );
	}
	
	TEST_FIXTURE( Fixture, Mat4_InvertArbitary )
	{
		// identity
		Mat4 r;
		Mat4::InvertArbitary( Mat4::Identity, r );
		CHECK( Mat4::Identity.Equal( r ) );
		
		// Fixture - singular
		Mat4::InvertArbitary( m_mat4, r );
		CHECK( Mat4::Zero.Equal( r ) );
		
		// rotation, should be invertable
		
		Mat4 r2;
		Mat4::InvertArbitary( m_rotate, r2 );
		CHECK( !Mat4::Zero.Equal( r2 ) );
		
		// result should be inverse
		Mat4 r3;
		Mat4::Multiply( r2, m_rotate, r3 );
		CHECK( Mat4::Identity.Equal( r3 ) );
	}
	
	TEST_FIXTURE( Fixture, Mat4_Serialise )
	{
		Archive a;
		size_t	written = Mat4::Store( m_mat4, a );
		CHECK_EQUAL( size_t(64), written );
		
		Mat4	m;
		size_t	offset = 0;
		Mat4::Extract( m, a, offset );
		
		CHECK_EQUAL( written, offset );
		CHECK( m_mat4.Equal( m, 0.0f ) );
	}
}

