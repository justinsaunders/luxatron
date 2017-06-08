/*
 *  quat.cpp
 *  model
 *
 *  Created by Justin on 30/10/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "quat.h"

#include "mat4.h"
#include "util.h"

namespace Lxt 
{
	const Quat Quat::Identity( 0.0f, 0.0f, 0.0f, 1.0f );
	const Quat Quat::Zero( 0.0f, 0.0f, 0.0f, 0.0f );
	
	void Quat::FromRotationMatrix(const Mat4& a_input )
	{
		float  tr, s, q[4];
		int    i, j, k;
		
		int nxt[3] = {1, 2, 0};
		
		tr = a_input(0,0) + a_input(1,1) + a_input(2,2);
		
		// check the diagonal
		if (tr > 0.0) {
			s = sqrt (tr + 1.0);
			m_w = s / 2.0;
			s = 0.5 / s;
			m_x = (a_input(1,2) - a_input(2,1)) * s;
			m_y = (a_input(2,0) - a_input(0,2)) * s;
			m_z = (a_input(0,1) - a_input(1,0)) * s;
		} else {		
			// diagonal is negative
			i = 0;
			if (a_input(1,1) > a_input(0,0)) i = 1;
			if (a_input(2,2) > a_input(i,i)) i = 2;
			j = nxt[i];
			k = nxt[j];
			
			s = sqrt ((a_input(i,i) - (a_input(j,j) + a_input(k,k))) + 1.0);
			
			q[i] = s * 0.5f;
			
			if (s != 0.0) s = 0.5f / s;
			
			q[3] = (a_input(j,k) - a_input(k,j)) * s;
			q[j] = (a_input(i,j) + a_input(j,i)) * s;
			q[k] = (a_input(i,k) + a_input(k,i)) * s;
			
			m_x = q[0];
			m_y = q[1];
			m_z = q[2];
			m_w = q[3];
		}
	}

	void Quat::ToRotationMatrix( Mat4& a_output ) const
	{
		float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
		
		// calculate coefficients
		x2 = m_x + m_x; y2 = m_y + m_y;
		z2 = m_z + m_z;
		xx = m_x * x2; xy = m_x * y2; xz = m_x * z2;
		yy = m_y * y2; yz = m_y * z2; zz = m_z * z2;
		wx = m_w * x2; wy = m_w * y2; wz = m_w * z2;
		
		a_output(0,0) = 1.0 - (yy + zz); a_output(1,0) = xy - wz;
		a_output(2,0) = xz + wy; a_output(3,0) = 0.0;
		
		a_output(0,1) = xy + wz; a_output(1,1) = 1.0 - (xx + zz);
		a_output(2,1) = yz - wx; a_output(3,1) = 0.0;
		
		a_output(0,2) = xz - wy; a_output(1,2) = yz + wx;
		a_output(2,2) = 1.0 - (xx + yy); a_output(3,2) = 0.0;
		
		a_output(0,3) = 0; a_output(1,3) = 0;
		a_output(2,3) = 0; a_output(3,3) = 1;
	}
	
	void Quat::FromAngleAxis( float a_degrees, float a_x, float a_y, float a_z )
	{
		float r = DegToRad(a_degrees)/2.0f;
		float s = sinf( r );
		
		m_x = a_x * s;
		m_y = a_y * s;
		m_z = a_z * s;
		m_w = cosf( r );
	}
	
	// Interpolation
	void Quat::Slerp( const Quat& a_q0, const Quat& a_q1, float a_t )
	{
		// Ensure inputs are unit length
		LXT_ASSERT( Lxt::Equal( a_q0.LengthSquared(), 1.0f ) );
		LXT_ASSERT( Lxt::Equal( a_q1.LengthSquared(), 1.0f ) );
		
		// Compute the cosine of the angle between the two vectors.
		float dot = a_q0.Dot( a_q1 );
			
		const float DOT_THRESHOLD = 0.99;
		
		if (dot > DOT_THRESHOLD) 
		{
			// If the inputs are too close for comfort, linearly interpolate
			// and normalize the result.
			Quat t = a_q1; t.Subtract( a_q0 ); t.Scale( a_t );
			Quat r = a_q0; r.Add( t );
			r.Normalise();
					
			this->Copy( r );
		}
		else
		{
			float theta_0 = acosf(dot);  // theta_0 = angle between input vectors
			float theta = theta_0 * a_t;    // theta = angle between v0 and result 
			
			Quat q0d = a_q0; q0d.Scale( dot );
			Quat q2  = a_q1; q2.Subtract( q0d );
			q2.Normalise();
			
			Quat r = a_q0; r.Scale( cosf(theta) );
			q2.Scale( sinf(theta) );
			
			r.Add( q2 );
			
			this->Copy( r );
		}
	}
}
