
/*
	#-------------------------------------------------------------------------------#
    |         _      ___________   _____ _____ ___ _____ _   _ _____				|
    |        | |    |  ___|  _  \ /  ___|_   _/ _ \_   _| | | /  ___|				|
    |        | |    | |__ | | | | \ `--.  | |/ /_\ \| | | | | \ `--.				|
    |        | |    |  __|| | | |  `--. \ | ||  _  || | | | | |`--. \				|
    |        | |____| |___| |/ /  /\__/ / | || | | || | | |_| /\__/ /				|
    |        \_____/\____/|___/   \____/  \_/\_| |_/\_/  \___/\____/				|
    |                                                                               |
	| Este Drive permite controle da LED de LED_VM para indicar se o DIMMER esta 	|
	| Ligado ou Desligado e também usado durante configuração do DIMMER				|
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
	|	.EQU	_LED_LED_VM_PORT_OUTPUT			= PORTB								|
	|	.EQU	_LED_LED_VM_PORT_DIR			= DDRB								|
	|	.EQU	_LED_LED_VM_PORT_INPUT			= PINB								|
	|																				|
	|	.EQU	_LED_LED_VM_BIT					= 0									|
	|																				|
	#-------------------------------------------------------------------------------#
	|	FUNÇÕES IMPLEMENTADAS														|
	#-------------------------------------------------------------------------------#
	|	INICIALIZA LED LED_VM DRIVE													|
	|																				|
	|	call/rcall	_LED_LED_VM_INIT												|
	#-------------------------------------------------------------------------------#
	|	DEFINE ESTADO DA LED LED_VM													|
	|																				|
	|	ldi	Acc,_ON						;Set LED CPU state to _ON or _OFF			|
	|	call/rcall	_LED_LED_VD_SET		;Define state								|
	#-------------------------------------------------------------------------------#
	|	PERMUDA ESTADO DA LED LED_VM												|
	|																				|
	|	call/rcall	_LED_LED_VD_SWAP	;Permuta Estado se _ON fica _OFF e viceversa|
	#-------------------------------------------------------------------------------#
 */

//DEFINE LED CPU DRIVE

#ifndef __LED_VM_DRIVE__
	#define __LED_VM_DRIVE__
	
	.message "+------------------+"
	.message "|   LED VM DRIVE   |" 
	.message "+------------------+"

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
	_LED_VM_INIT:
		push	Acc					;save Acc
		in		Acc,SREG			;save SREG
		push	Acc
		cli							;disable interrupts
		//DEFINE LED CPU AS OUTPUT LO LEVEL(OFF)
		;cbi		_LED_VM_PORT_OUTPUT,_LED_VM_BIT
		sbi		_LED_VM_PORT_DIR,_LED_VM_BIT
		sbi		_LED_VM_PORT_OUTPUT,_LED_VM_BIT	    ;turn led off
		pop		Acc					;Restore SREG
		out		SREG,Acc
		pop		Acc					;restore Acc
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
		cpi		Acc,_ON									;is ON?
		brne	_LED_VD_SET_10						;no, branch
		cbi		_LED_VM_PORT_OUTPUT,_LED_VM_BIT	;turn led on
		ret
	_LED_VD_SET_10:
		sbi		_LED_VM_PORT_OUTPUT,_LED_VM_BIT	;turn led off
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
		push	AccH

		in		Acc,_LED_VM_PORT_OUTPUT			    ;get actual state
		ldi		AccH,(1<<_LED_VM_BIT)		    	;set bit to swap
		eor		Acc,AccH							;swap bit state
		out		_LED_VM_PORT_OUTPUT,Acc
		
		pop		AccH								;restore Useds
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
	_LED_VM_PISCA2:

		push	Acc									;save useds
		push	AccH
		
		ldi Acc,_ON
		call _LED_VD_SET

		_M_DELAY_MILLISECONDS 200

		ldi Acc,_OFF
		call _LED_VD_SET

		_M_DELAY_MILLISECONDS 200

		ldi Acc,_ON
		call _LED_VD_SET

		_M_DELAY_MILLISECONDS 200

	    ldi Acc,_OFF
		call _LED_VD_SET

		pop		AccH								;restore Useds
		pop		Acc

		ret






#endif
.exit


