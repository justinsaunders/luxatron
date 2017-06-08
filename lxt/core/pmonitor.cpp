/*
 *  pmonitor.cpp
 *  test_runner
 *
 *  Created by Justin on 28/02/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "pmonitor.h"

namespace Lxt 
{
	PMonitor::PMonitor(	ZoneStack&	a_zs, 
						uint32_t	a_numFrames, 
						const char* a_outputFile )
		:	m_zoneStack( a_zs ),
			m_history( NULL ),
			m_numFrames( a_numFrames ),
			m_historySize( a_numFrames * ZoneRegistry::GetZoneCount() ),
			m_currentFrame( 0 ),
			m_lastTickCount( GetSystemTickCount() ),
			m_filePointer( NULL )
	{
		// Create history buffer
		m_history = new uint64_t[ m_historySize ];
		
		// Clear history
		for ( size_t i = 0 ; i < m_historySize; i++ )
		{
			m_history[ i ] = 0;
		}
		
		// If required, create a history file, erasing any previous
		if ( a_outputFile )
		{
			m_filePointer = fopen( a_outputFile, "w" );
		}
	}
	
	PMonitor::~PMonitor()
	{
		if ( m_filePointer ) 
			Flush();
		
		delete[] m_history;
	}
	
	void PMonitor::Update()
	{
		// We must have finished all user zones when PMonitor is updated
		LXT_ASSERT(m_zoneStack.CurrentZone() == ZR_DEFAULT_ZONE_ID);
		
		// Add time to default section
		Accumulate(ZR_DEFAULT_ZONE_ID);
		
		// Next frame
		m_currentFrame++;
			
		// Flush history and return to start of buffer if necessary.
		if (m_currentFrame == m_numFrames)
		{
			if (m_filePointer) 
				Flush();
			
			m_currentFrame = 0;
		}
        
        // clear old amount in all sections
        for (size_t i = 0; i < ZoneRegistry::GetZoneCount(); i++)
        {
            Sample(m_currentFrame, i) = 0;
        }
        
        // Hide update time so we don't accumulate into unaccounted section
        m_lastTickCount = GetSystemTickCount();
	}
	
	uint64_t PMonitor::GetFrameTotal( size_t a_frame ) const
	{
		LXT_ASSERT( a_frame < m_numFrames );
		
		uint64_t total = 0;
		for ( size_t i = 0; i < ZoneRegistry::GetZoneCount(); i++ )
		{
			total += GetSample( a_frame, i);
		}
		
		return total;
	}
	
	float PMonitor::GetFilteredFPS() const
	{
		size_t		frame	= GetLastFrame();
		uint64_t	tc		= GetFrameTotal( frame );	
		float		fps		= 0.0f;
		
		if ( tc > 0 )
		{
			fps = 1.0f / ( tc * GetSystemTickPeriod() );
		}
		
		static float	ffps = fps;
		
		ffps = ( fps * 0.1f ) + ( ffps * 0.9f );
		
		return ffps;
	}
	
	void PMonitor::Flush()
	{
		// not implemented yet
	}
}