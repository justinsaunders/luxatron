/*
 *  section_controller.cpp
 *  starkiller
 *
 *  Created by Justin on 23/05/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#include "section_controller.h"

#include "device_context.h"
#include "section.h"

using namespace Lxt;

SectionController::SectionController()
	:	m_currentSection( (size_t)-1 ),
		m_nextSection( (size_t)-1 )
{
}

SectionController::~SectionController()
{
	Finish();
}

void SectionController::Add( Section* a_section )
{
	m_sections.push_back( a_section );
}
	
bool SectionController::SetNextSection( const std::string& a_sectionName )
{
	m_nextSection = (size_t)-1;
	
	for ( size_t i = 0; i < m_sections.size(); i++ )
	{
		if ( a_sectionName == m_sections[i]->GetName() )
		{
			m_nextSection = i;
			break;
		}
	}
	
	// return false if we didn't find the next section, let caller handle it
	return m_nextSection != (size_t)-1;
}

bool SectionController::Play()
{
	// set up first section
	if ( m_currentSection == (size_t)-1 )
	{
		m_currentSection = m_nextSection;
	}
	
	// play current section
	Section* currentSection = m_sections[ m_currentSection ];
	
	if ( !currentSection->IsReady() )
	{
		// if it's not ready, prepare it
		currentSection->Prepare( NULL );
	}
	else if ( !currentSection->Play() )
	{
        // advance to next requested section
		m_currentSection = m_nextSection;
		if ( m_currentSection == (size_t)-1 )
		{
			LXT_LOG_SPAM("Finished all sections, SectionController exiting.");
			return false;
		}		
	}
	
	return true;
}

void SectionController::Render()
{
	Section* currentSection = m_sections[m_currentSection];
	if (currentSection->IsReady()) currentSection->Draw();
}

void SectionController::Finish()
{
	for ( size_t i = 0; i < m_sections.size(); i++ )
	{
		delete m_sections[i];
		m_sections[i] = NULL;
	}
	
	// todo - proper resize
	m_sections.resize(0);
}
