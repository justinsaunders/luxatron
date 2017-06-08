/*
 *  state_cache.cpp
 *  test_runner
 *
 *  Created by Justin on 26/10/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "state_cache.h"

using namespace Lxt;

StateCache::StateCache( size_t a_max_states )
{
	m_rasterStates.reserve( a_max_states );
	m_depthStates.reserve( a_max_states );
	m_blendStates.reserve( a_max_states );
}

template <typename STATE, typename DESC>
Lxt::Handle CreateState( const DESC&			a_description,
						 std::vector< STATE >&	a_container )
{	
	// Compile state
	STATE newState;
	bool status = newState.Compile( a_description );
	if ( !status )
	{
		LXT_LOG_ERROR("Unable to compile state buffer.");
		return INVALID_HANDLE;
	}
	
	// Find existing state
	const size_t	n		= a_container.size();
	size_t			i		= 0;
	bool			found	= false;
	
	for ( i = 0; i < n; i++ )
	{
		if ( a_container[i].Equal( newState ) ) 
		{
			found = true;
			break;
		}
	}
	
	// If we didn't find anything, create a new state unless capacity has been
	// reached.
	if ( !found )
	{
		if ( a_container.size() == a_container.capacity() )
		{
			i = INVALID_HANDLE;
		}
		else
		{
			a_container.push_back( newState );
		}
	}
	
	return i;
}

template <typename STATE>
const STATE* GetState( const std::vector< STATE >& a_container, size_t a_index )
{
	if ( a_index < a_container.size() )
	{
		return &(a_container[ a_index ]);
	}
	
	return NULL;
}

Lxt::Handle StateCache::CreateRasterState( const RasterStateDesc& a_description )
{
	return CreateState( a_description, m_rasterStates );
}

Lxt::Handle StateCache::CreateDepthState( const DepthStateDesc& a_description )
{
	return CreateState( a_description, m_depthStates );
}

Lxt::Handle StateCache::CreateBlendState( const BlendStateDesc& a_description )
{
	return CreateState( a_description, m_blendStates );
}

const RasterStateBuffer* StateCache::GetRasterState( Handle a_index )
{
	return GetState( m_rasterStates, a_index );
}

const DepthStateBuffer* StateCache::GetDepthState( Handle a_index )
{
	return GetState( m_depthStates, a_index );
}

const BlendStateBuffer* StateCache::GetBlendState( Handle a_index )
{
	return GetState( m_blendStates, a_index );
}