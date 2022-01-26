
;+-----------------------------------------------------------------+
;| Purpose: Find out the square root of the 16-bit number          |
;| 8-Bit version by Joao Dartagnan Antunes Oliveira (04/20/1993)   |
;| 16-Bit and AVR asm conv by Bruno Marcio Diogo Venancio (2007/14)|
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
_SQRTBI:
	
        sts _SQRX_L,Acc
		sts _SQRX_H,AccH
		
        ldi Acc,1
 		sts _COUNT_L,Acc              ;COUNTER <- 1

        ldi Acc,0
 		sts _COUNT_H,Acc

        ldi Acc,0                     ;ACUMULATOR <- 0
 		sts _ACC_L,Acc

		ldi Acc,0
		sts _ACC_H,Acc

sqrtbi00:
		lds Acc,_ACC_L                ;ACUMULATOR >= X ?
	    lds AccH,_SQRX_L
		sub Acc,AccH
		
		lds Acc,_ACC_H
		lds AccH,_SQRX_H
		sbc Acc,AccH

		brcc sqrtbi01                 ;IF YES, END

        lds Acc,_COUNT_L
		ldi AccH,2                   ;COUNTER <- COUNTER + 2
        Add Acc,AccH
		sts _COUNT_L,Acc

		lds Acc,_COUNT_H
        ldi AccH,0
		adc Acc,AccH
		sts _COUNT_H,Acc

		clc                           ;Sum odd numbers count
		lds Acc,_ACC_L
		lds AccH,_COUNT_L
		add Acc,AccH
		sts _ACC_L,Acc

        lds Acc,_ACC_H
		lds AccT,_COUNT_H
		adc Acc,AccT
		sts _ACC_H,Acc

		jmp sqrtbi00

sqrtbi01:
        clc
		lds Acc,_COUNT_H               ;COUNTER /2 = SQRT(X)
		ror Acc

		lds Acc,_COUNT_L
		ror Acc

		ret

		   




		   










