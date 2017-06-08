/*
 *  vec4_reference_impl.h
 *  Luxatron™
 *
 *  Created by Justin on 29/09/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_MATH_VEC4__
#error "This file should only be included by vec4.h, it can't be used directly."
#endif

////////////////////////////////////////////////////////////////////////////
// Scalar C implemenation
////////////////////////////////////////////////////////////////////////////
inline void Vec4::Set( float a_x, float a_y, float a_z, float a_w )
{
	m_x = a_x;
	m_y = a_y;
	m_z = a_z;
	m_w = a_w;
}

inline void Vec4::Copy( const Vec4 & a_source )
{
	m_x = a_source.m_x;
	m_y = a_source.m_y;
	m_z = a_source.m_z;
	m_w = a_source.m_w;
}

inline void Vec4::Add( const Vec4& a_rhs )
{
	m_x += a_rhs.m_x;
	m_y += a_rhs.m_y;
	m_z += a_rhs.m_z;
	m_w += a_rhs.m_w;		
}

inline void	Vec4::Subtract( const Vec4& a_rhs )
{
	m_x -= a_rhs.m_x;
	m_y -= a_rhs.m_y;
	m_z -= a_rhs.m_z;
	m_w -= a_rhs.m_w;
}

inline void Vec4::Scale( float a_scale )
{
	m_x *= a_scale;
	m_y *= a_scale;
	m_z *= a_scale;
	m_w *= a_scale;		
}

inline float Vec4::Dot( const Vec4& a_v2 ) const
{
	float result = m_x * a_v2.m_x;
	result		+= m_y * a_v2.m_y;
	result		+= m_z * a_v2.m_z;
	result		+= m_w * a_v2.m_w;
	
	return result;
}

inline void Vec4::Cross3( const Vec4& a_rhs, Vec4& a_result ) const
{
	LXT_ASSERT( &a_result != this ); // check for trashing input
	
	const Vec4&	lhs = *this;
	a_result.m_x = lhs.m_y * a_rhs.m_z - lhs.m_z * a_rhs.m_y;
	a_result.m_y = lhs.m_z * a_rhs.m_x - lhs.m_x * a_rhs.m_z;
	a_result.m_z = lhs.m_x * a_rhs.m_y - lhs.m_y * a_rhs.m_x;
	a_result.m_w = 0.0f;
}

inline float Vec4::Length() const
{
	return sqrtf( LengthSquared() ); 
}

inline float Vec4::LengthSquared() const
{
	return Dot( *this ); 
}

inline void Vec4::Normalise()
{
	const float ool = 1.0f / Length();
	Scale( ool );
}

inline bool Vec4::Equal( const Vec4& a_rhs, float a_error ) const
{
	return ( Lxt::Equal( a_rhs.m_x, m_x, a_error ) &&
			 Lxt::Equal( a_rhs.m_y, m_y, a_error ) &&
			 Lxt::Equal( a_rhs.m_z, m_z, a_error ) &&
			 Lxt::Equal( a_rhs.m_w, m_w, a_error ) );
}

inline Vec3 Vec4::Xyz() const
{
	return Vec3(m_x, m_y, m_z);
}

inline Vec4::Vec4()
{
	// un-initialised by default
}

inline Vec4::Vec4( float a_x, float a_y, float a_z, float a_w )
{
	Set( a_x, a_y, a_z, a_w );
}

inline Vec4::Vec4( const Vec4 & a_source )
{
	Copy( a_source );
}

// overloaded operators
inline Vec4& Vec4::operator=( const Vec4& a_rhs )
{
	Copy( a_rhs );
	return *this;
}
