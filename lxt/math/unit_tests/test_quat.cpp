/*
 *  test_quat.cpp
 *  test_runner
 *
 *  Created by Justin on 30/10/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include <UnitTest++.h>
#include "quat.h"
#include "mat4.h"

namespace Lxt 
{
	TEST( Quat_FromRotationMatrix )
	{
		// rotate 90 degrees around x
		Mat4 rot;
		MakeRotationMatrix( 90, 1.0f, 0.0f, 0.0f, rot );
		
		Quat q;
		q.FromRotationMatrix( rot );
		CHECK( q.Equal( Quat(0.7071f, 0.0f, 0.0f, 0.7071f) ) );
		
		Mat4 i = Mat4::Identity;
		q = Quat::Identity;
		
		q.ToRotationMatrix( i );
		CHECK( i.Equal(Mat4::Identity) );
	}
	
	TEST( Quat_FromAngleAxis )
	{
		// rotate 90 degrees around x
		Mat4 rot;
		MakeRotationMatrix( 90.0f, 1.0f, 0.0f, 0.0f, rot );
		
		Quat q;
		q.FromRotationMatrix( rot );
		
		Quat q2;
		q2.FromAngleAxis( 90.0f, 1.0f, 0.0f, 0.0f );
		
		CHECK( q.Equal( q2 ) );
	}
	
	TEST( Quat_Slerp )
	{
		// No rotation
		Quat q0 = Quat::Identity;
		
		// rotate 90 degrees around x
		Quat q1; q1.FromAngleAxis( 90, 1.0f, 0.0f, 0.0f );
		
		// rotate 45 degress around x
		Quat m; m.FromAngleAxis( 45, 1.0f, 0.0f, 0.0f );
		
		Quat r;
		r.Slerp( q0, q1, 0.5f );
		
		CHECK( r.Equal( m ) );
	}
}
