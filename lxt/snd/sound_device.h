/*
 *  sound_device.h
 *  starkiller
 *
 *  Created by Justin on 25/05/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_SOUND_DEVICE_H__
#define __LXT_SOUND_DEVICE_H__


namespace Lxt 
{
	class SoundDevice
	{
	public:
		bool Initialise();
		void Finish();
		
		// Global settings
		void SetGlobalVolume( float a_volume );
		void Listener_SetPosition( float a_x, float a_y, float a_z );
		void StopAllEffects();
		
		// Effects
		Handle Effect_Initialise( const Path& a_effectFilename );
		bool Effect_SetVolume( Handle a_effectHandle, float a_volume );
		bool Effect_SetPosition( Handle a_effectHandle, float a_x, float a_y, float a_z );
		bool Effect_Start( Handle a_effectHandle, bool a_loop = false );
		bool Effect_Stop( Handle a_effectHandle );
		bool Effect_IsPlaying( Handle a_effectHandle );
		
	private:
		struct SoundDeviceImp* m_imp;
	};
}


#endif // __LXT_SOUND_DEVICE_H__