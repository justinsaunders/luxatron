/*
 *  lig_scene.h
 *  convert_dae
 *
 *  Created by Justin on 8/07/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_LIG_SCENE_H__
#define __LXT_LIG_SCENE_H__

#include "lig_node.h"
#include "lig_material.h"

namespace Lxt 
{	
	typedef std::map< std::string, Lxt::LigMaterial >	LigMaterialMap;

	struct LigScene
	{
		LigNode*		m_root;			// node hierarchy
		LigMaterialMap	m_materials;	// material library for this scene
	};
}


#endif // __LXT_LIG_SCENE_H__

