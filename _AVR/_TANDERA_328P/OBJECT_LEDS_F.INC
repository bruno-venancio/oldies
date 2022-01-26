;						 ///////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\
;						//						\\
;-----------------------------------------------||  	      leds		||-----------------------------------------------------
;						\\						//
;						 \\\\\\\\\\\\\\\\\\\\\\\/////////////////////////
/*	
	.message "+------------------+"
	.message "|   LEDS DRIVER    |" 
	.message "+------------------+"

;>--------------------------------------------------------------<
;| OBJECT     : LEDS						|
;>--------------------------------------------------------------<
;| DESCRIPTION: This object set state of BUZZER		|
;>--------------------------------------------------------------<

;>----------------------<
;| OBJECT DEFINITIONS	|
;>----------------------<

;-------------------------< PORT DEFINITONS >----------------------------
 .EQU	_LEDS_OUTPUT = PORTC			 			
 .EQU	_LEDS_DIR	 = DDRC								
 .EQU	_LEDS_INPUT	 = PINC
 .EQU   _LED_1       = 1	
 .EQU   _LED_2       = 0		
 	
 
 .EQU	_LEDS_OUTPUTC = PORTC			 			
 .EQU	_LEDS_DIRC	  = DDRC								
 .EQU	_LEDS_INPUTC  = PINC
  
 .EQU   _LED_3       = 2
;-------------------------< CONSTANT DEFINITONS >------------------------


	;>--------------------------------------------------------------<
	;| OBJECT     : LEDS						|
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

_LEDS_INI:
		push	Acc						;save useds
		in	Acc,SREG					;save SREG
		cli					    		;disable interrupts during initialization
		push	Acc
			
		sbi _LEDS_DIR,_LED_1            ;OUTPUT
		sbi	_LEDS_OUTPUT,_LED_1     	;Output pull up
					
		sbi _LEDS_DIR,_LED_2            ;OUTPUT
		sbi	_LEDS_OUTPUT,_LED_2     	;Output pull up

		sbi _LEDS_DIRC,_LED_3            ;OUTPUT
		sbi	_LEDS_OUTPUTC,_LED_3     	;Output pull up

				
		pop	Acc						    ;restore SREG state
		out	SREG,Acc
		pop	Acc					    	;restore useds
		ret

 
 

	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   INICIALIZA LED CPU				|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;				/
	;				| Obs:  Durante a Inicialização deste Drive as interrupções serão
	;				|		desabilitadas e habilitadas no final da rotina
	;				\_______________________________________________________________________
	_LED_VD_INIT:
		push	Acc					;save Acc
		in		Acc,SREG			;save SREG
		push	Acc
		cli							;disable interrupts
		//DEFINE LED CPU AS OUTPUT LO LEVEL(OFF)
		 
		sbi		_LEDS_DIR,_LED_1
		sbi		_LEDS_OUTPUT,_LED_1	    ;turn led off
		pop		Acc					;Restore SREG
		out		SREG,Acc
		pop		Acc					;restore Acc
		ret
 

		
_LEDS_SET:
		 //
		//SAVE SREG
		//
		push	Acc
		in		Acc,SREG

		;_M_PUSH_LOWER_REGS

		lds Acc,_BYTE_BARRA

		bst Acc,0
		brtc LED_1_ON

		_LED1_OFF
		rjmp LED_1_JP
LED_1_ON:
       _LED1_ON
LED_1_JP:
	    bst Acc,1
		brtc LED_2_ON

		_LED2_OFF
		rjmp LED_2_JP
LED_2_ON:
       _LED2_ON
LED_2_JP:

        bst Acc,2
		brtc LED_3_ON

		_LED3_OFF
		rjmp LED_3_JP
LED_3_ON:
       _LED3_ON
LED_3_JP:

	    //
		//RESTORE REGISTERS
		//
		
		;_M_POP_LOWER_REGS
		//
		//RESTORE SREG
		//
		out		SREG,Acc
		pop		Acc

		ret
		
	
_LEDS_SET_ALL:
		
		_LED1_ON
		_LED2_ON
		_LED3_ON
		 			
		ret

_LEDS_CLEAR_ALL:
		
		_LED1_OFF
		_LED2_OFF
		_LED3_OFF
					
		ret



;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   PERMUTA ESTADO DA LED CPU		|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
_LED_VD_SWAP:

		push	Acc									;save useds
		
		lds Acc,_FLAG_LED1_ON
		cpi Acc,_ON
		breq  LED_VD_SW_L1

		_LED1_ON
			 
		rjmp    LED_VD_SW_L2
 
LED_VD_SW_L1:

        _LED1_OFF 

LED_VD_SW_L2:
				 							;restore Useds
		pop		Acc

		ret


;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   PERMUTA ESTADO DA LED CPU		|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
_LED_VM1_SWAP:

		push	Acc									;save useds
		
		lds Acc,_FLAG_LED2_ON
		cpi Acc,_ON
		breq  LED_VM1_SW_L1

		_LED2_ON
			 
		rjmp    LED_VM1_SW_L2
 
LED_VM1_SW_L1:

        _LED2_OFF 

LED_VM1_SW_L2:
				 							;restore Useds
		pop		Acc

		ret




;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   PERMUTA ESTADO DA LED CPU		|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
_LED_VM2_SWAP:

		push	Acc									;save useds
		
		lds Acc,_FLAG_LED3_ON
		cpi Acc,_ON
		breq  LED_VM2_SW_L1

		_LED3_ON
			 
		rjmp    LED_VM2_SW_L2
 
LED_VM2_SW_L1:

        _LED3_OFF 

LED_VM2_SW_L2:
				 							;restore Useds
		pop		Acc

		ret


 
    ;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   PERMUTA ESTADO DA LED CPU		|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_LED_VD_PISCA2:

		push	Acc									;save useds
		push	AccH
		
		_LED1_ON

		_M_DELAY_MILLISECONDS 200

		_LED1_OFF

		_M_DELAY_MILLISECONDS 200

		_LED1_ON

		_M_DELAY_MILLISECONDS 200

	    _LED1_OFF
		
		pop		AccH								;restore Useds
		pop		Acc

		ret

;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   DEFINE ESTADO DA LED CPU		|
	;|  								|
	;|\									|
	;| | Input  : Acc	_ON or _OFF		|
	;|/  Output : None					|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_LED_VD_SET:
		cpi		Acc,_ON								  ;is ON?
		brne	_LED_VD_SET_10						  ;no, branch
		_LED1_ON
		ret
	_LED_VD_SET_10:
		_LED1_OFF                                     ;turn led off
		ret

	;>--------------------------------------------------------------<
	;| OBJECT     : LEDS						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : LEDS OFF			|
	;>--------------------------------------------------------------<
	;| INPUT      :	_BYTE_BARRA						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
 	_LEDS_OFF:
		 //
		//SAVE SREG
		//
		push	Acc
		in		Acc,SREG
	
		//
		//SAVE REGISTERS
		//
		_M_PUSH_LOWER_REGS						;save r0..r15
		
		call _TIMER0_PWM_STOP
		_CLEAR _FLAG_BUZZER_ON
	
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
;-----------------------------------------------||  	      leds		||-----------------------------------------------------
;						\\						//
;						 \\\\\\\\\\\\\\\\\\\\\\\/////////////////////////

	
	.message "+------------------+"
	.message "|   LEDS DRIVER    |" 
	.message "+------------------+"

;>--------------------------------------------------------------<
;| OBJECT     : LEDS						|
;>--------------------------------------------------------------<
;| DESCRIPTION: This object set state of BUZZER		|
;>--------------------------------------------------------------<

;>----------------------<
;| OBJECT DEFINITIONS	|
;>----------------------<

;-------------------------< PORT DEFINITONS >----------------------------
 .EQU	_LEDS_OUTPUT = PORTD			 			
 .EQU	_LEDS_DIR	 = DDRD								
 .EQU	_LEDS_INPUT	 = PIND
 .EQU   _LED_1       = 6	
 .EQU   _LED_2       = 7		
 	
 
 .EQU	_LEDS_OUTPUTC = PORTC			 			
 .EQU	_LEDS_DIRC	  = DDRC								
 .EQU	_LEDS_INPUTC  = PINC
  
 .EQU   _LED_3       = 3
;-------------------------< CONSTANT DEFINITONS >------------------------


	;>--------------------------------------------------------------<
	;| OBJECT     : LEDS						|
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

_LEDS_INI:
		push	Acc						;save useds
		in	Acc,SREG					;save SREG
		cli					    		;disable interrupts during initialization
		push	Acc
			
		sbi _LEDS_DIR,_LED_1            ;OUTPUT
		sbi	_LEDS_OUTPUT,_LED_1     	;Output pull up
					
		sbi _LEDS_DIR,_LED_2            ;OUTPUT
		sbi	_LEDS_OUTPUT,_LED_2     	;Output pull up

		sbi _LEDS_DIRC,_LED_3            ;OUTPUT
		sbi	_LEDS_OUTPUTC,_LED_3     	;Output pull up

				
		pop	Acc						    ;restore SREG state
		out	SREG,Acc
		pop	Acc					    	;restore useds
		ret

 
 /*

	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   INICIALIZA LED CPU				|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;				/
	;				| Obs:  Durante a Inicialização deste Drive as interrupções serão
	;				|		desabilitadas e habilitadas no final da rotina
	;				\_______________________________________________________________________
	_LED_VD_INIT:
		push	Acc					;save Acc
		in		Acc,SREG			;save SREG
		push	Acc
		cli							;disable interrupts
		//DEFINE LED CPU AS OUTPUT LO LEVEL(OFF)
		 
		sbi		_LEDS_DIR,_LED_1
		sbi		_LEDS_OUTPUT,_LED_1	    ;turn led off
		pop		Acc					;Restore SREG
		out		SREG,Acc
		pop		Acc					;restore Acc
		ret
*/

		
_LEDS_SET:
		 //
		//SAVE SREG
		//
		push	Acc
		in		Acc,SREG

		;_M_PUSH_LOWER_REGS

		lds Acc,_BYTE_BARRA

		bst Acc,0
		brtc LED_1_ON

		_LED1_OFF
		rjmp LED_1_JP
LED_1_ON:
       _LED1_ON
LED_1_JP:
	    bst Acc,1
		brtc LED_2_ON

		_LED2_OFF
		rjmp LED_2_JP
LED_2_ON:
       _LED2_ON
LED_2_JP:

        bst Acc,2
		brtc LED_3_ON

		_LED3_OFF
		rjmp LED_3_JP
LED_3_ON:
       _LED3_ON
LED_3_JP:

	    //
		//RESTORE REGISTERS
		//
		
		;_M_POP_LOWER_REGS
		//
		//RESTORE SREG
		//
		out		SREG,Acc
		pop		Acc

		ret
		
	
_LEDS_SET_ALL:
		
		_LED1_ON
		_LED2_ON
		_LED3_ON
		 			
		ret

_LEDS_CLEAR_ALL:
		
		_LED1_OFF
		_LED2_OFF
		_LED3_OFF
					
		ret



;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   PERMUTA ESTADO DA LED CPU		|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
_LED_VD_SWAP:

		push	Acc									;save useds
		
		lds Acc,_FLAG_LED1_ON
		cpi Acc,_ON
		breq  LED_VD_SW_L1

		_LED1_ON
			 
		rjmp    LED_VD_SW_L2
 
LED_VD_SW_L1:

        _LED1_OFF 

LED_VD_SW_L2:
				 							;restore Useds
		pop		Acc

		ret


;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   PERMUTA ESTADO DA LED CPU		|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
;_LED_VM1_SWAP:

		push	Acc									;save useds
		
		lds Acc,_FLAG_LED2_ON
		cpi Acc,_ON
		breq  LED_VM1_SW_L1

		_LED2_ON
			 
		rjmp    LED_VM1_SW_L2
 
LED_VM1_SW_L1:

        _LED2_OFF 

LED_VM1_SW_L2:
				 							;restore Useds
		pop		Acc

		ret




;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   PERMUTA ESTADO DA LED CPU		|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
;_LED_VM2_SWAP:

		push	Acc									;save useds
		
		lds Acc,_FLAG_LED3_ON
		cpi Acc,_ON
		breq  LED_VM2_SW_L1

		;_LED3_ON
			 
		rjmp    LED_VM2_SW_L2
 
LED_VM2_SW_L1:

      ;  _LED3_OFF 

LED_VM2_SW_L2:
				 							;restore Useds
		pop		Acc

		ret


 
    ;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   PERMUTA ESTADO DA LED CPU		|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_LED_VD_PISCA2:

		push	Acc									;save useds
		push	AccH
		
		_LED1_ON

		_M_DELAY_MILLISECONDS 200

		_LED1_OFF

		_M_DELAY_MILLISECONDS 200

		_LED1_ON

		_M_DELAY_MILLISECONDS 200

	    _LED1_OFF
		
		pop		AccH								;restore Useds
		pop		Acc

		ret

;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   DEFINE ESTADO DA LED CPU		|
	;|  								|
	;|\									|
	;| | Input  : Acc	_ON or _OFF		|
	;|/  Output : None					|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_LED_VD_SET:
		cpi		Acc,_ON								  ;is ON?
		brne	_LED_VD_SET_10						  ;no, branch
		_LED1_ON
		ret
	_LED_VD_SET_10:
		_LED1_OFF                                     ;turn led off
		ret

	;>--------------------------------------------------------------<
	;| OBJECT     : LEDS						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : LEDS OFF			|
	;>--------------------------------------------------------------<
	;| INPUT      :	_BYTE_BARRA						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
/*	_LEDS_OFF:
		 //
		//SAVE SREG
		//
		push	Acc
		in		Acc,SREG
	
		//
		//SAVE REGISTERS
		//
		_M_PUSH_LOWER_REGS						;save r0..r15
		
		call _TIMER0_PWM_STOP
		_CLEAR _FLAG_BUZZER_ON
	
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
