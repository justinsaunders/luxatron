/*
 *  test_colour.cpp
 *  test_runner
 *
 *  Created by Justin on 6/05/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#include <UnitTest++.h>
#include "gfx/colour.h"

namespace Lxt 
{
	TEST( Colour_Byte_Float_Conversion )
	{
		Colour	c = Colour(0x80808080);
		Vec4	vc= Colour::GetRGBA( c );
		
		CHECK( Equal( 0.5f, vc.m_r, 0.01f ) );
	}
	
	TEST( Colour_Float_Byte_Conversion )
	{
		Colour c = Colour(Vec4(0.5f, 0.5f, 0.5f, 0.5f)); 
		
		CHECK_EQUAL( uint32_t( 0x80 ), uint32_t( c.m_r ) );
	}

	TEST( Colour_MakeRGBA )
	{
		Colour c = Colour(Vec4(1.0f, 0.0f, 0.0f, 0.0f)); 
		CHECK_EQUAL( uint32_t( 0x000000FF ), c.m_abgr );
		
		c = Colour(Vec4( 0.0f, 1.0f, 1.0f, 1.0f) ); 
		CHECK_EQUAL( uint32_t( 0xFFFFFF00 ), c.m_abgr );
	}
	
	TEST( Colour_Lerp )
	{
		Colour black = { 0x00000000 };
		Colour white = { 0xFFFFFFFF };
		Colour grey	 = Lerp( black, white, 0.5 );
		
		CHECK_EQUAL( uint32_t( 0x80808080 ), grey.m_abgr );
	}
}

