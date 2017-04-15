/******************************************************************************

    @file    IntrOS: oscore.h
    @author  Rajmund Szymanski
    @date    26.03.2017
    @brief   IntrOS port file for ARM Cotrex-M uC.

 ******************************************************************************

    IntrOS - Copyright (C) 2013 Rajmund Szymanski.

    This file is part of IntrOS distribution.

    IntrOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation; either version 3 of the License,
    or (at your option) any later version.

    IntrOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#ifndef __INTROSCORE_H
#define __INTROSCORE_H

#include <osbase.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

// task context

typedef struct __ctx ctx_t;

#if   defined(__ARMCC_VERSION)

struct __ctx
{
	unsigned r8, r9, r10, r11;
	fun_t  * pc;
	unsigned r4, r5, r6, r7;
	void   * sp;
#if __FPU_USED
	float    s[16]; // s16-s31
#endif
};

#elif defined(__GNUC__)

struct __ctx
{
	unsigned r4, r5, r6, r7, r8, r9, r10, r11;
	void   * sp;
	fun_t  * pc;
#if __FPU_USED
	float    s[16]; // s16-s31
#endif
};

#elif defined(__CSMC__)

struct __ctx
{
	unsigned r4, r5, r6, r7, r8, r9, r10, r11;
	fun_t  * pc;
	void   * sp;
#if __FPU_USED
	float    s[16]; // s16-s31
#endif
};

#endif

#if __FPU_USED
#define _CTX_INIT() { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, { 0 } }
#else
#define _CTX_INIT() { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
#endif

/* -------------------------------------------------------------------------- */

__STATIC_INLINE
void port_ctx_init( ctx_t *ctx, stk_t *sp, fun_t *pc )
{
	ctx->sp = sp;
	ctx->pc = pc;
}

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif//__INTROSCORE_H