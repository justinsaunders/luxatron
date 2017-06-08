/*
 *  camera.h
 *  Luxatron
 *
 *  Created by Justin on 19/10/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_GFX_CAMERA_H__
#define __LXT_GFX_CAMERA_H__

#include "math/mat4.h"
#include "math/vec3.h"

namespace Lxt 
{
	struct Camera
	{
		Camera()
		{
		}
		
		Camera( const Mat4& a_projection,
				const Mat4&	a_view )
		 :	m_projection( a_projection )
		{
			SetView( a_view );
		};
		
		const Mat4& Proj() const { return m_projection; }
		inline void SetProj( const Mat4& a_proj ) { m_projection = a_proj; }
		
		const Mat4&	View() const { return m_view; }
		inline void	SetView( const Mat4& a_view );
		
		const Mat4& InverseView() const { return m_inverseView; }
				
		// Utility function to create 2D Orthographic camera.
		void SetOrtho( float a_width, float a_height );
		
		// Utility function to create 3D Perspective camera.
		void SetPerspective( float a_width, float a_height, float a_fovDegrees,
							float a_near, float a_far, const Mat4& a_lookAt );
		
	private:
		Mat4	m_projection;
		Mat4	m_view;
		Mat4	m_inverseView;	// set when view is set
	};
	
	inline void Camera::SetView( const Mat4& a_view )
	{
		m_view = a_view;
		Mat4::InvertArbitary( a_view, m_inverseView ); 
	}
	
	
}


#endif // __LXT_GFX_CAMERA_H__
