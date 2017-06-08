/*
 *  archive.cpp
 *  test_runner
 *
 *  Created by Justin on 27/01/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "archive.h"
#include "core/log.h"
#include <stdio.h>

namespace
{
	// First eight bytes of an archive file
	struct Header
	{
		char		m_id[4]; // LXTA - Luxatron Archive
		uint32_t	m_length;// How many bytes follow.
	};

	const char kArchiveId[4] = { 'L', 'X', 'T', 'A' };
}

namespace Lxt 
{	
	bool Archive::Load( const Path& a_path )
	{
		bool status = false;
		FILE* fp	= fopen( a_path.c_str(), "rb" );
	
		// Remove existing storage
		m_storage.Clear();
		
		if ( fp )
		{
			Header header;
			
			if ( sizeof(header) != fread( &header, 1, sizeof(header), fp ) )
			{
				LXT_LOG_ERROR( "Couldn't read archive header." );
			}
			else
			{
				if ( strncmp( kArchiveId, header.m_id, sizeof( header.m_id ) ) )
				{
					LXT_LOG_ERROR( "Archive header was not valid" );
				}
				else
				{
					// Allocate storage
					m_storage.Resize( header.m_length );
					
					if ( header.m_length != 
						fread( m_storage.NonConstData(), 1, header.m_length, fp ) )
					{
						LXT_LOG_ERROR( "Archive contents were truncated." );
					}
					
					status = true;
				}
			}
			
			fclose( fp );
		}
		
		return status;
	}
	
	bool Archive::Save( const Path& a_path )
	{
		bool status = false;
		FILE* fp	= fopen( a_path.c_str(), "wb" );
		
		if ( fp )
		{
			// create header
			Header header;
			strncpy( header.m_id, kArchiveId, sizeof( header.m_id ) );
			header.m_length = m_storage.Size();
						
			if ( sizeof(header) != fwrite( &header, 1, sizeof(header), fp ) )
			{
				LXT_LOG_ERROR( "Couldn't write archive header." );
			}
			else
			{
				if ( header.m_length != 
						fwrite( m_storage.Data(), 1, header.m_length, fp ) )
				{
					LXT_LOG_ERROR( "Couldn't write archive contents." );
				}
				else
				{
					status = true;
				}
			}
			
			fclose( fp );
		}

		return status;
	}
}