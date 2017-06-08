/*
 *  vpad.cpp
 *  Luxatron
 *
 *  Created by Justin on 14/07/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "vpad.h"

#include "vpad_device.h"

namespace Lxt 
{
	
VPad::VPad()
	:	m_numButtons( 0 ),
		m_numSticks( 0 )
{
}
	
VPad::~VPad()
{
}

bool VPad::AddButton( const VPadButton& a_button )
{
	if ( m_numButtons == VPadState::MAX_BUTTONS )
		return false;
	
	m_buttons[m_numButtons++] = a_button;
	return true;
}
	
bool VPad::AddStick( const VPadStick& a_stick )
{
	if ( m_numSticks == VPadState::MAX_STICKS )
		return false;
	
	m_sticks[ m_numSticks++ ] = a_stick;
	return true;
}

bool VPad::CalculateState( const VPadDevice& a_device, VPadState& a_state ) const
{
	a_state.Clear();
	
	VPadDevice::States inputs = a_device.m_inputStates;
	
	for ( VPadDevice::States::const_iterator pli = inputs.begin();
		 pli != inputs.end(); pli++ )
	{	
		const VPadDevice::Position& input = pli->second;
		
		// dispatch touch to buttons - simple point in circle test
		for ( size_t b = 0; b < m_numButtons; b++ )
		{
			float xd = ( m_buttons[b].m_centerX - input.m_x );
			float yd = ( m_buttons[b].m_centerY - input.m_y );
			
			if ( (xd * xd) + (yd * yd ) < 
				(m_buttons[b].m_radius * m_buttons[b].m_radius) )
			{
				a_state.m_buttons |= 1 << b;
			}
		}
	}
	
	return inputs.size() > 0;
}

/*
 * Get information about count of each item.
 */
size_t VPad::GetNumButtons() const
{
	return m_numButtons;
}
	
size_t VPad::GetNumSticks() const
{
	return m_numSticks;
}
	
/*
 * Get information about button or stick. Returns NULL if out of bounds.
 */
const VPadButton* VPad::GetButton( size_t a_index ) const
{
	if ( a_index < m_buttons.Size() )
		return &m_buttons[a_index];
	
	return NULL;
}
	
const VPadStick* VPad::GetStick( size_t a_index) const
{
	if ( a_index < m_sticks.Size() )
		return &m_sticks[a_index];
	
	return NULL;
}	

} // namespace Ctrl