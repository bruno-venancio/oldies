;<$DS_HEADER>
;+--------------------------------------------------------------------------------+              
;|João D´Artagnan A. Oliveira Programmer and Author;                              |
;|                                                                                |
;| All rights reserved.                                                           |
;|                                                                                |
;|   Redistribution and use in source and binary forms, with or without           |
;|  modification, are permitted provided that the following conditions are met:   |
;|                                                                                |
;|   * Redistributions of source code must retain the above copyright             |
;|     notice, this list of conditions and the following disclaimer.              |
;|                                                                                |
;|   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  |
;|   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    |
;|   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   |
;|   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE     |
;|   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR          |
;|   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         |
;|   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS     |
;|   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      |
;|   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)      |
;|   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   |
;|   POSSIBILITY OF SUCH DAMAGE.                                                  |
;|                                                                                |
;|   Brasília, Brazil November 30,2015  06:28 am                                  |
;+--------------------------------------------------------------------------------+
;</$DS_HEADER>
;+----------------------------------------------------------------------+
;|                                                                      |
;| DATE:17/12/2001 VERSION 1.0                                          |
;>----------------------------------------------------------------------<
;| I N T E G E R  R O U T I N E S                                       |
;| S Q U A R E  R O O T                                                 |
;+----------------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM                     |
;+----------------------------------------------------------------------+
;+----------------------------------------------------------------+
;| SQRTL_U32U16                                                   |
;| Compute a Square Root of 32bits unsigned value and obtain      |
;| 16bits unsigned value and only mega MEGA AVRS                  | 
;|                                                                |
;| Input  : r3:r2:r1:r0			unsigned 32it value               |
;| Output : AccH:Acc            unsigned 16bit result             |
;| Destroy: R1,R2,R3,R4,R5,R7,R8,R9,R10,R11,R12,r13,Flags         |
;| Time   : Min=96+3 rcall  Max=112+3 rcall average 6.5us at 16Mhz| 
;+----------------------------------------------------------------+
#ifndef __INTEGER_SQRTL_U32U16__
        #define __INTEGER_SQRTL_U32U16__
        .message "[ (MATH) INTEGER SQUARE ROOT LONG FOR MEGA ]"
_SQRTL_U32U16:
		mov		r8,r0					;radicand=(r11:r10:r9:r8 input value to compare +1
		mov		r9,r1
		mov		r10,r2
		mov		r11,r3
		clr		r0						;to add 1
		inc		r0
		clr		r1						;zero
		add		r8,r0					;radicand+1
		adc		r9,r1
		adc		r10,r1
		adc		r11,r1
		cp		r8,r1					;check if after add result is 0
		cpc		r9,r1
		cpc		r10,r1
		cpc		r11,r1
		breq	_SQRTL_U32U16_30		;yes, return 0xffff
		clr		Acc						;accumulator
		clr		AccH
		clr		r12						;guess registers	= 0x8000
		clr		r13
		sec
		ror		r13
_SQRTL_U32U16_10:
		or		Acc,r12					;set new guess into accumulator
		or		AccH,r13
		rcall	_SQRUI_U16U32			;compute x^2
		cp		r4,r8					;square>value?
		cpc		r5,r9
		cpc		r6,r10
		cpc		r7,r11
		brlo	_SQRTL_U32U16_20		;no, branch
		com		r12						;else clear bit quess
		com		r13
		and		Acc,r12
		and		AccH,r13
		com		r12
		com		r13
_SQRTL_U32U16_20:
		lsr		r13						;next guess bit
		ror		r12
		brcc	_SQRTL_U32U16_10		;until done
		ret								;result int AccH:Acc
_SQRTL_U32U16_30:
		ldi		Acc,0xff				;return max value
		ldi		AccH,0xff
		ret
#endif
.exit

