
;  +-------------------------------------------------------------------------+
;  | PROGRAMMER: Bruno Marcio Diogo Venancio ( bruno.marcio@bol.com.br )     |
;  |             Joao Dartagnan A. de Oliveira (dartagnanmath@gmail.com)
;  | BRAZIL 07/20/2008                                                       |
;  |          THIS CODE CAN BE FREELY DISTRIBUTED WITHOUT CHANGES            |
;  +-------------------------------------------------------------------------+

;+----------------------------------------------------------------------+
;| DIV_U16U16U16F16							|
;| Divide unsigned 16bits/unsigned 16 bits result unsigned 16bits	|
;| integer part and 16bits fraction part				|
;|								  	|
;| Input  : R1:R0  Dividend				  		|
;|	    R3:R2  Divisor				  		|
;| Output : R1:R0  quotient fraction part		  		|
;|	    R3:R2  quotient integer part				|
;| Destroy: Flags,R4,R5,R6,R7					  	|
;+----------------------------------------------------------------------+

_DIV_U16U16U16F16:
		push	Work			;Save Work
		mov	work,#33		;Quotient bit count

		mov     a,R2    		;Move divisor to r5:r4
		mov     R4,a

                mov     a,R3
                mov     R5,a

                mov     a,R0    		;Move dividend to r3:r2
                mov     R2,a

                mov     a,R1
                mov     R3,a


                mov R0,#0                       ;Clear my accumulador
                mov R1,#0
                mov R6,#0
                mov R7,#0
		clr     C 			;Clear first carry

_DIV_UF1600:
                mov     a,R0       ;Rotate left dividend to see if fit inside quotient
                rlc     a
                mov     R0,a

                mov     a,R1
                rlc     a
                mov     R1,a

                mov     a,R2
                rlc     a
                mov     R2,a

                mov     a,R3
                rlc     a
                mov     R3,a

                mov     a,R6
                rlc     a
                mov     R6,a

                mov     a,R7
                rlc     a
                mov     R7,a

                clr C

                mov a,R6                       ; Fit?
                mov b,R4
                subb a,b

                mov a,R7
                mov b,R5
                subb a,b

                jc _DIV_UF1610                 ;no, branch

                mov a,R6                       ;else subtract divisor from dividend
                subb a,R4
                mov R6,a

                mov a,R7
                subb a,R5
                mov R7,a

_DIV_UF1610:
                djnz    Work,_DIV_UF1600  ;next bit until all done
                
                mov     a,R0
                cpl     a
                mov     R0,a

                mov     a,R1
                cpl     a
                mov     R1,a

                mov     a,R2
                cpl     a
                mov     R2,a

                mov     a,R3   ;Because all bit are inverted during computation
                cpl     a
                mov     R3,a


		pop	Work  ;Restore Work
		
		ret
		
		
		
		




;+----------------------------------------------------------------------+
;| DIV_U32U32U32							|
;| Divide unsigned 32bits/unsigned 32bits result unsigned 32bits	|
;|								  	|
;| Input  : R0:R1:R2 :R3  Dividend				  	|
;|	LSB R4:R5:R6 :R7  Divisor				  	|
;| Output : R0:R1:R2 :R3  Quotient				  	|
;|	    R8:R9:R10:R11 Rest						|
;| Destroy: Flags						  	|
;+----------------------------------------------------------------------+

_DIV_U32U32U32:
		push	Work			;Save Work
                mov     Work,#32                 ;quotient bit count
		mov	R8,#0			;clear rest
		mov	R9,#0
		mov	R10,#0
		mov 	R11,#0
		clr     C
_DIV_U32U32U32_00:
                mov     a,R0            	;see if dividend fit inside divisor
                rlc     a
                mov     R0,a

                mov     a,R1
                rlc     a
                mov     R1,a

                mov     a,R2
                rlc     a
                mov     R2,a

                mov     a,R3
                rlc     a
                mov     R3,a

                mov     a,R8
                rlc     a
                mov     R8,a

                mov     a,R9
                rlc     a
                mov     R9,a

                mov     a,R10
                rlc     a
                mov     R10,a

                mov     a,R11
                rlc     a
                mov     R11,a
                
                clr     C
                
                mov     a,R8     ;fit ?
                subb    a,R4

                mov     a,R9
                subb    a,R5

                mov     a,R10
                subb    a,R6

                mov     a,R11
                subb    a,R7
                
                jc      _DIV_U32U32U32_10       ;branch if no
                
                mov     a,R8                    ;else subtract divisor from dividend
                subb    a,R4
                mov     R8,a
                
                mov     a,R9
                subb    a,R5
                mov     R9,a
                
                mov     a,R10
                subb    a,R6
                mov     R10,a
                
                mov     a,R11
                subb    a,R7
                mov     R11,a

_DIV_U32U32U32_10:
                djnz    Work,_DIV_U32U32U32_00  ;next bit ,until all done
                
                mov     a,R0             ;place last bit result into quotient
                rlc     a
                mov     R0,a

                mov     a,R1
                rlc     a
                mov     R1,a

                mov     a,R2
                rlc     a
                mov     R2,a

                mov     a,R3
                rlc     a
                mov     R3,a

		mov     a,R0			;because all bits are inverted during computation
                cpl     a
                mov     R0,a
                
                mov     a,R1
                cpl     a
                mov     R1,a

                mov     a,R2
                cpl     a
                mov     R2,a

                mov     a,R3
                cpl     a
                mov     R3,a

		pop	Work
		ret


		
		
		


;--=| MULTIPLICA 16X16 BITS |=-------------------------------------
;
; ENTRADA: R0,R1  HIGH E LOW BYTE DO OPERANDO 1
;          R2,R3  HIGH E LOW BYTE DO OPERANDO 2
;
; SAIDA:   R7,R6,R5,R4 PRODUTO R7 MSB e R4 LSB
;
; METODO UTILIZADO
;
;
;         R2 R3
;       X R0 R1         multiplica produtos parciais
;     -----------
;           R1xR3       soma produtos parciais
;        R1xR2
;        R0xR3
;     R0xR2
; -----------------------------------------------------------------

_MUL16:

        push acc
        push psw
        push b

        mov r4,#0
        mov r5,#0
        mov r6,#0
        mov r7,#0
        mov a,r1                ;multiplica R1xR3
        mov b,r3
        mul ab
        mov r4,a
        mov r5,b
        mov a,r1                ;multiplica R1xR2
        mov b,r2
        mul ab
        add a,r5
        mov r5,a
        mov a,#0
        addc a,b
        mov r6,a
        mov a,r0                ;multiplica r0,r3
        mov b,r3
        mul ab
        add a,r5
        mov r5,a
        mov a,r6
        addc a,b
        mov r6,a
        push psw
        mov a,r0                ;multiplica r0,r2
        mov b,r2
        mul ab
        pop psw
        addc a,r6
        mov r6,a
        mov a,#0
        addc a,b
        mov r7,a

        pop b
        pop psw
        pop acc

        ret


;+-----------------------------------------------------------------+
;| Purpose: Find out the square root of the 16-bit number          |
;| 8-Bit version by Joao Dartagnan Antunes Oliveira (04/20/1993)   |
;| 16-Bit modification by Bruno Marcio Diogo Venancio (08/10/2007) |
;| Email: bruno.marcio@bol.com.br                                  |
;| Brazil                                                     v1.1 |
;| Input: B (MSB) and A (LSB)   (binary)                           |
;| Output: A                                                       |
;| Destroy : R2,R3,R4,R5,R6,R7                                     |
;|       THIS CODE CAN BE FREELY DISTRIBUTED WITHOUT CHANGES       |
;+-----------------------------------------------------------------+
;
;
; METHOD:
;
; Add in one accumulator an odd number counter until it cotains a value great or
; equal than X , at this moment, the counter value divided for 2 will be SQRT(X)
;

SQRTBI:

	mov SQRX_L,a
	mov SQRX_H,b

	mov COUNT_L,#1	        ;COUNTER <- 1
	mov COUNT_H,#0

	mov ACC_L,#0    	;ACUMULATOR <- 0
	mov ACC_H,#0

sqrtbi00:
	clr c
	mov a,ACC_L     	;ACUMULATOR >= X ?
	subb a,SQRX_L

        mov a,ACC_H
	subb a,SQRX_H

	jnc sqrtbi01	        ;IF YES, END

        clr C
        mov a,COUNT_L
        add a,#2                ;COUNTER <- COUNTER + 2
        mov COUNT_L,a

        mov a,COUNT_H
        addc a,#0
        mov COUNT_H,a


        clr C                  ;Sum odd numbers count
        mov a,ACC_L
        add a,COUNT_L
        mov ACC_L,a

        mov a,ACC_H
        addc a,COUNT_H
        mov ACC_H,a

	sjmp sqrtbi00

sqrtbi01:

        clr C

        mov a,COUNT_H            ;COUNTER /2 = SQRT(X)
        rrc a

        mov a,COUNT_L
        rrc a

	ret
	
	


;****************************************************************************
;
;  Description:
;	Unsigned Divide Of R2/3 By R4/5
;
;  Entry Requirements:
;	Divisor In R4/5
;	Dividend In R2/3
;
;  On Exit:
;	Quotient In R2/3
;	Remainder In R4/5
;
;  Affected:
;	PSW.CY, PSW.Z, PSW.P, Acc, B, R0, R1, R2, R3, R4, R5
;
;  Stack:
;	X Bytes, Not Including Space Used By Called Routines
;
;  Comments:
;	Swiped From A Now Defunct 'C' Compiler
;
_DIV16:
		clr	a
		mov	b,a			; initialize count
		mov	r0,a			; zero quotient
		mov	r1,a
		mov	a,r2			; check for zero dividend
		orl	a,r3
                jz      lp8
		mov	a,r4			; check for zero divisor
		orl	a,r5
                jnz     lp3
		ret
;
lp1:
                mov     a,r2
		clr	c
		subb	a,r4			; is divisor greater than dividend yet
                jc      lp4                    ; yes, go no further
                jnz     lp2
		mov	a,r3
		subb	a,r5
                jc      lp4
;
lp2:
                mov     a,r5                    ; shift divisor up one bit
		clr	c
		rlc	a
		mov	r5,a
		mov	a,r4
		rlc	a
		mov	r4,a
;
lp3:
                inc     b                       ; increment count
		mov	a,r4			; check for safe to shift some more
                jnb     acc.7,lp1              ; loop if top bit clear
;
lp4:
                mov     a,r2
		clr	c
                subb    a,r4            ; is divisor greater than dividend
                jc      lp5 
                jnz     lp6
		mov	a,r3
		subb	a,r5
                jnc     lp6
;
lp5:
                clr     c
                sjmp    lp7
;
lp6:
                clr     c                       ; subtract divisor from dividend
		mov	a,r3
		subb	a,r5
		mov	r3,a
		mov	a,r2
		subb	a,r4
		mov	r2,a
		setb	c			; now set bit for quotient
;
lp7:
                mov     a,r1
		rlc	a
		mov	r1,a
		mov	a,r0
		rlc	a
		mov	r0,a

		mov	a,r4			; shift divisor down
		clr	c
		rrc	a
		mov	r4,a
		mov	a,r5
		rrc	a
		mov	r5,a
                djnz    b,lp4                  ; and continue with the rest
;
lp8:
                mov     5,r3
		mov	4,r2
		mov	2,r0
		mov	3,r1
		ret


;+----------------------------------------------------------------------+
;| MUL_U32U32U32							|
;| Multiply unsigned 32bits x unsigned 32bits result unsigned 32bits	|
;|								  	|
;| Input  : R0:R1:R2:R3   1st operand				  	|
;|	    R4:R5:R6:R7   2nd operand				  	|
;| Output : R8:R9:R10:R11 Result				  	|
;| Destroy: R0,R1,R2,R3,R4,R5,R6,R7,Flags			  	|
;+----------------------------------------------------------------------+

_MUL_U32U32U32:
		push	B       		;Save useds

		mov	R8,#0                   ;clear accumulator
                mov     R9,#0
                mov     R10,#0
                mov     R11,#0

		mov	B,#32

_MUL_U32U32U32_00:

                clr     C

		mov     a,R3
                rrc	a 			;get out bit of 1st operand
                mov     R3,a

                mov     a,R2
		rrc	a
		mov     R2,a

                mov     a,R1
		rrc	a
		mov     R1,a

                mov     a,R0
		rrc	a
		mov     R0,a

		jnc 	_MUL_U32U32U32_10	;branch if out bit zero

                clr     C

                mov     a,R8
                add	a,R4			;else accumulate
                mov     R8,a

                mov     a,R9
                addc	a,R5
                mov     R9,a

                mov     a,R10
		addc	a,R6
		mov     R10,a

                mov     a,R11
		addc	a,R7
		mov     R11,a

_MUL_U32U32U32_10:

                clr C

                mov     a,R4
                rlc     a
                mov     R4,a    		;next power

                mov     a,R5
                rlc     a
                mov     R5,a

    		mov     a,R6
    		rlc     a
    		mov     R6,a

		mov     a,R7
		rlc      a
		mov     R7,a

		djnz B,_MUL_U32U32U32_00

		pop	B

		ret
