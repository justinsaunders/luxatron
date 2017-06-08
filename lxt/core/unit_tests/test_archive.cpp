/*
 *  test_archive.cpp
 *  test_runner
 *
 *  Created by Justin on 27/01/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#include <UnitTest++.h>
#include "core/archive.h"
#include "core/buffer.h"

namespace 
{
	struct Fixture
	{
		Lxt::Archive	  archive;
		std::vector<int>  v;
		Lxt::Buffer		  b;
		
		Fixture()
		{
			v.push_back( -1 );
			v.push_back( 0 );
			v.push_back( 65530 );
			
			b.Append( &v[0], v.size() * sizeof(int) );
		}
	};
};

namespace Lxt
{
	TEST_FIXTURE( Fixture, Archive_Construct )
	{
	}
		
	TEST( Archive_Save_Load_Empty )
	{
		Archive emptyArchive;
		Archive loadedArchive;
		
		Path tempPath;
		File::GetUserTempPath( tempPath );
		tempPath += "/empty_archive.lxta";
		
		CHECK( emptyArchive.Save( tempPath ) );
		CHECK( loadedArchive.Load( tempPath ) );
	}
	
	TEST_FIXTURE( Fixture, Archive_Save_Load_Simple )
	{
		Archive				simpleArchive;
		Archive				loadedArchive;
		std::vector<int>	loadedVector;
		
		Path tempPath;
		File::GetUserTempPath( tempPath );
		tempPath += "/simple_archive.lxta";
		
		simpleArchive.Storage().Append( v );
		
		CHECK( simpleArchive.Save( tempPath ) );
		CHECK( loadedArchive.Load( tempPath ) );
		
		loadedArchive.Storage().Read( loadedVector, 0 );
		
		CHECK( loadedVector.size() == v.size() );
		for ( size_t i = 0; i < v.size(); i++ )
		{
			CHECK_EQUAL( v[0], loadedVector[0] );
		}
	}
}