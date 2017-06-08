/*
 *  path.h
 *
 *  A class to represent a path on the file system, will be optimised for 
 *	storage and component comparison (eg path, file, extension).
 *
 *  Created by Justin on 4/04/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_FILE_PATH_H__
#define __LXT_FILE_PATH_H__

#include <string>

namespace Lxt
{
	typedef std::string Path;
	
	std::string Path_GetExtension( const Path& a_path );
	std::string Path_GetFilename( const Path& a_path );
};

#endif // __LXT_FILE_PATH_H__