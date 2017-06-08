/*
 *  lxt_splash.cpp
 *  starkiller
 *
 *  Created by Justin on 13/06/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "lxt_splash.h"

#include "core/timer.h"

#include "gfx/util.h"
#include "gfx/gfx.h"
#include "math/math.h"
#include "gfx/gles/gl_headers.h"
#include "snd/sound_device.h"
#include "device_context.h"
#include "game/section_controller.h"

using namespace Lxt;

LxtSplash::LxtSplash(SectionController& a_sectionController) 
	:	Section("LxtSplash", a_sectionController), 
		m_focusEffect( 0.0f, 1.0f, 5.0f, 1.0f, 1.0f ),
		m_alpha( 0.0f ),
		m_size( 1.0f ),
		m_duration( 3.0f )
{
}

LxtSplash::~LxtSplash()
{	
	// Splash section is hardcoded to play "Intro" section afterwards
	m_controller.SetNextSection("Intro");
}

void LxtSplash::Prepare( Lxt::ProgressInfo* a_pi )
{
	ScopedSpamTimer sst( "LxtSplash_Prepare" );
	
	// Create orthographic projection
	Lxt::Vec2 v; GetDC().m_renderer->GetView( v );
	m_2dCamera.SetOrtho( v.m_x, v.m_y );
	
	// Load Textures
	Lxt::Path	 path;
	Lxt::File::GetApplicationPath( path );
	path += "/";
	
	bool ok = false;
	ok = TextureLoad( path + "luxatron_splash.png", m_lxtTexture );
	LXT_ASSERT( ok );
	
	m_lxtSound = 
		GetDC().m_soundDevice->Effect_Initialise( path + "lxt.caf" );
	LXT_ASSERT( m_lxtSound != INVALID_HANDLE );	
	
	GetDC().m_soundDevice->Effect_SetVolume( m_lxtSound, 1.0f );
	
	m_lxtRect.m_x = 0;
	m_lxtRect.m_y = 0;
	m_lxtRect.m_width = m_lxtTexture.GetWidth();
	m_lxtRect.m_height= m_lxtTexture.GetHeight();
	
	m_lxtRect.SetCentre( 240, 160 );
		
	m_isReady = true;
}

bool LxtSplash::Play()
{
	static float time = 0.0f;
	
	m_focusEffect.Update( time );
	m_alpha = m_focusEffect.Alpha();
	m_size	= m_focusEffect.Size();
	
	static Rect orgRect = m_lxtRect;
	
	m_lxtRect = orgRect;
	m_lxtRect.Scale( m_size );
	
	if ( time == 0.0f )
	{
		GetDC().m_soundDevice->Effect_Start( m_lxtSound );
	}
	else if ( time >  m_duration ) 
	{
		// Exit section
		Yield("Intro"); // hardcoded to yield to "intro" section
		return false;
	}
	
	time += Lxt::GetSimulationStep();
	
	return true;
}

void LxtSplash::Draw()
{	
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	// Switch to 2D - also no culling, depth test or lighting.
	glDisable( GL_LIGHTING );
	glDisable( GL_CULL_FACE ) ;
	glDisable( GL_DEPTH_TEST );
	
	GetDC().m_renderer->SetProjection( m_2dCamera.Proj() );
	GetDC().m_renderer->SetModelView( m_2dCamera.InverseView() );
	
	// Lxt logo
	Colour blend = Colour(Vec4( 1.0f, 1.0f, 1.0f, m_alpha));
	
	Vec2 a = m_lxtRect.A();
	Vec2 b = m_lxtRect.B();
	
	Lxt::DrawRect( *(GetDC().m_renderer), 
				  a.m_x, a.m_y,
				  b.m_x, b.m_y,
				  blend, &m_lxtTexture );
}
