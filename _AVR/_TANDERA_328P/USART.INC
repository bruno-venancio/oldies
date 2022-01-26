;+------------------------------------------+---+
;| BY JOÃO DARTAGNAN ANTUNES OLIVEIRA			|
;| DATA:21/10/2002				             	|
;>------------------------------------------------------<
;| A V R   M 328P  U A R T  R O U T I N E S	        	|
;+------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM	|
;+------------------------------------------------------+
;>------------------------------------------------------<
;| I M P L E M E N T E D  F U N C T I O N S 		|
;+------------------------------------------------------+---------------------------------------+
;| _USART_SET_NORMAL_SPEED  SET USART NORMAL SPEED 					                        	|
;|												|
;|			call 	_USART_SET_NORMAL_SPEED	;NOW USART IN NORMAL SPEED							|
;|----------------------------------------------------------------------------------------------|
;| _USART_SET_DOUBLE_SPEED  SET USART DOUBLE SPEED 								|
;|																				|
;|			call 	_USART_SET_DOUBLE_SPEED	;NOW USART IN DOUBLE SPEED			|	
;|----------------------------------------------------------------------------------------------|
;| _USART_SET_BAUD_RATE DEFINE USART BAUD RATE	 						|
;|												|
;|			call 	_USART_SET_NORMAL_SPEED	;FOR RIGHT SPEED SETTINGS		|
;|			LDIAW	9600			;SET USART FOR 9600 BAUDS		|
;|			call 	_USART_SET_BAUD_RATE	;NOW USART OPERATING IN 9600 BAUDS	|
;|----------------------------------------------------------------------------------------------|
;| _USART_TX_ENABLE	ENABLE/DISABLE TX							|
;|												|
;|			LDI	ACC,_ON		;ENABLE TX					|
;|			call 	_USART_TX_ENABLE						|
;|----------------------------------------------------------------------------------------------|
;| _USART_RX_ENABLE	ENABLE/DISABLE RX							|
;|												|
;|			LDI	ACC,_ON		;ENABLE RX					|
;|			call 	_USART_RX_ENABLE						|
;|----------------------------------------------------------------------------------------------|
;| _USART_TX_INT_ENABLE	ENABLE/DISABLE TX INTERRUPT 						|
;|												|
;|			LDI	ACC,_ON		;ENABLE TX					|
;|			call 	_USART_TX_ENABLE						|
;|----------------------------------------------------------------------------------------------|
;| _USART_RX_INT_ENABLE	ENABLE/DISABLE RX INTERRUPT						|
;|												|
;|			LDI	ACC,_ON		;ENABLE RX					|
;|			call 	_USART_RX_ENABLE						|
;|----------------------------------------------------------------------------------------------|
;| _USART_TX		SEND A BYTE TO USART							|
;|												|
;|			LDI	ACC,'A'		;send a ascii 'A'				|
;|			call 	_USART_TX							|
;|												|
;| OBS: This routine wait last transmission done before send a new byte				|
;+----------------------------------------------------------------------------------------------+
;| _USART_RX		WAIT A BYTE FROM USART							|
;|												|
;|			LDIAWT	100		;wait 100ms for receive a byte			|
;|						;set this register to -1 to infinit wait	|
;|			call 	_USART_RX							|
;|												|
;+----------------------------------------------------------------------------------------------+
#ifndef __INTEGER_DIV_U32U32U32__
	.INCLUDE "MATH\INTEGER\DIV_U32U32U32\DIV_U32U32U32.hug"
#endif
#ifndef __AVR_M328P_USART__
	#define __AVR_M328P_USART__
	.message "+-----------------+"
	.message "|                 |"
	.message "| AVR M328P USART |"
	.message "|                 |"
	.message "+-----------------+"
	.CSEG

	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   USART SET NORMAL SPEED	  |
	;|  				  |
	;|\  Input : None		  |
	;| | Output: None		  |
	;|/  Destroy:None		  |
	;|   				  |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_______
	;				       \
	_USART_SET_NORMAL_SPEED:
		push	Acc
	
		lds	Acc,UCSR0A
		andi	Acc,~(1<<U2X0)
		sts	UCSR0A,Acc
	
		pop	Acc
		ret
	
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   USART SET DOUBLE SPEED	  |
	;|  				  |
	;|\  Input : None		  |
	;| | Output: None		  |
	;|/  Destroy:None		  |
	;|   				  |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_______
	;				       \
	_USART_SET_DOUBLE_SPEED:
		push	Acc
		lds	Acc,UCSR0A
		ori	Acc,(1<<U2X0)
		sts	UCSR0A,Acc
		pop	Acc
		ret
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   UART BAUD RATE		  |
	;|  				  |
	;|\  Input : AccH:Acc BAUD RATE   |
	;| | Output: None		  |
	;|/  Destroy:Flags,r0..r11	  |
	;|   obs: assume normal speed	  |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_______
	;				       \
	;					+---------------------------------------+
	;					| BAUD rate equation 			|
	;					|					|
	;					|		AVR_CLOCK		|
	;					| BAUD = ---------------------		|
	;					|		16(UBRR+1)		|
	;					|					|
	;					| Resolvin in UBRR,			|
	;					|					|
	;					|		AVR_CLOCK		|
	;					| UBRR = ------------------ - 1		|
	;					|		16.BAUD			|
	;					+---------------------------------------+
	_USART_SET_BAUD_RATE:
		push	Acc
		push	AccH
		mov	r4,Acc							;move baud rate to appropriate registers
		mov	r5,AccH
		clr	r6
		clr	r7
		ldi	Acc,4							;multiply by 16

	_USART_SET_BAU00:
		lsl	r4
		rol	r5
		rol	r6
		rol	r7
		dec	Acc
		brne	_USART_SET_BAU00
		ldi	Acc,low(lwrd(_AVR_CLOCK))				;get avr clock
		mov	r0,Acc
		ldi	Acc,high(lwrd(_AVR_CLOCK))				;get avr clock
		mov	r1,Acc
		ldi	Acc,low(hwrd(_AVR_CLOCK))				;get avr clock
		mov	r2,Acc
		ldi	Acc,high(hwrd(_AVR_CLOCK))				;get avr clock
		mov	r3,Acc
		call	_DIV_U32U32U32						;AVR_CLOCK/16*BAUD
		ldi	Acc,-1
		add	r0,Acc							;AVR_CLOCK/(16*BAUD)-1
		adc	r1,Acc
		adc	r2,Acc
		adc	r3,Acc

		sts	UBRR0L,r0						;set baud rate to comm 0
		sts	UBRR0H,r1

		pop	AccH
		pop	Acc
		ret

	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   UART ENABLE/DISABLE TX	  |
	;|  				  |
	;|\  Input : Acc ON/OFF	          |
	;| | Output: None		  |
	;|/  Destroy:None		  |
	;|   				  |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_________________________________________________________________________
	_USART_TX_ENABLE:
		push	Acc
		cpi	Acc,_ON						;ON ?
		brne	_USART_TX_10_10					;no, branch
		lds	Acc,UCSR0B					;enable tx comm 0
		ori	Acc,(1<<TXEN0)
	_USART_TX_10_00:
		sts	UCSR0B,Acc
		pop	Acc
		ret
	_USART_TX_10_10:
		lds	Acc,UCSR0B					;disable tx comm 0
		andi	Acc,~(1<<TXEN0)
		jmp 	_USART_TX_10_00
	
	;   |  |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+
	;|   UART ENABLE/DISABLE TX INTERRUPT   |
	;|  				  	|
	;|\  Input : Acc ON/OFF	          	|
	;| | Output: None		  	|
	;|/  Destroy:None		  	|
	;|   				  	|
	;|   			       	  	|
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_________________________________________________________________________
	_USART_TX_INT_ENABLE:
		
		push	Acc
		cpi	Acc,_ON						;ON ?
		brne	_USART_TXI_10_10				;no, branch
		lds	Acc,UCSR0B					;enable tx comm 0
		ori	Acc,(1<<TXCIE0)
		sts	UCSR0B,Acc
		jmp 	_USART_TXI_00

	_USART_TXI_10_10:
		lds	Acc,UCSR0B					;disable tx comm 0
		andi	Acc,~(1<<TXCIE0)
		sts	UCSR0B,Acc
		
   _USART_TXI_00:   
	    pop	Acc
		ret

	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   UART ENABLE/DISABLE RX	  |
	;|  				  |
	;|\  Input : Acc ON/OFF	          |
	;| | Output: None		  |
	;|/  Destroy:None		  |
	;|   				  |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_________________________________________________________________________
	_USART_RX_ENABLE:
		
		push	Acc
		cpi	Acc,_ON						;ON ?
		brne	_USART_RX_10_10					;no, branch
		lds	Acc,UCSR0B					;enable tx comm 0
		ori	Acc,(1<<RXEN0)
	_USART_RX_10_00:
		sts	UCSR0B,Acc
		jmp 	_USART_RX_00
	_USART_RX_10_10:
		lds	Acc,UCSR0B					;disable tx comm 0
		andi	Acc,~(1<<RXEN0)
        sts	UCSR0B,Acc
	_USART_RX_00:
		pop	Acc
		ret
	

	;   |  |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+
	;|   UART ENABLE/DISABLE RX INTERRUPT	|
	;|  				 	|
	;|\  Input : Acc ON/OFF	          	|
	;| | Output: None		  	|
	;|/  Destroy:None		  	|
	;|   				  	|
	;|   			       	  	|
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_________________________________________________________________________
	_USART_RX_INT_ENABLE:
	    push	Acc
	
		cpi	Acc,_ON						;ON ?
		brne	_USART_RXI_10_10		;No, branch
		lds	Acc,UCSR0B					;enable rx comm 0
		ori	Acc,(1<<RXCIE0)
	_USART_RXI_10_00:
		sts	UCSR0B,Acc
		jmp 	_USART_RXI_00
	_USART_RXI_10_10:
		lds	Acc,UCSR0B					;disable rx comm 0
		andi	Acc,~(1<<RXCIE0)
		sts	UCSR0B,Acc
	_USART_RXI_00:

		pop	Acc

		ret

	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   UART TX DIRECT		  |
	;|  				  |
	;|\  Input : Acc byte to be send  |
	;| | Output: None		  |	
	;|/  Destroy:none		  |
	;|   				  |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\________________________________________________________________________
	
	_USART_TX:

	    push Acc
		push R0

	_USART_TX_00:
		
		lds     R0,UCSR0A
		sbrs	R0,UDRE0					;wait last transmission done
		rjmp	_USART_TX_00					;*<NOCHANGE>*
   
		sts	UDR0,Acc					;set data to be sent
		
		pop R0
        pop Acc

		ret
		
	;   |  |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+
	;|   UART RX DIRECT			|
	;|  					|
	;|\  Input : AccT:AccTH Mileseconds	|
	;| |			Time Out	|
	;| | Output: cy=1 if time out occur 	|
	;| |	     Acc byte received		|
	;|/  Destroy:none		  	|
	;|   				  	|
	;|   			       	  	|
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\__\__\____________________________________________________________________
	;					/
	;					| Obs: if Time Out = 65535 then this routine no exit until
	;					|      UDR not receive a byte]
	;					\_________________________________________________________________
	/*
	_USART_RX:
	    push    R0
		push	AccT			;save useds
		push	AccTH
		cpi	AccT,0xFF		;see if time out = 65535
		brne	_USART_RX30		;no, branch to timeout receive
		cpi	AccTH,0xFF
		brne	_USART_RX30		;no, branch to timeout receive
	_USART_RX00:
        lds     R0,UCSR0A 
		 
		sbrs	R0,RXC0		;wait character reception for infinit time until character received
		jmp 	_USART_RX00
	_USART_RX10:
		lds	Acc,UDR0		;get reseived data
		clc				;set no time out 
	_USART_RX20:
		pop	AccTH			;restore useds
		pop	AccT
		pop R0
		ret
	_USART_RX30:
		subi	AccT,1			;decrement mileseconds time counter
		sbci	AccTH,0
		brcs	_USART_RX20		;time expired? branch if yes
		push	AccT			;save AccT:AccTH
		push	AccTH
		ldi	AccT,low((_AVR_CLOCK-2000)/6000)		;load constant for 1ms delay
		ldi	AccTH,high((_AVR_CLOCK-2000)/6000)
	
	_USART_RX40:
		subi	AccT,1			;1ms delay expired ?
		sbci	AccTH,0
		brcs	_USART_RX50		;yes, brach

		lds     R0,UCSR0A
		sbrs	R0,RXC0		;else see if data received
		jmp 	_USART_RX40		;no, wait data and process time out
		pop	AccTH			;received, restore AccT:AccTH and get data
		pop	AccT
		jmp 	_USART_RX10
	_USART_RX50:
		pop	AccTH			;restore AccT:AccTH
		pop	AccT
		jmp 	_USART_RX30		;decrement mileseconds time counter
	
	*/


_USART_RX:
        lds     Acc,UCSR0A
		sbrs	Acc,RXC0		;else see if data received
		rjmp 	_USART_RX		;no, wait data and process time out
		lds		Acc,UDR0		;get reseived data
		ret






#endif
