/*
 *  lig_material.h
 *  convert_dae
 *
 *  Created by Justin on 8/07/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_LIG_MATERIAL_H__
#define __LXT_LIG_MATERIAL_H__

#include <string>
#include "math/mat4.h"
#include "file/path.h"
#include "gfx/material.h"

namespace Lxt 
{
	struct LigMaterial
	{
		LigMaterial()		
		:	m_diffuseColour( 1.0f, 1.0f, 1.0f, 1.0f ),
			m_specularColour( 0.0f, 0.0f, 0.0f, 1.0f ),
			m_shininess( 0.0f ),
			m_blendType( MBT_OPAQUE ),
			m_lit( false )
		{
		}
		
		std::string			m_name;
		
		Path				m_texturePath;
		Vec4				m_diffuseColour;
		Vec4				m_specularColour;
		float				m_shininess;
		MaterialBlendType	m_blendType;
		bool				m_lit;
	};
}

#endif // __LXT_LIG_MATERIAL_H__
