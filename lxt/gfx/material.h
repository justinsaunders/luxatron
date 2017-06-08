/*
 *  material.h
 *  model
 *
 *  Created by Justin on 24/10/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_GFX_MATERIAL_H__
#define __LXT_GFX_MATERIAL_H__

#include "math/vec4.h"
#include "texture_pool.h"
#include "renderer.h"

namespace Lxt
{	
	class StateCache;
	class TexturePool;
	class Mesh;
	
	// Material blend types, mapped internally to render API constants.
	enum MaterialBlendType
	{
		MBT_OPAQUE	= 0,	// no blending
		MBT_ALPHA,			// alpha ( srcAlpha, 1 - srcAlpha ).
		MBT_ADD,			// additive
		MBT_SUBTRACT,		// subtractive
		MBT_MAX
	};
	
	// A simple fixed function material.
	struct Material
	{
		friend class Renderer;
		
		// Construction
		Material();
		
		// Sets a material, does not upload it
		void Set( Vec4				a_diffuse,
				  Vec4				a_specular,
				  float				a_shininess,
				  Texture*			a_texture,
				  MaterialBlendType	a_blendType,
				  bool				a_lit );
		
		// Serialization
		static bool Extract(Material&		a_material,  
							TexturePool&	a_tp, 
							Archive&		a_source,
							size_t&			a_offset );
		
		static size_t Store( const Material&	a_material, 
							 const TexturePool& a_tp,
							 Archive&			a_destination );
		
		// Dump as text for debugging
		void Dump();
		
	private:
		Vec4				m_diffuse;
		Vec4				m_specular;
		float				m_shininess;
		Texture*			m_texture;
		MaterialBlendType	m_blendType;
		bool				m_lit;			// does lighting apply?

		// Runtime states
		Handle	m_rasterState;
		Handle	m_depthState;
		Handle	m_blendState;		
	};
}

#endif // __LXT_GFX_MATERIAL_H__