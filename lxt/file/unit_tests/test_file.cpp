/*
 *  test_file.cpp
 *  test_runner
 *
 *  Created by Justin on 1/04/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#include <UnitTest++.h>

#include "file/file.h"

namespace Lxt
{
	TEST(File_Misc)
	{
		Path temp, file1, file2;
		File::GetUserTempPath( temp );
		file1 = temp + "test_file1";
		file2 = temp + "test_file2";
		
		CHECK( !temp.empty() );
		CHECK( File::IsFolder( temp ) );
		CHECK( File::Exists( temp ) );
		CHECK( !File::Exists( "wang" ) );
							 
		FILE* fp1 = fopen( file1.c_str(), "w" );
		sleep(1);
		FILE* fp2 = fopen( file2.c_str(), "w" );
		fclose( fp2 );
		fclose( fp1 ); 
		
		double i1 = File::GetModificationInterval( file1 );
		double i2=  File::GetModificationInterval( file2 );
		
		// fp2 is created, last should be older
		CHECK( i2 > i1 );
	}
}