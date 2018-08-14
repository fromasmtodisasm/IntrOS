/******************************************************************************

    @file    IntrOS: ossemaphore.h
    @author  Rajmund Szymanski
    @date    14.08.2018
    @brief   This file contains definitions for IntrOS.

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

#ifndef __INTROS_SEM_H
#define __INTROS_SEM_H

#include "oskernel.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#define semBinary    (  1U ) // binary semaphore
#define semCounting  ( ~0U ) // counting semaphore

/******************************************************************************
 *
 * Name              : semaphore
 *
 ******************************************************************************/

typedef struct __sem sem_t, * const sem_id;

struct __sem
{
	unsigned count; // semaphore's current value
	unsigned limit; // semaphore's value limit
};

/******************************************************************************
 *
 * Name              : _SEM_INIT
 *
 * Description       : create and initialize a semaphore object
 *
 * Parameters
 *   init            : initial value of semaphore counter
 *   limit           : maximum value of semaphore counter
 *                     semBinary: binary semaphore
 *                     semCounting: counting semaphore
 *                     otherwise: limited semaphore
 *
 * Return            : semaphore object
 *
 * Note              : for internal use
 *
 ******************************************************************************/

#define               _SEM_INIT( init, limit ) { init, limit }

/******************************************************************************
 *
 * Name              : _VA_SEM
 *
 * Description       : calculate maximum value of semaphore counter from optional parameter
 *
 * Note              : for internal use
 *
 ******************************************************************************/

#define               _VA_SEM( _limit ) \
                       ( (_limit + 0) ? (_limit + 0) : (semCounting) )

/******************************************************************************
 *
 * Name              : OS_SEM
 *
 * Description       : define and initialize a semaphore object
 *
 * Parameters
 *   sem             : name of a pointer to semaphore object
 *   init            : initial value of semaphore counter
 *   limit           : (optional) maximum value of semaphore counter
 *                     semBinary: binary semaphore
 *                     semCounting: counting semaphore (default)
 *                     otherwise: limited semaphore
 *
 ******************************************************************************/

#define             OS_SEM( sem, init, ... )                                      \
                       sem_t sem##__sem = _SEM_INIT( init, _VA_SEM(__VA_ARGS__) ); \
                       sem_id sem = & sem##__sem

/******************************************************************************
 *
 * Name              : static_SEM
 *
 * Description       : define and initialize a static semaphore object
 *
 * Parameters
 *   sem             : name of a pointer to semaphore object
 *   init            : initial value of semaphore counter
 *   limit           : (optional) maximum value of semaphore counter
 *                     semBinary: binary semaphore
 *                     semCounting: counting semaphore (default)
 *                     otherwise: limited semaphore
 *
 ******************************************************************************/

#define         static_SEM( sem, init, ... )                                      \
                static sem_t sem##__sem = _SEM_INIT( init, _VA_SEM(__VA_ARGS__) ); \
                static sem_id sem = & sem##__sem

/******************************************************************************
 *
 * Name              : SEM_INIT
 *
 * Description       : create and initialize a semaphore object
 *
 * Parameters
 *   init            : initial value of semaphore counter
 *   limit           : (optional) maximum value of semaphore counter
 *                     semBinary: binary semaphore
 *                     semCounting: counting semaphore (default)
 *                     otherwise: limited semaphore
 *
 * Return            : semaphore object
 *
 * Note              : use only in 'C' code
 *
 ******************************************************************************/

#ifndef __cplusplus
#define                SEM_INIT( init, ... ) \
                      _SEM_INIT( init, _VA_SEM(__VA_ARGS__) )
#endif

/******************************************************************************
 *
 * Name              : SEM_CREATE
 * Alias             : SEM_NEW
 *
 * Description       : create and initialize a semaphore object
 *
 * Parameters
 *   init            : initial value of semaphore counter
 *   limit           : (optional) maximum value of semaphore counter
 *                     semBinary: binary semaphore
 *                     semCounting: counting semaphore (default)
 *                     otherwise: limited semaphore
 *
 * Return            : pointer to semaphore object
 *
 * Note              : use only in 'C' code
 *
 ******************************************************************************/

#ifndef __cplusplus
#define                SEM_CREATE( init, ... ) \
           (sem_t[]) { SEM_INIT  ( init, _VA_SEM(__VA_ARGS__) ) }
#define                SEM_NEW \
                       SEM_CREATE
#endif

/******************************************************************************
 *
 * Name              : sem_init
 *
 * Description       : initialize a semaphore object
 *
 * Parameters
 *   sem             : pointer to semaphore object
 *   init            : initial value of semaphore counter
 *   limit           : maximum value of semaphore counter
 *                     semBinary: binary semaphore
 *                     semCounting: counting semaphore
 *                     otherwise: limited semaphore
 *
 * Return            : none
 *
 ******************************************************************************/

void sem_init( sem_t *sem, unsigned init, unsigned limit );

/******************************************************************************
 *
 * Name              : sem_wait
 *
 * Description       : try to lock the semaphore object,
 *                     wait indefinitely if the semaphore object can't be locked immediately
 *
 * Parameters
 *   sem             : pointer to semaphore object
 *
 * Return            : none
 *
 ******************************************************************************/

void sem_wait( sem_t *sem );

/******************************************************************************
 *
 * Name              : sem_take
 *
 * Description       : try to lock the semaphore object,
 *                     don't wait if the semaphore object can't be locked immediately
 *
 * Parameters
 *   sem             : pointer to semaphore object
 *
 * Return
 *   E_SUCCESS       : semaphore object was successfully locked
 *   E_FAILURE       : semaphore object can't be locked immediately
 *
 ******************************************************************************/

unsigned sem_take( sem_t *sem );

/******************************************************************************
 *
 * Name              : sem_send
 *
 * Description       : try to unlock the semaphore object,
 *                     wait indefinitely if the semaphore object can't be unlocked immediately
 *
 * Parameters
 *   sem             : pointer to semaphore object
 *
 * Return            : none
 *
 ******************************************************************************/

void sem_send( sem_t *sem );

/******************************************************************************
 *
 * Name              : sem_give
 *
 * Description       : try to unlock the semaphore object,
 *                     don't wait if the semaphore object can't be unlocked immediately
 *
 * Parameters
 *   sem             : pointer to semaphore object
 *
 * Return
 *   E_SUCCESS       : semaphore object was successfully unlocked
 *   E_FAILURE       : semaphore object can't be unlocked immediately
 *
 ******************************************************************************/

unsigned sem_give( sem_t *sem );

#ifdef __cplusplus
}
#endif

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus

/******************************************************************************
 *
 * Class             : Semaphore
 *
 * Description       : create and initialize a semaphore object
 *
 * Constructor parameters
 *   init            : initial value of semaphore counter
 *   limit           : maximum value of semaphore counter
 *                     semBinary: binary semaphore
 *                     semCounting: counting semaphore (default)
 *                     otherwise: limited semaphore
 *
 ******************************************************************************/

struct Semaphore : public __sem
{
	Semaphore( const unsigned _init, const unsigned _limit = semCounting ): __sem _SEM_INIT(_init, _limit) {}

	void     wait( void ) {        sem_wait(this); }
	unsigned take( void ) { return sem_take(this); }
	void     send( void ) {        sem_send(this); }
	unsigned give( void ) { return sem_give(this); }
};

/******************************************************************************
 *
 * Class             : BinarySemaphore
 *
 * Description       : create and initialize a binary semaphore object
 *
 * Constructor parameters
 *   init            : initial value of semaphore counter
 *
 ******************************************************************************/

struct BinarySemaphore : public Semaphore
{
	BinarySemaphore( const unsigned _init = 0 ): Semaphore(_init, semBinary) {}
};

/******************************************************************************
 *
 * Class             : CountingSemaphore
 *
 * Description       : create and initialize a counting semaphore object
 *
 * Constructor parameters
 *   init            : initial value of semaphore counter
 *
 ******************************************************************************/

struct CountingSemaphore : public Semaphore
{
	CountingSemaphore( const unsigned _init = 0 ): Semaphore(_init, semCounting) {}
};

#endif//__cplusplus

/* -------------------------------------------------------------------------- */

#endif//__INTROS_SEM_H
