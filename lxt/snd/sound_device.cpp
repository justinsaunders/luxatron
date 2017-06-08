/*
 *  sound_devicer.cpp
 *  starkiller
 *
 *  Created by Justin on 25/05/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#include "core/core.h"
#include "sound_device.h"

#include "OpenAL/al.h"
#include "OpenAL/alc.h"
#include "MyOpenALSupport.h"

#include "math/vec3.h"

using namespace Lxt;

namespace Lxt 
{
	typedef ALuint EffectInstance;
	
	struct EffectBuffer
	{
		ALuint	m_id;
		void*	m_data;
	};
	
	struct SoundDeviceImp
	{
		SoundDeviceImp()
		{
			m_listenerPosition.m_x	= 0.0f;
			m_listenerPosition.m_y  = 0.0f;
			m_listenerPosition.m_z	= 0.0f;
			m_globalVolume			= 1.0f;
			m_numEffectInstances	= 0;
		}
		
		// Listener position and global volume for device
		Vec3						m_listenerPosition;
		float						m_globalVolume;
		
		// Map of all loaded buffers
		typedef std::map< Path, EffectBuffer >	BufferMap;
		BufferMap								m_buffers;
		
		// Array of currently initialised effects
		Array< EffectInstance, 256 >	m_effectInstances;
		size_t							m_numEffectInstances;
	};
}

bool InitBuffer( const char* a_bufferFile, EffectBuffer& a_buffer )
{
	ALenum  error = AL_NO_ERROR;
	
	// Create some OpenAL Buffer Objects
	alGenBuffers(1, &a_buffer.m_id );
	if((error = alGetError()) != AL_NO_ERROR) {
		LXT_LOG_ERROR("Error Generating Buffers: %x", error);
		exit(1);
	}
	
	ALenum  format;
	ALsizei size;
	ALsizei freq;
	
	a_buffer.m_data = MyGetOpenALAudioData( a_bufferFile, &size, &format, &freq);
	if( a_buffer.m_data == NULL )
	{
		LXT_LOG_ERROR("error loading sound: %x\n", error);
		return false;
	}
		
	// use the static buffer data API
	alBufferDataStaticProc( a_buffer.m_id, format, a_buffer.m_data, size, freq);
		
	if((error = alGetError()) != AL_NO_ERROR) 
	{
		LXT_LOG_ERROR("error attaching audio to buffer: %x\n", error);
		free( a_buffer.m_data );
		a_buffer.m_data = NULL;
		return false;
	}
	
	return true;
}

// Create a source given a buffer
ALuint InitSource( ALuint bufferId )
{
	ALenum error = AL_NO_ERROR;
	ALuint sourceId;
	
	// Create some OpenAL Source Objects
	alGenSources( 1, &sourceId );
	if(alGetError() != AL_NO_ERROR) 
	{
		LXT_LOG_ERROR("Error generating sources! %x\n", error);
		exit(1);
	}
	
	alGetError(); // Clear the error
    
	alSourcei( sourceId, AL_LOOPING, AL_FALSE );
	
	// Set Source Position
	float sourcePosAL[] = { 0.0f, 0.0f, 0.0f};
	alSourcefv( sourceId, AL_POSITION, sourcePosAL);
	
	// Set Source Reference Distance
	alSourcef( sourceId, AL_REFERENCE_DISTANCE, 100.0f);
	
	// attach OpenAL Buffer to OpenAL Source
	alSourcei( sourceId, AL_BUFFER, bufferId );
	
	if((error = alGetError()) != AL_NO_ERROR) 
	{
		printf("Error attaching buffer to source: %x\n", error);
		exit(1);
	}
	
	return sourceId;
}

bool SoundDevice::Initialise()
{
	ALCcontext*	newContext	= NULL;
	ALCdevice*	newDevice	= NULL;
	
	m_imp					= NULL;
	
	// Create a new OpenAL Device
	// Pass NULL to specify the system’s default output device
	newDevice = alcOpenDevice(NULL);
	if (newDevice != NULL)
	{
		// Create a new OpenAL Context
		// The new context will render to the OpenAL Device just created 
		newContext = alcCreateContext(newDevice, 0);
		if (newContext != NULL)
		{
			// Make the new context the Current OpenAL Context
			alcMakeContextCurrent(newContext);
			LXT_ASSERT( alGetError() == AL_NO_ERROR );
			
			// Create implementation
			m_imp = new Lxt::SoundDeviceImp;
		}
	}
	
	return m_imp != NULL;
}

void SoundDevice::Finish()
{
	ALCcontext	*context = NULL;
    ALCdevice	*device = NULL;
	
	// Delete the Sources
	for ( uint32_t i = 0; i < m_imp->m_numEffectInstances; i++ )
	{
		alDeleteSources(1, &m_imp->m_effectInstances[i] );		
	}
	
	for ( SoundDeviceImp::BufferMap::iterator b = m_imp->m_buffers.begin(); 
		 b != m_imp->m_buffers.end(); b++ )
	{		
		// Delete the Buffers
		alDeleteBuffers(1, &(b->second.m_id) );
		
		// Free the data
		free( b->second.m_data );
	}
	
	delete m_imp;
	
	//Get active context (there can only be one)
    context = alcGetCurrentContext();
    //Get device for active context
    device = alcGetContextsDevice(context);
    //Release context
    alcDestroyContext(context);
    //Close device
    alcCloseDevice(device);
}

void SoundDevice::Listener_SetPosition( float a_x, float a_y, float a_z )
{
	float position[3] = { a_x, a_y, a_z };
	alListenerfv( AL_POSITION, position );	
}

void SoundDevice::StopAllEffects()
{
	for ( Handle i = 0; i < m_imp->m_numEffectInstances; i++ )
	{
		if ( Effect_IsPlaying( i ) )
		{
			Effect_Stop( i );
		}
	}
}

Lxt::Handle SoundDevice::Effect_Initialise( const Path& a_effectFilename )
{
	SoundDeviceImp::BufferMap::iterator b = 
						m_imp->m_buffers.find( a_effectFilename );
	ALuint bufferId;
	
	if ( b == m_imp->m_buffers.end() )
	{
		// Create AL buffer
		EffectBuffer eb = { 0, NULL };
		
		 if ( InitBuffer( a_effectFilename.c_str(), eb ) )
		 {
			 // Insert entry
			 m_imp->m_buffers.insert( std::make_pair( a_effectFilename, eb ) );
			 bufferId = eb.m_id;
		 }
	}
	else
	{
		// Used cached entry
		bufferId = b->second.m_id;
	}
	
	// Create new instance
	EffectInstance& newEffect = m_imp->m_effectInstances[ m_imp->m_numEffectInstances ];
	
	newEffect = InitSource( bufferId );
	
	return m_imp->m_numEffectInstances++;
}

bool SoundDevice::Effect_SetVolume( Handle a_effectHandle, float a_volume )
{
	ALenum error;
	
	ALuint source = m_imp->m_effectInstances[a_effectHandle];
	
	// Begin playing our source file
	alSourcef( source, AL_GAIN, a_volume );
	
	if((error = alGetError()) != AL_NO_ERROR)
	{
		LXT_LOG_ERROR("error setting volume on source: %x\n", error);
		return false;
	}
	
	return true;	
}

bool SoundDevice::Effect_SetPosition( Handle a_effectHandle, float a_x,
									  float a_y, float a_z )
{
	ALuint source = m_imp->m_effectInstances[a_effectHandle];
	float sourcePosAL[] = { a_x, a_y, a_z };
	alSourcefv( source, AL_POSITION, sourcePosAL);
	
	ALenum error;
	if((error = alGetError()) != AL_NO_ERROR)
	{
		LXT_LOG_ERROR("error starting source: %x\n", error);
		return false;
	}
	
	return true;
}

bool SoundDevice::Effect_Start( Handle a_effectHandle, bool a_looping )
{
	ALenum error;
	
	ALuint source = m_imp->m_effectInstances[a_effectHandle];
	
	// Begin playing our source file
	alSourcei( source, AL_LOOPING, a_looping ? AL_TRUE : AL_FALSE );
	alSourcePlay( source );
	
	if((error = alGetError()) != AL_NO_ERROR)
	{
		LXT_LOG_ERROR("error starting source: %x\n", error);
		return false;
	}
	
	return true;
}

bool SoundDevice::Effect_Stop( Handle a_effectHandle )
{
	ALenum error;
	
    if (!Effect_IsPlaying(a_effectHandle)) return true;
    
	// Stop playing our source file
	alSourceStop( m_imp->m_effectInstances[a_effectHandle] );

	if((error = alGetError()) != AL_NO_ERROR)
	{
		LXT_LOG_ERROR("error stopping source: %x\n", error);
		return false;
	}
	
	return true;
}

bool SoundDevice::Effect_IsPlaying( Handle a_effectHandle )
{
	ALenum state = 0;
	alGetSourcei( m_imp->m_effectInstances[a_effectHandle], 
				 AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}	
