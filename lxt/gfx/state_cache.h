/*
 *  state_cache.h
 *  A cache of render states
 *
 *  Created by Justin on 26/10/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_GFX_STATE_CACHE_H__
#define __LXT_GFX_STATE_CACHE_H__

#include "gles/state_buffer.h"
#include "state_description.h"

namespace Lxt 
{
	class StateCache
	{
	public:
		// Constructor
		StateCache( size_t a_max_states );
		
		// State
		Handle CreateRasterState( const RasterStateDesc& a_description );
		const RasterStateBuffer* GetRasterState( Handle a_rasterState );
		
		Handle CreateDepthState( const DepthStateDesc& a_description );
		const DepthStateBuffer*	GetDepthState( Handle a_depthState );

		Handle CreateBlendState( const BlendStateDesc& a_description );
		const BlendStateBuffer*	GetBlendState( Handle a_blendState );
	
	private:
		std::vector<RasterStateBuffer>	m_rasterStates;
		std::vector<DepthStateBuffer>	m_depthStates;
		std::vector<BlendStateBuffer>	m_blendStates;
	};
}


#endif // __LXT_GFX_STATE_CACHE_H__
