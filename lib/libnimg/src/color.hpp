/*

	This file is part of libnimg.

	color.hpp
	Color

	Copyright (C) 2012
	Papadopoulos Nikolaos

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 3 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General
	Public License along with this program; if not, write to the
	Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
	Boston, MA 02110-1301 USA

*/

#ifndef NIMG_COLOR_HPP_INCLUDED
#define NIMG_COLOR_HPP_INCLUDED

#include "declspec.h"
#include "precision.h"

namespace NImg {

// Forward declarations
class ColorRGBf;
class ColorRGBAf;

// COLOR RGB
class DECLSPEC ColorRGBf
{
	public:
		explicit ColorRGBf(const scalar_t r = 0.0f, const scalar_t g = 0.0f, const scalar_t b = 0.0f);
		ColorRGBf(const ColorRGBf &rhs);
		ColorRGBf(const ColorRGBAf &rhs);

		// Operators
		inline ColorRGBf &operator +=(const ColorRGBf &rhs);
		inline ColorRGBf &operator -=(const ColorRGBf &rhs);
		inline ColorRGBf &operator *=(const ColorRGBf &rhs);
		inline ColorRGBf &operator *=(const scalar_t s);

		inline void r(const scalar_t r);
		inline void g(const scalar_t g);
		inline void b(const scalar_t b);

		inline scalar_t r() const;
		inline scalar_t g() const;
		inline scalar_t b() const;

	private:
		scalar_t m_r, m_g, m_b;

};

// Auxiliary operators
inline DECLSPEC ColorRGBf operator +(const ColorRGBf &lhs, const ColorRGBf &rhs);
inline DECLSPEC ColorRGBf operator -(const ColorRGBf &lhs, const ColorRGBf &rhs);
inline DECLSPEC ColorRGBf operator *(const ColorRGBf &lhs, const ColorRGBf &rhs);
inline DECLSPEC ColorRGBf operator *(const ColorRGBf &lhs, const scalar_t s);
inline DECLSPEC ColorRGBf operator *(const scalar_t s, const ColorRGBf &lhs);

/*
    COLOR RGBA
*/
class DECLSPEC ColorRGBAf
{
	public:
		explicit ColorRGBAf(const scalar_t r = 0.0f, const scalar_t g = 0.0f, const scalar_t b = 0.0f, const scalar_t a = 1.0f);
		ColorRGBAf(const ColorRGBf &rhs);
		ColorRGBAf(const ColorRGBAf &rhs);

		// Operators
		inline ColorRGBAf &operator +=(const ColorRGBAf &rhs);
		inline ColorRGBAf &operator -=(const ColorRGBAf &rhs);
		inline ColorRGBAf &operator *=(const ColorRGBAf &rhs);
		inline ColorRGBAf &operator *=(const scalar_t s);

		void r(const scalar_t r);
		void g(const scalar_t g);
		void b(const scalar_t b);
		void a(const scalar_t a);

		scalar_t r() const;
		scalar_t g() const;
		scalar_t b() const;
		scalar_t a() const;

	private:
		scalar_t m_r, m_g, m_b, m_a;

};

// Auxiliary operators
inline DECLSPEC ColorRGBAf operator +(const ColorRGBAf &lhs, const ColorRGBAf &rhs);
inline DECLSPEC ColorRGBAf operator -(const ColorRGBAf &lhs, const ColorRGBAf &rhs);
inline DECLSPEC ColorRGBAf operator *(const ColorRGBAf &lhs, const ColorRGBAf &rhs);
inline DECLSPEC ColorRGBAf operator *(const ColorRGBAf &lhs, const scalar_t s);
inline DECLSPEC ColorRGBAf operator *(const scalar_t s, const ColorRGBAf &lhs);

} /* namespace NImg */

#include "color.inl"

#endif /* NIMG_COLOR_HPP_INCLUDED */
