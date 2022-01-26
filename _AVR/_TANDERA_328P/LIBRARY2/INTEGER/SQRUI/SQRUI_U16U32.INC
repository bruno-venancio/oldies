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
;| S Q U A R E                                                          |
;+----------------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM                     |
;+----------------------------------------------------------------------+
;+----------------------------------------------------------------+
;| SQRUI                                                          |
;| Compute a Square unsigned 16bits                               |
;|                                                                |
;| Input  : AccH:Acc unsigned 16bit value                         |
;| Output : R7:R6:R5:R4 unsigned 32bit                            |
;| Destroy: r0,r1,r2,r3,Flags                                     |
;| Time   : Min=22+3 rcall  Max=22+3 rcall average 1.4us at 16Mhz | 
;+----------------------------------------------------------------+
#ifndef __INTEGER_SQRTUI_U16U32__
        #define __INTEGER_SQRTUI_U16U32__
        .message "[ (MATH) INTEGER SQUARE ]"
_SQRUI_U16U32:
        clr     r2
        clr     r3                              ;zero
        clr     r4
        clr     r5
        clr     r6
        clr     r7
        mul     Acc,Acc
        movw    r5:r4,r1:r0
        mul     Acc,AccH
        lsl     r0
        rol     r1
        rol     r2
        add     r5,r0
        adc     r6,r1
        adc     r7,r3                           ;zero
        mul     AccH,AccH
        add     r6,r0
        adc     r7,r1
        ret
#endif
.exit

