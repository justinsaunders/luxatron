/*
 *  state_descriptions.h
 *  Public descriptions for state buffer objects.
 *
 *  Created by Justin on 11/10/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_GFX_STATE_DESCRIPTIONS_H__
#define __LXT_GFX_STATE_DESCRIPTIONS_H__

namespace Lxt
{
	// Triangle Culling Mode
	enum CullMode
	{
		CM_NONE,			// always draw
		CM_FRONT,			// cull front facing triangles
		CM_BACK				// cull back facing triangles
	};
	
	// Depth Comparison Function
	enum DepthFunc
	{
		DF_NEVER,			// never pass
		DF_LESS,			// pass if incoming depth is less than stored value
		DF_EQUAL,			// path if incoming depth is equal 
		DF_GREATER,			// pass if incoming depth is greater
		DF_ALWAYS,			// always pass
	};
	
	// Alpha Blend Factor
	enum BlendFact 
	{
		BF_ZERO,
		BF_ONE,
		BF_SRC_COLOR,
		BF_ONE_MINUS_SRC_COLOR,
		BF_DST_COLOR,
		BF_ONE_MINUS_DST_COLOR,
		BF_SRC_ALPHA,
		BF_ONE_MINUS_SRC_ALPHA,
		BF_DST_ALPHA,
		BF_ONE_MINUS_DST_ALPHA,
		BF_SRC_ALPHA_SATURATE
	};
	
	// Rasterizer state
	struct RasterStateDesc
	{
		CullMode	m_cullMode;
	};
	
	// Depth write/test state
	struct DepthStateDesc
	{
		bool		m_depthTestEnabled;
		bool		m_depthWriteEnabled;
		DepthFunc	m_depthFunc;
	};	
	
	// Blend state
	struct BlendStateDesc
	{
		bool		m_blendEnabled;
		BlendFact	m_srcBlend;
		BlendFact	m_dstBlend;
	};
}

#endif // __LXT_GFX_STATE_DESCRIPTIONS_H__