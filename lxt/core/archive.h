/*
 *  archive.h
 *  test_runner
 *
 *  Created by Justin on 27/01/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_CORE_ARCHIVE_H__
#define __LXT_CORE_ARCHIVE_H__

#include <vector>
#include "file/file.h"
#include "core/buffer.h" // to define LxtArchive
#include "core/debug.h"

namespace Lxt
{	
	// The archive class handles appending and extracting types from storage.
	class Archive
	{
	public:
		typedef Buffer StorageType;
		
		Archive() {};
		~Archive() {};
		
		// Access storage
		StorageType& Storage() { return m_storage; }
		
		// Disk access
		bool Load( const Path& a_path );
		bool Save( const Path& a_path );
				
	public:
		StorageType	m_storage;
	};
}

#endif
