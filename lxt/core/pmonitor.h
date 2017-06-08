/*
 *  pmonitor.h
 *  
 *
 *  Created by Justin on 28/02/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_PMONITOR_H__
#define __LXT_PMONITOR_H__

#include "core.h"
#include "zone_stack.h"
#include "timer.h"

namespace Lxt 
{
	class PMonitor
	{
	public:
		// Create a PMonitor given a ZoneStack. PMonitor will call Enter/Leave
		// on its associated ZoneStack. Monitoring begins as soon as this is
		// created.
		PMonitor(	ZoneStack&	a_zs, 
					uint32_t	a_numFrames, 
					const char* a_outputFile );
		
		~PMonitor();
		
		void			EnterZone( ZoneId );
		void			LeaveZone( ZoneId );
		
		void			Update();
		
		// Get a sample from a given frame and zone
		uint64_t		GetSample( size_t a_frame, ZoneId a_zone ) const;
		
		// Get total time across all zones on a given frame
		uint64_t		GetFrameTotal( size_t a_frame ) const;
		
		// Get the current frame index
		size_t			GetLastFrame() const;
		
		// Get the filtered frames per second, call after Update(). Because
		// PMonitor is not a true circular buffer this will return 0 every
		// m_numFrames.
		float			GetFilteredFPS() const;
        
        // Get number of frames this monitor tracks
        size_t          GetNumFrames() const;
        
	private:
		uint64_t&		Sample( size_t a_frame, ZoneId a_zone );
		void			Accumulate( ZoneId a_zi );
		void			Flush();
		
		// Stack
		ZoneStack&		m_zoneStack;

		// History array - is number of zones * number of frames
		uint64_t*		m_history;
		const size_t	m_numFrames;
		const size_t	m_historySize;
		
		size_t			m_currentFrame;
		uint64_t		m_lastTickCount;
		
		// Output for history file
		FILE*			m_filePointer;
	};
	
	inline uint64_t PMonitor::GetSample( size_t a_frame, ZoneId a_zi ) const
	{
		const size_t offset = ZoneRegistry::GetZoneCount() * a_frame + a_zi;
		LXT_ASSERT( offset < m_historySize );
		return m_history[ offset ];		
	}
	
	inline uint64_t& PMonitor::Sample( size_t a_frame, ZoneId a_zi )
	{
		const size_t offset = ZoneRegistry::GetZoneCount() * a_frame + a_zi;
		LXT_ASSERT( offset < m_historySize );
		return m_history[ offset ];
	}
	
	inline size_t PMonitor::GetLastFrame() const
	{
		return m_currentFrame == 0 ? 0 : m_currentFrame - 1;
	}
	
    inline size_t PMonitor::GetNumFrames() const
    {
        return m_numFrames;
    }
    
	inline void PMonitor::Accumulate( ZoneId a_zi )
	{
		// Address correct sample in history
		uint64_t&	sample = Sample( m_currentFrame, a_zi );
		
		// Update sample with accumulated ticks
		uint64_t currentTicks	= GetSystemTickCount();
		sample					+= currentTicks - m_lastTickCount; 
		m_lastTickCount			= currentTicks;
	}
	
	inline void PMonitor::EnterZone( ZoneId a_zi )
	{
		Accumulate( m_zoneStack.CurrentZone() );
		m_zoneStack.EnterZone( a_zi );
	}
	
	inline void PMonitor::LeaveZone( ZoneId a_zi )
	{
		Accumulate( a_zi );
		m_zoneStack.LeaveZone( a_zi );
	}
	
	// A handly scoped zone wrapper
	class ScopedZone
	{
	public:
		ScopedZone( PMonitor* a_monitor, ZoneId a_zi) 
			: m_monitor( a_monitor ), m_zoneId( a_zi )
		{
			m_monitor->EnterZone( m_zoneId );
		}
		
		~ScopedZone()
		{
			m_monitor->LeaveZone( m_zoneId );
		}
		
	private:
		PMonitor*	m_monitor;
		ZoneId		m_zoneId;
	};
			
}

// Zone Macros
namespace Lxt
{
    extern PMonitor* g_pmonitor; // global pmonitor, should be per thread
}

// Register a zone
#define LXT_ZONE_REGISTER( name ) \
 Lxt::ZoneId LxtZone_##name = Lxt::ZoneRegistry::DeclareZone( #name );

// Declare a zone - it must be registered elsewhere
#define LXT_ZONE_DECLARE( name ) \
 extern Lxt::ZoneId LxtZone_##name;

// Enter a zone.
#define LXT_ZONE_ENTER( name ) \
 extern Lxt::ZoneId LxtZone_##name; \
 g_pmonitor->EnterZone( LxtZone_##name );
 
// Leave a zone
#define LXT_ZONE_LEAVE( name ) \
 extern Lxt::ZoneId LxtZone_##name; \
 g_pmonitor->LeaveZone( LxtZone_##name );

// Auto zone
#define LXT_ZONE_AUTO( name ) \
 extern Lxt::ZoneId LxtZone_##name; \
 Lxt::ScopedZone LxtScoped_##name( Lxt::g_pmonitor, LxtZone_##name );

#endif // __LXT_PMONITOR_H__
