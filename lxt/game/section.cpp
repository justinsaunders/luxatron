/*
 *  section.cpp
 *  test_runner
 *
 *  Created by Justin on 20/04/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "section.h"
#include "section_controller.h"

namespace Lxt
{
	ProgressInfo::ProgressInfo( std::string a_taskName, float a_start, 
								float a_end )
		:	m_taskName( a_taskName ),
			m_start( a_start ), 
			m_end( a_end ), 
			m_current( 0.0f )
	{
	}

	Section::Section( const std::string& a_name, SectionController& a_controller )
		:	m_name( a_name ),
			m_controller( a_controller ),
            m_isReady(false)
	{
	}
	
	/*virtual*/ void Section::Yield( const std::string& a_nextSectionName ) 
	{
		bool ok = m_controller.SetNextSection( a_nextSectionName );
		LXT_ASSERT( ok && "Couldn't find destination section" );
	}
	
	Section::~Section()
	{
	}
}