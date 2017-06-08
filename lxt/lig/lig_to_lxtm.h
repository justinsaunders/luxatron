/*
 *  lig_to_lxtm.h
 *  convert_dae
 *
 *  Created by Justin on 8/07/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_LIG_TO_LXTM_H__
#define __LXT_LIG_TO_LXTM_H__

#include "gfx/model.h"
#include "lig_scene.h"

namespace Lxt 
{
	// Convert a LIG scene into a LXT model, updating a texture pool with
	// referenced textures.
	bool ConvertLIGScenetoLXTModel( const LigScene&	a_scene, 
									TexturePool&	a_texturePool, 
									Model&			a_model );
	
	// Convert a LIG mesh to a LXT mesh, updating a texture pool with referenced
	// textures.
	bool ConvertLIGMeshToLXTMesh(const LigMesh&			a_ligMesh,
								 const LigMaterialMap&	a_materials,
								 TexturePool&			a_texturePool,
								 Mesh&					a_lxtMesh );
}

#endif // __LXT_LIG_TO_LXTM_H__