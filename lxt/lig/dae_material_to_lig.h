/*
 *  dae_material.h
 *  convert_dae
 *
 *  Created by Justin on 24/03/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_DAE_MATERIAL_TO_LIG_H__
#define __LXT_DAE_MATERIAL_TO_LIG_H__

#include <map>
#include <string>

class daeElement;
class domMaterial;

namespace Lxt
{
	class LigMaterial;

	// Convert a COLLADA material to an lig material	
	bool ConvertDomMaterialToLigMaterial(domMaterial*	a_material,
										 LigMaterial&	a_lxtMaterial );
};

#endif // __LXT_DAE_MATERIAL_TO_LIG_H__
