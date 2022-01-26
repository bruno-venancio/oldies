;						 ///////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\
;						//						\\
;-----------------------------------------------||  	       C P U  G O O D  L E D 		||-----------------------------------------------------
;						\\						//
;						 \\\\\\\\\\\\\\\\\\\\\\\/////////////////////////

;>--------------------------------------------------------------<
;| OBJECT     : BUZZER						|
;>--------------------------------------------------------------<
;| DESCRIPTION: This object set state of BUZZER		|
;>--------------------------------------------------------------<

;>----------------------<
;| OBJECT DEFINITIONS	|
;>----------------------<
/*
;-------------------------< PORT DEFINITONS >----------------------------
 .EQU	_BUZZER_PWM_OUTPUT	= PORTD			 			
 .EQU	_BUZZER_PWM_DIR	=     DDRD								
 .EQU	_BUZZER_PWM_INPUT	= PIND
 .EQU   _BUZZER_PWM_BIT = 6		
 

;-------------------------< CONSTANT DEFINITONS >------------------------


	;>--------------------------------------------------------------<
	;| OBJECT     : BUZZER						|
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

_BUZZER_INI:
		push	Acc						;save useds
		in	Acc,SREG					;save SREG
		cli					    		;disable interrupts during initialization
		push	Acc
			
		sbi	_BUZZER_PWM_OUTPUT,_BUZZER_PWM_BIT	;Output pull up
		sbi _BUZZER_PWM_DIR,_BUZZER_PWM_BIT     ;OUTPUT
					
		pop	Acc						;restore SREG state
		out	SREG,Acc
		pop	Acc						;restore useds
		ret


	;>--------------------------------------------------------------<
	;| OBJECT     : BUZZER						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : Altenate BUZZER state 				|
	;>--------------------------------------------------------------<
	;| INPUT      :	None						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_BUZZER_ALTERNATE:
		 //
		//SAVE SREG
		//
		push	Acc
		in		Acc,SREG
			    
		//
		//SAVE REGISTERS
		//
		_M_PUSH_LOWER_REGS						;save r0..r15
		
        lds Acc,_FLAG_BUZZER_ON
		cpi Acc,_ON
		breq BUZZER_DESLIGA
			
		cbi	_BUZZER_PWM_OUTPUT,_BUZZER_PWM_BIT	;Output pull up
		 
		_SET _FLAG_BUZZER_ON

		lds Acc,_FLAG_MENU
		cpi Acc,_ON

		breq BUZZER_EXIT
			 
		_LED2_ON

		rjmp BUZZER_EXIT

BUZZER_DESLIGA:
       
        sbi	_BUZZER_PWM_OUTPUT,_BUZZER_PWM_BIT	;Output pull up
		
		_CLEAR _FLAG_BUZZER_ON

		lds Acc,_FLAG_MENU
		cpi Acc,_ON

		breq BUZZER_EXIT

		_LED2_OFF
				
BUZZER_EXIT:
							
	
		//
		//RESTORE REGISTERS
		//
		
		_M_POP_LOWER_REGS
		//
		//RESTORE SREG
		//
		
		out		SREG,Acc
		pop		Acc

		ret





	;>--------------------------------------------------------------<
	;| OBJECT     : BUZZER						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : BUZZER ON			|
	;>--------------------------------------------------------------<
	;| INPUT      :	None						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_BUZZER_ON:
		 //
		//SAVE SREG
		//
		push	Acc
		in		Acc,SREG
	
		//
		//SAVE REGISTERS
		//
		_M_PUSH_LOWER_REGS						;save r0..r15

		lds Acc,_FLAG_VOLUME
		tst Acc
		breq BUZZER_ON_EXIT
		 
		cbi	_BUZZER_PWM_OUTPUT,_BUZZER_PWM_BIT	;Output pull up
		  		
		_SET _FLAG_BUZZER_ON
		   
BUZZER_ON_EXIT:
	
		//
		//RESTORE REGISTERS
		//
		
		_M_POP_LOWER_REGS
		//
		//RESTORE SREG
		//
		out		SREG,Acc
		pop		Acc

		ret



		

	;>--------------------------------------------------------------<
	;| OBJECT     : BUZZER						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : BUZZER OFF			|
	;>--------------------------------------------------------------<
	;| INPUT      :	None						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_BUZZER_OFF:
		 //
		//SAVE SREG
		//
		push	Acc
		in		Acc,SREG
	
		//
		//SAVE REGISTERS
		//
		_M_PUSH_LOWER_REGS						;save r0..r15
		
        sbi	_BUZZER_PWM_OUTPUT,_BUZZER_PWM_BIT	;Output pull up
				 
		_CLEAR _FLAG_BUZZER_ON

	
BUZZER_OFF_EXIT:
	
   		//
		//RESTORE REGISTERS
		//
		
		_M_POP_LOWER_REGS
		//
		//RESTORE SREG
		//
		out		SREG,Acc
		pop		Acc

		ret

*/

;						 ///////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\
;						//						\\
;-----------------------------------------------||  	       C P U  G O O D  L E D 		||-----------------------------------------------------
;						\\						//
;						 \\\\\\\\\\\\\\\\\\\\\\\/////////////////////////

;>--------------------------------------------------------------<
;| OBJECT     : BUZZER						|
;>--------------------------------------------------------------<
;| DESCRIPTION: This object set state of BUZZER		|
;>--------------------------------------------------------------<

;>----------------------<
;| OBJECT DEFINITIONS	|
;>----------------------<

;-------------------------< PORT DEFINITONS >----------------------------
 .EQU	_BUZZER_PWM_OUTPUT	= PORTC			 			
 .EQU	_BUZZER_PWM_DIR	=     DDRC								
 .EQU	_BUZZER_PWM_INPUT	= PINC
 .EQU   _BUZZER_PWM_BIT = 2		
 

;-------------------------< CONSTANT DEFINITONS >------------------------


	;>--------------------------------------------------------------<
	;| OBJECT     : BUZZER						|
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

_BUZZER_INI:
		push	Acc						;save useds
		in	Acc,SREG					;save SREG
		cli					    		;disable interrupts during initialization
		push	Acc
			
		sbi	_BUZZER_PWM_OUTPUT,_BUZZER_PWM_BIT	;Output pull up
		sbi _BUZZER_PWM_DIR,_BUZZER_PWM_BIT     ;OUTPUT
					
		pop	Acc						;restore SREG state
		out	SREG,Acc
		pop	Acc						;restore useds
		ret


	;>--------------------------------------------------------------<
	;| OBJECT     : BUZZER						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : Altenate BUZZER state 				|
	;>--------------------------------------------------------------<
	;| INPUT      :	None						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_BUZZER_ALTERNATE:
		 //
		//SAVE SREG
		//
		push	Acc
		in		Acc,SREG
			    
		//
		//SAVE REGISTERS
		//
		_M_PUSH_LOWER_REGS						;save r0..r15
		
        lds Acc,_FLAG_BUZZER_ON
		cpi Acc,_ON
		breq BUZZER_DESLIGA

		;call _TIMER0_PWM_ON
		cbi	_BUZZER_PWM_OUTPUT,_BUZZER_PWM_BIT
		_SET _FLAG_BUZZER_ON

		lds Acc,_FLAG_MENU
		cpi Acc,_ON

		breq BUZZER_EXIT
		
		_LED2_ON

		lds Acc,_VAR_BEEP
		cpi Acc,6
		brge BUZZER_EXIT
		_LED3_ON

		rjmp BUZZER_EXIT

BUZZER_DESLIGA:
       ; call _TIMER0_PWM_STOP
	    sbi	_BUZZER_PWM_OUTPUT,_BUZZER_PWM_BIT	
		_CLEAR _FLAG_BUZZER_ON

		lds Acc,_FLAG_MENU
		cpi Acc,_ON

		breq BUZZER_EXIT

		_LED2_OFF
		_LED3_OFF
				
BUZZER_EXIT:
							
	
		//
		//RESTORE REGISTERS
		//
		
		_M_POP_LOWER_REGS
		//
		//RESTORE SREG
		//
		
		out		SREG,Acc
		pop		Acc

		ret





	;>--------------------------------------------------------------<
	;| OBJECT     : BUZZER						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : BUZZER ON			|
	;>--------------------------------------------------------------<
	;| INPUT      :	None						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_BUZZER_ON:
		 //
		//SAVE SREG
		//
		push	Acc
		in		Acc,SREG
	
		//
		//SAVE REGISTERS
		//
		_M_PUSH_LOWER_REGS						;save r0..r15
		
		;call _TIMER0_PWM_ON
		cbi	_BUZZER_PWM_OUTPUT,_BUZZER_PWM_BIT

		_SET _FLAG_BUZZER_ON
		   
BUZZER_ON_EXIT:
	
		//
		//RESTORE REGISTERS
		//
		
		_M_POP_LOWER_REGS
		//
		//RESTORE SREG
		//
		out		SREG,Acc
		pop		Acc

		ret



		

	;>--------------------------------------------------------------<
	;| OBJECT     : BUZZER						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : BUZZER OFF			|
	;>--------------------------------------------------------------<
	;| INPUT      :	None						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_BUZZER_OFF:
		 //
		//SAVE SREG
		//
		push	Acc
		in		Acc,SREG
	
		//
		//SAVE REGISTERS
		//
		_M_PUSH_LOWER_REGS						;save r0..r15
		
		;call _TIMER0_PWM_STOP
		sbi	_BUZZER_PWM_OUTPUT,_BUZZER_PWM_BIT	
		_CLEAR _FLAG_BUZZER_ON

	
BUZZER_OFF_EXIT:
	
   		//
		//RESTORE REGISTERS
		//
		
		_M_POP_LOWER_REGS
		//
		//RESTORE SREG
		//
		out		SREG,Acc
		pop		Acc

		ret



