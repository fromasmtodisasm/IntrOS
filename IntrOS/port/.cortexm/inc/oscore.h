/******************************************************************************

    @file    IntrOS: oscore.h
    @author  Rajmund Szymanski
    @date    12.11.2018
    @brief   IntrOS port file for ARM Cotrex-M uC.

 ******************************************************************************

   Copyright (c) 2018 Rajmund Szymanski. All rights reserved.

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.

 ******************************************************************************/

#ifndef __INTROSCORE_H
#define __INTROSCORE_H

#include "osbase.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#ifndef OS_STACK_SIZE
#define OS_STACK_SIZE       256 /* default task stack size in bytes           */
#endif

/* -------------------------------------------------------------------------- */

#ifndef OS_FUNCTIONAL

#if   defined(__ARMCOMPILER_VERSION)
#define OS_FUNCTIONAL         6
#elif defined(__GNUC__)
#define OS_FUNCTIONAL         4
#else
#define OS_FUNCTIONAL         0 /* c++ functional library header not included */
#endif

#elif   OS_FUNCTIONAL
#error  OS_FUNCTIONAL is an internal port definition!
#endif//OS_FUNCTIONAL

/* -------------------------------------------------------------------------- */

typedef uint32_t              lck_t;
typedef uint64_t              stk_t;

/* -------------------------------------------------------------------------- */

extern  stk_t               __initial_sp[];
#define MAIN_TOP            __initial_sp

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

#elif defined(__ICCARM__)

struct __ctx
{
	unsigned r4, r5, r6, r7, r8, r9, r10, r11;
	void   * sp;
	fun_t  * pc;
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
// init task context

__STATIC_INLINE
void port_ctx_init( ctx_t *ctx, stk_t *sp, fun_t *pc )
{
	ctx->sp = sp;
	ctx->pc = pc;
}

/* -------------------------------------------------------------------------- */
// get current stack pointer

__STATIC_INLINE
void * port_get_sp( void )
{
	uint32_t sp;
#if   defined(__CC_ARM)
	sp = __current_sp();
#elif defined(__CSMC__)
	sp = __ASM ("mov r0, sp");
#elif defined(__ICCARM__)
	sp = __get_SP();
#else
	__ASM volatile ("mov %0, sp" : "=r" (sp));
#endif
	return (void *) sp;
}

/* -------------------------------------------------------------------------- */

#if   defined(__CSMC__)

__STATIC_INLINE
void __disable_irq( void )
{
	__ASM("cpsid i");
}

__STATIC_INLINE
void __enable_irq( void )
{
	__ASM("cpsie i");
}

#endif

/* -------------------------------------------------------------------------- */

__STATIC_INLINE
lck_t port_get_lock( void )
{
	return __get_PRIMASK();
}

__STATIC_INLINE
void port_put_lock( lck_t lck )
{
	__set_PRIMASK(lck);
}

__STATIC_INLINE
void port_set_lock( void )
{
	__disable_irq();
}

__STATIC_INLINE
void port_clr_lock( void )
{
	__enable_irq();
}

__STATIC_INLINE
void port_set_barrier( void )
{
	__ISB();
}

__STATIC_INLINE
void port_set_sync( void )
{
#if   defined(__ARMCC_VERSION)
	__schedule_barrier();
#elif defined(__GNUC__)
	__ASM volatile ("" ::: "memory");
#elif defined(__ICCARM__)
	__ASM ("" ::: "memory");
#endif	
}

/* -------------------------------------------------------------------------- */

#ifndef OS_MULTICORE
#if __CORTEX_M > 0
#define OS_MULTICORE

__STATIC_INLINE
void port_spn_lock( volatile unsigned *lock )
{
    while (__LDREXW((volatile uint32_t *)lock) || __STREXW(1, (volatile uint32_t *)lock));
}

#endif
#else
#error  OS_MULTICORE is an internal port definition!
#endif//OS_MULTICORE

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif//__INTROSCORE_H
