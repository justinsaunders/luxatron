/*
 *  camera.cpp
 *  Luxatron
 *
 *  Created by Justin on 19/10/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "camera.h"

using namespace Lxt;

// Utility function to create 2D Orthographic camera.
void Camera::SetOrtho( float a_width, float a_height)
{
	// Create default projection
	Mat4 proj;
	
	// Create orthographic projection
	MakeOrthographicMatrix( a_width, 0.0f, -a_height, 0.0f, -1.0f, 1.0f, 
						   proj );
	
	// create camera for 2d scene
	SetProj( proj );
	SetView( Mat4::Identity );
}

void Camera::SetPerspective( float a_width, float a_height, float a_fovDegrees,
							float a_near, float a_far, const Mat4& a_lookAt )
{
	// See http://www.sjbaker.org/steve/omniv/love_your_z_buffer.html for good
	// distance calculators.
	Mat4 proj;
	MakePerspectiveMatrix( a_fovDegrees, a_width/a_height, a_near, a_far, 
						  proj );
	
	SetProj( proj );
	SetView( a_lookAt );
}
