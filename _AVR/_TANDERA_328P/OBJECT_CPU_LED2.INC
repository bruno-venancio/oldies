;						 ///////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\
;						//						\\
;-----------------------------------------------||  	       C P U  G O O D  L E D 		||-----------------------------------------------------
;						\\						//
;						 \\\\\\\\\\\\\\\\\\\\\\\/////////////////////////

;>--------------------------------------------------------------<
;| OBJECT     : CPULED2						|
;>--------------------------------------------------------------<
;| DESCRIPTION: This object set state of cpu led		|
;>--------------------------------------------------------------<

;>----------------------<
;| OBJECT DEFINITIONS	|
;>----------------------<

;-------------------------< PORT DEFINITONS >----------------------------

.EQU	_CPULED2_PORT_OUTPUT	= PORTD
.EQU	_CPULED2_PORT_DIR	= DDRD 
.EQU	_CPULED2_PORT_INPUT	= PIND


;-------------------------< CONSTANT DEFINITONS >------------------------

.EQU	_CPULED2_LED_BIT		= 3 

	;>--------------------------------------------------------------<
	;| OBJECT     : CPULED						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : Initialize Object				|
	;>--------------------------------------------------------------<
	;| INPUT      :	None						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	Disable interrupts during initialization	|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|


_CPULED2_INI:
		push	Acc						;save useds
		in	Acc,SREG					;save SREG
	    cli						    	;disable interrupts during initialization
		push	Acc
	;lds	Acc,_CPULED_PORT_OUTPUT					;get old state
;	andi	Acc,~(1<<_CPULED_LED_BIT)			;set led enable
;	out	_CPULED_PORT_OUTPUT,Acc
		in	Acc,_CPULED2_PORT_DIR				;get old state
		ori	Acc,(1<<_CPULED2_LED_BIT)			;set as output
	  	out	_CPULED2_PORT_DIR,Acc
		pop	Acc						;restore SREG state
		out	SREG,Acc
		pop	Acc						;restore useds
		ret


	;>--------------------------------------------------------------<
	;| OBJECT     : CPULED2						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : Altenate CPU LED state 				|
	;>--------------------------------------------------------------<
	;| INPUT      :	None						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_CPULED2_ALTERNATE:
		push	Acc						;save useds
		in	Acc,SREG					;get SREG
		cli							;disable interrupts
		pushaw
		
        in Acc,_CPULED2_PORT_OUTPUT
        com Acc
        
		bst Acc,_CPULED2_LED_BIT

        in Acc,_CPULED2_PORT_OUTPUT
        bld Acc,_CPULED2_LED_BIT
		out _CPULED2_PORT_OUTPUT,Acc

		popaw				
		out	SREG,Acc					;restore SREG
		pop	Acc
		ret



	;>--------------------------------------------------------------<
	;| OBJECT     : CPULED2						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : Altenate CPU LED to ON 				|
	;>--------------------------------------------------------------<
	;| INPUT      :	None						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_CPULED2_ON:
		push	Acc						;save useds
		in	Acc,SREG					;get SREG
		cli							;disable interrupts
		pushaw
		
		in AccH,_CPULED2_PORT_OUTPUT
		set
        bld AccH,_CPULED2_LED_BIT
		out _CPULED2_PORT_OUTPUT,AccH

		popaw				
		out	SREG,Acc					;restore SREG
		pop	Acc
		ret
 


	;>--------------------------------------------------------------<
	;| OBJECT     : CPULED2						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : Altenate CPU LED to OFF 				|
	;>--------------------------------------------------------------<
	;| INPUT      :	None						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_CPULED2_OFF:
		push	Acc						;save useds
		in	Acc,SREG					;get SREG
		cli							;disable interrupts
		pushaw
		
		in  AccH,_CPULED2_PORT_OUTPUT
		clt
        bld AccH,_CPULED2_LED_BIT
		out _CPULED2_PORT_OUTPUT,AccH

		popaw				
		out	SREG,Acc					;restore SREG
		pop	Acc
		ret
 

















