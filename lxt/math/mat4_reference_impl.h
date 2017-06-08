/*
 *  mat4_reference_impl.h
 *  render
 *
 *  Created by Justin on 13/10/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_MATH_MAT4__
#error "This file should only be included by mat4.h, it can't be used directly."
#endif

inline Mat4& Mat4::operator=( const Mat4& a_rhs )
{
	m_column[0] = a_rhs.m_column[0];
	m_column[1] = a_rhs.m_column[1];
	m_column[2] = a_rhs.m_column[2];
	m_column[3] = a_rhs.m_column[3];
	return *this;
}

inline float Mat4::operator()( size_t i, size_t j ) const
{
	return m_column[i].m_v[j];
}

inline float& Mat4::operator()( size_t i, size_t j )
{
	return m_column[i].m_v[j];
}

inline void	Mat4::SetColumn( size_t index, const Vec4& a_column )
{
	m_column[index] = a_column;
}

inline void Mat4::GetColumn( size_t index, Vec4& a_column ) const
{
	a_column = m_column[index];
}

inline void Mat4::SetRow( size_t index, const Vec4& a_row )
{
	m_column[0].m_v[index] = a_row.m_v[0];
	m_column[1].m_v[index] = a_row.m_v[1];
	m_column[2].m_v[index] = a_row.m_v[2];
	m_column[3].m_v[index] = a_row.m_v[3];
}

inline void Mat4::GetRow( size_t index, Vec4& a_row ) const
{
	a_row.m_v[0] = m_column[0].m_v[index]; 
	a_row.m_v[1] = m_column[1].m_v[index];
	a_row.m_v[2] = m_column[2].m_v[index];
	a_row.m_v[3] = m_column[3].m_v[index];
}

inline void	Mat4::Multiply( const Mat4& a_m, const Vec4& a_v, 
						   Vec4& a_result )
{
	LXT_ASSERT( &a_v != &a_result );
	
	for ( size_t i = 0; i < 4; i++ )
	{
		a_result.m_v[i]=a_v.m_v[0] * a_m.m_column[0].m_v[i] + 
						a_v.m_v[1] * a_m.m_column[1].m_v[i] + 
						a_v.m_v[2] * a_m.m_column[2].m_v[i] + 
						a_v.m_v[3] * a_m.m_column[3].m_v[i];
	}
}

inline void	Mat4::Multiply( const Mat4& a_lhs, const Mat4& a_rhs, 
						   Mat4& a_result )
{
	LXT_ASSERT( (&a_lhs != &a_result) && (&a_rhs != &a_result) );
	
	Multiply( a_lhs, a_rhs.m_column[0], a_result.m_column[0] );
	Multiply( a_lhs, a_rhs.m_column[1], a_result.m_column[1] );
	Multiply( a_lhs, a_rhs.m_column[2], a_result.m_column[2] );
	Multiply( a_lhs, a_rhs.m_column[3], a_result.m_column[3] );
}

inline void Mat4::InvertArbitary( const Mat4& a_input, Mat4& a_result )
{
	LXT_ASSERT( &a_input != &a_result );
	
	// from geometrictools.com
	float fA0 = a_input(0,0)*a_input(1,1) - a_input(0,1)*a_input(1,0);
	float fA1 = a_input(0,0)*a_input(1,2) - a_input(0,2)*a_input(1,0);
	float fA2 = a_input(0,0)*a_input(1,3) - a_input(0,3)*a_input(1,0);
	float fA3 = a_input(0,1)*a_input(1,2) - a_input(0,2)*a_input(1,1);
	float fA4 = a_input(0,1)*a_input(1,3) - a_input(0,3)*a_input(1,1);
	float fA5 = a_input(0,2)*a_input(1,3) - a_input(0,3)*a_input(1,2);
	float fB0 = a_input(2,0)*a_input(3,1) - a_input(2,1)*a_input(3,0);
	float fB1 = a_input(2,0)*a_input(3,2) - a_input(2,2)*a_input(3,0);
	float fB2 = a_input(2,0)*a_input(3,3) - a_input(2,3)*a_input(3,0);
	float fB3 = a_input(2,1)*a_input(3,2) - a_input(2,2)*a_input(3,1);
	float fB4 = a_input(2,1)*a_input(3,3) - a_input(2,3)*a_input(3,1);
	float fB5 = a_input(2,2)*a_input(3,3) - a_input(2,3)*a_input(3,2);
	float fDet = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
	
	if ( !Lxt::Equal(0.0f, fDet) )
	{
		a_result(0,0) = + a_input(1,1)*fB5 - a_input(1,2)*fB4 + a_input(1,3)*fB3;
		a_result(1,0) = - a_input(1,0)*fB5 + a_input(1,2)*fB2 - a_input(1,3)*fB1;
		a_result(2,0) = + a_input(1,0)*fB4 - a_input(1,1)*fB2 + a_input(1,3)*fB0;
		a_result(3,0) = - a_input(1,0)*fB3 + a_input(1,1)*fB1 - a_input(1,2)*fB0;
		a_result(0,1) = - a_input(0,1)*fB5 + a_input(0,2)*fB4 - a_input(0,3)*fB3;
		a_result(1,1) = + a_input(0,0)*fB5 - a_input(0,2)*fB2 + a_input(0,3)*fB1;
		a_result(2,1) = - a_input(0,0)*fB4 + a_input(0,1)*fB2 - a_input(0,3)*fB0;
		a_result(3,1) = + a_input(0,0)*fB3 - a_input(0,1)*fB1 + a_input(0,2)*fB0;
		a_result(0,2) = + a_input(3,1)*fA5 - a_input(3,2)*fA4 + a_input(3,3)*fA3;
		a_result(1,2) = - a_input(3,0)*fA5 + a_input(3,2)*fA2 - a_input(3,3)*fA1;
		a_result(2,2) = + a_input(3,0)*fA4 - a_input(3,1)*fA2 + a_input(3,3)*fA0;
		a_result(3,2) = - a_input(3,0)*fA3 + a_input(3,1)*fA1 - a_input(3,2)*fA0;
		a_result(0,3) = - a_input(2,1)*fA5 + a_input(2,2)*fA4 - a_input(2,3)*fA3;
		a_result(1,3) = + a_input(2,0)*fA5 - a_input(2,2)*fA2 + a_input(2,3)*fA1;
		a_result(2,3) = - a_input(2,0)*fA4 + a_input(2,1)*fA2 - a_input(2,3)*fA0;
		a_result(3,3) = + a_input(2,0)*fA3 - a_input(2,1)*fA1 + a_input(2,2)*fA0;
		
		float fInvDet = 1.0f/fDet;
		a_result(0,0) *= fInvDet;
		a_result(0,1) *= fInvDet;
		a_result(0,2) *= fInvDet;
		a_result(0,3) *= fInvDet;
		a_result(1,0) *= fInvDet;
		a_result(1,1) *= fInvDet;
		a_result(1,2) *= fInvDet;
		a_result(1,3) *= fInvDet;
		a_result(2,0) *= fInvDet;
		a_result(2,1) *= fInvDet;
		a_result(2,2) *= fInvDet;
		a_result(2,3) *= fInvDet;
		a_result(3,0) *= fInvDet;
		a_result(3,1) *= fInvDet;
		a_result(3,2) *= fInvDet;
		a_result(3,3) *= fInvDet;
	}
	else		
	{
		// Source matrix is singular, can't be inverted, so set result
		// as zero matrix.
		a_result = Mat4::Zero;
	}
}

inline bool Mat4::Equal( const Mat4& a_rhs, float a_error ) const
{
	return m_column[0].Equal( a_rhs.m_column[0], a_error )
	&& m_column[1].Equal( a_rhs.m_column[1], a_error )
	&& m_column[2].Equal( a_rhs.m_column[2], a_error )
	&& m_column[3].Equal( a_rhs.m_column[3], a_error );
}

inline bool Mat4::IsOrthonormal() const
{
	if ( Lxt::Equal( m_column[0].LengthSquared(), 1.0f ) &&
		Lxt::Equal( m_column[1].LengthSquared(), 1.0f ) &&
		Lxt::Equal( m_column[2].LengthSquared(), 1.0f ) )
	{
		Vec4 cross;
		m_column[0].Cross3( m_column[1], cross );
		return cross.Equal( m_column[2] );
	}
	
	return false;
}

inline Mat4::Mat4()
{
}

inline Mat4::Mat4(	float a[16] )
{
	m_column[0] = Vec4( a[0], a[1], a[2], a[3] );
	m_column[1] = Vec4( a[4], a[5], a[6], a[7] );
	m_column[2] = Vec4( a[8], a[9], a[10], a[11] );
	m_column[3] = Vec4( a[12], a[13], a[14], a[15] );
}

inline Mat4::Mat4(const Vec4& a_c0,
				  const Vec4& a_c1,
				  const Vec4& a_c2,
				  const Vec4& a_c3 )
{
	m_column[0] = a_c0;
	m_column[1] = a_c1;
	m_column[2] = a_c2;
	m_column[3] = a_c3;
}

inline Mat4::Mat4( const Mat4& a_other )
{
	*this = a_other;
}

