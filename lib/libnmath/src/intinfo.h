/*

    This file is part of the libnmath.

    intinfo.h
    Intinfo structure 

    Copyright (C) 2011
    Papadopoulos Nikolaos

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General
    Public License along with this library; if not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301 USA

*/

#ifndef NMATH_INTINFO_H_INCLUDED
#define NMATH_INTINFO_H_INCLUDED

#include "declspec.h"
#include "vector.h"
#include "precision.h"
#include "geometry.h"

namespace NMath {

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#ifdef __cplusplus
}   /* extern "C" */

class DECLSPEC IntInfo
{
	public:
		IntInfo();

		Vector3f normal;
		Vector3f point;
		Vector2f texcoord;
		scalar_t t;
		const Geometry* geometry;
};

#endif /* __cplusplus */

} /* namespace NMath */

#endif /* NMATH_INTINFO_H_INCLUDED */
