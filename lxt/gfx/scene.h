/*
 *  scene.h
 *  A class describing a simple scene with cameras, lights and meshes.
 *
 *  Created by Justin on 24/10/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_GFX_SCENE_H__
#define __LXT_GFX_SCENE_H__

#include <list>
#include "math/mat4.h"

namespace Lxt
{
	struct Camera;
	class Model;
	struct Renderer;
	
	class Scene
	{
	public:
		Scene();
		~Scene();
		
		void Create( Camera* a_camera );
		void Add( Model* a_model );
		void Remove( Model* a_model );
		void Draw( Renderer& a_renderer );
		
	private:
		Camera*					m_currentCamera;
		std::list<Model*>		m_models;
	};
}

#endif // __LXT_GFX_SCENE_H__
