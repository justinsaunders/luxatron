/*
 *  util.cpp
 *  Luxatron
 *
 *  Created by Justin on 14/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "util.h"
#include "math.h"

namespace Lxt 
{
	float EaseInEaseOut( float a_a, float a_b, float a_t )
	{
		float a = (a_b - a_a)/2.0f;				// half amplitude of curve (wave)
		float t = Lerp( -LXT_PI, 0.0f, a_t );	// time between min and max
		
		float c = cosf( t );
		float h = ( c * a ) + a + a_a;			// height, offset to start at 0
		return h;
	}
	
	void MakePerspectiveMatrix(	float a_FOV, 
								float a_aspect, 
								float a_near, 
								float a_far,
								Mat4& a_output )
	{
		float top	 =  a_near * tanf( a_FOV * 0.5f);
		float bottom =  -top;
		float left	 =  bottom * a_aspect;
		float right  =  top * a_aspect;
		
		float dx = right - left;
		float dy = top	 - bottom;
		float dz = a_far - a_near;
		
		float x = (2.0f * a_near) / dx;
		float y = (2.0f * a_near) / dy;
		float a = (right + left) / dx;
		float b = (top + bottom) / dy;
		float c = -(a_far + a_near) / dz;
		float d = -(2.0f * a_far * a_near) / dz;
		
		a_output.SetColumn( 0, Vec4( x,   0.0f, 0.0f, 0.0f ) );
		a_output.SetColumn( 1, Vec4( 0.0f,   y, 0.0f, 0.0f ) );
		a_output.SetColumn( 2, Vec4( a,      b,    c, -1.f ) ) ;
		a_output.SetColumn( 3, Vec4( 0.0f, 0.0f,   d, 0.0f ) );
	}
	
	
	void MakeOrthographicMatrix( float a_left,
								 float a_right,
								 float a_bottom,
								 float a_top,
								 float a_near,
								 float a_far,
								 Mat4& a_output )
	{
		float dx = a_right - a_left;
		float dy = a_top - a_bottom;
		float dz = a_far - a_near;
		
		float x =  2.0f/dx;
		float y =  2.0f/dy;
		float z =  2.0f/dz;
		
		float tx = ( a_right + a_left ) / dx;
		float ty = ( a_top + a_bottom ) / dy;
		float tz = ( a_far + a_near )   / dz;
		
		a_output.SetColumn( 0, Vec4( x,   0.0f, 0.0f, 0.0f ) );
		a_output.SetColumn( 1, Vec4( 0.0f,   y, 0.0f, 0.0f ) );
		a_output.SetColumn( 2, Vec4( 0.0f, 0.0f,  -z, 0.0f ) );
		a_output.SetColumn( 3, Vec4( -tx,   -ty, -tz, 1.0f ) );
	}
	
	void MakeRotationMatrix( float a_degrees,
						 	float a_x,
							float a_y,
							float a_z,
							Mat4& a_output )
	{
		Vec4 v( a_x, a_y, a_z, 0 );
		v.Normalise();
		
		float x = v.m_x;
		float y = v.m_y;
		float z = v.m_z;
		
		float r = DegToRad( a_degrees );
		float c = cosf( r );
		float _c = 1 - c;
		float s = sinf( r );
		
		float xs = x * s;
		float ys = y * s;
		float zs = z * s;
		
		float x2 = x * x;
		float xy = x * y;
		float xz = x * z;
		float y2 = y * y;
		float yz = y * z;
		float z2 = z * z;
		
		a_output.SetColumn( 0, Vec4( ( x2 * _c ) + c, 
									 ( xy * _c ) + zs, 
									 ( xz * _c ) - ys, 
									 0.0f ) );
		
		a_output.SetColumn( 1, Vec4( ( xy * _c ) - zs, 
									 ( y2 * _c ) + c, 
									 ( yz * _c ) + xs, 
									 0.0f ) );
		
		a_output.SetColumn( 2, Vec4( ( xz * _c ) + ys,
									 ( yz * _c ) - xs,
									 ( z2 * _c ) + c,
									 0.0f ) );
		
		a_output.SetColumn( 3, Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
		
	}
	
	void MakeRotationAroundLineMatrix( const float	a_degrees, 
									   const float	a_a,
									   const float	a_b,
									   const float	a_c,
									   const float	a_u,
									   const float	a_v,
									   const float	a_w,
									   Mat4&		a_matrix )
	{	
		// Code from http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/ArbitraryAxisRotation.html
		
		// Set some intermediate values.
		const float u2 = a_u * a_u;
		const float v2 = a_v * a_v;
		const float w2 = a_w * a_w;
		const float theta = DegToRad( a_degrees );
		const float cosT = cosf( theta );
		const float sinT = sinf( theta );
		const float l2 = u2 + v2 + w2;
		const float l =  sqrtf(l2);
			
		LXT_ASSERT( l2 > 0.000001f );
			
		// Build the matrix entries element by element. 
		a_matrix(0,0) = (u2 + (v2 + w2) * cosT)/l2;
		a_matrix(1,0) = (a_u*a_v * (1 - cosT) - a_w*l*sinT)/l2;
		a_matrix(2,0) = (a_u*a_w * (1 - cosT) + a_v*l*sinT)/l2;
		a_matrix(3,0) = (a_a*(v2 + w2) - a_u*(a_b*a_v + a_c*a_w) 
				   + (a_u*(a_b*a_v + a_c*a_w) - a_a*(v2 + w2))*cosT 
						 + (a_b*a_w - a_c*a_v)*l*sinT)/l2;
			
		a_matrix(0,1) = (a_u*a_v * (1 - cosT) + a_w*l*sinT)/l2;
		a_matrix(1,1) = (v2 + (u2 + w2) * cosT)/l2;
		a_matrix(2,1) = (a_v*a_w * (1 - cosT) - a_u*l*sinT)/l2;
		a_matrix(3,1) = (a_b*(u2 + w2) - a_v*(a_a*a_u + a_c*a_w) 
				   + (a_v*(a_a*a_u + a_c*a_w) - a_b*(u2 + w2))*cosT 
						 + (a_c*a_u - a_a*a_w)*l*sinT)/l2;
			
		a_matrix(0,2) = (a_u*a_w * (1 - cosT) - a_v*l*sinT)/l2;
		a_matrix(1,2) = (a_v*a_w * (1 - cosT) + a_u*l*sinT)/l2;
		a_matrix(2,2) = (w2 + (u2 + v2) * cosT)/l2;
		a_matrix(3,2) = (a_c*(u2 + v2) - a_w*(a_a*a_u + a_b*a_v) 
				   + (a_w*(a_a*a_u + a_b*a_v) - a_c*(u2 + v2))*cosT 
						 + (a_a*a_v - a_b*a_u)*l*sinT)/l2;
		
		a_matrix(0,3) = 0.f;
		a_matrix(1,3) = 0.f;
		a_matrix(2,3) = 0.f;
		a_matrix(3,3) = 1.f;		
	}		
	
	void MakeLookAtMatrix( const Vec4&	a_pos, 
						  const Vec4&	a_target, 
						  const Vec4&	a_up,
						  Mat4&			a_mat )
	{
		// Forward
		Vec4 f = a_pos; f.Subtract( a_target ); f.Normalise();
		
		// Right
		Vec4 r; a_up.Cross3( f, r ); r.Normalise();
		
		// Up
		Vec4 u; f.Cross3( r, u ); 
		
		a_mat.m_column[0].Copy( r );
		a_mat.m_column[1].Copy( u );
		a_mat.m_column[2].Copy( f );
		a_mat.m_column[3].Copy( a_pos );
	}
}

