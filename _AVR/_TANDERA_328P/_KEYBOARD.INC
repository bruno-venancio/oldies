

;						 ///////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\
;						//						\\
;-----------------------------------------------||  	       	 K E Y B O A R D 		||-----------------------------------------------------
;						\\						//
;						 \\\\\\\\\\\\\\\\\\\\\\\/////////////////////////

;>--------------------------------------------------------------<
;| OBJECT     : KEYBOARD					|
;>--------------------------------------------------------------<
;| DESCRIPTION: This object allow user read states of keyboard	|
;|		keys 						|
;>--------------------------------------------------------------<

;>----------------------<
;| OBJECT DEFINITIONS	|
;>----------------------<


;-------------------------< PORT DEFINITONS >----------------------------

.EQU	_KEYBOARD_PORT_OUTPUT		= PORTD
.EQU	_KEYBOARD_PORT_DIR		    = DDRD
.EQU	_KEYBOARD_PORT_INPUT		= PIND

.EQU	_KEYBOARD_PORT_OUTPUT2  	= PORTC
.EQU	_KEYBOARD_PORT_DIR2		    = DDRC
.EQU	_KEYBOARD_PORT_INPUT2		= PINC
;-------------------------< CONSTANT DEFINITONS >------------------------

.EQU	_KEYBOARD_KEY_ENTER_BIT		= 3
.EQU	_KEYBOARD_KEY_UP_BIT		= 2
.EQU	_KEYBOARD_KEY_DOWN_BIT		= 2    ; pc2


.EQU	_KEY_UP		= 0X02
.EQU	_KEY_DOWN	= 0X03

.EQU	_KEYBOARD_DEBOUNCE_TIME	= 80		;in mileseconds
.EQU	_KEYBOARD_TRIGGER_TIME	= 110		;trigger time for repeat mode
.EQU	_KEYBOARD_REPEAT_TIME	= 25		;repeat time for repeat mode

;-------------------------< SRAM VARIABLES >-----------------------------
.DSEG
	_KEYBOARD_CHAR_LAST:	.BYTE 	_BYTE	;hold  last char pressed by KEYBOARD_READ
	_KEYBOARD_TIME_COUNTER:	.BYTE	_BYTE	;hold a time of keyboard trigger time for repeat and repeat time
.CSEG
	;>--------------------------------------------------------------<
	;| OBJECT     : KEYBOARD					|
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
	_KEYBOARD_INIT:
		push	Acc						;save useds
		in	Acc,SREG					;save SREG
		cli							;disable interrupts during initialization
		push	Acc
		sbi	_KEYBOARD_PORT_OUTPUT,_KEYBOARD_KEY_ENTER_BIT	;all keys with pull up
		sbi	_KEYBOARD_PORT_OUTPUT,_KEYBOARD_KEY_UP_BIT
		sbi	_KEYBOARD_PORT_OUTPUT2,_KEYBOARD_KEY_DOWN_BIT
		cbi	_KEYBOARD_PORT_DIR,_KEYBOARD_KEY_ENTER_BIT	;all input
		cbi	_KEYBOARD_PORT_DIR,_KEYBOARD_KEY_UP_BIT
		cbi	_KEYBOARD_PORT_DIR2,_KEYBOARD_KEY_DOWN_BIT
		ldi	Acc,-1
		sts	_KEYBOARD_CHAR_LAST,Acc
		pop	Acc						;restore SREG state
		out	SREG,Acc
		pop	Acc						;restore useds
		ret
	;>--------------------------------------------------------------<
	;| OBJECT     : KEYBOARD					|
	;>--------------------------------------------------------------<
	;| FUNCTION   : Read keyboard without waiting			|
	;>--------------------------------------------------------------<
	;| INPUT      :	None						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|								
	_KEYBOARD_READ_FLASH:
		ldi	Acc,_KEY_ENTER					;assume KEY ENTER
		sbis	_KEYBOARD_PORT_INPUT,_KEYBOARD_KEY_ENTER_BIT	;skip if no
		rjmp	_KEYBOARD_READ_FLASH_EXIT
		ldi	Acc,_KEY_UP					;assume KEY UP
		sbis	_KEYBOARD_PORT_INPUT,_KEYBOARD_KEY_UP_BIT	;skip if no
		rjmp	_KEYBOARD_READ_FLASH_EXIT
		ldi	Acc,_KEY_DOWN					;assume KEY DOWN
		sbis	_KEYBOARD_PORT_INPUT2,_KEYBOARD_KEY_DOWN_BIT	;skip if no
		rjmp	_KEYBOARD_READ_FLASH_EXIT
		ldi	Acc,_KEY_NONE					;otherwise NONE KEY pressed
	_KEYBOARD_READ_FLASH_EXIT:
		ret


	;>--------------------------------------------------------------<
	;| OBJECT     : KEYBOARD					|
	;>--------------------------------------------------------------<
	;| FUNCTION   : Read keyboard and if already pressed key is 	|
	;|		igual a parameter key then wait			|
	;>--------------------------------------------------------------<
	;| INPUT      :	Acc Key code (parameter passed)			|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|						
	/*_KEYBOARD_READ_AND_WAIT:
		push	Acc
		mov	AccH,Acc				;code to wait in AccH
	_KEYBOARD_READ_AND_WAIT_10:
		rcall	_KEYBOARD_READ_FLASH			;get key
		cp	Acc,AccH				;igual to a parameter
		breq	_KEYBOARD_READ_AND_WAIT_10		;yes, wait release
		pop	Acc					;return
		ret
		*/
	;>--------------------------------------------------------------<
	;| OBJECT     : KEYBOARD					|
	;>--------------------------------------------------------------<
	;| FUNCTION   : Read keyboard and wait key will be pressed	|
	;>--------------------------------------------------------------<
	;| INPUT      :	None						|
	;| OUTPUT     :	Acc Key Code					|
	;|		cy=1 if exit in repeat time			|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|					
	;| if in DEBUG_MODE define sequency of pressed key in _KEY_TB
	;|	
	
	
	;>--------------------------------------------------------------<
	;| OBJECT     : KEYBOARD					|
	;>--------------------------------------------------------------<
	;| FUNCTION   : Read keyboard without repeat time   		|
	;>--------------------------------------------------------------<
	;| INPUT      :	None						|
	;| OUTPUT     :	Acc Key Code					|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|					
	;| if in DEBUG_MODE define sequency of pressed key in _KEY_TB
	;|	
	
	

	;>--------------------------------------------------------------<
	;| OBJECT     : KEYBOARD					|
	;>--------------------------------------------------------------<
	;| FUNCTION   : Wait a debound time				|
	;>--------------------------------------------------------------<
	;| INPUT      :	None						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|						
	_KEYBOARD_DEBOUNCE:
		_M_DELAY_MILLISECONDS	_KEYBOARD_DEBOUNCE_TIME
		ret

