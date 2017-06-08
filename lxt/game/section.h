/*
 *  section.h
 *  Luxatron™
 *
 *  Created by Justin on 20/04/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_GAME_SECTION_H__
#define	__LXT_GAME_SECTION_H__

#include <string>
#include "core/types.h"

namespace Lxt 
{
	struct DeviceContext;
	class SectionController;
	
	// Helper object - when a task has a duration, this can be used to record
	// progress to display to the user. Once created, the task name and current
	// values can be modified. 
	struct ProgressInfo : private Noncopyable
	{
		ProgressInfo( std::string a_taskName, float a_start = 0.0f, 
					  float a_end = 1.0f );
		
		float		Start()	const { return m_start; }
		float		End()	const { return m_end; }
		
		float		m_current;
		std::string	m_taskName;
		
	private:
		float		m_start, m_end;
	};
	
	// A section object holds a period of a game. For example in chronological
	// order, a section can represent each of the follow game features:
	// A splash screen, a introduction movie, a front end menu, a game level,
	// a final score screen.
	class Section : private Noncopyable
	{
	public:
		Section( const std::string& a_name, SectionController& a_controller );
		virtual ~Section();
		
		// Entry state for section - loading can be done here, and function
		// will set IsReady to true when finished. 
        //
		// ProgressInfo object it will be updated if supplied
		virtual void Prepare( ProgressInfo* a_pi = NULL ) { m_isReady = true; }
		
        // Is section ready to play
        bool IsReady() { return m_isReady; }
        
		// Main state of section - will return false when done.
		virtual bool Play() = 0;
	
		// Render the section
		virtual void Draw() = 0;
		
		// Leave section, and start next section.
		virtual void Yield( const std::string& a_nextSectionName );
		
		// Name accessor
		const std::string& GetName() { return m_name; }
		
	protected:
		std::string			m_name;
		SectionController&	m_controller;
        bool                m_isReady;
	};
}


#endif // __LXT_GAME_SECTION_H__