/*
 *  actor.h
 *  
 *	An actor is an entity that "does something" in a game.
 *
 *  Created by Justin on 28/04/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_GAME_ACTOR_H__
#define __LXT_GAME_ACTOR_H__

#include "math/mat4.h"
#include "math/quat.h"

namespace Lxt 
{
	class DeviceContext;
	class Camera;
	
	class Actor
	{
	public:
		Actor();
		~Actor();
	
		void Update();
		
		void Render(const Camera& a_camera);
		
		void SetWorld(const Mat4& a_transform) { m_world = a_transform; };
		const Mat4& GetWorld() { return m_world; };
		
	protected:
		// World transform
		Mat4	m_world;
	};
}

#endif
