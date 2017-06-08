/*
 *  texture_pool.cpp
 *  test_runner
 *
 *  Created by Justin on 16/03/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#include "core/core.h"
#include "texture_pool.h"

#include "texture.h"

namespace Lxt 
{
	TexturePool::~TexturePool()
	{
		// Delete textures.		
		TextureDictionary::iterator it = m_textures.begin();
		
		for ( ; it != m_textures.end(); it++ )
		{
			delete it->second;
		}
	}
	
	Texture* TexturePool::Add( const TextureKey& a_textureKey )
	{
		if ( a_textureKey.empty() )
			return NULL;
		
		TextureDictionary::iterator it = m_textures.find( a_textureKey );
		Texture*					rv = NULL;

		if (  it == m_textures.end() )
		{
			// create new texture
			rv = new Texture();

#if LXT_PLATFORM_CURRENT == LXT_PLATFORM_IPHONE
			// TextureLoad uses bundle path, only need filename part.
			Path appPath; File::GetApplicationPath( appPath );
			Path texturePath = appPath + Path_GetFilename( a_textureKey );
			if ( TextureLoad( texturePath.c_str(), *rv ) )
			{
				m_textures[ a_textureKey ] = rv;
			}
			else
			{
				LXT_LOG_SPAM("Failed to add '%s' to texture pool.\n", texturePath.c_str() );
				
				delete rv;
				rv = NULL;
			}
#else
			m_textures[ a_textureKey ] = rv;
#endif
		}
		else
		{
			// return existing
			rv = it->second;
		}
		
		return rv;
	}
	
	void TexturePool::Replace( const TextureKey& a_textureKey, Texture* a_new )
	{
		TextureDictionary::iterator it = m_textures.find( a_textureKey );
		
		if (  it != m_textures.end() )
		{
			delete it->second;
			it->second = a_new;
		}
	}

	void TexturePool::GetTextureNames( std::vector< std::string >& a_texNames )
	{
		TextureDictionary::iterator it = m_textures.begin();
		
		for ( ; it != m_textures.end(); it++ )
		{
			a_texNames.push_back( it->first );
		}
	}
		
	bool TexturePool::Extract(	TexturePool&	a_texturePool, 
								Archive&		a_archive, 
								uint32_t		a_offset )
	{
		Buffer& storage = a_archive.Storage();
		
		uint32_t numTextures = 0;
		a_offset += storage.Read( numTextures, a_offset );
		
		for ( size_t i = 0; i < numTextures; i++ )
		{
			// Read key and buffer
			TextureKey key;
			a_offset += storage.Read( key, a_offset );
			
			Buffer buffer;
			a_offset += storage.Read( buffer, a_offset );
			
			// Insert new texture into map
			a_texturePool.m_textures[ key ] = new Texture(/*buffer*/);
		}
		
		return true;
	}
	
	bool TexturePool::Store(const TexturePool&	a_texturePool, 
							Archive&			a_archive )
	{
		Buffer&						storage		= a_archive.Storage();
		const TextureDictionary&	textures	= a_texturePool.m_textures;
		
		uint32_t numTextures = a_texturePool.m_textures.size();
		storage.Append( numTextures );
		
		for ( TextureDictionary::const_iterator it = textures.begin();
			 it != textures.end(); it++ )
		{
			// store key
			storage.Append( it->first );
			
			// store texture file
			Buffer file;
			// TODO actually load file into buffer
			storage.Append( file ); 
		}
		
		return true;
	}
	
	bool TexturePool::Link(	const std::vector< PathDate >&	a_inputs,
							File::TimeInterval				a_poolDate,
							bool							a_doAppend,
							TexturePool&					a_pool )
	{
		bool				updated	= false;
		TextureDictionary&	textures= a_pool.m_textures;
		
		// Remove any unreferenced textures if we are not in append mode.
		if ( !a_doAppend )
		{
			for ( TextureDictionary::iterator ti = textures.begin();
				 ti != textures.end(); ti++ )
			{
				bool found = false;
				
				for ( uint32_t i = 0; i < a_inputs.size(); i++ )
				{
					if ( ti->first == a_inputs[i].first )
					{
						found = true;
						break;
					}
				}
				
				if ( !found )
				{
					LXT_LOG_SPAM("Removing '%s', no longer referenced.\n",
								 ti->first.c_str() );
					textures.erase( ti ); // ti is invalid
					updated = true;
				}
			}
		}
		
		// Check that each input exists
		for ( size_t i = 0; i < a_inputs.size(); i++ )
		{
			// See if file exists
			TextureDictionary::const_iterator ti = 
											textures.find( a_inputs[i].first );
			
			if ( ti != textures.end() )
			{
				File::TimeInterval inputDate = a_inputs[i].second;
				
				// Yes, and file is newer than pool date, so replace.
				if ( inputDate > a_poolDate )
				{
					LXT_LOG_SPAM("Replacing '%s', source is newer.\n", 
								 a_inputs[i].first.c_str());
					// Replace with a NULL texture, this will be ignored when the
					// texturepool is stored because the "key" is re-loaded.
					a_pool.Replace( a_inputs[i].first, NULL );
					updated = true;
				}
			}
			else
			{
				LXT_LOG_SPAM("Adding '%s'.\n", a_inputs[i].first.c_str() );
				// No, its a new texture so add it.
				a_pool.Add( a_inputs[i].first );
				updated = true;
			}
		}
		
		return updated;
	}	
	
} // namespace Lxt