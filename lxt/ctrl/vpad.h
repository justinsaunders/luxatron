/*
 *  vpad.h
 *  Luxatron
 *
 *  Created by Justin on 14/07/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_CTRL_VPAD_H__
#define __LXT_CTRL_VPAD_H__

#include "core/array.h"
#include <map>

namespace Lxt 
{

struct VPadDevice;
	
// A structure representing the state of the virtual control pad.
struct VPadState
{
	// Constants 
	enum { MAX_BUTTONS = 16 };
	enum { MAX_STICKS = 2 };

	VPadState()
	{
		Clear();
	}
	
	void Clear()
	{
		m_buttons	= 0;
		m_stickX[0] = m_stickX[1] = 0;
		m_stickY[0] = m_stickY[1] = 0;
	}
	
	bool GetButtonState( u_int8_t index )
	{
		return (m_buttons & (1 << index)) > 0;
	}
	
	// Bitmask of device's digital buttons
	u_int16_t	m_buttons;
	
	// Position of each joystick relative to center
	int16_t		m_stickX[2];
	int16_t		m_stickY[2];
};

// A structure representing a physical joystick.
struct VPadStick
{
	int16_t		m_centerX;	// x position of rest point
	int16_t		m_centerY;	// y position of rest point
	u_int16_t	m_radius;	// maximum range from rest point
	float		m_tension;	// 0 (none), 1 (maximum) factor used to return stick
							// to rest postion.
	
	VPadStick()
	{
		clear();
	}
	
	VPadStick( int16_t cx, int16_t cy, u_int16_t r, float t )
		:	m_centerX( cx ),
			m_centerY( cy ),
			m_radius( r ),
			m_tension( t )
	{
	}
	
	void clear()
	{
		m_centerX	= m_centerY = 0;
		m_radius	= 0;
		m_tension	= 0.0f;
	}	
};

// A structure representing a physical circular button.
struct VPadButton
{	
	int16_t		m_centerX;	// x position of button center
	int16_t		m_centerY;	// y position of button center
	u_int16_t	m_radius;	// radius of button

	VPadButton()
	{
		clear();
	}
	
	VPadButton( int16_t cx, int16_t cy, u_int16_t r )
		:	m_centerX( cx ),
			m_centerY( cy ),
			m_radius( r )
	{
	}
	
	void clear()
	{
		m_centerX = m_centerY = 0;
		m_radius = 0;
	}
};
	
/*
 * This class represents a virtual controller with up to two joysticks and 
 * sixteen buttons.
 */
class VPad
{
public:
	// Structors	
	VPad();
	~VPad();
	
	/*
	 * Create a button with given dimensions.
	 */
	bool				AddButton( const VPadButton& a_button );
	
	/*
	 * Create a virtual joystick with given dimensions.
	 */
	bool				AddStick( const VPadStick& a_stick );
	
	/*
	 * Get the state of all the controls on the pad. Returns true if any input
	 * (including off button area) was recieved.
	 */
	bool				CalculateState(  const VPadDevice& a_device, 
										VPadState& a_state ) const;
	
	/*
	 * Get information about count of each item.
	 */
	size_t				GetNumButtons() const;
	size_t				GetNumSticks() const;
	
	/*
	 * Get information about button or stick. Returns NULL if out of bounds.
	 */
	const VPadButton*	GetButton( size_t a_index ) const;
	const VPadStick*	GetStick( size_t a_index) const;
		 
private:
	// Buttons and sticks
	Array<VPadButton, VPadState::MAX_BUTTONS>			m_buttons;
	Array<VPadStick, VPadState::MAX_STICKS>				m_sticks;
	size_t	m_numButtons;
	size_t	m_numSticks;
}; // class VPad
	
} // namespace Lxt

#endif // __LXT_CTRL_VPAD_H__