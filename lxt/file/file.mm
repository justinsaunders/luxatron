//
//  file.mm
//  Luxatron™
//
//  Created by Justin on 15/09/08.
//  Copyright 2008 Monkey Style Games. All rights reserved.
//

#include  "file.h"

#include "core/array.h"

namespace Lxt
{
	void File::GetApplicationPath( Path& a_applicationPath)
	{
		NSString* nsPath = [[NSBundle mainBundle] resourcePath];
		
		// fill C buffer
		char buffer[512];
		[nsPath getCString:buffer 
				maxLength:LXT_ARRAY_LEN(buffer)
				encoding:NSASCIIStringEncoding];
		
		// Copy to path
		a_applicationPath = buffer;
		a_applicationPath += "/";		
	}
	
	void File::GetUserHomePath( Path& a_userHomePath )
	{
		NSString* nsPath = NSHomeDirectory();
		
		// fill C buffer
		char buffer[512];
		[nsPath getCString:buffer 
				 maxLength:LXT_ARRAY_LEN(buffer)
				  encoding:NSASCIIStringEncoding];
		
		// Copy to path
		a_userHomePath = buffer;
		a_userHomePath += "/";	
	}
	
	void File::GetUserTempPath( Path& a_userTempPath )
	{
		NSString* nsPath = NSTemporaryDirectory();
		
		// fill C buffer
		char buffer[512];
		[nsPath getCString:buffer 
				 maxLength:LXT_ARRAY_LEN(buffer)
				  encoding:NSASCIIStringEncoding];
		
		// Copy to path
		a_userTempPath = buffer;
		a_userTempPath += "/";	
	}
		
	bool File::IsFolder( const Path& a_path )
	{
		bool status = false;
		
		NSFileManager *fileManager = [NSFileManager defaultManager];
		NSString* nsPath = [NSString stringWithUTF8String:a_path.c_str()];
		
		BOOL isDir;
		if ( [fileManager fileExistsAtPath:nsPath isDirectory:&isDir] )
		{															
			status = isDir;
		}
			
		return status;
	}
		
	bool File::Exists( const Path& a_path )
	{
		bool status = false;

		NSFileManager *fileManager = [NSFileManager defaultManager];
		NSString* nsPath = [NSString stringWithUTF8String:a_path.c_str()];
		
		BOOL isDir;
		if ( [fileManager fileExistsAtPath:nsPath isDirectory:&isDir] )
		{
			status = true;
		}
				
		return status;
	}
	
	
	File::TimeInterval File::GetModificationInterval( const Path& a_path )
	{
		// Note this function leaks, see https://monkeystyle.fogbugz.com/default.asp?pgx=EV&ixBug=43
		NSFileManager *fileManager = [NSFileManager defaultManager];
		NSString* path = [NSString stringWithUTF8String:a_path.c_str()];
	
		NSDictionary *fileAttributes = 
					[fileManager fileAttributesAtPath:path traverseLink:YES];
		NSDate* fileModDate = [fileAttributes objectForKey:NSFileModificationDate];
	
		File::TimeInterval interval = [fileModDate timeIntervalSinceReferenceDate];
		
		return interval;
	}	

}