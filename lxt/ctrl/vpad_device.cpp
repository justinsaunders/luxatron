/*
 *  vpad_device.cpp
 *  starkiller
 *
 *  Created by Justin on 24/05/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "vpad_device.h"

using namespace Lxt;

void VPadDevice::Push( uint32_t a_eventId, uint16_t a_x, uint16_t a_y )
{
	Position p = { a_x, a_y };
	
	m_inputStates.insert( std::make_pair( a_eventId, p ) );
}

void VPadDevice::Release( uint32_t a_eventId )
{
	States::iterator ii = m_inputStates.find( a_eventId );
	
	if ( ii != m_inputStates.end() )
	{	
		m_inputStates.erase( ii );
	}
}

void VPadDevice::Reset()
{
	m_inputStates.clear();
}
