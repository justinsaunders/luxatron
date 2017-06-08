/*
 *  material.cpp
 *  model
 *
 *  Created by Justin on 24/10/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "material.h"
#include "state_cache.h"

namespace Lxt 
{
	Material::Material()
	:	m_diffuse( 1.0f, 1.0f, 1.0f, 1.0f ),
		m_specular( 0.0f, 0.0f, 0.0f, 1.0f ),
		m_shininess( 0.0f ),
		m_texture( NULL ),
		m_blendType( MBT_OPAQUE ),
		m_lit( false ),
		m_rasterState( INVALID_HANDLE ),
		m_depthState( INVALID_HANDLE ),
		m_blendState( INVALID_HANDLE )
	{
	}
	
	void Material::Set( Vec4				a_diffuse,
						Vec4				a_specular,
						float				a_shininess,
						Texture*			a_texture,
						MaterialBlendType	a_blendType,
						bool				a_lit )
	{
		m_diffuse	= a_diffuse;
		m_specular	= a_specular;
		m_shininess = a_shininess;
		m_texture	= a_texture;
		m_blendType = a_blendType;
		m_lit		= a_lit;
	}
		
	// Serialization
	bool Material::Extract( Material&		a_material,  
							TexturePool&	a_texPool,
							Archive&		a_source,
							size_t&			a_offset )
	{
		TextureKey textureKey;
		
		Buffer& src = a_source.Storage();
		
		// Extract each component, moving through buffer		
		a_offset+= src.Read( &a_material.m_diffuse.m_v, sizeof( a_material.m_diffuse.m_v ), a_offset );
		a_offset+= src.Read( &a_material.m_specular.m_v, sizeof( a_material.m_specular.m_v ), a_offset );
		a_offset+= src.Read( a_material.m_shininess, a_offset );
		a_offset+= src.Read( textureKey, a_offset );
		a_offset+= src.Read( (uint32_t&) a_material.m_blendType, a_offset );
		a_offset+= src.Read( (uint8_t&) a_material.m_lit, a_offset );
		
		// Map material texture path to something we can use.
		a_material.m_texture = a_texPool.Add( textureKey );
		
		return true;
	}
	
	size_t Material::Store(	const Material&		a_material, 
							const TexturePool&	a_texPool,
							Archive&			a_destination )
	{
		std::string	texturePath = "<unknown>";
		
		// Find texture name and store that with material
		const TextureDictionary& textures = a_texPool.GetTextures();
		for ( TextureDictionary::const_iterator it = textures.begin();
				it != textures.end(); it++ )
		{
			if ( it->second == a_material.m_texture )
				texturePath = it->first;
		}
		
		Buffer& dst		= a_destination.Storage();
		size_t	written = 0;
		
		written += dst.Append( &a_material.m_diffuse.m_v, 
								sizeof( a_material.m_diffuse.m_v ) );
		written += dst.Append( &a_material.m_specular.m_v, 
								sizeof( a_material.m_specular.m_v ) );
		written += dst.Append( a_material.m_shininess );
		written += dst.Append( texturePath );
		written += dst.Append( (uint32_t) a_material.m_blendType );
		written += dst.Append( (uint8_t&) a_material.m_lit );
		
		return written;
	}
	
	void Material::Dump()
	{	
		Texture*  t = m_texture;
		
		LXT_LOG_SPAM(" Texture: %p\n", t );
		
		LXT_LOG_SPAM(" Diffuse: %f,%f,%f,%f\n", 
					 m_diffuse.m_v[0],
					 m_diffuse.m_v[1],
					 m_diffuse.m_v[2],
					 m_diffuse.m_v[3] );
		
		if ( m_lit == true )
		{
			LXT_LOG_SPAM(" Specular: %f,%f,%f,%f\n", 
						 m_specular.m_v[0],
						 m_specular.m_v[1],
						 m_specular.m_v[2],
						 m_specular.m_v[3] );
			LXT_LOG_SPAM(" Shininess: %f", m_shininess );
		}
		else 
		{
			LXT_LOG_SPAM(" Not lit\n" );
		}
		
		LXT_LOG_SPAM(" Blend Mode: %d\n", m_blendType);
	}	
}