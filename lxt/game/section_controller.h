/*
 *  section_controller.h
 *  starkiller
 *
 *  Created by Justin on 23/05/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_GAME_SECTION_CONTROLLER_H__
#define __LXT_GAME_SECTION_CONTROLLER_H__

#include <string>

namespace Lxt 
{
	class Section;
	class Renderer;
	class VPad;
	struct Vec3;
	
	class SectionController
	{
	public:
		SectionController();
		~SectionController();
		
		// Add section to controller, which will own it.
		void Add( Section* a_section );
		
        // Switch sections immediately
        void SetSection( const std::string& a_sectionName );
        
		// Set the next section to play and render
		bool QueueNextSection( const std::string& a_sectionName );
		
		// Update/Draw current section. Play returns false when all sections are
		// complete
		bool Play();
		void Render();
		
		// Free all sections and wind up
		void Finish();
		
	private:
		std::vector<Section*>   m_sections;
		size_t					m_currentSection;
		size_t					m_nextSection;
	};
	
}

#endif // __LXT_GAME_SECTION_CONTROLLER_H__
