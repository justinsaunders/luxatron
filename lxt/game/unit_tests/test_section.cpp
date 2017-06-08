/*
 *  test_section.cpp
 *  test_runner
 *
 *  Created by Justin on 20/04/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include <UnitTest++.h>
#include "game/section.h"

namespace 
{
	class MyGameSection : public Lxt::Section
	{
	public:
		virtual bool Starting( Lxt::ProgressInfo* a_pi )
		{
			return true;
		}
		
		virtual bool Playing( float deltaSeconds )
		{
			return true;
		}
		
		virtual void End()
		{
		}
	};
	
	struct Fixture 
	{
		// To be honest, I'm too bored to test this class.
	};
}