/*
 *  lxt.cpp
 *  test_runner
 *
 *  Created by Justin on 1/03/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "lxt.h"

#include "core/pmonitor.h"

#include "ctrl/vpad.h"
#include "ctrl/vpad_device.h"
#include "ctrl/tilt_device.h"
#include "gfx/renderer.h"
#include "snd/sound_device.h"
#include "device_context.h"

namespace
{
    float s_simulationStep = 1.f/30.f;
}

namespace Lxt 
{
	// Global zone stack, this should be per thread, but LXT is single threaded
	// at the moment.
	ZoneStack*		g_zoneStack	= NULL;
	PMonitor*		g_pmonitor	= NULL;
	DeviceContext	g_DC;
	
	bool Startup()
	{
		g_zoneStack = new ZoneStack;
		g_pmonitor = new PMonitor( *g_zoneStack, 256, NULL);
		
		// Initialise VPad/Tilt device
		VPadDevice* vpadDevice = new VPadDevice;
		TiltDevice* tiltDevice = new TiltDevice;
		
		// Initialise rende00rer 
		Renderer* renderer = new Renderer;
		renderer->Create( 320, 480 ); // landscape mode
		
		// Initialise sound
		SoundDevice* soundDevice = new SoundDevice;
		soundDevice->Initialise();
		
		g_DC.m_vpadDevice	= vpadDevice;
		g_DC.m_tiltDevice	= tiltDevice;
		g_DC.m_renderer		= renderer;
		g_DC.m_soundDevice	= soundDevice;
		
		return true;
	}
	
	void Shutdown()	
	{
		g_DC.m_soundDevice->Finish();
		delete g_DC.m_soundDevice;
		delete g_DC.m_renderer; 
		delete g_DC.m_tiltDevice; 
		delete g_DC.m_vpadDevice;
		
		delete g_pmonitor; 
		g_pmonitor = NULL;
		
		delete g_zoneStack;
		g_zoneStack = NULL;
	}
	
	DeviceContext& GetDC()
	{
		return g_DC;
	}
    
    float GetSimulationStep()
    {
        return s_simulationStep;
    }
    
    void SetSimulationStep(float a_seconds)
    {
        s_simulationStep = a_seconds;
    }
}