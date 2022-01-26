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
	|	.EQU	_SERIALSOFT_PORT_OUTPU	T	= PORTD									|
	|	.EQU	_SERIALSOFT_PORT_DIR		= DDRD									|
	|	.EQU	_SERIALSOFT_PORT_INPUT		= PIND									|
	|																				|
	|	.EQU	_SERIALSOFT_DATA_BIT		= 1										|
	|																				|
	#-------------------------------------------------------------------------------#
	|	FUNÇÕES IMPLEMENTADAS														|
	#-------------------------------------------------------------------------------#
	|	INICIALIZA SERIAL SOFT														|
	|																				|
	|	call/rcall	_SERIALSOFT_INIT												|
	#-------------------------------------------------------------------------------#
	|	TRANSMITE DATA																|
	|																				|
	|	ldi	Acc,data					;load Acc with serial data to be send		|
	|	call/rcall	_SERIALSOFT_TX		;transmite data at 115200baudrate			|
	#-------------------------------------------------------------------------------#
	|	TRANSMITE STRING DATA FROM FLASH											|
	|																				|
	|	ldiaw	Z,message*2				;z-->flash data string						|
	|	call/rcall	_SERIALSOFT_PUTSF	;send string from flasth to serial soft tx	|
	#-------------------------------------------------------------------------------#
	|	TRANSMITE STRING DATA FROM SRAM												|
	|																				|
	|	ldiaw	Z,message				;z-->sram data string						|
	|	call/rcall	_SERIALSOFT_PUTS	;send string from sram to serial soft tx	|
	#-------------------------------------------------------------------------------#

 */


//DEFINE SERIAL SOFT DRIVE

#ifndef __SERIALSOFT_DRIVE__
	#define __SERIALSOFT_DRIVE__
	
	.message "+---------------------+"
	.message "|   SERIALSOFT DRIVE  |" 
	.message "+---------------------+"

	.equ	 _SERIALSOFT_BAUDRATE = 115200
	.equ	 _SERIALSOFT_BAUDRATE_ADJUST = 6

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
	_SERIALSOFT_INIT:
		push	Acc					;save Acc
		in		Acc,SREG			;save SREG
		push	Acc
		cli							;disable interrupts
		//define serial data port as output hi level
		sbi		_SERIALSOFT_PORT_OUTPUT,_SERIALSOFT_DATA_BIT
		sbi		_SERIALSOFT_PORT_DIR,_SERIALSOFT_DATA_BIT
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
	_SERIALSOFT_PUTC:
		push	AccH					;save AccH
		in		AccH,SREG				;save SREG
		push	AccH
		push	AccT					;save AccT
		cli								;disable interrupts
		clc								;send start bit(always 0)
		rcall	_SERIALSOFT_TX_BIT
		ldi		AccH,8					;number of bits to tx
	_SERIALSOFT_PUTC_LOOP:
		ror		Acc						;set bit to transmit
		rcall	_SERIALSOFT_TX_BIT		;send a bit
		dec		AccH					;last bit send?
		brne	_SERIALSOFT_PUTC_LOOP	;no, transmit again
		sec								;send stop bit(always 1)
		rcall	_SERIALSOFT_TX_BIT
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
	_SERIALSOFT_TX_BIT:
		in		AccT,_SERIALSOFT_PORT_OUTPUT					;get old state
		andi	AccT,~(1<<_SERIALSOFT_DATA_BIT)					;assume data=0
		brcc	_SERIALSOFT_TX_BIT_10							;branch if yes
		ori		AccT,(1<<_SERIALSOFT_DATA_BIT)					;else set data=1
	_SERIALSOFT_TX_BIT_10:
		out		_SERIALSOFT_PORT_OUTPUT,AccT					;set new data to be send
		ldi		AccT,(_AVR_CLOCK/(_SERIALSOFT_BAUDRATE*3))-_SERIALSOFT_BAUDRATE_ADJUST		;set delay for 115200 baud
	_SERIALSOFT_TX_BIT_DELAY:
		subi	AccT,1
		brne	_SERIALSOFT_TX_BIT_DELAY						;delay finished ? no loop
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
	_SERIALSOFT_PUTSF:
		push	Acc						;save Acc
		pushw	Z						;save pointer
	_SERIALSOFT_PUTSF_LOOP:	
		lpm		Acc,Z+					;get data from flash
		cpi		Acc,0					;zero end?
		breq	_SERIALSOFT_PUTSF_EXIT	;yes, exit
		rcall	_SERIALSOFT_PUTC			;send a data
		rjmp	_SERIALSOFT_PUTSF_LOOP	;get new data
	_SERIALSOFT_PUTSF_EXIT:			
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
	_SERIALSOFT_PUTS:
		push	Acc						;save Acc
		pushw	Z						;save pointer
	_SERIALSOFT_PUTS_LOOP:	
		ld		Acc,Z+					;get data from flash
		cpi		Acc,0					;zero end?
		breq	_SERIALSOFT_PUTS_EXIT	;yes, exit
		rcall	_SERIALSOFT_PUTC		;send a data
		rjmp	_SERIALSOFT_PUTS_LOOP	;get new data
	_SERIALSOFT_PUTS_EXIT:			
		popw	Z						;restore Z
		pop		Acc						;restore Acc
		ret		
#endif
.exit

