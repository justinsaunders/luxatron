//
//  file.h
//  Luxatron™
//
//  Created by Justin on 15/09/08.
//  Copyright 2008 Monkey Style Games. All rights reserved.
//

#ifndef __LXT_FILE_FILE_H__
#define __LXT_FILE_FILE_H__

#include <string>
#include "path.h"

namespace Lxt
{
	namespace File
	{
		typedef double		TimeInterval; // seconds on MacOS
		
		// Returns the full path to this application (including .app on MacOS 
		// systems). 
		void GetApplicationPath( Path& a_applicationPath );
		
		// Returns the full path to the user's home directory.
		void GetUserHomePath( Path& a_userHomePath );
	
		// Returns the full path to the user's temporary directory.
		void GetUserTempPath( Path& a_userTempPath );
		
		// Returns true if given path represents a folder. Will return false
		// if path does not exist.
		bool IsFolder( const Path& a_path );
		
		// Returns true if given path exists (it may be a folder or a file)
		bool Exists( const Path& a_path );
		
		// Returns modification interval of given file
		TimeInterval GetModificationInterval( const Path& a_path );
	}
	
}

#endif // __LXT_FILE_FILE_H__

