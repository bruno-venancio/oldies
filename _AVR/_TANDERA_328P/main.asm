;    +--------------------=-=-=-=-=-=-=-=-=-=-=-----------------+
;    | Projeto: Velocimetro Tandera 2019                        |
;  -=| Microcontrolador: ATMEL ATMEGA328P em 20MHz              |=-
;    | Programador: Bruno Marcio Diogo Venancio   v.1.00        |
;    | Data: 09/04/2018                                         |
;    +--------------------=-=-=-=-=-=-=-=-=-=-=-----------------+

/*
	#-------------------------------------------------------#
	| INCLUSÃO DE DEFINIÇOES GLOBAIS DA LIBRARY 2 NA PASTA	|
	| LIBRARY2, ESTA INCLUSÃO DEVE SER SEMPRE A 1A INCLUSAO	|
	| NO PROJETO, QUALQUER DÚVIDA OLHE NA PASTA _MANUAL		|
	| LIBRARY2_REFERENCE_MANUAL_FREE.PDF					|
	#-------------------------------------------------------#
*/
       
 	
     //DEFINE FREQUENCIA DA CPU EM HZ
	.EQU	_AVR_CLOCK		= 20000000
  
	.INCLUDE	"DEFS\GLOBAL_DEFINITIONS\GLOBDEFS.INC"
	.INCLUDE	"DEFS\M328P_FILES\M328PDEF.INC"				

/*
	#-------------------------------------------------------#
	| DEFINIÇÕES GLOBAIS DO TIPO DE BOOT E FREQUENCIA DA CPU|
	| NESTE PROJETO SERÁ USADA A FREQUENCIA DE 8MHZ USANDO	|
	| O CLOCK INTERNO DO ATMEGA								|
	#-------------------------------------------------------#
*/

	//HABILITA LIMPEZA DE MEMORIA DURANTE BOOT
	.EQU	_SRAM_BOOT_TYPE	=_SRAM_CLEAR

	
/*
	#-------------------------------------------------------#
	| INCLUSÃO DAS DEFINIÇÕES DA CPU E MANIPULADORES DE IN-	|
	| TERRUPÇÃO												|
	#-------------------------------------------------------#
*/

	.INCLUDE	"DEFS\M328P_FILES\M328PHDC.INC"				;DEFINIÇÕES DO MANIPULADOR DE INTERRUPÇÃO


    ;+------------------------------------
	;| MATH ROUTINES
	;+------------------------------------

	.INCLUDE	"DEFS\MATH_DEFINITIONS\MATHCONS.INC"		    ;MATH DEFINITIONS
	.INCLUDE	"MATH\INTEGER\MULDIV_U8U8U8\MULDIV_U8U8U8U8.HUG";(a*b)/c unsigned 8 bits
	.INCLUDE	"MATH\INTEGER\DIV_U16U16U16\DIV_U16U16U16.HUG"	;unsigned integer divide 16 bits
	;.INCLUDE	"MATH\INTEGER\DIV_U32U32U32\DIV_U32U32U32.HUG"	;unsigned integer divide 32 bits
	.INCLUDE	"MATH\INTEGER\MUL_U16U16U32\MUL_U16U16U32.INC"	;unsigned 16 X 16
	.INCLUDE	"MATH\INTEGER\DECBIN\DECBIN.INC"	        	; ASCII to BIN
	.INCLUDE	"MATH\INTEGER\ULIBTOS\ULIBTOS.INC"		        ; unsigned long to string
	.INCLUDE	"MATH\INTEGER\DIV_U32U32U32\DIV_U32U32U32.INC"		
	.INCLUDE	"MATH\SQRT\SQRT.INC"	
	.INCLUDE    "MATH\INTEGER\INT_ATN2PTS\INT_ATN2PTS.INC"	
	
/*
	#-------------------------------------------------------#
	| INCLUSÃO DE ROTINAS GLOBAIS							|
	#-------------------------------------------------------#
*/
	.INCLUDE "DEFS\MATH_DEFINITIONS\MATHCONS.INC"			;constantes necessárias para rotinas que usam matemática
	.INCLUDE "TIME\DELAYS\DELAYS.HUG"						;rotinas de Delay
	.INCLUDE "MATH\INTEGER\ITOS\ITOS.HUG"
/*
	#-------------------------------------------------------#
	| INCLUSÃO DE ROTINAS DEPENDENTES DE HARDWARE			|
	| NESTA INCLUSÃO SERÁ DEFINIDO O DRIVE DO SSD1306		|
	#-------------------------------------------------------#
*/
	.INCLUDE	"HARDWARE_DEFINITIONS.INC"	
/*
	#-------------------------------------------------------#
	| INCLUSÃO DE ROTINAS NÃO DEPENDENTES DE HARDWARE		|
	| APENAS FUNÇÕES GLOBAIS DO USUARIO(TRANSPARENCIA)		|
	#-------------------------------------------------------#
*/
	.INCLUDE	"LOCAL_DEFINITIONS.INC"

/*
	#-------------------------------------------------------#
	| INCLUSÃO DE ROTINAS DEMONSTRATIVAS DO DRIVE SSD1306	|
	#-------------------------------------------------------#
*/
		
	.INCLUDE	"RESOURCES\logo_flat.inc"
	.INCLUDE	"TELA_SAT.INC"
	
	;+------------------------------------
	;| COMMUNICATIONS ROUTINES
	;+------------------------------------

	.INCLUDE	"USART.INC"	       ; UART routines
	.INCLUDE    "INT_SERIAL.INC"   ; Interrupcao serial
	.INCLUDE    "SerialSoftware.inc"
	;+------------------------------------
	;| SYSTEM ROUTINES
	;+------------------------------------

	.INCLUDE    "LOAD_SRAM.INC"
	.INCLUDE    "MISC.INC"
	.INCLUDE    "COMP_ATN.INC"
	.INCLUDE    "DIRECAO.INC"
	.INCLUDE    "PROCPONT.INC"
	.INCLUDE    "ESP.INC"
	.INCLUDE    "MENU.INC"
	;.INCLUDE    "PROTO.INC"
	;.INCLUDE    "_KEYBOARD.INC"

	.INCLUDE    "GPS_INFO.INC"
	.INCLUDE    "ATUALIZA_PONTOS.INC"
	;.INCLUDE    "MNU_CONFIG.INC"
	.INCLUDE    "_TIMER_DRIVE.INC"
	.INCLUDE    "MARCA_PNT.INC"
	.INCLUDE    "APAGA_PNT.INC"
	.INCLUDE    "MOD_PNT.INC"
	.INCLUDE    "TMP_AVISO.INC"
	.INCLUDE    "VOLUME.INC"
	;.INCLUDE    "CONTRAST.INC"


 ; Int 0
	.INCLUDE    "INT_0.INC"


	.INCLUDE    "INT_TMR1.INC"

	

/*
	#-----------------------------------------------------------------------#
    |                         ___  ___      _								|
    |                         |  \/  |     (_)								|
    |                         | .  . | __ _ _ _ __							|
    |                         | |\/| |/ _` | | '_ \							|
    |                         | |  | | (_| | | | | |						|
    |                         \_|  |_/\__,_|_|_| |_|						|
    |																		|
    |																		|
    |        _____      _ _   _       _ _          _   _					|
    |       |_   _|    (_) | (_)     | (_)        | | (_)					|
    |         | | _ __  _| |_ _  __ _| |_ ______ _| |_ _  ___  _ __			|
    |         | || '_ \| | __| |/ _` | | |_  / _` | __| |/ _ \| '_ \		|
    |        _| || | | | | |_| | (_| | | |/ / (_| | |_| | (_) | | | |		|
    |        \___/_| |_|_|\__|_|\__,_|_|_/___\__,_|\__|_|\___/|_| |_|		|
    |																		|
    |																		|
	#-----------------------------------------------------------------------#
	
*/
_MAIN_INIT:

    	_SET_HANDLE _HDC_USART_RXC_VECT,INT_RX_HANDLE
 		_SET_HANDLE	_HDC_INT0_VECT,_INT0_INTERRUPT_HANDLE
		  			
	  ;  call _LED_VD_INIT
		call _LEDS_INI
		call _BUZZER_INI
		call _BUZZER_OFF
		;call _TIMER0_PWM_STOP
		sbi	_BUZZER_PWM_OUTPUT,_BUZZER_PWM_BIT	
    	
		sbi	_AT45DB161B_PORT_DIR,_AT45DB161B_WP_BIT		;WP=low(WRITE PROTECTED) output
		cbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_WP_BIT	;WP=low(WRITE PROTECTED)
				
	 	call _ESP_TX_INIT   ; Inicia transmissao
	    call _ESP_INIT      ; Inicia recepcao

        ; Ini Flags

        _CLEAR _FLAG_POSSUI_DADOS_GPS
        _CLEAR _FLAG_START
		_CLEAR _FLAG_MENOR_ZERO
		_CLEAR _FLAG_SAT_FOUND
		_CLEAR _FLAG_APAGA_EEPROM
		;_CLEAR _FLAG_CARREGA_R7
		_CLEAR _HEXFLAG
		_CLEAR _FLAG_APITA
		_CLEAR _FLAG_PONTO_ENCONTRADO
		_CLEAR _FLAG_ATAN_OK
		_CLEAR _FLAG_ANG_ERRO
		_CLEAR _FLAG_PARDAL_LOCALIZADO
		_CLEAR _FLAG_CARRO_PARADO
        _CLEAR _FLAG_TESTA_OK
		_SET   _FLAG_1_MINUTO
		_SET   _PISCA_PONTOS
		_CLEAR _FLAG_MARCA_PONTO
		_CLEAR _FLAG_APAGA_PONTO
		 
		;_CLEAR _FLAG_INICIOU_LINHA
		_CLEAR _FLAG_WIFI
		_CLEAR _FLAG_ATUAL_VEL_CORRENTE
		_CLEAR _FLAG_SWAP_HORA
		_SET   _FLAG_CARREGA_SRAM
		_CLEAR _FLAG_MAIOR39
		_CLEAR _FLAG_ACIMA_120KM

	    sbi _INT0_PORT_INPUT,_INT0_BIT   ; PULL UP
		cbi _INT0_PORT_DIR,_INT0_BIT	 ; ENTRADA 
				
        ldi Acc,0 
		sts _RAM_PONTOS_CONT,Acc
		sts _CONTADOR8,Acc
		sts PRR,Acc
		sts UCSR0B,Acc
		sts _GUARDA_R4,Acc
		
		sts _CONTADOR16_L,Acc
 		sts _CONTADOR16_H,Acc

		ldi Acc,3
		sts _GUARDA_R4,Acc
         
        ldi Acc,94
        sts _CONTADOR_BUF,Acc
		
		ldi Acc,90
		sts _VAR_BEEP,Acc

		ldi Acc,3
		sts _VAR_3_MINUTOS,Acc

        ldi Acc,12
		sts _TICKS,Acc

		ldi Acc,0
		sts _GPS_TICKS_COUNT,Acc
		sts _VEL_CORRENTE,Acc

		;Inicia quadrante normal
		ldi Acc,low(7000)
		sts _VAR_QUAD_L,Acc

		ldi Acc,high(7000)
		sts _VAR_QUAD_H,Acc

		ldi Acc,0xff
		sts _NOSAT_VAR,Acc                        ; Contador de variavel fora do ar
						
        // Carrega contador de pontos da memoria
        
        ldi AccH,LOW(_EE_CONT_PONTOS_L)
		ldi AccT,HIGH(_EE_CONT_PONTOS_L)
		call _EEPROM_READ       

		sts _CONT_PONTOS_L,Acc

        ldi AccH,LOW(_EE_CONT_PONTOS_H)
		ldi AccT,HIGH(_EE_CONT_PONTOS_H)
		call _EEPROM_READ       

		sts _CONT_PONTOS_H,Acc
				    
        cpi Acc,0xff
		brne CONTADOR_PONTOS_CONTINUA

		lds ACC,_CONT_PONTOS_L
		cpi Acc,0Xff

		// Verifica primeiro boot
		ldi AccH,low(_EE_FIRST_BOOT)
		ldi AccT,high(_EE_FIRST_BOOT)
		call _EEPROM_READ

		cpi Acc,'F'
		
		breq CONTADOR_PONTOS_CONTINUA

		// Grava o primeiro boot com os pontos de fabrica

		call _GRAVA_PONTOS_FABRICA

		// grava byte do first boot
        ldi AccH,low(_EE_FIRST_BOOT)
		ldi AccT,high(_EE_FIRST_BOOT)
		ldi Acc,'F'
		call _EEPROM_WRITE
		   		 
CONTADOR_PONTOS_CONTINUA:

        // Verifica se Nro. pontos < 31500

		lds Acc,_CONT_PONTOS_L
		lds AccH,LOW(20000)
		
		lds Temp,_CONT_PONTOS_H
		ldi TempH,HIGH(20000)
		
		cp  Acc,accH		
		cpc Temp,TempH

		brsh TEMP_CHEGADA_CONTINUA

		call _BEEP
		call _BEEP
		call _BEEP
	    
		call _GRAVA_PONTOS_FABRICA
		
TEMP_CHEGADA_CONTINUA:

        // Verifica se Nro. pontos > 65000

		lds Acc,_CONT_PONTOS_L
		lds AccH,LOW(65000)
		
		lds Temp,_CONT_PONTOS_H
		ldi TempH,HIGH(65000)
		
		cp  Acc,accH		
		cpc Temp,TempH

		brlo TEMP_CHEGADA_CONTINUA2

		call _BEEP
		call _BEEP
		call _BEEP
	    
		call _GRAVA_PONTOS_FABRICA

TEMP_CHEGADA_CONTINUA2:

        ldi AccH,LOW(_EE_TEMPO_CHEGADA)
		ldi AccT,HIGH(_EE_TEMPO_CHEGADA)
		call _EEPROM_READ       

		sts _TEMPO_CHEGADA,Acc

		cpi Acc,20
        
		brlo CONTADOR_PONTOS_CONTINUA2
		breq CONTADOR_PONTOS_CONTINUA2
        
        ;if > 15 then TEMPO_CHEGADA=15

		ldi Acc,20
		sts _TEMPO_CHEGADA,Acc

CONTADOR_PONTOS_CONTINUA2:

        lds Acc,_TEMPO_CHEGADA
		cpi Acc,11

        brsh CONTADOR_PONTOS_CONTINUA3

		;if <6 then TEMPO_CHEGADA=6
        ldi Acc,10
     	sts _TEMPO_CHEGADA,Acc

CONTADOR_PONTOS_CONTINUA3:
     
		; _FLAG_VEL_CORRENTE <- EEPROM

        ldi AccH,LOW(_EE_VEL_CORRENTE)
		ldi AccT,HIGH(_EE_VEL_CORRENTE)
		call _EEPROM_READ       

		cpi Acc,'1'
		brne VEL_CORRENTE_PULA
				
		_SET _FLAG_VEL_CORRENTE
		rjmp VEL_CORRENTE_CONT

VEL_CORRENTE_PULA:
        _CLEAR _FLAG_VEL_CORRENTE
	 
VEL_CORRENTE_CONT:

        ldi AccH,LOW(_EE_FLAG_HALT)
		ldi AccT,HIGH(_EE_FLAG_HALT)
		call _EEPROM_READ

		sts _FLAG_HALT,Acc
		 	
		; Carrega pardais totais e acima
		ldi AccH,LOW(_EE_PARDAIS_TOTAIS)
		ldi AccT,HIGH(_EE_PARDAIS_TOTAIS)
		call _EEPROM_READ
		cpi Acc,0xff
		brne PARDAIS_TOTAIS_PL
		ldi Acc,0
PARDAIS_TOTAIS_PL:
		sts _PARDAIS_TOTAIS,Acc
				
		ldi AccH,LOW(_EE_PARDAIS_ACIMA)
		ldi AccT,HIGH(_EE_PARDAIS_ACIMA)
		call _EEPROM_READ
		cpi Acc,0xff
		brne PARDAIS_ACIMA_PL
		ldi Acc,0
PARDAIS_ACIMA_PL:
		sts _PARDAIS_ACIMA,Acc

		;VOLUME
		ldi AccH,LOW(_EE_VOLUME)
		ldi AccT,HIGH(_EE_VOLUME)
		call _EEPROM_READ  

		cpi Acc,0xff
		brne PARDAIS_ACIMA_PL1

		ldi Acc,10
				
PARDAIS_ACIMA_PL1:		 
		sts _FLAG_VOLUME,Acc   
					   		
		; Inicializa serial
	 	
		call _USART_SET_NORMAL_SPEED
        LDIAW 9600
        call _USART_SET_BAUD_RATE
	
		ldi Acc,_OFF
		call _USART_TX_INT_ENABLE
		
        ldi Acc,_OFF
		call _USART_TX_ENABLE
 
        ldi Acc,_ON
		call _USART_RX_ENABLE
	  			
		_M_DELAY_MILLISECONDS 50
		        
	    call _SSD1306_Init					    // Inicia SSD1306
				
		ldi Acc,255
		call _SSD1306_Set_Contrast
      
   	    call _AT45DB161B_INIT                   // Inicia memoria flash
        _M_DELAY_MILLISECONDS 50
		


; TESTE DE BUZZER 
		;sbis _INT0_PORT_INPUT,_INT0_BIT
	    ;jmp _BUZZER_TEST
		;JMP PROTOCOLO
		 	
;MAIN_INI_CONT:
		      
		; Carrega Gira Tela
		ldi AccH,LOW(_EE_GIRA_TELA)
		ldi AccT,HIGH(_EE_GIRA_TELA)
		call _EEPROM_READ       

		sts _FLAG_GIRA_TELA,Acc

		call _GIRA_TELA

		call _BUZZER_ON
		
		ldi  Acc,_ON
		call _LED_VD_SET
		
		_M_DELAY_MILLISECONDS 100
			
		call _BUZZER_OFF
		ldi  Acc,_OFF
		call _LED_VD_SET
		
		_M_DELAY_MILLISECONDS 100
		
       
        ; botar rotina de protocolo depois
		          ;        jmp  CONTINUA_NORMALMENTE
		call _TESTA_MEMORIA
			
		lds Acc,_FLAG_TESTA_OK
		cpi Acc,_ON
		    
		brne CONTINUA_NORMALMENTE
			
		ldi		Acc,_WHITE							    ;Set White Logical Mode
		call	_SSD1306_Set_Operator			
		ldi		Acc,1								    ;Set Scale to 2
		call	_SSD1306_Set_Text_Scale			
		call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
		call	_SSD1306_Cls
	
		// LCD <- ERRO DE MEMORIA!
		ldiw	Z,MSG_ERRO_MEMORIA*2	                      
		ldiw    X,13
		ldiw	Y,16
		call	_SSD1306_Locate							;locate msg
		call	_SSD1306_PutSF
        call    _SSD1306_Refresh
		
		call _BUZZER_ON
		ldi  Acc,_ON
		call _LED_VD_SET

		_M_DELAY_MILLISECONDS 500

		call _BUZZER_OFF

		ldi  Acc,_OFF
		call _LED_VD_SET

		_M_DELAY_MILLISECONDS 800

		call	_SSD1306_Cls
        
		rjmp CONTINUA_NORMALMENTE
		
MSG_ERRO_MEMORIA:
	    .DB "CHECKSUM MEMORIA!",0 					
		
CONTINUA_NORMALMENTE:	


    //    call _KEYBOARD_READ_FLASH
	 //   cpi Acc,_KEY_UP
      //  brne CONTINUA_NORMALMENTE2	

		//ldiw Z,0
//		call _MENU_SERVICO
		
CONTINUA_NORMALMENTE2:
		 
	    ; Carrega BMP de apresentaçao
     
	    ldi		Acc,_INVERT							;Set White Logical Mode
        call	_SSD1306_Set_Operator			
        ldiw	z,bmp_logo*2
	    ldiw	x,0
	    ldiw	y,0
	    call	_SSD1306_Draw_bmp

		; Titulo abaixo da logo

		ldi		Acc,_WHITE							    ;Set White Logical Mode
		call	_SSD1306_Set_Operator			
		ldi		Acc,1								    ;Set Scale to 2
		call	_SSD1306_Set_Text_Scale			
		call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
		; Titulo abaixo da logo

		ldi		Acc,_WHITE							    ;Set White Logical Mode
		call	_SSD1306_Set_Operator			
		ldi		Acc,1								    ;Set Scale to 2
		call	_SSD1306_Set_Text_Scale			
		call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
		ldiw	Z,MSG_TITULO_LOGO*2	                        
		
		ldiw    X,20
		ldiw	Y,16
		call	_SSD1306_Locate							;locate msg
		call	_SSD1306_PutSF

		call    _SSD1306_Refresh
		
		_M_DELAY_MILLISECONDS 1000
		
	  	call    fn_Show_pbar_apres
		
		call _BUZZER_ON
		
		ldi  Acc,_ON
		call _LED_VD_SET

		_M_DELAY_MILLISECONDS 10 
		call _BUZZER_OFF
		
		ldi  Acc,_OFF
		call _LED_VD_SET
    			   
		_M_DELAY_MILLISECONDS 800 

		call _tela_qty_pontos

		;call _BUZZER_ON
		;ldi  Acc,_ON
		;call _LED_VD_SET

		;_M_DELAY_MILLISECONDS 5 

		;call _BUZZER_OFF
		;;ldi  Acc,_OFF
		;call _LED_VD_SET
    			   
		_M_DELAY_MILLISECONDS 1500 
					
        call _FRASE_INICIAL

		ldiw Z,AT_AT*2
		call _SEND_STRING_ESP
		  	
	 	;call _WIFI_INI

		call _INT0_INI
			
TELA_PROC_SAT:
        						
	    call  fn_ShowSatIni
	    call _TIMER1_INIT
		call _TIMER1_STOP	
		
		call _INT0_ENABLE
		    
/*
	#-----------------------------------------------------------------------#
    |   ___  ___      _        ______										|
    |   |  \/  |     (_)       | ___ \										|			
    |   | .  . | __ _ _ _ __   | |_/ / __ ___   __ _ _ __ __ _ _ __ ___		|
    |   | |\/| |/ _` | | '_ \  |  __/ '__/ _ \ / _` | '__/ _` | '_ ` _ \	|
    |   | |  | | (_| | | | | | | |  | | | (_) | (_| | | | (_| | | | | | |	|
    |   \_|  |_/\__,_|_|_| |_| \_|  |_|  \___/ \__, |_|  \__,_|_| |_| |_|	|
    |                                           __/ |						|
    |                                          |___/						|
    |           _____      _               ______     _       _				|
    |          |  ___|    | |              | ___ \   (_)     | |			|
    |          | |__ _ __ | |_ _ __ _   _  | |_/ /__  _ _ __ | |_			|
    |          |  __| '_ \| __| '__| | | | |  __/ _ \| | '_ \| __|			|
    |          | |__| | | | |_| |  | |_| | | | | (_) | | | | | |_			|
    |          \____/_| |_|\__|_|   \__, | \_|  \___/|_|_| |_|\__|			|
    |                                __/ |									|
    |                               |___/									|
	#-----------------------------------------------------------------------#
	 
*/
REINICIA:
        
        ldi Acc,_ON
		call _USART_RX_INT_ENABLE
        //
	    //Enable global interrupt
	    //
        sei
REIN_L0:

        lds Acc,_FLAG_MARCA_PONTO
	    cpi Acc,_ON

		brne REIN_L1

        lds Acc,_FLAG_CARRO_PARADO   // nao marca se tiver com o carro parado
		cpi Acc,_ON
		breq REIN_L1
		
		_CLEAR _FLAG_MARCA_PONTO
				
		ldiw Z,0
        call _MARCA_PONTO	
		
		rjmp REIN_L0
		
REIN_L1:
        lds Acc,_FLAG_WIFI
	    cpi Acc,_ON

		brne REIN_L2

		cli 
		
REI_L1_RETIFY:
        sbis _INT0_PORT_INPUT,_INT0_BIT     ; So passa daqui quando soltar o botao
        rjmp REI_L1_RETIFY

		_M_DELAY_MILLISECONDS 50
		
		_CLEAR _FLAG_WIFI
		

		call _INT0_DISABLE
		sei //**
		ldiw Z,0
        call _MENU	

		call _INT0_ENABLE

		rjmp REINICIA

REIN_L2:		  

        lds Acc,_FLAG_HALT
		cpi Acc,_ON
		breq REI_L2_CONT
		
		call _SYSTEM_HALT
		cli
		ldiw Z,0
        call _MENU	

		jmp REINICIA
				
REI_L2_CONT:		       
 		        	         
		lds Acc,_FLAG_POSSUI_DADOS_GPS
		cpi Acc,_ON
              
        lbrne REIN_L0
	    _CLEAR _FLAG_POSSUI_DADOS_GPS	
			
	    ; Desliga int serial RX	
		ldi Acc,_OFF
		call _USART_RX_INT_ENABLE 	 
	    	    

; $GPRMC,053750.003,A,1548.8757,S,04758.5823,W,000.0,070.4,290409,,,A*64
; $GPRMC,144529.000,A,1550.8060,S,04758.5758,W,25.82,91.56,041016,,,A*

;
; Pega os dados vindos do GPS,converte para binario e organiza na memoria
;		

MONTAGEM:
      
        LDIX _BUFFER_GPS
		ldi AccT,2

ACHA_STATUS:
	    
		ld Acc,X
		adiw X,1
		
		cpi Acc,','
		brne ACHA_STATUS
		
		subi AccT,1
		tst AccT
		brne ACHA_STATUS 

		ld Acc,X

		cpi Acc,'A'
		brne STATUS_INVALIDO

	    ldi Acc,4

		jmp MONTAGEM_CONT

STATUS_INVALIDO:

        _CLEAR _FLAG_POSSUI_DADOS_GPS
		_CLEAR _FLAG_SAT_FOUND

		; decrementa variavel satelite fora do ar
		lds Acc,_NOSAT_VAR
		dec Acc
		sts _NOSAT_VAR,Acc
		tst Acc
		brne STATUS_INVALIDO_CONT

		call _BEEP2
		ldi Acc,'1'
		sts _CONT2,Acc
		call fn_ShowInstru1

		_M_DELAY_MILLISECONDS 9000
				
		call _BEEP2
		ldi Acc,'2'
		sts _CONT2,Acc
		call fn_ShowInstru1

		_M_DELAY_MILLISECONDS 9000

		call _BEEP2
		ldi Acc,'3'
		sts _CONT2,Acc
		call fn_ShowInstru1

		_M_DELAY_MILLISECONDS 9000

		ldi Acc,0xff
		sts _NOSAT_VAR,Acc
		
STATUS_INVALIDO_CONT:

		call _TIMER1_STOP

        call _BUZZER_OFF
		_CLEAR _FLAG_PARDAL_LOCALIZADO

		;call fn_ShowSatIni

		call _TELA_TIME
	
		
		;ldi Acc,'*'
		;call _SSD1306_putC

		;call _SSD1306_Refresh

 ; TESTAR PARTIDA FRIA
			     
STATUS_INVALIDO_PULA:

		_SET _FLAG_CARRO_PARADO
		
AJUSTA:
        lds Acc,_FLAG_WIFI
		cpi Acc,_ON
		brne TELA_RESUMO_DIARIO_OFFLINE  

		cli

AJUSTA_RETIFY:
        sbis _INT0_PORT_INPUT,_INT0_BIT     ; So passa daqui quando soltar o botao
        rjmp AJUSTA_RETIFY

		_M_DELAY_MILLISECONDS 50

		call _INT0_DISABLE

		_CLEAR _FLAG_WIFI
		sei   //**
		ldiw Z,0
		call _MENU

		call _INT0_ENABLE
		
		jmp TELA_PROC_SAT
;
;
;
TELA_RESUMO_DIARIO_OFFLINE:
        lds Acc,_FLAG_SWAP_HORA
		cpi Acc,_ON
		lbrne REINICIA   

		call fn_tela_pontuacao

TELA_RESUMO_DIARIO_OFFLINE_L1:
        lds Acc,_FLAG_SWAP_HORA        
		cpi Acc,_ON
		breq TELA_RESUMO_DIARIO_OFFLINE_L1

		rjmp REINICIA
		

; *** ONLINE (GPS Sincronizado) *** 		

MONTAGEM_CONT:
    
		_SET _FLAG_SAT_FOUND
						
		; +---------------------------+
        ; | Envia coordenada Latitude |
		; +---------------------------+
		call fn_parsing_lat

		; +------------------------------+
		; | Converte decimal para degree | 
		; +------------------------------+
		call fn_degree_to_minutes_lat

		; +---------------------------+
        ; | Envia coordenada Longitude|
        ; +---------------------------+
		call fn_parsing_lon

        ; +------------------------------+
		; | Converte decimal para degree | 
		; +------------------------------+
		call fn_degree_to_minutes_lon

		;+--------------------------------------------------------------------------------+
        ;|Chegando aqui, as coordenadas X e Y ja estao no buffer definitivo em binario 32b|
        ;+--------------------------------------------------------------------------------+

		;+-----------------+
		;| Pega Velocidade |
		;+-----------------+
		call fn_parsing_vel
				
        ; Testa se o carro esta parado ou muito devagar
		lds Acc,_VELOCIDADE
        cpi Acc,4

		brlo TRANSF_VEL_PARADO 
        breq TRANSF_VEL_PARADO
		brge TRANSF_VEL_MOVIMENTANDO   
		         
TRANSF_VEL_PARADO:

        lds Acc,_FLAG_CARRO_PARADO     ; se ja estiver parado, pula
		cpi Acc,_ON
		lbreq TRANSF_VEL_CONT3

        _SET _FLAG_CARRO_PARADO

		; Forca carrega ram
		ldi Acc,23
		sts _GPS_TICKS_COUNT,Acc
		
        rjmp TRANSF_VEL_CONT3

TRANSF_VEL_MOVIMENTANDO:
 
        _CLEAR _FLAG_CARRO_PARADO
	    _CLEAR _FLAG_SWAP_HORA
							 				 
TRANSF_VEL_CONT:

        lds Acc,_FLAG_PARDAL_LOCALIZADO    ; pardal esta apitando?
		cpi Acc,_ON
		breq TRANSF_VEL_CONT3              ; sim, continua

        ;+---------------------------------+  
		;| Testa velocidade > vel corrente |
		;+---------------------------------+

		lds Acc,_FLAG_VEL_CORRENTE    ; testa se funcao esta habilitada em configuracoes
		cpi Acc,_ON
		brne TRANSF_VEL_CONT3

		lds Acc,_VEL_CORRENTE
		cpi Acc,0
		breq TRANSF_VEL_CONT3

		; ** ATUALIZA VELOCIDADE CORRENTE

		lds Acc,_FLAG_ATUAL_VEL_CORRENTE
		cpi Acc,_ON
		brne TRANSF_VEL_CONT2

		_CLEAR _FLAG_ATUAL_VEL_CORRENTE

		lds Acc,_VELOCIDADE
		call _ARREDONDA_VEL2

		sts _VEL_CORRENTE,Acc
		
		cpi Acc,0
		breq TRANSF_VEL_CONT3   ; Se for acima de 100km/h, sai
		
TRANSF_VEL_CONT2:	
			
		lds Acc,_VEL_CORRENTE
		sts _VARIAVEL,Acc   
		
		lds Acc,_VELOCIDADE     ; Pega velocidade atual
		lds AccH,_VARIAVEL      ; Pega velocidade gravada na eeprom  
		
		cp Acc,AccH 

		breq TRANSF_VEL_CONT3
        brlo TRANSF_VEL_CONT3
		
		call fn_Conv_Knot2Km
		call fn_show_vel_acima
		; Estando acima da velocidade CORRENTE, da dois beeps curtos
		call _BEEP2
		
		rjmp TRANSF_VEL_CONT_JP	 	
		;
		;
		;

TRANSF_VEL_CONT3:
		
		lds Acc,_FLAG_PARDAL_LOCALIZADO
		cpi Acc,_ON
		lbreq TRANSF_VEL_CONT_JP 
	 
		call fn_Conv_Knot2Km                        ; Converter Knot 2 Km/H
	    		
        ; +----------------------------------+
        ; | PEGA DATA E HORA E COLOCA NA RAM |
        ; +----------------------------------+

        call fn_parsing_time

		; ** Testa primeira sincronia **
	    lds Acc,_FLAG_1_MINUTO
		cpi Acc,_ON
		brne TRANSF_TIME_PL
		
		; Verifica se a data de hoje eh a mesma da EEPROM
		call _COMPARA_DATA_EE
		lds Acc,_FLAG_ERRO
		cpi Acc,_ON
		brne  TRANSF_GRAVA_CONTADORES

		; Grava data atual na eeprom
		LDIW Y,_SRAM_DATA

		ldi AccH,low(_EE_PONTUACAO_DATA)
		ldi AccT,high(_EE_PONTUACAO_DATA)
		ld Acc,Y+
		call _EEPROM_WRITE

		ldi AccH,low(_EE_PONTUACAO_DATA+1)
		ldi AccT,high(_EE_PONTUACAO_DATA+1)
		ld Acc,Y+
		call _EEPROM_WRITE

		adiw YH:YL,1

		ldi AccH,low(_EE_PONTUACAO_DATA+2)
		ldi AccT,high(_EE_PONTUACAO_DATA+2)
		ld Acc,Y+
		call _EEPROM_WRITE

		ldi AccH,low(_EE_PONTUACAO_DATA+3)
		ldi AccT,high(_EE_PONTUACAO_DATA+3)
		ld Acc,Y
		call _EEPROM_WRITE

		; Zera CONTADORES
		ldi AccH,LOW(_EE_PARDAIS_ACIMA)
		ldi AccT,HIGH(_EE_PARDAIS_ACIMA)
		ldi Acc,0x00
		sts _PARDAIS_ACIMA,Acc
		call _EEPROM_WRITE
		
		ldi AccH,LOW(_EE_PARDAIS_TOTAIS)
		ldi AccT,HIGH(_EE_PARDAIS_TOTAIS)
		ldi Acc,0x00
		sts _PARDAIS_TOTAIS,Acc
		call _EEPROM_WRITE
		
		rjmp TRANSF_TIME_PL

TRANSF_GRAVA_CONTADORES:

        ; Grava CONTADORES
		ldi AccH,LOW(_EE_PARDAIS_ACIMA)
		ldi AccT,HIGH(_EE_PARDAIS_ACIMA)
		lds Acc,_PARDAIS_ACIMA
		call _EEPROM_WRITE
		
		ldi AccH,LOW(_EE_PARDAIS_TOTAIS)
		ldi AccT,HIGH(_EE_PARDAIS_TOTAIS)
		lds Acc,_PARDAIS_TOTAIS
		call _EEPROM_WRITE
			
		
TRANSF_TIME_PL:

        lds Acc,_FLAG_SWAP_HORA
		cpi Acc,_ON
		breq TRANSF_SWAP_TELAS

		; Tela velocidade
									
	    call fn_Show_Vel                            ; Mostra velocidade no display
		rjmp TRANSF_VEL_CONT_JP

TRANSF_SWAP_TELAS:

		call fn_tela_pontuacao	

TRANSF_VEL_CONT_JP:         
		 
	    ;+--------------+
		;| Pega Direcao |
		;+--------------+
	 	call fn_parsing_dir
 
;********************************************************************************

        ; Se o botao de gravacao foi pressionado, grava na flash

        lds Acc,_FLAG_MARCA_PONTO
	    cpi Acc,_ON

		brne APAGA_PONTO_VERIFICA

        lds Acc,_FLAG_CARRO_PARADO   // nao marca se tiver com o carro parado
		breq APAGA_PONTO_VERIFICA
		
		_CLEAR _FLAG_MARCA_PONTO
				
		ldiw Z,0
        call _MARCA_PONTO	

		ldi Acc,15  
		sts _GPS_TICKS_COUNT,Acc
				
APAGA_PONTO_VERIFICA:

        lds Acc,_FLAG_APAGA_PONTO
	    cpi Acc,_ON
		brne PROCURA_PARDAL

		_CLEAR _FLAG_APAGA_PONTO
				
		ldiw Z,0
        call _APAGA_PONTO
		
		ldi Acc,15  
		sts _GPS_TICKS_COUNT,Acc	
		
		_CLEAR _FLAG_PARDAL_LOCALIZADO
		call _TIMER1_STOP

		call _BUZZER_OFF
				
		jmp REINICIA
		
;***********************************
;*                                 *
;* Rotina de localizacao de pardal *
;*                                 *
;***********************************          

PROCURA_PARDAL:
        ;Carrega a cada minuto
		lds Acc,_FLAG_CARREGA_SRAM
		cpi Acc,_ON
		brne PROCURA_PARD01    

	    _CLEAR _FLAG_CARREGA_SRAM

		lds Acc,_FLAG_PARDAL_LOCALIZADO
		cpi Acc,_ON
		breq PROCURA_PARD01

		;Numero maximo de repeticoes  =2
		ldi Acc,2
		sts _VAR_COUNT,Acc
P_REPEAT_ENTRY:

 		call _CARREGA_SRAM
		;call _beep

		lds Acc,_VAR_COUNT
        dec Acc
		sts _VAR_COUNT,Acc
		tst Acc
		breq PROCURA_P_CONT

		lds Acc,_FLAG_MAIOR39
	    cpi Acc,_ON
		brne PROCURA_P_CONT
		; Se tiver mais que 39, procura na RAM e depois repete a carga da RAM
		call _PROCURA_PONTO_RAM
		
		_CLEAR _FLAG_MAIOR39
			
		lds Acc,_FLAG_PONTO_ENCONTRADO
		cpi Acc,_ON
		brne P_REPEAT_ENTRY
		
		rjmp P_PARD01_ENTRY  
				
PROCURA_P_CONT:
		   		   
		lds Acc,_RAM_PONTOS_CONT
		tst Acc
		brne PROCURA_PARD01
		
        rjmp REINICIA

PROCURA_PARD01:
      
 	    call _PROCURA_PONTO_RAM
				
		lds Acc,_FLAG_PONTO_ENCONTRADO
		cpi Acc,_ON
		brne PROCURA_PAR_NAO_ENC

P_PARD01_ENTRY:
            				 
        call _VER_ABERTURA

        lds Acc,_FLAG_DIRECAO_ERRO
		cpi Acc,_ON
		breq PROCURA_PAR_NAO_ENC
				     
		;Calcula ATAN do ponto
				
        call _INT_ATN2PTS
		       
		sts _DIR_COMP_L,Acc

        sts _DIR_COMP_H,AccH
				   		   		   								
 		call _COMPARA_ATAN  

        lds Acc,_FLAG_ANG_ERRO
		cpi Acc,_ON
		breq PROCURA_PAR_NAO_ENC
					
        lds Acc,_FLAG_CARRO_PARADO
		cpi Acc,_ON
		
		breq PROCURA_PAR_NAO_ENC
				
		rjmp CALCULA_DISTANCIA

PROCURA_PAR_NAO_ENC:
      
		call _TIMER1_STOP
			     
        call _BUZZER_OFF
		_LED1_OFF
		_LED2_OFF
		 
			        
		_CLEAR _FLAG_PARDAL_LOCALIZADO
		_CLEAR _FLAG_APITA

        lds Acc,_CONTADOR8
		inc Acc
		sts _CONTADOR8,Acc
		
		lds Acc,_CONTADOR8
        lds AccH,_RAM_PONTOS_CONT

		cpse Acc,AccH
        rjmp PROCURA_PARDAL

		ldi Acc,0
    	sts _CONTADOR8,Acc
		;sts _RAM_PONTOS_CONT,Acc

		rjmp REINICIA

;+----------------------------------------------------+
;| Rotina para calcular a distancia entre dois pontos |
;| Realiza o calculo: D=SQR((X2-X1)^2+(Y2-Y1)^2)      |
;+----------------------------------------------------+

CALCULA_DISTANCIA:
        lds Acc,_FLAG_PARDAL_LOCALIZADO
		cpi Acc,_ON
		breq  PONTUACAO_PULA

        lds Acc,_PARDAIS_TOTAIS
		inc Acc
		sts _PARDAIS_TOTAIS,Acc

PONTUACAO_PULA:
       		
		call _TIMER1_START
		_SET _FLAG_PARDAL_LOCALIZADO

    	sei
			
        ;Carrega LATITUDE DA SRAM

        LDIX _SRAM_PONTOS                   ; Aponta para buffer de pontos na SRAM
 		call _RODA_DPTR_SRAM2               ; Faz DPTR +(CONTADOR8 * 11)

		ld Acc,X+
		sts _X2_H,Acc
		
		ld Acc,X+
		sts _X2_MH,Acc
		 
     	ld Acc,X+
		sts _X2_ML,Acc
	
	    ld Acc,X+
		sts _X2_L,Acc
        
	    ;Carrega LONGITUDE DA SRAM
		
     	ld Acc,X+
		sts _Y2_H,Acc
		
		ld Acc,X+
		sts _Y2_MH,Acc
		 
     	ld Acc,X+
		sts _Y2_ML,Acc
	
	    ld Acc,X+
		sts _Y2_L,Acc
     
        ; Calcula X2-X1
		
		lds Acc,_LATITUDE_L
		lds AccH,_X2_L
		sub Acc,AccH
        sts _RESULTADO1_L,Acc
		
		lds Acc,_LATITUDE_ML
		lds AccH,_X2_ML
		sbc Acc,AccH
        sts _RESULTADO1_ML,Acc
		    
		lds Acc,_LATITUDE_MH
		lds AccH,_X2_MH
		sbc Acc,AccH
        sts _RESULTADO1_MH,Acc
	
	    lds Acc,_LATITUDE_H
		lds AccH,_X2_H
		sbc Acc,AccH
        sts _RESULTADO1_H,Acc
		
	    brcc DEU_POSITIVO_X

		;Se deu negativo, então realiza um complemento 2 em 32bits e deixa
        ;somente a magnitude

        lds Acc,_RESULTADO1_L              ; complementa 1
		com Acc                            ;  e adiciona 1
	    sts _RESULTADO1_L,Acc
        
		lds Acc,_RESULTADO1_ML             ; Pega proximo byte
        com Acc                            ; complementa
        sts _RESULTADO1_ML,Acc 

		lds Acc,_RESULTADO1_MH
		com Acc
		sts _RESULTADO1_MH,Acc

        lds Acc,_RESULTADO1_H
		com Acc
		sts _RESULTADO1_H,Acc

		; add + 1

		lds Acc,_RESULTADO1_L 
		ldi AccT,1
		add Acc,AccT     
		sts _RESULTADO1_L,Acc
        
		lds Acc,_RESULTADO1_ML                      
		ldi AccT,0
		adc Acc,AccT     
        sts _RESULTADO1_ML,Acc 

		lds Acc,_RESULTADO1_MH
		ldi AccT,0
		adc Acc,AccT     
        sts _RESULTADO1_MH,Acc

        lds Acc,_RESULTADO1_H
		ldi AccT,0
		adc Acc,AccT     
        sts _RESULTADO1_H,Acc
		
DEU_POSITIVO_X:
	    
		;DIVIDE O _RESULTADO POR 10 PARA FACILITAR O CALCULO
		
		lds R0,_RESULTADO1_L
        lds R1,_RESULTADO1_ML
        lds R2,_RESULTADO1_MH
        lds R3,_RESULTADO1_H	
		
		ldi Acc,10
		mov R4,Acc
		
		ldi Acc,0
		mov R5,Acc
		mov R6,Acc
		mov R7,Acc
      
	    call _DIV_U32U32U32

		sts _RESULTADO1_L,R0
        sts _RESULTADO1_ML,R1
        sts _RESULTADO1_MH,R2
        sts _RESULTADO1_H,R3

        ; Eleva o _RESULTADO1 ao quadrado  (SOMENTE 16 BITS)

        lds R0,_RESULTADO1_L
		lds R1,_RESULTADO1_ML

		lds R2,_RESULTADO1_L
		lds R3,_RESULTADO1_ML	 
         
        call _MUL_U16U16U32

        sts _RESULTADO1_L,R4
        sts _RESULTADO1_ML,R5
        sts _RESULTADO1_MH,R6
        sts _RESULTADO1_H,R7

        ; Calcula Y2-Y1

		lds Acc,_LONGITUDE_L
		lds AccH,_Y2_L
		sub Acc,AccH
        sts _RESULTADO2_L,Acc

		lds Acc,_LONGITUDE_ML
		lds AccH,_Y2_ML
		sbc Acc,AccH
        sts _RESULTADO2_ML,Acc

		lds Acc,_LONGITUDE_MH
		lds AccH,_Y2_MH
		sbc Acc,AccH
        sts _RESULTADO2_MH,Acc

   		lds Acc,_LONGITUDE_H
		lds AccH,_Y2_H
		sbc Acc,AccH
        sts _RESULTADO2_H,Acc
		
		brcc DEU_POSITIVO_Y

    	;Se deu negativo, então realiza um complemento 2 em 32bits e deixa
        ;somente a magnitude

        lds Acc,_RESULTADO2_L              ; complementa 1
		com Acc                            ;  e adiciona 1
	    sts _RESULTADO2_L,Acc
        
		lds Acc,_RESULTADO2_ML  ; Pega proximo byte
        com Acc                 ; complementa
        sts _RESULTADO2_ML,Acc 

		lds Acc,_RESULTADO2_MH
		com Acc
		sts _RESULTADO2_MH,Acc

        lds Acc,_RESULTADO2_H
		com Acc
		sts _RESULTADO2_H,Acc

		; add + 1
		lds Acc,_RESULTADO2_L 
		ldi AccT,1
		add Acc,AccT     
		sts _RESULTADO2_L,Acc
        
		lds Acc,_RESULTADO2_ML                      
		ldi AccT,0
		adc Acc,AccT     
        sts _RESULTADO2_ML,Acc 

		lds Acc,_RESULTADO2_MH
		ldi AccT,0
		adc Acc,AccT     
        sts _RESULTADO2_MH,Acc

        lds Acc,_RESULTADO2_H
		ldi AccT,0
		adc Acc,AccT     
        sts _RESULTADO2_H,Acc
		
DEU_POSITIVO_Y:
		
        ;DIVIDE O _RESULTADO POR 10 PARA FACILITAR O CALCULO

 		lds R0,_RESULTADO2_L
        lds R1,_RESULTADO2_ML
        lds R2,_RESULTADO2_MH
        lds R3,_RESULTADO2_H
		
        ldi Acc,10
		mov R4,Acc
        
		ldi Acc,0
		mov R5,Acc
        mov R6,Acc
        mov R7,Acc
        
		call _DIV_U32U32U32

        sts _RESULTADO2_L,R0
        sts _RESULTADO2_ML,R1
        sts _RESULTADO2_MH,R2
        sts _RESULTADO2_H,R3

        ; Eleva o _RESULTADO2 ao quadrado

        lds R0,_RESULTADO2_L
		lds R1,_RESULTADO2_ML

		lds R2,_RESULTADO2_L
		lds R3,_RESULTADO2_ML	 
         
        call _MUL_U16U16U32

        sts _RESULTADO2_L,R4
        sts _RESULTADO2_ML,R5
        sts _RESULTADO2_MH,R6
        sts _RESULTADO2_H,R7
		
        ; Soma os resultados e coloca RESULTADO total em _RESULTADO2

		lds Acc,_RESULTADO1_L
		lds AccH,_RESULTADO2_L
        add Acc,AccH
		sts _RESULTADO2_L,Acc

        lds Acc,_RESULTADO1_ML
		lds AccH,_RESULTADO2_ML
		adc Acc,AccH
		sts _RESULTADO2_ML,Acc

		lds Acc,_RESULTADO1_MH
		lds AccH,_RESULTADO2_MH
		adc Acc,AccH
		sts _RESULTADO2_MH,Acc

		lds Acc,_RESULTADO1_H
		lds AccH,_RESULTADO2_H
		adc Acc,AccH
		sts _RESULTADO2_H,Acc

 		; Calcula a raiz quadrada do resultado (Somente em 16 bits)

   		lds Acc,_RESULTADO2_L
        lds AccH,_RESULTADO2_ML	

        call _SQRTBI
		
        ; Transforma em metros multiplicando por 18 (1,8metros)

        mov R0,Acc
        ldi AccT,0
		mov R1,AccT    ; R1 = SQRT

        ldi Acc,18
		mov R2,Acc
		ldi Acc,0
		mov R3,Acc

        call _MUL_U16U16U32  ; Multiplica por 18

        mov R0,R4
        mov R1,R5
		
        ldi Acc,10     ; Divide por 10 para tirar uma casa decimal
        mov R2,Acc

        ldi Acc,0
		mov R3,Acc

        call _DIV_U16U16U16
        ;-
        sts _DISTANCIA_L,R0
        sts _DISTANCIA_H,R1
					
        call fn_Conv_Knot2Km

		call fn_Show_Pardal_Enc            ; Imprime tela de pardal encontrado 				
					
        ; +----------------------------------+
        ; | Se VEL for > ou = entao continua |
        ; +----------------------------------+   
		
		LDIX _SRAM_PONTOS+8     ; pega velocidade
		call _RODA_DPTR_SRAM2
		
		ld Acc,X
		sts _VARIAVEL,Acc
		sts _VEL_CORRENTE,Acc   ; Atualiza velocidade corrente da via ***    
		
		lds Acc,_VELOCIDADE     ; Pega velocidade atual
		lds AccH,_VARIAVEL      ; Pega velocidade gravada na eeprom  
		
		cp Acc,AccH 
		
	 	lbrlo NAO_APITA
		
        ; Calcula o tempo de chegada ao ponto (T=D/V)

		ldi Acc,low(514)
		mov R0,Acc

		ldi Acc,high(514)
		mov R1,Acc

		lds Acc,_VELOCIDADE
		mov R2,Acc
		
		ldi Acc,0
		mov R3,Acc
		
		call _MUL_U16U16U32   ; R4,R5  = VELOCIDADE EM M/S

        ; Pega velocidade e divide por 1000 para ficar 0,514

        mov R0,R4
		mov R1,R5

        ldi Acc,low(1000)
        mov R2,Acc

		ldi Acc,high(1000)
		mov R3,Acc

        call _DIV_U16U16U16

        ; R0 contem a velocidade em M/S
		
        mov AccT,R0        ; Salva velocidade

        lds Acc,_DISTANCIA_L
		mov R0,Acc

		lds Acc,_DISTANCIA_H
		mov R1,Acc

        mov R2,AccT       ; Restaura velocidade
		
		ldi Acc,0   
		mov R3,Acc

        call _DIV_U16U16U16

		mov Acc,R0
        sts _TEMPO,Acc               ; Guarda tempo atual

        ; Se TEMPO FOR > TEMPO_CHEGADA NAO APITA

        lds Acc,_TEMPO
        lds AccH,_TEMPO_CHEGADA

		cp Acc,Acch

		lbrsh NAO_APITA

		lds Acc,_TEMPO                 ; Se TEMPO for =0 procura outros pontos
		tst Acc

		breq PROCURA_PARDAL_SAIDA

		; ** Rotina de pontuacao 2 **

		lds Acc,_FLAG_APITA
		cpi Acc,_ON
		breq ACIONA_APITA_NORMAL

	    lds Acc,_PARDAIS_ACIMA
		inc Acc
		sts _PARDAIS_ACIMA,Acc

		;**************************

ACIONA_APITA_NORMAL:
			
	    sei
        _SET _FLAG_APITA
		call _TIMER1_START
		_SET _FLAG_PARDAL_LOCALIZADO
						
		rjmp REINICIA                   ; Atualiza posicao  

NAO_APITA:
        _CLEAR _FLAG_APITA
		call _BUZZER_OFF

		rjmp REINICIA

PROCURA_PARDAL_SAIDA:

	    _CLEAR _FLAG_APITA
		call _TIMER1_STOP
		_LED1_OFF
		_LED2_OFF
		 
		
		; Apaga ponto da memoria
	;	LDIX _SRAM_PONTOS                
	;	call _RODA_DPTR_SRAM2
	;	ldi Temp,11
PROCURA_PARDAL_S_APAGA_L1:
    ;    ldi Acc,' '
	;	st X+,Acc
	;	dec Temp
	;	brne PROCURA_PARDAL_S_APAGA_L1
		;
		
		_CLEAR _FLAG_PARDAL_LOCALIZADO
		call _BUZZER_OFF
				
        rjmp REINICIA          


.DB "Obrigado Senhor por tudo e todos. "
