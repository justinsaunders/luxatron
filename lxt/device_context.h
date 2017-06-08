/*
 *  device_context.h
 *  starkiller
 *
 *  Created by Justin on 23/05/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_GAME_DEVICE_CONTEXT_H__
#define __LXT_GAME_DEVICE_CONTEXT_H__

namespace Lxt 
{
	class	VPadDevice;
	struct	TiltDevice;
	class	Renderer;
	class	SoundDevice;
	
	// Global information about the hardware device
	struct DeviceContext
	{
		VPadDevice*		m_vpadDevice;	// pointer to joypad device
		TiltDevice*		m_tiltDevice;	// pointer to device acc values
		Renderer*		m_renderer;		// pointer to a renderer
		SoundDevice*	m_soundDevice;  // a sound device
	};	
}

#endif // __LXT_GAME_DEVICE_INFO_H__
