/*
 *  lig_stream.h
 *  convert_dae
 *
 *  Created by Justin on 8/07/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_LIG_STREAM_H__
#define __LXT_LIG_STREAM_H__

#include <vector>

namespace Lxt 
{
	struct LigIndexStream
	{
		std::vector<uint32_t> 	m_elements;
	};
	
	struct LigVertexStream
	{
		uint32_t				m_stride;
		std::vector<float>		m_elements;
	};
}

#endif // __LXT_LIG_STREAM_H__
