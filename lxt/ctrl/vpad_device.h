/*
 *  vpad_device.h
 *  starkiller
 *
 *  Created by Justin on 24/05/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_VPAD_DEVICE_H__
#define __LXT_VPAD_DEVICE_H__

#include <map>

namespace Lxt
{
	// VPadDevice records touches as they occur.
	struct VPadDevice
	{
		void Push( uint32_t a_eventId, uint16_t a_x, uint16_t a_y );
		void Release( uint32_t a_eventId );
		void Reset();
		
		// Types
		struct Position
		{
			uint16_t m_x, m_y;
		};
		
		// Maximum number of inputs, any more are ignored.
		enum { MAX_INPUTS = 16 };
		
		typedef std::map< uint32_t, Position > States; 
		
		// The current input state
		States m_inputStates;
	};
}

#endif // __LXT_VPAD_DEVICE_H__
