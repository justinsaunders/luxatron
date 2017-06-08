/*
 *  lxt.h
 *  Entry point for Luxatron library
 *
 *  Created by Justin on 1/03/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_LXT_H__
#define __LXT_LXT_H__

#include "core/core.h"
#include "device_context.h"

namespace Lxt 
{	
	// Startup and shutdown Luxatron
	bool Startup();
    
	void Shutdown();
	
    // Return a context for all hardware devices
	DeviceContext& GetDC();
    
    // Return the time for game simulation frames
    float GetSimulationStep();
    void SetSimulationStep(float a_step);
}

#endif // __LXT_LXT_H__