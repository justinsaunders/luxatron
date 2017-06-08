/*
 *  test_path.cpp
 *  mac_test_runner
 *
 *  Created by Justin on 5/04/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#include <UnitTest++.h>
#include "file/path.h"

namespace Lxt 
{
	TEST( Test_Path_GetExtension )
	{
		Path no_extension		= "file";
		Path empty_extension	= "file.";
		Path single_extension	= "file.a";
		Path double_extension	= "file.a.b";
		Path real_file			= 
		"/Users/Justin/Projects/msg/src/prototypes/model/build/Debug-iphoneos/Quack!.app/duck_triangulate.lxtm.ref";
		
		CHECK_EQUAL( "", Path_GetExtension( no_extension ) );
		CHECK_EQUAL( ".", Path_GetExtension( empty_extension ) );
		CHECK_EQUAL( ".a", Path_GetExtension( single_extension ) );
		CHECK_EQUAL( ".b", Path_GetExtension( double_extension ) );
		CHECK_EQUAL( ".ref", Path_GetExtension( real_file ) );
	}
	
	TEST( Test_Path_GetFilename )
	{
		Path no_filename		= "";
		Path simple_filename	= "file";
		Path simple_filename2	= "/file";
		Path folder_filename	= "folder/file";
		Path real_file			= 
		"/Users/Justin/Projects/msg/src/prototypes/model/build/Debug-iphoneos/Quack!.app/duck_triangulate.lxtm.ref";
		
		CHECK_EQUAL( "",		Path_GetFilename( no_filename ) );
		CHECK_EQUAL( "file",	Path_GetFilename( simple_filename ) );
		CHECK_EQUAL( "file",	Path_GetFilename( simple_filename2 ) );
		CHECK_EQUAL( "file",	Path_GetFilename( folder_filename ) );
		CHECK_EQUAL( "duck_triangulate.lxtm.ref",	Path_GetFilename( real_file ) );
	}
}
