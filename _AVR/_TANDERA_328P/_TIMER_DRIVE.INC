
/*
	#-------------------------------------------------------------------------------#
	|																				|
        |    _____ ________  ___ ___________  ____________ _____ _   _ _____			|
   	|   |_   _|_   _|  \/  ||  ___| ___ \ |  _  \ ___ \_   _| | | |  ___|			|
        |     | |   | | | .  . || |__ | |_/ / | | | | |_/ / | | | | | | |__				|
        |     | |   | | | |\/| ||  __||    /  | | | |    /  | | | | | |  __|			|
        |     | |  _| |_| |  | || |___| |\ \  | |/ /| |\ \ _| |_\ \_/ / |___			|
        |     \_/  \___/\_|  |_/\____/\_| \_| |___/ \_| \_|\___/ \___/\____/			|
        |																				|
	| Este Drive implementa funções para manipulação de tempo , tais como delay		|
	| time_handle, time measure etc.												|
	#-------------------------------------------------------------------------------#
	| Este documento usa TAB 4 para formatação										|
	#-------------------------------------------------------------------------------#
	| Date: Brasilia ,12 de Agosto de 2016											|
	#-------------------------------------------------------------------------------#
	| Para usar este Drive devera ser definido pelo usuario as contantes que segue	|
	| Frequencia em Herts para geração de interrupção de timer e valor de prescaler	|
	|																				|
	|	.EQU	_TIMER_INTERRUPT_FREQUENCY	= 10000									|
	|	.EQU	_TIMER_PRESCALER			= 8										|
	|																				|
	|	Tome cuidado para que o valor do divisor esteja na faixa de 0..255			|
	|	ou seja _AVR_CLOCK/_TIMER_PRESCALER/_TIMER_INTERRUPT_FREQUENCY tem que estar|
	|	na faixa dos 8 bits ou seja entre 0 e 255									|
	|																				|
	#-------------------------------------------------------------------------------#
	|	FUNÇÕES IMPLEMENTADAS														|
	#-------------------------------------------------------------------------------#
	|	INICIALIZA TIMER DRIVE														|
	|																				|
	|	call/rcall	_TIMER_INIT														|
	#-------------------------------------------------------------------------------#
 */

//DEFINE TIMER  DRIVE

#ifndef __TIMER_DRIVE__
	#define __TIMER_DRIVE__
	
	.message "+-------------------------+"
	.message "|  TIMER  DRIVE			|" 
	.message "+-------------------------+"
	
/*
	#-------------------------------------------------------------------------------#
	| TIMER DRIVE																	|
	#-------------------------------------------------------------------------------#
	| DEFINIÇÃO DE CONSTANTES USADOS PELO DRIVE										|
	#-------------------------------------------------------------------------------#
*/

		// Below parameters is valid because 8000000/8/10000 igual 200 ok in range 0..255

		.EQU	_TIMER_INTERRUPT_FREQUENCY	= 101	
		.EQU	_TIMER_PRESCALER			= 1024

	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   INICIALIZA TIMER DRIVE			|
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
 _TIMER1_INIT:
		push	Acc					;save Acc
		in		Acc,SREG			;save SREG
		pushaw
		cli							;disable interrupts
		//
		//Set wgm = 010 CTC mode
		lds		Acc,TCCR1A
		andi	Acc,~((1<<WGM11)|(1<<WGM10))
		//ori		Acc,(1<<WGM11);
		sts		TCCR1A,Acc
		lds		Acc,TCCR1B
		ori   	Acc,(1<<WGM12)
		sts		TCCR1B,Acc
		//
		//Program Timer1 divisor to 1024x
		//
	  	lds		Acc,TCCR1B
	  	andi	Acc,~((1<<CS12)|(1<<CS11)|(1<<CS10))
	  	ori		Acc,(1<<CS10)
	  	ori		Acc,(1<<CS12)

        sts		TCCR1B,Acc
		//
		//Program OCR1A to generate _TIMER_INTERRUPT_FREQUENCY interrupt frequency
		//
		ldi Acc,0
		sts OCR1AH,ACC

		ldi		Acc,(_AVR_CLOCK/_TIMER_PRESCALER/_TIMER_INTERRUPT_FREQUENCY)-1
		sts		OCR1AL,Acc
		//
		//Enable Timer1 compare math A interrupt and set handle
		//
		_SET_HANDLE	_HDC_TIMER1_COMPA_VECT,_TIMER_INTERRUPT_HANDLE
		lds		Acc,TIMSK1
		ori		Acc,(1<<OCIE1A)
		sts		TIMSK1,Acc
		popaw						;Restore SREG
		out		SREG,Acc
		pop		Acc					;restore Acc
		ret
		 

	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   TIMER 1 COMPARE A MATH 		|
	;|   INTERRUPT SERVICE ROUTINE		|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;				/
	;				| This ISR save all registers 0..31 and restore at the end
	;				\______________________________________________________________________
	
	.EQU	_TIMER_DIV	= _TIMER_INTERRUPT_FREQUENCY/1000

	.DSEG
		_TIMER_DIV_TO_MILLISECONDS:			.BYTE	_BYTE				;Divisor to obtain 1ms pump tick timer
		_TIMER_MILLISECONDS:				.BYTE	_LONG				;Long timer with milliseconds of precision
	.CSEG

_TIMER1_STOP:

        push Acc
		push AccT
		in		AccT,SREG
				 
        lds Acc,TCCR1B
		andi	Acc,~((1<<CS12)|(1<<CS11)|(1<<CS10))                   ; configura prescaler para STOPED
		sts TCCR1B,Acc

		call _BUZZER_OFF
		
		ldi  Acc,_OFF
		call _LED_VD_SET
				        		
		out SREG,AccT
		pop AccT
		pop Acc
		ret

_TIMER1_START:

        push Acc
		push AccT
		in		AccT,SREG

        //
		//Program Timer0 divisor to 1024x
		//
		lds		Acc,TCCR1B
		andi	Acc,~((1<<CS12)|(1<<CS11)|(1<<CS10))
		ori		Acc,(1<<CS10)
		ori		Acc,(1<<CS12)

		sts		TCCR1B,Acc
		
		out SREG,AccT
		pop AccT
		pop Acc
		ret
 

.EXIT


/*

	#-------------------------------------------------------------------------------#
	|																				|
    |    _____ ________  ___ ___________  ____________ _____ _   _ _____			|
    |   |_   _|_   _|  \/  ||  ___| ___ \ |  _  \ ___ \_   _| | | |  ___|			|
    |     | |   | | | .  . || |__ | |_/ / | | | | |_/ / | | | | | | |__				|
    |     | |   | | | |\/| ||  __||    /  | | | |    /  | | | | | |  __|			|
    |     | |  _| |_| |  | || |___| |\ \  | |/ /| |\ \ _| |_\ \_/ / |___			|
    |     \_/  \___/\_|  |_/\____/\_| \_| |___/ \_| \_|\___/ \___/\____/			|
    |																				|
	| Este Drive implementa funções para manipulação de tempo , tais como delay		|
	| time_handle, time measure etc.												|
	#-------------------------------------------------------------------------------#
	| Este documento usa TAB 4 para formatação										|
	#-------------------------------------------------------------------------------#
	| Date: Brasilia ,12 de Agosto de 2016											|
	#-------------------------------------------------------------------------------#
	| Para usar este Drive devera ser definido pelo usuario as contantes que segue	|
	| Frequencia em Herts para geração de interrupção de timer e valor de prescaler	|
	|																				|
	|	.EQU	_TIMER_INTERRUPT_FREQUENCY	= 10000									|
	|	.EQU	_TIMER_PRESCALER			= 8										|
	|																				|
	|	Tome cuidado para que o valor do divisor esteja na faixa de 0..255			|
	|	ou seja _AVR_CLOCK/_TIMER_PRESCALER/_TIMER_INTERRUPT_FREQUENCY tem que estar|
	|	na faixa dos 8 bits ou seja entre 0 e 255									|
	|																				|
	#-------------------------------------------------------------------------------#
	|	FUNÇÕES IMPLEMENTADAS														|
	#-------------------------------------------------------------------------------#
	|	INICIALIZA TIMER DRIVE														|
	|																				|
	|	call/rcall	_TIMER_INIT														|
	#-------------------------------------------------------------------------------#
 */
  
//DEFINE TIMER  DRIVE

#ifndef __TIMER_DRIVE__
	#define __TIMER_DRIVE__
	
	.message "+-------------------------+"
	.message "|  TIMER  DRIVE			|" 
	.message "+-------------------------+"
	
/*
	#-------------------------------------------------------------------------------#
	| TIMER DRIVE																	|
	#-------------------------------------------------------------------------------#
	| DEFINIÇÃO DE CONSTANTES USADOS PELO DRIVE										|
	#-------------------------------------------------------------------------------#
*/

		// Below parameters is valid because 8000000/8/10000 igual 200 ok in range 0..255

 		.EQU	_TIMER_INTERRUPT_FREQUENCY	= 101	
		.EQU	_TIMER_PRESCALER			= 1024

	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   INICIALIZA TIMER DRIVE			|
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
	_TIMER0_INIT:
		push	Acc					;save Acc
		in		Acc,SREG			;save SREG
		pushaw
		cli							;disable interrupts
		//
		//Set wgm = 010 CTC mode
		in		Acc,TCCR0A
		andi	Acc,~((1<<WGM01)|(1<<WGM00))
		ori		Acc,(1<<WGM01);
		out		TCCR0A,Acc
		in		Acc,TCCR0B
		andi	Acc,~(1<<WGM02)
		out		TCCR0B,Acc
		//
		//Program Timer0 divisor to 1024x
		//
	  	in		Acc,TCCR0B
	  	andi	Acc,~((1<<CS02)|(1<<CS01)|(1<<CS00))
	  	ori		Acc,(1<<CS00)
	  	ori		Acc,(1<<CS02)

        out		TCCR0B,Acc
		//
		//Program OCR1A to generate _TIMER_INTERRUPT_FREQUENCY interrupt frequency
		//
		ldi		Acc,(_AVR_CLOCK/_TIMER_PRESCALER/_TIMER_INTERRUPT_FREQUENCY)-1
		out		OCR0A,Acc
		//
		//Enable Timer1 compare math A interrupt and set handle
		//
		_SET_HANDLE	_HDC_TIMER0_COMPA_VECT,_TIMER_INTERRUPT_HANDLE
		lds		Acc,TIMSK0
		ori		Acc,(1<<OCIE0A)
		sts		TIMSK0,Acc
		popaw						;Restore SREG
		out		SREG,Acc
		pop		Acc					;restore Acc
		ret
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   TIMER 1 COMPARE A MATH 		|
	;|   INTERRUPT SERVICE ROUTINE		|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;				/
	;				| This ISR save all registers 0..31 and restore at the end
	;				\______________________________________________________________________
	
	.EQU	_TIMER_DIV	= _TIMER_INTERRUPT_FREQUENCY/1000

	.DSEG
		_TIMER_DIV_TO_MILLISECONDS:			.BYTE	_BYTE				;Divisor to obtain 1ms pump tick timer
		_TIMER_MILLISECONDS:				.BYTE	_LONG				;Long timer with milliseconds of precision
	.CSEG

_TIMER0_STOP:

        push Acc
		push AccT
		in		AccT,SREG
				 
        in Acc,TCCR0B
		andi	Acc,~((1<<CS02)|(1<<CS01)|(1<<CS00))                   ; configura prescaler para STOPED
		out TCCR0B,Acc

		call _BUZZER_OFF
				        		
		out SREG,AccT
		pop AccT
		pop Acc
		ret

_TIMER0_START:

        push Acc
		push AccT
		in		AccT,SREG

        //
		//Program Timer0 divisor to 1024x
		//
		in		Acc,TCCR0B
		andi	Acc,~((1<<CS02)|(1<<CS01)|(1<<CS00))
		ori		Acc,(1<<CS00)
		ori		Acc,(1<<CS02)

		out		TCCR0B,Acc
		
		out SREG,AccT
		pop AccT
		pop Acc
		ret


.EXIT

 












