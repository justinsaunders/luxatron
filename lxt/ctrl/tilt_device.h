/*
 *  tilt_device.h
 *  starkiller
 *
 *  Created by Justin on 24/05/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_TILT_DEVICE_H__
#define __LXT_TILT_DEVICE_H__

#include "math/vec3.h"

namespace Lxt 
{
	struct TiltDevice
	{
		void Update( float a_rawX, float a_rawY, float a_rawZ,
					float a_filterFactor = 0.1f );
		
		Vec3 m_raw;
		Vec3 m_filtered;
	};
}

#endif // __LXT_TILT_DEVICE_H__
