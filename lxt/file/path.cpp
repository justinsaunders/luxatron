/*
 *  path.cpp
 *  build_texpool
 *
 *  Created by Justin on 4/04/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "path.h"

namespace Lxt 
{
	std::string Path_GetExtension( const Path& a_path )
	{
		std::string				extension;
		std::string::size_type	i = a_path.find_last_of('.');
		
		if ( i != std::string::npos )
		{
			extension = a_path.substr( i, a_path.size() );
		}
		
		return extension;
	};
	
	std::string Path_GetFilename( const Path& a_path )
	{
		// get file name, starting from one after last slash
		// of beginning of string if no slash.
		size_t sp = a_path.find_last_of('/');
		sp = ( sp == Path::npos ) ? 0 : sp+1;
		
		// get path name
		Path fileName = a_path.substr( sp, a_path.size() );
		
		return fileName;
	}
}