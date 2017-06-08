/*
 *  actor.cpp
 *  model
 *
 *  Created by Justin on 28/04/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "actor.h"

#include "gfx/model.h"
#include "gfx/renderer.h"

namespace Lxt
{
	Actor::Actor()
		: m_world( Mat4::Identity )
	{}
	
	Actor::~Actor()
	{
	}
}
