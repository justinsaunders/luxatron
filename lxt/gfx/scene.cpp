/*
 *  scene.cpp
 *  A class describing a simple scene with cameras, lights and meshes.
 *
 *  Created by Justin on 24/10/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "scene.h"

#include "camera.h"
#include "renderer.h"
#include "model.h"

namespace Lxt
{
	Scene::Scene()
	:  m_currentCamera( NULL )
	{
	}

	Scene::~Scene()
	{
	}
	
	void Scene::Create( Camera* a_camera )
	{
		m_currentCamera = a_camera;
	}
	
	void Scene::Add( Model* a_model )
	{
		m_models.push_back( a_model );
	}
	
	void Scene::Remove( Model* a_model )
	{
		LXT_ASSERT( false );
	}
	
	void Scene::Draw( Renderer& a_renderer )
	{
		// Set up camera
		a_renderer.SetProjection( m_currentCamera->Proj() );
		
		std::list<Model*>::iterator li = m_models.begin();
		while ( li != m_models.end() )
		{
			// reset matrix for this model
			a_renderer.SetModelView( m_currentCamera->InverseView() );
		
			// draw
			(*li)->Draw( a_renderer );
			
			// next model
			li++;
		}
	}
		
}
