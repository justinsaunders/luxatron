/*
 *  mat4.cpp
 *  Luxatron™
 *
 *  Created by Justin on 29/09/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "mat4.h"
#include "core/archive.h"

namespace Lxt
{
	float identity[16] = 
	{	1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f  };
	
	float zero[16] = 
	{	0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f };
	
	const Mat4 Mat4::Identity( identity );
	const Mat4 Mat4::Zero( zero	);
	
	// Serialisation
	size_t Mat4::Store( const Mat4& a_mat4, Archive& a_destination )
	{
		Buffer& dst		= a_destination.Storage();
		size_t	written = 0;
		
		written += dst.Append( &a_mat4.m_column[0], sizeof(Vec4) );
		written += dst.Append( &a_mat4.m_column[1], sizeof(Vec4) );
		written += dst.Append( &a_mat4.m_column[2], sizeof(Vec4) );
		written += dst.Append( &a_mat4.m_column[3], sizeof(Vec4) );
		
		return written;
	}
	
	bool Mat4::Extract( Mat4& a_mat4, Archive& a_from, 
						size_t& a_offset )
	{
		Buffer& from = a_from.Storage();
		
		a_offset += from.Read( &a_mat4.m_column[0], sizeof(Vec4), a_offset );
		a_offset += from.Read( &a_mat4.m_column[1], sizeof(Vec4), a_offset );
		a_offset += from.Read( &a_mat4.m_column[2], sizeof(Vec4), a_offset );
		a_offset += from.Read( &a_mat4.m_column[3], sizeof(Vec4), a_offset );
		
		return true;
	}
}