/*
 *  tilt_device.cpp
 *  starkiller
 *
 *  Created by Justin on 24/05/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "tilt_device.h"

using namespace Lxt;

void TiltDevice::Update( float a_rawX, float a_rawY, float a_rawZ, 
				   float a_filterFactor )
{
	m_raw.m_x = a_rawX;
	m_raw.m_y = a_rawY;
	m_raw.m_z = a_rawZ;
	
	const float OneMinusFF = 1.0f - a_filterFactor;
	
	m_filtered.m_x = a_rawX * a_filterFactor + m_filtered.m_x * OneMinusFF;
	m_filtered.m_y = a_rawY * a_filterFactor + m_filtered.m_y * OneMinusFF;
	m_filtered.m_z = a_rawZ * a_filterFactor + m_filtered.m_z * OneMinusFF;
}
