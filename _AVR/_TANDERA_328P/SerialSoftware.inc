/*
	#-------------------------------------------------------------------------------#
	|	 _____           _       _   _____        __ _    ______      _				|
	|	/  ___|         (_)     | | /  ___|      / _| |   |  _  \    (_)			|
	|	\ `--.  ___ _ __ _  __ _| | \ `--.  ___ | |_| |_  | | | |_ __ ___   _____	|
	|	 `--. \/ _ \ '__| |/ _` | |  `--. \/ _ \|  _| __| | | | | '__| \ \ / / _ \	|
	|	/\__/ /  __/ |  | | (_| | | /\__/ / (_) | | | |_  | |/ /| |  | |\ V /  __/	|
	|	\____/ \___|_|  |_|\__,_|_| \____/ \___/|_|  \__| |___/ |_|  |_| \_/ \___|	|
	|																				|
	#-------------------------------------------------------------------------------#                                                           
	| Este Drive Implementa todas as rotinas necessárias para transmissão serial	|
	| com taxa de baudrate de 115200bauds 8 databits e 1 stop bits, no momento so-	|
	| mente a transmissão esta implementada. 										|
	#-------------------------------------------------------------------------------#
	| Este documento usa TAB 4 para formatação										|
	#-------------------------------------------------------------------------------#
	| Date: Brasilia ,27 de Julho de 2016											|
	#-------------------------------------------------------------------------------#
	| Para usar este Drive devera ser definido pelo usuario as contantes que segue	|
	| Exemplo abaixo com PORTAS e BITS definidos aleatoriamente como exemplo		|
	|																				|
	| DEFINIÇOES DA PORTA DE COMUNICAÇÃO E BITS										|
	|																				|
	|	.EQU	_ESP_TX_PORT_OUTPU	T	= PORTD										|
	|	.EQU	_ESP_TX_PORT_DIR		= DDRD										|
	|	.EQU	_ESP_TX_PORT_INPUT		= PIND										|
	|																				|
	|	.EQU	_ESP_TX_DATA_BIT		= 1											|
	|																				|
	#-------------------------------------------------------------------------------#
	|	FUNÇÕES IMPLEMENTADAS														|
	#-------------------------------------------------------------------------------#
	|	INICIALIZA SERIAL SOFT														|
	|																				|
	|	call/rcall	_ESP_TX_INIT													|
	#-------------------------------------------------------------------------------#
	|	TRANSMITE DATA																|
	|																				|
	|	ldi	Acc,data					;load Acc with serial data to be send		|
	|	call/rcall	_ESP_TX_TX		;transmite data at 115200baudrate				|
	#-------------------------------------------------------------------------------#
	|	TRANSMITE STRING DATA FROM FLASH											|
	|																				|
	|	ldiaw	Z,message*2				;z-->flash data string						|
	|	call/rcall	_ESP_TX_PUTSF	;send string from flasth to serial soft tx		|
	#-------------------------------------------------------------------------------#
	|	TRANSMITE STRING DATA FROM SRAM												|
	|																				|
	|	ldiaw	Z,message				;z-->sram data string						|
	|	call/rcall	_ESP_TX_PUTS	;send string from sram to serial soft tx		|
	#-------------------------------------------------------------------------------#

 */

		.EQU	_ESP_TX_PORT_OUTPUT  	= PORTD								
		.EQU	_ESP_TX_PORT_DIR		= DDRD									
		.EQU	_ESP_TX_PORT_INPUT		= PIND									
																					
		.EQU	_ESP_TX_DATA_BIT		= 1


//DEFINE SERIAL SOFT DRIVE

#ifndef __ESP_TX_DRIVE__
	#define __ESP_TX_DRIVE__
	
	.message "+---------------------+"
	.message "|   SERIALSOFT DRIVE  |" 
	.message "+---------------------+"

	.equ	 _ESP_TX_BAUDRATE = 115200
	.equ	 _ESP_TX_BAUDRATE_ADJUST = 6

	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   INICIALIZA SOFTSERIAL			|
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
	_ESP_TX_INIT:
		push	Acc					;save Acc
		in		Acc,SREG			;save SREG
		push	Acc
		cli							;disable interrupts
		//define serial data port as output hi level
		; desliga pullup
		cbi		_ESP_TX_PORT_DIR,_ESP_TX_DATA_BIT
		cbi		_ESP_TX_PORT_OUTPUT,_ESP_TX_DATA_BIT

		_m_delay_milliseconds 50

		sbi		_ESP_TX_PORT_DIR,_ESP_TX_DATA_BIT
		sbi		_ESP_TX_PORT_OUTPUT,_ESP_TX_DATA_BIT
		 
		pop		Acc					;Restore SREG
		out		SREG,Acc
		pop		Acc					;restore Acc
		ret
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|  TRANSMITE DADO					|
	;|  								|
	;|\									|
	;| | Input  : Acc Data				|
	;|/  Output : None					|
	;|   Destroy: Flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;				/
	;				| Obs:  Durante a transmissão a interrupção global sera desabilitada
	;				|		e restaurada no final da transmissão
	;				\_______________________________________________________________________
	_ESP_TX_PUTC:
		push	AccH					;save AccH
		in		AccH,SREG				;save SREG
		push	AccH
		push	AccT					;save AccT
		cli								;disable interrupts
		clc								;send start bit(always 0)
		rcall	_ESP_TX_TX_BIT
		ldi		AccH,8					;number of bits to tx
	_ESP_TX_PUTC_LOOP:
		ror		Acc						;set bit to transmit
		rcall	_ESP_TX_TX_BIT		    ;send a bit
		dec		AccH					;last bit send?
		brne	_ESP_TX_PUTC_LOOP	    ;no, transmit again
		sec								;send stop bit(always 1)
		rcall	_ESP_TX_TX_BIT
		pop		AccT					;restore AccT
		pop		AccH					;restore SREG
		out		SREG,AccH
		pop		AccH					;restore AccH
		ret
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|  TRANSMITE BIT					|
	;|  								|
	;|\									|
	;| | Input  : Cy bit 1 or 0			|
	;|/  Output : None					|
	;|   Destroy: Flags,AccT			|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_ESP_TX_TX_BIT:
		in		AccT,_ESP_TX_PORT_OUTPUT					;get old state
		andi	AccT,~(1<<_ESP_TX_DATA_BIT)					;assume data=0
		brcc	_ESP_TX_TX_BIT_10							;branch if yes
		ori		AccT,(1<<_ESP_TX_DATA_BIT)					;else set data=1
	_ESP_TX_TX_BIT_10:
		out		_ESP_TX_PORT_OUTPUT,AccT					;set new data to be send
		ldi		AccT,(_AVR_CLOCK/(_ESP_TX_BAUDRATE*3))-_ESP_TX_BAUDRATE_ADJUST		;set delay for 115200 baud
	_ESP_TX_TX_BIT_DELAY:
		subi	AccT,1
		brne	_ESP_TX_TX_BIT_DELAY						;delay finished ? no loop
		ret			;return
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|  TRANSMITE STRING FROM FLASH		|
	;|  								|
	;|\									|
	;| | Input  : Z->sting into flash	|
	;| |		  zero(0) terminated	|
	;|/  Output : None					|
	;|   Destroy: Flags,AccT			|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_ESP_TX_PUTSF:
		push	Acc						;save Acc
		pushw	Z						;save pointer
	_ESP_TX_PUTSF_LOOP:	
		lpm		Acc,Z+					;get data from flash
		cpi		Acc,0					;zero end?
		breq	_ESP_TX_PUTSF_EXIT	;yes, exit
		rcall	_ESP_TX_PUTC		;send a data
		rjmp	_ESP_TX_PUTSF_LOOP	;get new data
	_ESP_TX_PUTSF_EXIT:			
		popw	Z						;restore Z
		pop		Acc						;restore Acc
		ret
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|  TRANSMITE STRING FROM SRAM		|
	;|  								|
	;|\									|
	;| | Input  : Z->sting into sram	|
	;| |		  zero(0) terminated	|
	;|/  Output : None					|
	;|   Destroy: Flags,AccT			|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_ESP_TX_PUTS:
		push	Acc						;save Acc
		pushw	Z						;save pointer
	_ESP_TX_PUTS_LOOP:	
		ld		Acc,Z+					;get data from flash
		cpi		Acc,0					;zero end?
		breq	_ESP_TX_PUTS_EXIT	;yes, exit
		rcall	_ESP_TX_PUTC		;send a data
		rjmp	_ESP_TX_PUTS_LOOP	;get new data
	_ESP_TX_PUTS_EXIT:			
		popw	Z						;restore Z
		pop		Acc						;restore Acc
		ret	
		
		

	
;+--------------------------------------------------------------+---+
;| BY JOÃO DARTAGNAN ANTUNES OLIVEIRA			|
;| DATA:21/10/2002					|
;>------------------------------------------------------<
;| A V R  U A R T  R O U T I N E S  B Y  S O F T W A R E|
;+------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM	|
;+------------------------------------------------------+
;>------------------------------------------------------<
;| I M P L E M E N T E D  F U N C T I O N S 		|
;+------------------------------------------------------+---------------------------------------+
;| _ESP_INIT		INITIALIZE SCOMM							|
;|														|
;| .EQU			_ESP_PORT_DATA_OUTPUT	=PORTX	;Set SCOMM data ports			|
;| .EQU			_ESP_PORT_DATA_DIR	=DDRX						|
;| .EQU			_ESP_PORT_DATA_INPUT	=PINX						|
;|												|
;| .EQU			_ESP_PORT_CTRL_OUTPUT	=PORTX	;Set SCOMM control ports		|
;| .EQU			_ESP_PORT_CTRL_DIR	=DDRX						|
;| .EQU			_ESP_PORT_CTRL_INPUT	=PINX						|
;|												|
;| .EQU			_ESP_TX_BIT		= BITx	;Set SCOMM bits				|
;| .EQU			_ESP_RX_BIT		= BITx						|
;| .EQU			_ESP_RTS_BIT		= BITx						|
;| .EQU			_ESP_CLS_BIT		= BITx						|
;|												|
;+----------------------------------------------------------------------------------------------+

		.EQU			_ESP_PORT_DATA_OUTPUT	=PORTC	;Set SCOMM data ports			
		.EQU			_ESP_PORT_DATA_DIR	=DDRC							
		.EQU			_ESP_PORT_DATA_INPUT	=PINC	
		
		.EQU			_ESP_RX_BIT		= 5				
		

	;>--------------------------------------------------------------<
	;| OBJECT     : _ESP	 					|
	;>--------------------------------------------------------------<
	;| DESCRIPTION: This object implements some functions(methods)	|
	;|		do handdle serial communication using software	|
	;|		routines. fixed baud rate 115200, 8 data bits	|
	;|		and 2 stop bits optimized to use 16Mhz crystal, |
	;|		but other values is allowed since that certain  |
	;|		constants is tuned				|
	;>--------------------------------------------------------------<
	;>----------------------<
	;| OBJECT DEFINITIONS	|
	;>----------------------<
	;
	;-------------------------< CONSTANT DEFINITONS >------------------------
	;
	.EQU	_ESP_TIMEOUT_ERROR	= 1
	.EQU	_ESP_STARTBIT_ERROR	= 2
	;
	
	;-------------------------< M A C R O S  >-------------------------------
	;
	;@-------------------@
	;@ delay bit time    @
	;@-------------------@
	.MACRO	_M_ESP_DELAY_BIT
		ldi	AccH,@0
		dec	AccH
		brne	PC-1
	.ENDMACRO
	 


	;>--------------------------------------------------------------<
	;| OBJECT     : _ESP						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : Initialize					|
	;>--------------------------------------------------------------<
	;| INPUT      : None						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	Interrupts are disabled during initialization	|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_ESP_INIT:
		push	Temp								;save useds
		in	Temp,SREG								;save SREG
		cli											;disable interrupts
        cbi	_ESP_PORT_DATA_DIR,_ESP_RX_BIT			;RX is input
		cbi	_ESP_PORT_DATA_OUTPUT,_ESP_RX_BIT		;desliga RX with pull up
		out	SREG,Temp
		pop	Acc
		ret	


    ;>--------------------------------------------------------------<
	;| OBJECT     : _ESP						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : Get data from RX line				|
	;>--------------------------------------------------------------<
	;| INPUT      : None						|
	;| OUTPUT     :	Acc	Data received if cy=0			|
	;|		Acc	Error Code if cy=1			|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	Interrupts are disabled during reception	|
	;|		this routine have a fixed time to wait a data	|
	;|		(about 0.5 sec at 16Mhz) after this time none 	|
	;|		received cy=1 and timeout code is set.		|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|		
	_ESP_DATA_RX:
		push	AccH						;save useds
		in	AccH,SREG						;save SREG
	;	cli									;disable interrupts
		push	AccH
		
		;+-----------------------------------
		;| WAIT RX LINE GOES LOW
		;+-----------------------------------
	_ESP_DATA_RX_10:
		sbis	_ESP_PORT_DATA_INPUT,_ESP_RX_BIT		;skip if no start bit yet
		rjmp	_ESP_DATA_RX_20				;else check if start bit
		rjmp _ESP_DATA_RX_10
		;+-----------------------------------
		;| FINISH ROUTINE WITH ERROR CODE CY=1
		;+-----------------------------------
	_ESP_DATA_RX_ERROR_EXIT:
	
		pop	AccH						;restore SREG
		out	SREG,AccH
		sec							;set error
		pop	AccH						;restore useds
		ret		
		;+-----------------------------------
		;| CHECK IF START BIT
		;+-----------------------------------
	_ESP_DATA_RX_20:

		_M_ESP_DELAY_BIT	24				;wait halt time data bit
		sbis	_ESP_PORT_DATA_INPUT,_ESP_RX_BIT		;RX high, skip if yes
		rjmp	_ESP_DATA_RX_30				;else wait full data time *<NOCHANGE>*
		ldi	Acc,_ESP_STARTBIT_ERROR			;set invalid start bit
		rjmp	_ESP_DATA_RX_ERROR_EXIT			;exit with error *<NOCHANGE>*
	_ESP_DATA_RX_30:
		ldi	AccT,8						;data bits count
     	_M_ESP_DELAY_BIT	56				;wait full time 20mhz
		//_M_ESP_DELAY_BIT	44				;wait full time data bit 16mhz
	_ESP_DATA_RX_40:
		clc							;assume data bit 0
		sbic	_ESP_PORT_DATA_INPUT,_ESP_RX_BIT		;skip if yes
		sec							;else data bit 1
		ror	Acc						;put into Acc
		_M_ESP_DELAY_BIT	56				;wait full time 20mhz
		//_M_ESP_DELAY_BIT	44				;wait full time 16mhz

		 							 
		dec	AccT						;bitcount --
		brne	_ESP_DATA_RX_40				;until all got

    _ESP_PULA:
		sbis	_ESP_PORT_DATA_INPUT,_ESP_RX_BIT		;skip if yes
		rjmp _ESP_PULA

		pop	AccH
		out	SREG,AccH					;restore SREG
		clc							;set data ok
		pop	AccH
		ret		


#endif
.exit