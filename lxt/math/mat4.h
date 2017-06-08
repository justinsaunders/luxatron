/*
 *  mat4.h
 *  Luxatron™
 *
 *  Created by Justin on 29/09/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_MATH_MAT4__
#define __LXT_MATH_MAT4__

#include "vec4.h"

namespace Lxt 
{
	class Archive;
	
	struct Mat4
	{
		// Idenity/Zero Matrix
		const static Mat4 Identity;
		const static Mat4 Zero;
		
		// overloaded operators
		inline Mat4&	operator=( const Mat4& a_rhs );
		inline float	operator()( size_t column, size_t row ) const;
		inline float&	operator()( size_t column, size_t row );
		
		// methods
		inline void		SetColumn( size_t index, const Vec4& a_column );
		inline void 	SetRow( size_t index, const Vec4& a_row );
		inline void		GetColumn( size_t index, Vec4& a_column ) const;
		inline void 	GetRow( size_t index, Vec4& a_row ) const;
		
		// these methods do not check that result overwrites parameters 
		// - caller beware
		static inline void	Multiply( const Mat4& a_m, const Vec4& a_v, 
									  Vec4& a_result );
		static inline void	Multiply( const Mat4& a_lhs, const Mat4& a_rhs, 
									  Mat4& a_result );
		static inline void	InvertArbitary( const Mat4& a_m, Mat4& a_result );
		
		// comparison
		inline bool Equal(  const Mat4& a_rhs, 
						    float		a_error = LXT_EPSILON ) const;
		inline bool IsOrthonormal() const;
			
		// creation
		inline Mat4();
		inline Mat4( float a_f[16] );
		inline Mat4( const Vec4& a_c1,
					 const Vec4& a_c2,
					 const Vec4& a_c3,
					 const Vec4& a_c4 );
		inline Mat4( const Mat4& a_other );
		
		inline ~Mat4() {};
		
		// Serialisation
		static size_t	Store( const Mat4& a_mat4, Archive& a_destination );
		static bool		Extract( Mat4& a_mat4, Archive& a_from, 
								size_t& a_offset );
	public:
		Vec4 m_column[4];
	};
	
// Include regular cpp implementation
#include "mat4_reference_impl.h"
	
} // namespace Lxt

#endif // __LXT_MATH_MAT4__

