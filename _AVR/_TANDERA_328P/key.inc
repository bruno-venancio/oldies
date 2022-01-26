
/*
	#-------------------------------------------------------------------------------#
	|																				|
	|                   _   __            ______      _								|
	|                  | | / /            |  _  \    (_)							|
	|                  | |/ /  ___ _   _  | | | |_ __ ___   _____					|
	|                  |    \ / _ \ | | | | | | | '__| \ \ / / _ \					|
	|                  | |\  \  __/ |_| | | |/ /| |  | |\ V /  __/					|
	|                  \_| \_/\___|\__, | |___/ |_|  |_| \_/ \___|					|
	|                               __/ |											|
	|                              |___/											|
	| Este Drive permite ler estado da tecla , saber se esta pressionada ou não		|
	| entre outras funções										|					|
	#-------------------------------------------------------------------------------#
	| Este documento usa TAB 4 para formatação										|
	#-------------------------------------------------------------------------------#
	| Date: Brasilia ,12 de Agosto de 2016											|
	#-------------------------------------------------------------------------------#
	| Para usar este Drive devera ser definido pelo usuario as contantes que segue	|
	| Exemplo abaixo com PORTAS e BITS definidos aleatoriamente como exemplo		|
	|																				|
	| DEFINIÇOES DA PORTA DE COMUNICAÇÃO E BITS										|
	|																				|
	|	.EQU	_KEY_PORT_OUTPUT			= PORTB									|
	|	.EQU	_KEY_PORT_DIR				= DDRB									|
	|	.EQU	_KEY_PORT_INPUT				= PINB									|
	|																				|
	|	.EQU	_KEY_BIT					= 4										|
	|																				|
	#-------------------------------------------------------------------------------#
	|	FUNÇÕES IMPLEMENTADAS														|
	#-------------------------------------------------------------------------------#
	|	INICIALIZA CONFIG KEY DRIVE													|
	|																				|
	|	call/rcall	_KEY_INIT														|
	#-------------------------------------------------------------------------------#
	|	LE ESTADO DA TECLA DE CONFIGURAÇÃO											|
	|																				|
	|	call/rcall	_KEY_READ		;after this call cy=1 if key pressed			|									
	#-------------------------------------------------------------------------------#
 */

//DEFINE CONFIG KEY DRIVE

#ifndef __KEY_DRIVE__
	#define __KEY_DRIVE__
	
	.message "+-------------------------+"
	.message "|   KEY DRIVE				|" 
	.message "+-------------------------+"

	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   INICIALIZA KEY DRIVE			|
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
	_KEY_INIT:
		push	Acc					;save Acc
		in		Acc,SREG			;save SREG
		push	Acc
		cli							;disable interrupts
		//DEFINE CONFIG KEY AS INPUT WITH PULL-UP
		sbi		_KEY_PORT_OUTPUT,_KEY_BIT
		cbi		_KEY_PORT_DIR,_KEY_BIT
		pop		Acc					;Restore SREG
		out		SREG,Acc
		pop		Acc					;restore Acc
		ret
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   LED ESTADO DA KEY				|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : CY=1 if pressed		|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_KEY_READ:
		sec												;assume key pressed?
		sbic	_KEY_PORT_INPUT,_KEY_BIT				;skip if pressed
		clc												;else not pressed
		ret
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|  AGUARDA KEY SER LIBERADA		|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_KEY_RELEASE:
		_M_DELAY_MILLISECONDS	20						;check read time
		rcall	_KEY_READ								;read key
		brcs	_KEY_RELEASE							;read until pressed
		_M_DELAY_MILLISECONDS	100						;debounce time
		ret

#endif
.exit


