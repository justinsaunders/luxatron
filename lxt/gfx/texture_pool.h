/*
 *  texture_pool.h
 *  test_runner
 *
 *  Created by Justin on 16/03/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_GFX_TEXTURE_POOL_H__
#define __LXT_GFX_TEXTURE_POOL_H__

#include "file/file.h"
#include <map>
#include <vector>
#include "core/buffer.h"

namespace Lxt
{
	class Texture;
	class Archive;
	
	typedef	Path										TextureKey;
	typedef std::map< TextureKey, Texture* >			TextureDictionary;
	typedef std::vector< TextureKey >					TextureNames;
	typedef std::pair< TextureKey, File::TimeInterval > PathDate;
	
	class TexturePool
	{
	public:
		TexturePool()
		{
		}
		
		~TexturePool();
		
		// Add and Remove entries
		Texture* Add( const TextureKey& a_textureKey );
		void	 Replace( const TextureKey& a_textureKey, Texture* a_new );
		
		// Take a peek at what textures are in the pool
		const TextureDictionary& GetTextures() const { return m_textures; }
		
		// Get a list of texture names in the pool
		void GetTextureNames( TextureNames& a_texNames );
		
		// Serialize
		static bool Extract( TexturePool& a_texturePool, Archive& a_archive, 
							uint32_t a_offset );
		static bool Store( const TexturePool& a_texturePool, Archive& a_archive );
		
		// Update a pool given inputs and modification date. If append mode,
		// then unreferenced entries are not deleted.
		// Returns true if pool was modified.
		static bool Link(	const std::vector< PathDate >&	a_inputs,
							File::TimeInterval				a_poolDate,
							bool							a_doAppend,
							TexturePool&					a_pool );
		
	protected:
		TextureDictionary m_textures;
	};
}


#endif // __LXT_GFX_TEXTURE_POOL_H__