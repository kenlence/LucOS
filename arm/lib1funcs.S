/*
 * linux/arch/arm/lib/lib1funcs.S: Optimized ARM division routines
 *
 * Author: Nicolas Pitre <nico@fluxnic.net>
 *   - contributed to gcc-3.4 on Sep 30, 2003
 *   - adapted for the Linux kernel on Oct 2, 2003
 */

/* Copyright 1995, 1996, 1998, 1999, 2000, 2003 Free Software Foundation, Inc.

This file is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file into combinations with other programs,
and to distribute those combinations without any restriction coming
from the use of this file.  (The General Public License restrictions
do apply in other respects; for example, they cover modification of
the file, and distribution when not linked into a combine
executable.)

This file is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */


#include "linkage.h"
#include "unwind.h"
#include "assembler.h"

.macro ARM_DIV_BODY dividend, divisor, result, curbit
	clz	\curbit, \divisor
	clz	\result, \dividend
	sub	\result, \curbit, \result
	mov	\curbit, #1
	mov	\divisor, \divisor, lsl \result
	mov	\curbit, \curbit, lsl \result
	mov	\result, #0

	@ Division loop
1:	cmp	\dividend, \divisor
	subhs	\dividend, \dividend, \divisor
	orrhs	\result,   \result,   \curbit
	cmp	\dividend, \divisor,  lsr #1
	subhs	\dividend, \dividend, \divisor, lsr #1
	orrhs	\result,   \result,   \curbit,  lsr #1
	cmp	\dividend, \divisor,  lsr #2
	subhs	\dividend, \dividend, \divisor, lsr #2
	orrhs	\result,   \result,   \curbit,  lsr #2
	cmp	\dividend, \divisor,  lsr #3
	subhs	\dividend, \dividend, \divisor, lsr #3
	orrhs	\result,   \result,   \curbit,  lsr #3
	cmp	\dividend, #0			@ Early termination?
	movnes	\curbit,   \curbit,  lsr #4	@ No, any more bits to do?
	movne	\divisor,  \divisor, lsr #4
	bne	1b
.endm


.macro ARM_DIV2_ORDER divisor, order
	clz	\order, \divisor
	rsb	\order, \order, #31
.endm


.macro ARM_MOD_BODY dividend, divisor, order, spare
	clz	\order, \divisor
	clz	\spare, \dividend
	sub	\order, \order, \spare
	mov	\divisor, \divisor, lsl \order

	@ Perform all needed substractions to keep only the reminder.
	@ Do comparisons in batch of 4 first.
	subs	\order, \order, #3		@ yes, 3 is intended here
	blt	2f

1:	cmp	\dividend, \divisor
	subhs	\dividend, \dividend, \divisor
	cmp	\dividend, \divisor,  lsr #1
	subhs	\dividend, \dividend, \divisor, lsr #1
	cmp	\dividend, \divisor,  lsr #2
	subhs	\dividend, \dividend, \divisor, lsr #2
	cmp	\dividend, \divisor,  lsr #3
	subhs	\dividend, \dividend, \divisor, lsr #3
	cmp	\dividend, #1
	mov	\divisor, \divisor, lsr #4
	subges	\order, \order, #4
	bge	1b

	tst	\order, #3
	teqne	\dividend, #0
	beq	5f

	@ Either 1, 2 or 3 comparison/substractions are left.
2:	cmn	\order, #2
	blt	4f
	beq	3f
	cmp	\dividend, \divisor
	subhs	\dividend, \dividend, \divisor
	mov	\divisor,  \divisor,  lsr #1
3:	cmp	\dividend, \divisor
	subhs	\dividend, \dividend, \divisor
	mov	\divisor,  \divisor,  lsr #1
4:	cmp	\dividend, \divisor
	subhs	\dividend, \dividend, \divisor
5:
.endm


ENTRY(__udivsi3)
ENTRY(__aeabi_uidiv)
UNWIND(.fnstart)

	subs	r2, r1, #1
	reteq	lr
	bcc	Ldiv0
	cmp	r0, r1
	bls	11f
	tst	r1, r2
	beq	12f

	ARM_DIV_BODY r0, r1, r2, r3

	mov	r0, r2
	ret	lr

11:	moveq	r0, #1
	movne	r0, #0
	ret	lr

12:	ARM_DIV2_ORDER r1, r2

	mov	r0, r0, lsr r2
	ret	lr

UNWIND(.fnend)
ENDPROC(__udivsi3)
ENDPROC(__aeabi_uidiv)

ENTRY(__umodsi3)
UNWIND(.fnstart)

	subs	r2, r1, #1			@ compare divisor with 1
	bcc	Ldiv0
	cmpne	r0, r1				@ compare dividend with divisor
	moveq   r0, #0
	tsthi	r1, r2				@ see if divisor is power of 2
	andeq	r0, r0, r2
	retls	lr

	ARM_MOD_BODY r0, r1, r2, r3

	ret	lr

UNWIND(.fnend)
ENDPROC(__umodsi3)

ENTRY(__divsi3)
ENTRY(__aeabi_idiv)
UNWIND(.fnstart)

	cmp	r1, #0
	eor	ip, r0, r1			@ save the sign of the result.
	beq	Ldiv0
	rsbmi	r1, r1, #0			@ loops below use unsigned.
	subs	r2, r1, #1			@ division by 1 or -1 ?
	beq	10f
	movs	r3, r0
	rsbmi	r3, r0, #0			@ positive dividend value
	cmp	r3, r1
	bls	11f
	tst	r1, r2				@ divisor is power of 2 ?
	beq	12f

	ARM_DIV_BODY r3, r1, r0, r2

	cmp	ip, #0
	rsbmi	r0, r0, #0
	ret	lr

10:	teq	ip, r0				@ same sign ?
	rsbmi	r0, r0, #0
	ret	lr

11:	movlo	r0, #0
	moveq	r0, ip, asr #31
	orreq	r0, r0, #1
	ret	lr

12:	ARM_DIV2_ORDER r1, r2

	cmp	ip, #0
	mov	r0, r3, lsr r2
	rsbmi	r0, r0, #0
	ret	lr

UNWIND(.fnend)
ENDPROC(__divsi3)
ENDPROC(__aeabi_idiv)

ENTRY(__modsi3)
UNWIND(.fnstart)

	cmp	r1, #0
	beq	Ldiv0
	rsbmi	r1, r1, #0			@ loops below use unsigned.
	movs	ip, r0				@ preserve sign of dividend
	rsbmi	r0, r0, #0			@ if negative make positive
	subs	r2, r1, #1			@ compare divisor with 1
	cmpne	r0, r1				@ compare dividend with divisor
	moveq	r0, #0
	tsthi	r1, r2				@ see if divisor is power of 2
	andeq	r0, r0, r2
	bls	10f

	ARM_MOD_BODY r0, r1, r2, r3

10:	cmp	ip, #0
	rsbmi	r0, r0, #0
	ret	lr

UNWIND(.fnend)
ENDPROC(__modsi3)

ENTRY(__div0)
UNWIND(.fnstart)
	bl	__div0

UNWIND(.fnend)
ENDPROC(__div0)

Ldiv0:
UNWIND(.fnstart)
UNWIND(.pad #4)
UNWIND(.save {lr})
	str	lr, [sp, #-8]!
	bl	__div0
	mov	r0, #0			@ About as wrong as it could be.
	ldr	pc, [sp], #8
UNWIND(.fnend)
ENDPROC(Ldiv0)
