
/*
	#-------------------------------------------------------------------------------#
	|	 _                     _  ______      __ _       _ _   _					|
    |	| |                   | | |  _  \    / _(_)     (_) | (_)					|
    |	| |     ___   ___ __ _| | | | | |___| |_ _ _ __  _| |_ _  ___  _ __  ___	|
    |	| |    / _ \ / __/ _` | | | | | / _ \  _| | '_ \| | __| |/ _ \| '_ \/ __|	|
    |	| |___| (_) | (_| (_| | | | |/ /  __/ | | | | | | | |_| | (_) | | | \__ \	|
    |	\_____/\___/ \___\__,_|_| |___/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/	|
    |																				|
	#-------------------------------------------------------------------------------#                                                           
	| Todas as funcoes de usuario não dependentes de hardware serao implementadas	|
	| aqui.																			|
	#-------------------------------------------------------------------------------#
	| Este documento usa TAB 4 para formatação										|
	#-------------------------------------------------------------------------------#
	| Date: Brasilia ,23 de Setembro de 2016										|
	#-------------------------------------------------------------------------------#
 */
    ;////////////////////
	;// BUFFERS
	;////////////////////

	.DSEG 
	   _SSD1306_DISPLAY_BUFFER:	.BYTE 1024 
	   _BUFFER_MINI:      .BYTE 8
	   _BUFFER_MONTAGEM:  .BYTE 28
	   _BUFFER_GPS:		  .BYTE 94
	   _SRAM_DATA:	      .BYTE 6
	   _SRAM_HORA:        .BYTE 6
	   _SRAM_PONTOS:      .BYTE 418
	   ;_BUFFER_REDE1:     .BYTE 20
	   ;_BUFFER_REDE2:     .BYTE 20
	   _BUFFER_REDE1:     .BYTE 1
	   _BUFFER_REDE2:     .BYTE 1
	  
	  
	   _BUFFER_RESULTADO: .BYTE 25
	   _BUFFER_MAC:       .BYTE 18   
      _SRAM_NRO_PACOTE:   .BYTE 6
	   

	;////////////////////
	;// SYSTEM VARIABLES
	;////////////////////
        
       
       _EE_PARDAL_POINT_L:  .BYTE _BYTE
       _EE_PARDAL_POINT_ML: .BYTE _BYTE
       _EE_PARDAL_POINT_MH: .BYTE _BYTE
       _EE_PARDAL_POINT_H:  .BYTE _BYTE

	   _CONTADOR_BUF:  .BYTE _BYTE

	   _CONT_PONTOS_H: .BYTE _BYTE
	   _CONT_PONTOS_L: .BYTE _BYTE

       _CONTADOR16_H: .BYTE _BYTE
	   _CONTADOR16_L: .BYTE _BYTE

       _VELOCIDADE: .BYTE _BYTE
	   _VARIAVEL:   .BYTE _BYTE
	   
	   _DIRECAO_L:  .BYTE _BYTE
	   _DIRECAO_H:  .BYTE _BYTE

       _DIR_COMP_H: .BYTE _BYTE
	   _DIR_COMP_L: .BYTE _BYTE
	   _FLAG_VOLUME: .BYTE _BYTE
	   
	   _ACIMA_360:    .BYTE _BYTE
	   _LSB:          .BYTE _BYTE
	   _VAR_CONTRAST: .BYTE _BYTE
	   
	 /*_ACUMULADOR_L:  .BYTE _BYTE
	   _ACUMULADOR_ML: .BYTE _BYTE
	   _ACUMULADOR_MH: .BYTE _BYTE
	   _ACUMULADOR_H:  .BYTE _BYTE   
     */
       _BUFFER_BYTE: .BYTE _BYTE
	   _TEMP: .BYTE _BYTE

       _GPS_TICKS_COUNT: .BYTE _BYTE
	   _RAM_PONTOS_CONT: .BYTE _BYTE

	   _X2_L:  .BYTE _BYTE
       _X2_ML: .BYTE _BYTE
	   _X2_MH: .BYTE _BYTE
	   _X2_H:  .BYTE _BYTE

       _Y2_L:  .BYTE _BYTE
	   _Y2_ML: .BYTE _BYTE
	   _Y2_MH: .BYTE _BYTE
	   _Y2_H:  .BYTE _BYTE

       _RESULTADO1_L:  .BYTE _BYTE
	   _RESULTADO1_ML: .BYTE _BYTE
	   _RESULTADO1_MH: .BYTE _BYTE
	   _RESULTADO1_H:  .BYTE _BYTE

       _RESULTADO2_L:  .BYTE _BYTE
	   _RESULTADO2_ML: .BYTE _BYTE
	   _RESULTADO2_MH: .BYTE _BYTE
	   _RESULTADO2_H:  .BYTE _BYTE

       _TEMP_VAR_LAT_L:  .BYTE _BYTE
	   _TEMP_VAR_LAT_ML: .BYTE _BYTE
	   _TEMP_VAR_LAT_MH: .BYTE _BYTE
	   _TEMP_VAR_LAT_H:  .BYTE _BYTE

       _TEMP_VAR_LON_L:  .BYTE _BYTE
	   _TEMP_VAR_LON_ML: .BYTE _BYTE
	   _TEMP_VAR_LON_MH: .BYTE _BYTE
	   _TEMP_VAR_LON_H:  .BYTE _BYTE
     /*
	   _CONTADOR24_L: .BYTE _BYTE
	   _CONTADOR24_H: .BYTE _BYTE
	   _CONTADOR24_HH: .BYTE _BYTE
    */
       _DISTANCIA_L: .BYTE _BYTE
	   _DISTANCIA_H: .BYTE _BYTE

	   _TEMPO_CHEGADA: .BYTE _BYTE
	   _REPEAT: .BYTE _BYTE

       _CONTADOR8: .BYTE _BYTE
	   
	   _TICKS: .BYTE _BYTE
	   _VAR_BEEP: .BYTE _BYTE
	   _DADO_SERIAL: .BYTE _BYTE

       _DEGREE: .BYTE _BYTE
	   _MINUTE: .BYTE _BYTE
	   _DEC_MINUTE_L: .BYTE _BYTE
	   _DEC_MINUTE_H: .BYTE _BYTE

	   _WORK: .BYTE _BYTE
	   _WORKH: .BYTE _BYTE

       _WORK2L: .BYTE _BYTE
	   _WORK2H: .BYTE _BYTE
	   
	   _XL:  .BYTE _BYTE
	   _XML: .BYTE _BYTE
	   _XMH: .BYTE _BYTE
	   _XH:  .BYTE _BYTE

	   _YL:  .BYTE _BYTE
	   _YML: .BYTE _BYTE
	   _YMH: .BYTE _BYTE
	   _YH:  .BYTE _BYTE

	   _SRAM_PONTO_L:   .BYTE _BYTE
	   _SRAM_PONTO_ML:  .BYTE _BYTE
	   _SRAM_PONTO_MH:  .BYTE _BYTE
	   _SRAM_PONTO_H:   .BYTE _BYTE
	  
	   _GUARDA_XL: .BYTE _BYTE
	   _GUARDA_XH: .BYTE _BYTE

	
	   _TEMPO:  .BYTE _BYTE
	   _CONT_I: .BYTE _BYTE
	   _CONT_X: .BYTE _BYTE
	   _CONT2:  .BYTE _BYTE
	   _PACKETS_L: .BYTE _BYTE
	   _PACKETS_H: .BYTE _BYTE

	   _PCK_COUNT_L: .BYTE _BYTE
	   _PCK_COUNT_H: .BYTE _BYTE

	   _VAR_HEM_LAT:  .BYTE _BYTE
	   _VAR_HEM_LON:  .BYTE _BYTE
	   _COD_ERRO:     .BYTE _BYTE
	   _VAR_BEEP_ANT: .BYTE _BYTE
	   _VAR_TELA:     .BYTE _BYTE
	   _CONTADOR_REG: .BYTE _BYTE
	   _TROCA_BANCO:  .BYTE _BYTE
       _TEMP_I:       .BYTE _BYTE
	   _VEL_CORRENTE: .BYTE _BYTE
	   _NOSAT_VAR:    .BYTE _BYTE
	   _PARDAIS_TOTAIS: .BYTE _BYTE
	   _PARDAIS_ACIMA:  .BYTE _BYTE
	   _BYTE_BARRA:     .BYTE _BYTE
 	   _PISCA_PONTOS: .BYTE _BYTE
       _GUARDA_R4: .BYTE _BYTE
	   _GUARDA_YL: .BYTE _BYTE
       _COUNT_L:  .BYTE _BYTE
       _COUNT_H:  .BYTE _BYTE
	   _ACC_L:    .BYTE _BYTE
	   _ACC_H:    .BYTE _BYTE
	   _SQRX_L:   .BYTE _BYTE
	   _SQRX_H:   .BYTE _BYTE
	    
       _VAR_GERAL:.BYTE _BYTE

	   _LATITUDE_L:  .BYTE _BYTE
	   _LATITUDE_ML: .BYTE _BYTE
	   _LATITUDE_MH: .BYTE _BYTE
	   _LATITUDE_H:  .BYTE _BYTE
 
       _LONGITUDE_L:  .BYTE _BYTE
	   _LONGITUDE_ML: .BYTE _BYTE
	   _LONGITUDE_MH: .BYTE _BYTE
	   _LONGITUDE_H:  .BYTE _BYTE
	   _VAR_QUAD_L:   .BYTE _BYTE
	   _VAR_QUAD_H:   .BYTE _BYTE
	   _VAR_COUNT:    .BYTE _BYTE
	   _VAR_3_MINUTOS: .BYTE _BYTE

    ;////////////////////
	;// SYSTEM FLAGS
	;////////////////////
	 
	_FLAG_START:            .BYTE _BYTE
	
	_FLAG_DIRECAO_ERRO:     .BYTE _BYTE
	_FLAG_MENOR_ZERO:       .BYTE _BYTE
	
	_FLAG_SAT_FOUND:        .BYTE _BYTE
	_FLAG_ANG_ERRO:         .BYTE _BYTE
	_FLAG_APITA:            .BYTE _BYTE
	;_FLAG_CARREGA_R7:       .BYTE _BYTE
	_hexflag:               .BYTE _BYTE

	_FLAG_ATAN_OK:          .BYTE _BYTE
	_FLAG_PONTO_ENCONTRADO: .BYTE _BYTE
	_FLAG_TESTA_OK:         .BYTE _BYTE
	_FLAG_PARDAL_LOCALIZADO:  .BYTE _BYTE
	_FLAG_APAGA_EEPROM:       .BYTE _BYTE
	_FLAG_CARRO_PARADO:       .BYTE _BYTE
	_FLAG_1_MINUTO:           .BYTE _BYTE
	_FLAG_OK_ERRO:	          .BYTE _BYTE
	_FLAG_MARCA_PONTO:        .BYTE _BYTE
	_FLAG_APAGA_PONTO:        .BYTE _BYTE
	_FLAG_BUZZER_ON:          .BYTE _BYTE
	 ;_FLAG_INICIOU_LINHA:      .BYTE _BYTE
	_FLAG_TROCA_BANCO:        .BYTE _BYTE
	_FLAG_WIFI:               .BYTE _BYTE
	_FLAG_GIRA_TELA:          .BYTE _BYTE
	_FLAG_VEL_CORRENTE:       .BYTE _BYTE
	_FLAG_ATUAL_VEL_CORRENTE: .BYTE _BYTE
	_FLAG_MENU:               .BYTE _BYTE
	_FLAG_HALT:				  .BYTE _BYTE
	_FLAG_GRAVA_HALT:         .BYTE _BYTE
	_FLAG_ERRO:               .BYTE _BYTE
	_FLAG_SWAP_HORA:          .BYTE _BYTE
	_FLAG_CARREGA_SRAM:       .BYTE _BYTE
	_FLAG_LED1_ON:            .BYTE _BYTE
    _FLAG_LED2_ON:            .BYTE _BYTE
	_FLAG_LED3_ON:            .BYTE _BYTE
	_FLAG_POSSUI_DADOS_GPS:   .BYTE _BYTE
	_FLAG_MAIOR39:            .BYTE _BYTE
	_FLAG_ACIMA_120KM:        .BYTE _BYTE
	 

    .ESEG

    ;////////////////////
	;// EEPROM DEFINITIONS
	;////////////////////

    _EE_CONT_PONTOS_H: .BYTE _BYTE 
    _EE_CONT_PONTOS_L: .BYTE _BYTE 

    _EE_TEMPO_CHEGADA: .BYTE _BYTE
	_EE_NRO_PACOTE:    .BYTE 6 
	_EE_FRASE_INICIAL:    .BYTE 80
	_EE_FRASE_ATIVA:      .BYTE 1
	_EE_FRASE_TEMPO:      .BYTE 1
	_EE_VOLUME:           .BYTE 1
	_EE_FIRST_BOOT:       .BYTE 1
	_EE_CONTRAST:         .BYTE 1
	_EE_GIRA_TELA:        .BYTE 1
	_EE_VEL_CORRENTE:     .BYTE 1 
	_EE_FLAG_HALT:        .BYTE 1
	_EE_PONTUACAO_DATA:   .BYTE 4
	_EE_PARDAIS_ACIMA:    .BYTE 1
	_EE_PARDAIS_TOTAIS:   .BYTE 1
	_EE_PARDAIS_DATA:     .BYTE 1
	_EE_PARDAIS_HORA:     .BYTE 1
	 
/*
	#---------------------------------------------------------------#
	| Purpose : Generate a random number between 1..255				|
	| Name    : Rnd													|
	| Input   : None												|
	| Output  : Acc RANDOM NUMBER 1..255							|
	| Destroy :														|
	| Remarks :														|	
	#---------------------------------------------------------------#
*/
/*		
		.CSEG
_RND:
		push	AccH						;save useds
		lds	Acc,_RND_SEED					;get seed
		tst	Acc								;check if seed is zero
		brne	_RND_10						;no, branch
		ldi		Acc,7
_RND_10:
		push	Acc							;save it
		ldi	AccH,0							;clear poly sum
		eor	AccH,Acc						;x^8
		lsl	Acc
		lsl	Acc							
		eor	AccH,Acc						;x^6
		lsl	Acc
		eor	AccH,Acc						;x^5
		lsl	Acc
		eor	AccH,Acc						;x^4
		lsl	AccH							;cy=result x^8+x^6+x^5+x^4
		pop	Acc								;restore original
		rol	Acc								;bound with orinal
		sts	_RND_SEED,Acc
		pop	AccH							;restore useds
		ret
*/
/*
	#---------------------------------------------------------------#
	| Purpose : Show Press Key Message								|
	| Name    : ShowPressKey										|
	| Input   : Acc Operator										|
	| Output  :														|
	| Destroy :														|
	| Remarks :														|	
	#---------------------------------------------------------------#
*/
/*	
	message_press_key:
		.db	"localizado!",0

fn_ShowLocalizado:
	push	Temp
	pushaw
	pushw	Z
	pushw	X
	pushw	Y
	mov		Temp,Acc							
	call	_SSD1306_Get_Operator					;get actual operator
	push	Acc
	mov		Acc,Temp
	call	_SSD1306_Set_Operator					;set new operator
	ldiw	Z,message_press_key*2					;z-->press key message
	call	_SSD1306_Get_Text_Center_X				;put message on center y
	movw	XH:XL,AccH:Acc
	call	_SSD1306_Get_Char_Height				;get Char Height
	ldiw	Y,_SSD1306_LCD_HEIGHT					;put message above height
	sub		YL,Acc
	sbc		YH,AccH
	
	ldi Acc,11
	ldi accH,0
	sub		YL,Acc
	sbc		YH,AccH

	call	_SSD1306_Locate							;Locate Text
	call	_SSD1306_PutSF							;print message
	pop		Acc
	call	_SSD1306_Set_Operator					;Restore operator
	call    _SSD1306_Refresh

	popw	Y
	popw	X
	popw	Z
	popaw
	pop		Temp
	ret

	*/
/*
	#---------------------------------------------------------------#
	| Purpose : Show Message On Center SCreen						|
	| Name    : ShowMessageCenter									|
	| Input   : Acc Operator										|
	| Output  :														|
	| Destroy :														|
	| Remarks :														|	
	#---------------------------------------------------------------#
*/
    .CSEG
   
fn_ShowMessageCenter:
	pushaw
	pushw	X
	pushw	Y
	call	_SSD1306_Get_Text_Center_X
	movw	XH:XL,AccH:Acc
	call	_SSD1306_Get_Text_Center_Y
	movw	YH:YL,AccH:Acc
	call	_SSD1306_Locate
	call	_SSD1306_PutSF
	call	_SSD1306_Refresh
	popw	Y
	popw	X
	popaw
	ret
/*
	#---------------------------------------------------------------#
	| Purpose : Show Screen Border									|
	| Name    : ShowScreenBorder									|
	| Input   :														|
	| Output  :														|
	| Destroy :														|
	| Remarks :														|	
	#---------------------------------------------------------------#
*/
fn_ShowScreenBorder:
	pushaw
	pushawt
	pushw	X
	pushw	Y
	ldiw	x,0
	ldiw	y,0
	ldiaw	_SSD1306_LCD_WIDTH-1
	ldiawt	_SSD1306_LCD_HEIGHT-1
	call	_SSD1306_Draw_Box
	;call	_SSD1306_Refresh
	popw	Y
	popw	X
	popawt
	popaw
	ret


fn_ShowSatIni:
    //
	//SAVE SREG
	//
	push	Acc
    in		Acc,SREG

    _M_PUSH_LOWER_REGS						        ;save r0..r15
    _M_PUSH_UPPER_REGS						        ;save r16..r31 
	
	ldi		Acc,_WHITE							;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,1								;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			;Set Screen Normal 
	call	_SSD1306_Cls
	//
	//draw start message
	//
	ldiw	Z,_PROCURANDO_SAT1*2	;Progress
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,_SSD1306_LCD_HEIGHT/2-(_SSD1306_FONT_HEIGHT*2)+4
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF

	ldiw	Z,_PROCURANDO_SAT2*2	;Bar
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,_SSD1306_LCD_HEIGHT/2
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF

	call	fn_ShowScreenBorder						;show border
	call    _SSD1306_Refresh

   //RESTORE REGISTERS
	//
	_M_POP_UPPER_REGS						;restore r16..r31
	_M_POP_LOWER_REGS
	//
	//RESTORE SREG
	//
    out		SREG,Acc
	pop		Acc	
	ret

_PROCURANDO_SAT1:
    .DB "Procurando",_nc,0

_PROCURANDO_SAT2:
	.DB "Satelites...",_nc,0    
		
;
;
;

fn_ShowInstru1:
    //
	//SAVE SREG
	//
	push	Acc
    in		Acc,SREG

    _M_PUSH_LOWER_REGS						        ;save r0..r15
    _M_PUSH_UPPER_REGS						        ;save r16..r31 
	
	ldi		Acc,_WHITE							;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,1								;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			;Set Screen Normal 
	call	_SSD1306_Cls
	//
	//draw start message
	//

	lds		Acc,_CONT2
	cpi     Acc,'1'
	breq    CARREGA1A
	cpi     Acc,'2'
	breq    CARREGA1B
    
	rjmp    CARREGA1C

CARREGA1A:
	ldiw	Z,_INSTRUCOES_1A*2	;Progress
	rjmp    CARREGA1_EXIT

CARREGA1B:
	ldiw	Z,_INSTRUCOES_2A*2	;Progress
	rjmp    CARREGA1_EXIT

CARREGA1C:
	ldiw	Z,_INSTRUCOES_3A*2	;Progress
	
CARREGA1_EXIT:
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,_SSD1306_LCD_HEIGHT/2-(_SSD1306_FONT_HEIGHT*2)+4
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF

	; Carrega segundo
	lds		Acc,_CONT2
	cpi     Acc,'1'
	breq    CARREGA2A
	cpi     Acc,'2'
	breq    CARREGA2B
    
	rjmp    CARREGA2C

CARREGA2A:
	ldiw	Z,_INSTRUCOES_1B*2	;Progress
	rjmp    CARREGA2_EXIT

CARREGA2B:
	ldiw	Z,_INSTRUCOES_2B*2	;Progress
	rjmp    CARREGA2_EXIT

CARREGA2C:
	ldiw	Z,_INSTRUCOES_3B*2	;Progress
	
CARREGA2_EXIT:

	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,_SSD1306_LCD_HEIGHT/2
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF

	call	fn_ShowScreenBorder						;show border
	call    _SSD1306_Refresh

   //RESTORE REGISTERS
	//
	_M_POP_UPPER_REGS						;restore r16..r31
	_M_POP_LOWER_REGS
	//
	//RESTORE SREG
	//
    out		SREG,Acc
	pop		Acc	
	ret

_INSTRUCOES_1A:
    .DB "1-Verifique se plug",0

_INSTRUCOES_1B:
	.DB "acendedor esta firme",_nc,0  

_INSTRUCOES_2A:
    .DB "2-Fixe corretamente",0            
_INSTRUCOES_2B:
	.DB "Aparelho no painel",0,_NC
  
_INSTRUCOES_3A:
    .DB "3-Verifique posicao",0          
_INSTRUCOES_3B:
	.DB "instalada no painel",0	

;
; Mostra velocidade no display
;
fn_Show_Vel:
	
	pushaw
	pushawt
	pushw	X
	pushw	Y

	push R0  
	push R1

    ; Carrega plano de fundo (satelite)

      ldi		Acc,_WHITE							;Set White Logical Mode
      call	_SSD1306_Set_Operator			
      ldiw	z,tela_sat*2
      ldiw	x,0
      ldiw	y,0
	  call	_SSD1306_Draw_bmp

			 
    ldi		Acc,_WHITE						         ;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,4								     ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			     ;Set Screen Normal 

	; Carrega Km/H
	ldi		Acc,2								     ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale	
	
	ldiw	Z,MSG_KMH*2	         
    ldiw    X,_SSD1306_LCD_WIDTH-(_SSD1306_FONT_WIDTH*8)-1
	ldiw    Y,_SSD1306_LCD_HEIGHT/2+5-_SSD1306_FONT_HEIGHT*2

	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF

    ldi		Acc,3								    ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale	
	
	//
	//Mostra velocidade
	//
	ldiz    _DF_STR                                 ; Aponta para o Buffer
    ldiw    X,_SSD1306_LCD_WIDTH/2-(_SSD1306_FONT_WIDTH*9)+3
	ldiw	Y,_SSD1306_LCD_HEIGHT/2-(_SSD1306_FONT_HEIGHT*2)-1
	call	_SSD1306_Locate							;locate msg

	pop R1
	pop R0

    mov Acc,R0                        ; resultado da conversao da velocidade
	mov AccH,R1
    ldi AccT,0
	ldi AccTH,0

	call    _ULTOS                    ; converte em ASCII

	ldi Acc,8
	ldi AccH,0
	add ZL,Acc
	adc ZH,AccH

    ; Remove left zero

	ld Acc,Z
	cpi Acc,'0'
	brne MOSTRA_VEL_JP2
	ldi Acc,' '
	st Z,Acc

MOSTRA_VEL_JP2:

    call	_SSD1306_PutS
 				
    ;Imprime numero de satelites

	ldi		Acc,_INVERT							;Set White Logical Mode
 	call	_SSD1306_Set_Operator			
	ldi		Acc,1								;Set Scale to 2
	call	_SSD1306_Set_Text_Scale	
	
	ldi     Temp,4
 	call    _fn_Recebe_GGA
	
	mov		Acc,R4
	ldi		AccH,0
	ldi		AccT,0
	ldi		AccTH,0
	
	call    _ULTOS                              ; Converte em ASCII
	
    ldiz    _DF_STR+9     		
	ldiw	x,51
	ldiw	y,25
	call	_SSD1306_Locate						; Set value location
		
    ; Remove left zero
	ld Acc,Z
	cpi Acc,'0'
	brne MOSTRA_VEL_JP6
	ldi Acc,' '
	st Z,Acc

MOSTRA_VEL_JP6:		
		
	ldiz    _DF_STR+9
	call	_SSD1306_PutS	
	 
    ; Mostra hora

    ldi		Acc,_White						     	;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,1								    ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale	
	
    ldiz    _SRAM_HORA    		
	ldiw	x,0
	ldiw	y,25
	call	_SSD1306_Locate						    ; Set value location
										    
    ldiz    _SRAM_HORA
	call	_SSD1306_PutS	

   ; ldi		Acc,_White							;Set White Logical Mode
;	call	_SSD1306_Set_Operator			
	;ldi		Acc,1								;Set Scale to 2
	;call	_SSD1306_Set_Text_Scale	
	
	;Mostra Data

    ldiz    _SRAM_DATA    		
	ldiw	X,_SSD1306_LCD_WIDTH-_SSD1306_FONT_WIDTH*5
	ldiw	y,25
	 
	call	_SSD1306_Locate					    	; Set value location
										    
    ldiz    _SRAM_DATA
	call	_SSD1306_PutS	

    ldiw	x,0
	ldiw	y,14
	call	_SSD1306_Locate						; Set value loc
		       
	call   _ENVIA_PONTO_CARDEAL

    mov    Acc,AccTH
	call   _SSD1306_PutC
 
    mov    Acc,AccT
	call   _SSD1306_PutC



	 
/*
	; MOSTRA NUMERO DE PARDAIS
	LDS 	Acc,_RAM_PONTOS_CONT
	
	ldi		AccH,0
	ldi		AccT,0
	ldi		AccTH,0
	
	call    _ULTOS                              ; Converte em ASCII
	
    ldiz    _DF_STR+9     		
	ldiw	x,0
	ldiw	y,0
	call	_SSD1306_Locate						; Set value location
			
	ldiz    _DF_STR+8
	call	_SSD1306_PutS	
	 */
	
	lds Acc,_FLAG_VOLUME  
    tst Acc 
	brne MOSTRA_VEL_JP6_L00

    ldiw	x,0
	ldiw	y,0
	call	_SSD1306_Locate						; Set value loc
		       
	ldi Acc,'~' 
	call   _SSD1306_PutC

	rjmp MOSTRA_VEL_JP6_PULA

MOSTRA_VEL_JP6_L00:

	ldiw	x,0
	ldiw	y,0
	call	_SSD1306_Locate						; Set value loc
		       
	ldi Acc,' ' 
	call   _SSD1306_PutC
	

MOSTRA_VEL_JP6_PULA:
	
	call _SSD1306_Refresh
	
	popw	Y
	popw	X
	popawt
	popaw

	ret

 

fn_Showtime:

	pushaw
	pushawt
	pushw	X
	pushw	Y
	 

    ; Carrega plano de fundo (satelite)

    ldi		Acc,_WHITE							;Set White Logical Mode
    call	_SSD1306_Set_Operator			
    ldiw	z,tela_sat*2
    ldiw	x,0
    ldiw	y,0
	call	_SSD1306_Draw_bmp

	;Imprime numero de satelites

	ldi		Acc,_INVERT							;Set White Logical Mode
 	call	_SSD1306_Set_Operator			
	ldi		Acc,1								;Set Scale to 2
	call	_SSD1306_Set_Text_Scale	
	
 ;	call    _fn_Recebe_GGA
    lds    Acc,_GUARDA_R4
	
	;mov		Acc,R4
	ldi		AccH,0
	ldi		AccT,0
	ldi		AccTH,0
	
	call    _ULTOS                              ; Converte em ASCII
	
    ldiz    _DF_STR+9     		
	ldiw	x,51
	ldiw	y,25
	call	_SSD1306_Locate						; Set value location
		
    ; Remove left zero
	ld Acc,Z
	cpi Acc,'0'
	brne MOSTRA_VEL_JP7
	ldi Acc,' '
	st Z,Acc

MOSTRA_VEL_JP7:		
		
	ldiz    _DF_STR+9
	call	_SSD1306_PutS	
	 
    ; Mostra hora

    ldi		Acc,_White						     	;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,2								    ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale	
	
    ldiz    _SRAM_HORA    		
	ldiw	x,0
	ldiw	y,0
	call	_SSD1306_Locate						    ; Set value location
										    
    ldiz    _SRAM_HORA
	call	_SSD1306_PutS	

    
	;Mostra Data

    ldiz    _SRAM_DATA    		
	ldiw	X,70
	ldiw	y,0
	 
	call	_SSD1306_Locate					    	; Set value location
										    
    ldiz    _SRAM_DATA
	call	_SSD1306_PutS	
	   
	
	call _SSD1306_Refresh
	
	popw	Y
	popw	X
	popawt
	popaw

	ret

 
















fn_show_pardal_enc:
    pushaw
	pushawt
	pushw	X
	pushw	Y
	push R0
	push R1
	
	;xx
	ldi		Acc,_WHITE                          ;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,2								;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			;Set Screen Normal 
    call	_SSD1306_Cls
		
	;Imprime 1a linha : Vel:
	ldiw	Z,MSG_PARDAL_ENC1*2	         
	ldix    0
	ldiy    0
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
	
	;Imprime 2a linha : Vel:
	ldiw	Z,MSG_PARDAL_ENC2*2	         
	ldix    0
	ldiy    18
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
		
	ldi		Acc,1								    ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale		
	;Km/H
	ldiw	Z,MSG_KMH*2	         
    ldiw    X,_SSD1306_LCD_WIDTH-(_SSD1306_FONT_WIDTH*2)-31
	ldiy   1
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
	;Km/H
	ldiw	Z,MSG_KMH*2	         
    ldiw    X,_SSD1306_LCD_WIDTH-(_SSD1306_FONT_WIDTH*2)-31
	ldiy    17
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
	
	; Mostra Velocidade

    ldi		Acc,2								;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			;Set Screen Normal
	
    ldiz    _DF_STR                             ; Aponta para o Buffer
    ldiw    X,_SSD1306_LCD_WIDTH/2-7-_SSD1306_FONT_WIDTH*2
	ldiw	Y,0
	call	_SSD1306_Locate							;locate msg

	pop R1
	pop R0

    mov Acc,R0                        ; resultado da conversao da velocidade
	mov AccH,R1
    ldi AccT,0
	ldi AccTH,0

	call    _ULTOS                    ; Converte em ASCII

	ldi Acc,8
	ldi AccH,0
	add ZL,Acc
	adc ZH,AccH
    ; Remove left zero
	ld Acc,Z
	cpi Acc,'0'
	brne MOSTRA_VEL_JP
	ldi Acc,' '
	st Z,Acc

MOSTRA_VEL_JP:

    call	_SSD1306_PutS
	
    ; Mostra Velocidade via

    ldi		Acc,2							    	;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal		    	;Set Screen Normal
		
	LDIX _SRAM_PONTOS+8                             ; pega velocidade
	call _RODA_DPTR_SRAM2
	
	ld Acc,X
	ldi AccH,0
    ldi AccT,0
	ldi AccTH,0

	call    _ARREDONDA_VEL

	call    _ULTOS                    ; Converte em ASCII
	
    ldiz    _DF_STR+8                     
    ldiw    X,_SSD1306_LCD_WIDTH/2-7-_SSD1306_FONT_WIDTH*2
	ldiw	Y,18
	call	_SSD1306_Locate							;locate msg
	
	; Remove left zero
	ld Acc,Z
	cpi Acc,'0'
	brne MOSTRA_VEL_JP3
	ldi Acc,' '
	st Z,Acc

MOSTRA_VEL_JP3:
	
	call	_SSD1306_PutS	

    ; Progress bar

	ldiw    X,_SSD1306_LCD_WIDTH-(_SSD1306_FONT_WIDTH*2)-31
	ldiw	y,27
	
	call	_SSD1306_Set_Progress_Bar_Coords		;bar start coords
   ; ldiw	x,_SSD1306_LCD_WIDTH-8*2
    LDIW    X,43
	ldiw	y,5
	
	call	_SSD1306_Set_Progress_Bar_Sizes			;Bar Sizes
	ldiw	x,0										
	ldiw	y,340  
	call	_SSD1306_Set_Progress_Bar_Range			;Bar Range

	ldi AccT,low(340)
	ldi AccTH,high(340)

	lds		Acc,_DISTANCIA_L
	lds		AccH,_DISTANCIA_H

	; Verifica se e maior que 340
	
	ldi Temp,low(340)
	ldi TempH,high(340)
	
	cp Acc,Temp
	cpc AccH,TempH
	
	brlo MOSTRA_VEL_CONT1
	; Se distancia for > 340, distancia = 340
	ldi Acc,low(340)
	ldi	AccH,high(340)

MOSTRA_VEL_CONT1:

    sub AccT,Acc
	mov Acc,AccT

	sbc AccTH,AccH
	mov AccH,AccTH
	
	ldiawt	00

	call	_SSD1306_Set_Progress_Bar_Value			;set value
	call	_SSD1306_Draw_Progress_Bar				;draw bar

	;Imprime distancia na barra de progresso
	/*
 	ldi		Acc,_INVERT							;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,1								;Set Scale to 2
	call	_SSD1306_Set_Text_Scale	
	

	lds		Acc,_DISTANCIA_L
	lds		AccH,_DISTANCIA_H
	ldi		AccT,0
	ldi		AccTH,0
	
	call    _ULTOS                              ; Converte em ASCII
	
    ldiz    _DF_STR+7     		
	ldiw	x,45
	ldiw	y,54
	call	_SSD1306_Locate						; Set value location
		
	ldi Acc,'-'
    st Z+,Acc
	
; Remove left zero
	ld Acc,Z
	cpi Acc,'0'
	brne MOSTRA_VEL_JP4
	ldi Acc,' '
	st Z,Acc

MOSTRA_VEL_JP4:				

	ldiz    _DF_STR+7
										    
	call	_SSD1306_PutS	

 ; Imprime string "metros"

    ldiz    MSG_METROS*2
    ldiw	x,72
	ldiw	y,54
	call	_SSD1306_Locate		
	call	_SSD1306_PutSF

	*/
    	
	call    _SSD1306_Refresh
	
    popw	Y
	popw	X
	popawt
	popaw
	ret

MSG_PARDAL_ENC1:
    .DB "Vel:   ",_CR,_LF,_CR,_LF,0

MSG_PARDAL_ENC2:	
	.DB "Via:   ",0    

MSG_KMH:
    .DB "Km/h",_NC,0

MSG_METROS:
    .DB "METROS",_NC,0
MSG_VEL_VIA:
    .DB "60",_NC,0





;
;  Tela velocidade acima 
;
fn_show_vel_acima:
    pushaw
	pushawt
	pushw	X
	pushw	Y
	push R0
	push R1
	
	;xx
	ldi		Acc,_WHITE                          ;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,2								;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			;Set Screen Normal 
    call	_SSD1306_Cls
		
	;Imprime 1a linha : Vel:
	ldiw	Z,MSG_PARDAL_ENC1*2	         
	ldix    0
	ldiy    0
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
	
	;Imprime 2a linha : Vel:
	ldiw	Z,MSG_PARDAL_ENC2*2	         
	ldix    0
	ldiy    18
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
		
	ldi		Acc,1								    ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale		
	;Km/H
	ldiw	Z,MSG_KMH*2	         
    ldiw    X,_SSD1306_LCD_WIDTH-(_SSD1306_FONT_WIDTH*2)-31
	ldiy   1
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
	;Km/H
	ldiw	Z,MSG_KMH*2	         
    ldiw    X,_SSD1306_LCD_WIDTH-(_SSD1306_FONT_WIDTH*2)-31
	ldiy    17
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
	
	; Mostra Velocidade

    ldi		Acc,2								;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			;Set Screen Normal
	
    ldiz    _DF_STR                             ; Aponta para o Buffer
    ldiw    X,_SSD1306_LCD_WIDTH/2-7-_SSD1306_FONT_WIDTH*2
	ldiw	Y,0
	call	_SSD1306_Locate							;locate msg

	pop R1
	pop R0

    mov Acc,R0                        ; resultado da conversao da velocidade
	mov AccH,R1
    ldi AccT,0
	ldi AccTH,0

	call    _ULTOS                    ; Converte em ASCII

	ldi Acc,8
	ldi AccH,0
	add ZL,Acc
	adc ZH,AccH
    ; Remove left zero
	ld Acc,Z
	cpi Acc,'0'
	brne MOSTRA_VEL_ACIMA_JP
	ldi Acc,' '
	st Z,Acc

MOSTRA_VEL_ACIMA_JP:

    call	_SSD1306_PutS
	
    ; Mostra Velocidade via

    ldi		Acc,2							    	;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal		    	;Set Screen Normal
	
	lds Acc,_VEL_CORRENTE
	ldi AccH,0
    ldi AccT,0
	ldi AccTH,0

	call    _ARREDONDA_VEL

	call    _ULTOS                    ; Converte em ASCII
	
    ldiz    _DF_STR+8                     
    ldiw    X,_SSD1306_LCD_WIDTH/2-7-_SSD1306_FONT_WIDTH*2
	ldiw	Y,18
	call	_SSD1306_Locate							;locate msg
	
	; Remove left zero
	ld Acc,Z
	cpi Acc,'0'
	brne MOSTRA_VEL_ACIMA_JP3
	ldi Acc,' '
	st Z,Acc

MOSTRA_VEL_ACIMA_JP3:
	
	call	_SSD1306_PutS	
    	
	call    _SSD1306_Refresh
	
    popw	Y
	popw	X
	popawt
	popaw
	ret





;
; Apresenta barra de carregamento na inicialização
;
fn_Show_pbar_apres:

    pushaw
	pushawt
	pushw	X
	pushw	Y

	; Progress bar

	ldiw	x,8
	ldiw	y,27
	call	_SSD1306_Set_Progress_Bar_Coords		;bar start coords
	
	ldiw	x,_SSD1306_LCD_WIDTH-8*2
	ldiw	y,5
	call	_SSD1306_Set_Progress_Bar_Sizes			;Bar Sizes
	
	ldiw	x,0										
	ldiw	y,100
	call	_SSD1306_Set_Progress_Bar_Range			;Bar Range
	
	ldiaw	0
	ldiawt	0

	ldi     Temp,33
PBAR_L1:

    pushaw

	call	_SSD1306_Set_Progress_Bar_Value			;set value
	call	_SSD1306_Draw_Progress_Bar				;draw bar

    call _SSD1306_Refresh
	
	popaw

	adiw Acc:Acch,3
	dec Temp
	brne PBAR_L1
	adiw Acc:Acch,4

    call	_SSD1306_Set_Progress_Bar_Value			;set value
	call	_SSD1306_Draw_Progress_Bar				;draw bar

    call _SSD1306_Refresh
	
    popw	Y
	popw	X
	popawt
	popaw
	ret



;MSG_TITULO_LOGO:
      ;  .DB "Velocimetro GPS",0  
 

;+-------------------------------+
;| Mostra velocimetro no display |
;+-------------------------------+
; Transforma Knots em KM/h

fn_Conv_Knot2Km:
        
		_M_PUSH_UPPER_REGS						;save r16..r31
        push Acc
		push AccH

        ldi Acc,185
		mov R0,Acc

		ldi Acc,0
		mov R1,Acc

		lds Acc,_VELOCIDADE
		mov R2,Acc

		ldi Acc,0
		mov R3,Acc	 
         
        call _MUL_U16U16U32            ; Multiplica por 18
		       
        mov R0,R4
        mov R1,R5
		
        ldi Acc,100                    ; Divide por 100 para tirar duas casa decimal
        mov R2,Acc

        ldi Acc,0
		mov R3,Acc

        call _DIV_U16U16U16            ; Resultado em R0 e R1
       
        //
		//RESTORE REGISTERS
		//
		pop AccH
		pop Acc
		_M_POP_UPPER_REGS						;restore r16..r31
		
	    ret


; +----------------------------------+
; | PEGA DATA E HORA E COLOCA NA RAM |
; +----------------------------------+

fn_parsing_time:

     ldix _BUFFER_GPS

ACHA_HORA:

     ld Acc,X+ ; PEGA BYTE DO BUFFER_DADOS
	 cpi Acc,','
	 brne ACHA_HORA

     ldiy _SRAM_HORA
    
GRAVA_HORA:

     ld Acc,X+
     st Y+,Acc
   
     ld Acc,X+
     st Y+,Acc
    	
     ldi Acc,0
     st Y,Acc
	  

	 ; Ajusta hora para o fuso-horario do Brasil
     pushw X
	 pushw Y
	 pushw Z
	      
	 ldix _SRAM_HORA
     call _DECBIN

     mov Acc,R4
     subi Acc,3

     brmi VERIFICA_GPS_CARRY
	      
     jmp VERIFICA_GPS_CARRY_JMP

VERIFICA_GPS_CARRY:

     com Acc
     ldi AccH,1
	 add Acc,AccH

     mov AccH,Acc
     ldi Acc,24
     sub Acc,AccH

VERIFICA_GPS_CARRY_JMP:

     ldi		AccH,0
	 ldi		AccT,0
	 ldi		AccTH,0
	
	 call    _ULTOS                              ; Converte em ASCII
	
     ldiz    _DF_STR+9   
	 ldix    _SRAM_HORA
	 
	 ld Acc,Z+
	 st X+,Acc

	 ld Acc,Z
	 st X,Acc
	 
	 popw Z
	 popw Y
	 popw X

	 ;PISCA OS DOIS PONTOS NA TELA
     lds Acc,_PISCA_PONTOS
	 com Acc
	 sts _PISCA_PONTOS,Acc

	 cpi Acc,0xFF
	 breq REI_L2
	 
	 ldi Acc,':'
     st Y+,Acc
	 rjmp REI_L3
REI_L2:
	 ldi Acc,' '
     st Y+,Acc
REI_L3:
    
     ld Acc,X+
     st Y+,Acc
 	    
     ld Acc,X
     st Y+,Acc
      
     ldi Acc,0
	 st Y,Acc    
	 
     ldix _BUFFER_GPS
     ldi AccT,9

ACHA_DATA:
     ld Acc,X+
	 cpi Acc,','
	 brne ACHA_DATA
	 dec AccT
	 brne ACHA_DATA
	 
	 ldiy _SRAM_DATA

     ld Acc,X+
     st Y+,Acc
   
     ld Acc,X+
     st Y+,Acc
     
     ldi Acc,'/'
     st Y+,Acc
	 
     ld Acc,X+
     st Y+,Acc
 	    
     ld Acc,X
     st Y+,Acc
      
     ldi Acc,0
	 st Y,Acc    
	 
	 ret

       

;-------------






; +---------------------------+
; | Envia coordenada Latitude |
; +---------------------------+
fn_parsing_lat:

 		LDIX _BUFFER_GPS
		ldi AccT,3           ; Numero de virgulas a serem contadas para chegar a latitude

ENVIA_COORDENADA_LATITUDE:
	    
		ld Acc,X+		
		
		cpi Acc,','
		brne ENVIA_COORDENADA_LATITUDE
		
		subi AccT,1
		tst AccT
		brne ENVIA_COORDENADA_LATITUDE 

		LDIY _BUFFER_MONTAGEM

TRANSF_BUFFER_LATITUDE:

		ld Acc,X+
		st Y+,Acc
	
        cpi Acc,','
		brne TRANSF_BUFFER_LATITUDE

		LDIY _BUFFER_MONTAGEM

PRINT_LAT_LOOP:

		adiw Y,1

		ldi AccT,low(_BUFFER_MONTAGEM+10)
		ldi AccTH,high(_BUFFER_MONTAGEM+10)

        cp YL,AccT
		cpc YH,AccTH

    	brne PRINT_LAT_JMP

		jmp PRINT_LAT_CONT

PRINT_LAT_JMP:

		ld Acc,Y
		cpi Acc,','
		brne PRINT_LAT_LOOP
		
		 
PRINT_LAT_CONT:

      	; ********  Carrega as variaveis DEGREE, MINUTE E DEC_MINUTE ********	

	    ;### Carrega a DEGREE ###
		
		LDIX _BUFFER_MONTAGEM   ; no buffer_montagem , a coordenada tem '.'
		LDIY _BUFFER_MINI       ; e vai ser transferida para o buffer_mini sem o '.'

		ldi AccT,2

TRANSF_LAT_LOAD_DEGREE:
	    
        ld Acc,X+
		st Y+,Acc
		
		subi AccT,1
		tst AccT
		brne TRANSF_LAT_LOAD_DEGREE

		ldi Acc,0
		st Y,Acc                ; Null caractere para a rotina DECBIN

        ; Transforma os ASCII da memoria em um binario

        LDIX _BUFFER_MINI
		call _DECBIN

		mov Acc,R4 		        ; LSB do RESULTADO1 do DECBIN
           
		sts _DEGREE,Acc
				
		;### Carrega MINUTE ###

LOAD_MINUTE_LAT:

	    LDIX _BUFFER_MONTAGEM   ; no buffer_montagem , a coordenada tem '.'
		LDIY _BUFFER_MINI       ; e vai ser transferida para o buffer_mini sem o '.'

LOAD_MINUTE_LAT_LOOP:

	    ld Acc,X

		cpi Acc,'.'
		brne ACHA_PONTO_LAT_MINUTE      ; Se achou o ´.´

        sbiw X,2                 ; Volta duas casa decimais do minutes

        ld Acc,X+   				 ; Transfere para o Buffer_mini
		st Y+,Acc
		
        ld Acc,X   				 ; Transfere para o Buffer_mini
		st Y+,Acc
	
        ldi Acc,0
		st Y,Acc                 ; Grava null caractere para o decbin
        

; Transforma os ASCII da memoria em um binario

	    LDIX _BUFFER_MINI
		call _DECBIN

		mov Acc,R4               ; LSB do RESULTADO1 do DECBIN
          
        sts _MINUTE,Acc
        
        jmp LOAD_DEC_MINUTE_LAT

ACHA_PONTO_LAT_MINUTE:

        adiw X,1
		
		jmp  LOAD_MINUTE_LAT_LOOP

	    ;### Carrega DEC_MINUTE ###

LOAD_DEC_MINUTE_LAT:

		LDIX _BUFFER_MONTAGEM   ; no buffer_montagem , a coordenada tem '.'
		LDIY _BUFFER_MINI       ; e vai ser transferida para o buffer_mini sem o '.'

LOAD_DEC_MINUTE_LAT_LOOP:
	     
	    ld Acc,X
		cpi Acc,'.'

		brne ACHA_PONTO_LAT_DEC_MINUTE   ; Se achou o ´.´

		adiw X,1

		ld Acc,X+
		st Y+,Acc                ; pega os 3 caracteres depois do '.' e carrega no DEC_MINUTE
				
		ld Acc,X+
		st Y+,Acc
				    
        ld Acc,X
		st Y+,Acc
		        
		ldi Acc,0
		st Y,Acc

; Transforma os ASCII da memoria em um binario

        LDIX _BUFFER_MINI
        call _DECBIN

        ;multiplica o resultado por 10
        
		ldi Acc,10 
		mov R0,acc
		ldi Acc,0
		mov R1,Acc

        mov R3,R5
		mov R2,R4
        
     	call _MUL_U16U16U32

        mov Acc,R4   ; LSB do RESULTADO1 do DECBIN
		sts _DEC_MINUTE_L,Acc

		mov Acc,R5   ; MSB do RESULTADO1 do DECBIN
        sts _DEC_MINUTE_H,Acc

		ret


ACHA_PONTO_LAT_DEC_MINUTE:

        adiw X,1

        jmp LOAD_DEC_MINUTE_LAT_LOOP
		






; ******** Converte decimal para degree *****

fn_degree_to_minutes_lat:

		
		lds R0,_DEGREE	   ; Multiplica DEGREE X 60

		ldi Acc,0
		mov R1,Acc

        ldi Acc,60 
		mov R2,Acc

		ldi Acc,0
		mov R3,Acc

		call _MUL_U16U16U32

        lds Acc,_MINUTE     ; Soma resultado com MINUTE

        clc

        Add Acc,R4          ; Multiplica resultado por 10000
		mov R4,Acc

		mov Acc,R5
		ldi AccT,0
		adc Acc,AccT

		mov R5,Acc

        mov R0,R4
		mov R1,R5
        
		ldi Acc,low(10000)
		mov R2,Acc
		
		ldi Acc,high(10000)
		mov R3,Acc

        call _MUL_U16U16U32

        mov Acc,R4           ; Soma resultado com DEC_MINUTE e coloca em LATITUDE(32BITS)
        lds AccT,_DEC_MINUTE_L
		add Acc,AccT
		sts _LATITUDE_L,Acc

        mov Acc,R5
		lds AccT,_DEC_MINUTE_H
		adc Acc,AccT
		sts _LATITUDE_ML,Acc
		
		mov Acc,R6
		ldi AccT,0
		adc Acc,AccT
		sts _LATITUDE_MH,Acc

		mov Acc,R7
		ldi AccT,0
		Adc Acc,AccT
		sts _LATITUDE_H,Acc

 		; Chegando aqui, a variavel LATITUDE (32BITS) esta carregada com os dados em
        ; MINUTES
		ret
		              



; +---------------------------+
; | Envia coordenada Longitude|
; +---------------------------+

fn_parsing_lon:

 		LDIX _BUFFER_GPS
		ldi AccT,5           ; Numero de virgulas a serem contadas para chegar a longitude

ENVIA_COORDENADA_LONGITUDE:
	    
		ld Acc,X
		adiw X,1
		
		cpi Acc,','
		brne ENVIA_COORDENADA_LONGITUDE
		
		subi AccT,1
		tst AccT
		brne ENVIA_COORDENADA_LONGITUDE 

		LDIY _BUFFER_MONTAGEM

TRANSF_BUFFER_LONGITUDE:

		ld Acc,X
		st Y,Acc

		adiw X,1
		adiw Y,1

        cpi Acc,','
		brne TRANSF_BUFFER_LONGITUDE

		LDIY _BUFFER_MONTAGEM

PRINT_LON_LOOP:

		adiw Y,1

		ldi AccT,low(_BUFFER_MONTAGEM+10)
		ldi AccTH,high(_BUFFER_MONTAGEM+10)

        cp YL,AccT
		cpc YH,AccTH

    	brne PRINT_LON_JMP

		jmp PRINT_LON_CONT

PRINT_LON_JMP:

		ld Acc,Y
		cpi Acc,','
		brne PRINT_LON_LOOP
		
		 
PRINT_LON_CONT:

      	; ********  Carrega as variaveis DEGREE, MINUTE E DEC_MINUTE ********	

	    ;### Carrega a DEGREE ###
		
		LDIX _BUFFER_MONTAGEM   ; no buffer_montagem , a coordenada tem '.'
		LDIY _BUFFER_MINI       ; e vai ser transferida para o buffer_mini sem o '.'

		ldi AccT,3

TRANSF_LON_LOAD_DEGREE:
	    
        ld Acc,X
		st Y,Acc

		adiw X,1
		adiw Y,1

		subi AccT,1
		tst AccT
		brne TRANSF_LON_LOAD_DEGREE

		ldi Acc,0
		st Y,Acc                ; Null caractere para a rotina DECBIN

        ; Transforma os ASCII da memoria em um binario

        LDIX _BUFFER_MINI
		call _DECBIN
		
		mov Acc,R4 		        ; LSB do RESULTADO1 do DECBIN
        sts _DEGREE,Acc
		
		;### Carrega MINUTE ###

LOAD_MINUTE_LON:

	    LDIX _BUFFER_MONTAGEM   ; no buffer_montagem , a coordenada tem '.'
		LDIY _BUFFER_MINI       ; e vai ser transferida para o buffer_mini sem o '.'

LOAD_MINUTE_LON_LOOP:

	    ld Acc,X

		cpi Acc,'.'
		brne ACHA_PONTO_LON_MINUTE      ; Se achou o ´.´

        sbiw X,2                 ; Volta duas casa decimais do minutes

        ld Acc,X   				 ; Transfere para o Buffer_mini
		st Y,Acc

		adiw X,1
		adiw Y,1

        ld Acc,X   				 ; Transfere para o Buffer_mini
		st Y,Acc

		adiw Y,1

        ldi Acc,0
		st Y,Acc                 ; Grava null caractere para o decbin


; Transforma os ASCII da memoria em um binario

	    LDIX _BUFFER_MINI
		call _DECBIN

		mov Acc,R4               ; LSB do RESULTADO1 do DECBIN

        sts _MINUTE,Acc

        jmp LOAD_DEC_MINUTE_LON

ACHA_PONTO_LON_MINUTE:

        adiw X,1
		
		jmp  LOAD_MINUTE_LON_LOOP

	    ;### Carrega DEC_MINUTE ###

LOAD_DEC_MINUTE_LON:

		LDIX _BUFFER_MONTAGEM   ; no buffer_montagem , a coordenada tem '.'
		LDIY _BUFFER_MINI       ; e vai ser transferida para o buffer_mini sem o '.'

LOAD_DEC_MINUTE_LON_LOOP:
	     
	    ld Acc,X
		cpi Acc,'.'

		brne ACHA_PONTO_LON_DEC_MINUTE   ; Se achou o ´.´

		adiw X,1

		ld Acc,X
		st Y,Acc                ; pega os 3 caracteres depois do '.' e carrega no DEC_MINUTE
		
		adiw X,1
		adiw Y,1
		
		ld Acc,X
		st Y,Acc
		
		adiw X,1
		adiw Y,1
		    
        ld Acc,X
		st Y,Acc
		 
        adiw Y,1

		ldi Acc,0
		st Y,Acc

; Transforma os ASCII da memoria em um binario

        LDIX _BUFFER_MINI
        call _DECBIN

        ;multiplica o resultado por 10
        
		ldi Acc,10 
		mov R0,acc
		ldi Acc,0
		mov R1,Acc

        mov R3,R5
		mov R2,R4
        
     	call _MUL_U16U16U32

        mov Acc,R4   ; LSB do RESULTADO1 do DECBIN
		sts _DEC_MINUTE_L,Acc

		mov Acc,R5   ; MSB do RESULTADO1 do DECBIN
        sts _DEC_MINUTE_H,Acc

		ret


ACHA_PONTO_LON_DEC_MINUTE:

        adiw X,1

        jmp LOAD_DEC_MINUTE_LON_LOOP 	
		
		




; ******** Converte decimal para degree *****
fn_degree_to_minutes_lon:
		
	 	lds R0,_DEGREE	   ; Multiplica DEGREE X 60

		ldi Acc,0
		mov R1,Acc

        ldi Acc,60 
		mov R2,Acc

		ldi Acc,0
		mov R3,Acc

		call _MUL_U16U16U32

        lds Acc,_MINUTE     ; Soma resultado com MINUTE

        clc

        Add Acc,R4          ; Multiplica resultado por 10000
		mov R4,Acc

		mov Acc,R5
		ldi AccT,0
		adc Acc,AccT

		mov R5,Acc

        mov R0,R4
		mov R1,R5
        
		ldi Acc,low(10000)
		mov R2,Acc
		
		ldi Acc,high(10000)
		mov R3,Acc

        call _MUL_U16U16U32

        mov Acc,R4           ; Soma resultado com DEC_MINUTE e coloca em LATITUDE(32BITS)
        lds AccT,_DEC_MINUTE_L
		add Acc,AccT
		sts _LONGITUDE_L,Acc

        mov Acc,R5
		lds AccT,_DEC_MINUTE_H
		adc Acc,AccT
		sts _LONGITUDE_ML,Acc
		
		mov Acc,R6
		ldi AccT,0
		adc Acc,AccT
		sts _LONGITUDE_MH,Acc

		mov Acc,R7
		ldi AccT,0
		Adc Acc,AccT
		sts _LONGITUDE_H,Acc

 		; Chegando aqui, a variavel LONGITUDE (32BITS) esta carregada com os dados em
        ; MINUTES
		ret
		
		
		
		
;+-----------------+
;| Pega Velocidade |
;+-----------------+
fn_parsing_vel:

	    LDIX _BUFFER_GPS
		ldi AccT,7
		
ENVIA_VELOCIDADE:
         
        ld Acc,X  ; PEGA BYTE DO BUFFER_DADOS

		adiw X,1

        cpi Acc,','
		brne ENVIA_VELOCIDADE

		subi AccT,1
		tst AccT
		brne ENVIA_VELOCIDADE

        LDIY _BUFFER_MONTAGEM
		 
TRANSF_BUFFER_VELOCIDADE:
	    		 
  	    ld Acc,X+
		st Y+,Acc
				
		cpi Acc,','
		brne TRANSF_BUFFER_VELOCIDADE
		
		; Transfere Buffer_montagem para buffer_mini sem o '.'

        LDIX _BUFFER_MONTAGEM
		LDIY _BUFFER_MINI

TRANSF_BUFFER_VELO_LOOP:
	
	    ld Acc,X
		
		cpi Acc,'.'
		brne  TRANSF_BUFFER_VELO_LOOP_2

        ldi Acc,0  ; Grava caractere nulo usada na rotina Decbin
        st Y,Acc

		jmp TRANSF_BUFFER_VELO_SAIDA

TRANSF_BUFFER_VELO_LOOP_2:
        
		st Y+,Acc
		
		adiw X,1
		
		jmp TRANSF_BUFFER_VELO_LOOP

TRANSF_BUFFER_VELO_SAIDA:

        ; Transforma os ASCII da memoria em um binario de 32 bits
		
		LDIX _BUFFER_MINI
		call _DECBIN
		
		mov Acc,R4          ; LSB do RESULTADO1 do DECBIN
		sts _VELOCIDADE,Acc
		
    	;Verifica se esta acima de 120km
		cpi Acc,64
		brlo TRANSF_BUFFER_VELO_SAIDA_ABAIXO
		
		lds Acc,_FLAG_ACIMA_120KM
		cpi Acc,_ON
		breq ACIMA120_SAIDA

		push Acc
		_SET _FLAG_ACIMA_120KM
		ldi Acc,23
		sts _GPS_TICKS_COUNT,Acc
		call _BEEP2
		pop acc
ACIMA120_SAIDA:		
		ret
TRANSF_BUFFER_VELO_SAIDA_ABAIXO:
        _CLEAR _FLAG_ACIMA_120KM
		ret

;+--------------+
;| Pega Direcao |
;+--------------+
fn_parsing_dir:

        LDIX _BUFFER_GPS
		ldi AccT,8

ENVIA_DIRECAO:

        ld Acc,X+
        
		cpi Acc,','
		brne ENVIA_DIRECAO

		subi AccT,1
		tst AccT
		brne ENVIA_DIRECAO

		LDIY _BUFFER_MONTAGEM

TRANSF_BUFFER_DIRECAO:
	    
		ld Acc,X+
		st Y+,Acc
		
		cpi Acc,','
		brne TRANSF_BUFFER_DIRECAO

        ; Transfere Buffer_montagem para buffer_mini sem o '.'

        LDIX _BUFFER_MONTAGEM
		LDIY _BUFFER_MINI

TRANSF_BUFFER_DIR_LOOP:
		
        ld Acc,X
		cpi Acc,'.'
		brne TRANSF_BUFFER_DIR_LOOP_2

		ldi Acc,0
		st Y,Acc

        rjmp TRANSF_BUFFER_DIR_SAIDA

TRANSF_BUFFER_DIR_LOOP_2:
		
		st Y+,Acc

		adiw X,1
		
	    rjmp TRANSF_BUFFER_DIR_LOOP

TRANSF_BUFFER_DIR_SAIDA:

		; Agora transforma os ASCII da memoria em um binario de 32 bits

		LDIX _BUFFER_MINI
		call _DECBIN

		sts _DIRECAO_L,R4
		sts _DIRECAO_H,R5
		
		ret

RECEBE_LOOP_TS:

        dec TEMP
		brne RECEBE_LOOP

		jmp REC_GGA_SAIDA
  

;
; Pega numero de satelites
; Saida : R4
; Procura sentença: $GPGSV
; Ex: $GPGSV,4,1,13,02,02,213,,03,-3,000,,11,00,121,,14,13,172,05*67 
; $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
; $GPGGA,000943.261,,,,,0,0,,,M,,M,,*43
					
_fn_Recebe_GGA:
      
	    //
		//SAVE SREG
		//
		push	Acc
	      
		//
		//SAVE REGISTERS
		//
		_M_PUSH_UPPER_REGS						 ;save r16..r31

		cli

		ldi Temp,6                               ;Numero de tentativas de receber a sentença certa
		
		lds R4,_GUARDA_R4

		lds Acc,_FLAG_1_MINUTO
	    cpi Acc,_ON

        brne REC_GGA_SAIDA 
		  
        _CLEAR _FLAG_1_MINUTO 

		lds Acc,_VAR_3_MINUTOS
		dec Acc
		sts _VAR_3_MINUTOS,Acc
		
		tst Acc
		brne REC_GGA_SAIDA
		
		ldi Acc,3
		sts _VAR_3_MINUTOS,Acc
		
		  
RECEBE_LOOP:

        ldiw   X,_BUFFER_GPS                      ; Aponta o ponteiro X para BUFFER GPS

FN_RECEBE_L1:
     	call _USART_RX
	
		cpi Acc,'$'
		
		brne FN_RECEBE_L1 
		
		st X+,Acc

FN_RECEBE_L2:
		call _USART_RX
	
		st X+,Acc

		cpi Acc,'*'

		brne FN_RECEBE_L2

		ldiw  X,_BUFFER_GPS+4                     ; Aponta o ponteiro X para BUFFER GPS

FN_RECEBE_L3:
        ld Acc,X
		cpi Acc,'G'

		brne RECEBE_LOOP_TS

        ldi TempH,7   ; Numero de virgulas para chegar na sentenca correta

FN_PULA_VIG1:
        ld Acc,X+
		cpi Acc,','
		brne FN_PULA_VIG1
		dec TempH
		brne FN_PULA_VIG1
		
		ld AccH,X+
		ld Acc,X
		
		ldiw X,_BUFFER_MINI
		st X+,AccH

		cpi Acc,','
		brne FN_PULA_CONT

		ldi Acc,0
		st X,Acc

		rjmp FN_PULA_CONT2

FN_PULA_CONT:

		st X+,Acc
		ldi Acc,0
		st X,Acc

FN_PULA_CONT2:

		ldiw X,_BUFFER_MINI
		call _DECBIN
		mov Acc,R4
		sts _GUARDA_R4,Acc

REC_GGA_SAIDA:
  
        sei

		//
		//RESTORE REGISTERS
		//
		_M_POP_UPPER_REGS						  ;restore r16..r31
		//
		//RESTORE SREG
		//
		pop		Acc

		RET




;
; Carrega endereco MAC
;
fn_carrega_MAC:

      //
	  //SAVE SREG
      //
      push	Acc
      in		Acc,SREG
      //
	  //SAVE REGISTERS
	  //

      _M_PUSH_LOWER_REGS						  ;save r0..r15
      _M_PUSH_UPPER_REGS						  ;save r16..r31

	  ldi AccH,2
	  ldi TempH,5

fn_carrega_MAC_ini:

	  ldiw Z,AT_CIPSTAMAC*2
	  call _SEND_STRING_ESP

CARREGA_MAC_REC:
      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs CARREGA_MAC_REC

	  cpi Acc,'+'
      breq  CARREGA_MAC_REC_CONT

	  dec TempH
	  brne CARREGA_MAC_REC
      
	  _M_DELAY_MILLISECONDS 250

	  dec AccH
	  brne fn_carrega_MAC_ini
	  ; GRAVA QUALQUER COISA NO BUFFER
	  ldiw X,_BUFFER_MAC+17
	  ldi Acc,'"'
	  st X,Acc 
	  rjmp CARREGA_MAC_SAIDA
	  
CARREGA_MAC_REC_CONT:

	  ldiw X,_BUFFER_MAC
	  ldi TEMPH,17

CARREGA_MAC_L1:
      call _ESP_DATA_RX
	  brcs CARREGA_MAC_L1

	  cpi Acc,'"'    ; Procura aspas
	  brne CARREGA_MAC_L1

CARREGA_MAC_L2:
       call _ESP_DATA_RX
	   brcs CARREGA_MAC_L2
	   
	   st X+,Acc
	   
	   cpi Acc,'"'
	   brne CARREGA_MAC_L2

	   subiw X,1
	   ldi Acc,0
	   st X,Acc

CARREGA_MAC_SAIDA:
	   
	   //
	  //RESTORE REGISTERS
	  //
	  _M_POP_UPPER_REGS						      ;restore r16..r31
	  _M_POP_LOWER_REGS
	  
	  //
	  //RESTORE SREG
	  //
	  out		SREG,Acc
	  pop		Acc
	
	  RET

AT_CIPSTAMAC:
      .DB "AT+CIPSTAMAC?",_CR,_LF,0



_SYSTEM_HALT:
      // 
	  //SAVE SREG
      //
         push	Acc
         in		Acc,SREG
      //
	  //SAVE REGISTERS
	  //

        _M_PUSH_LOWER_REGS						  ;save r0..r15
        _M_PUSH_UPPER_REGS

         // LCD <- HALT
		ldiw	Z,MSG_HALT*2	                      
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

		_M_DELAY_MILLISECONDS 4000

		call	_SSD1306_Cls

		 //
	  //RESTORE REGISTERS
	  //
	  _M_POP_UPPER_REGS						      ;restore r16..r31
	  _M_POP_LOWER_REGS
	  
	  //
	  //RESTORE SREG
	  //
	  out		SREG,Acc
	  pop		Acc
	
        
		RET


 
 MSG_HALT:
 	    .DB "SYSTEM HALT!",0,_nc  
		
		
				 
fn_tela_pontuacao:

		//
		//SAVE SREG
		//
		push	Acc
		in		Acc,SREG

		_M_PUSH_LOWER_REGS						        ;save r0..r15
	    _M_PUSH_UPPER_REGS						        ;save r16..r31 
			
		ldi		Acc,_WHITE							;Set White Logical Mode
		call	_SSD1306_Set_Operator			
		ldi		Acc,1								;Set Scale to 2
		call	_SSD1306_Set_Text_Scale			
		call	_SSD1306_Set_Display_Normal			;Set Screen Normal 
		call	_SSD1306_Cls
		//
		//draw start message
		//
		ldiw	z,MSG_PONTUACAO*2	;Progress
		;ldiw    x,0
		call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	    movw	XH:XL,AccH:Acc
		ldiw    y,0
		call	_SSD1306_Locate							;locate msg
		call	_SSD1306_PutSF
		  
		ldiw	z,MSG_PONTUACAO_BAIXO*2	;Progress
		ldiw    x,0
		ldiw    y,25
		call	_SSD1306_Locate							;locate msg
		call	_SSD1306_PutSF

		ldi		Acc,_WHITE							;Set White Logical Mode
		call	_SSD1306_Set_Operator			
		ldi		Acc,2								;Set Scale to 2
		call	_SSD1306_Set_Text_Scale			
		call	_SSD1306_Set_Display_Normal			;Set Screen Normal 
		
		ldiw    z,_DF_STR
		
		ldiw    x,23
		ldiw    y,9
		call	_SSD1306_Locate							;locate msg

        ; Imprime pardais acima
		lds Acc,_PARDAIS_ACIMA
		ldi AccH,0
               
		call    _UITOS									; Converte em ASCII
	
		ldiz    _DF_STR+3     
		call   _SSD1306_PutS

		ldi Acc,'/'
		call   _SSD1306_PutC

		; Imprime pardais totais
		lds Acc,_PARDAIS_TOTAIS
		ldi AccH,0
             
		call    _UITOS									; Converte em ASCII
	
		ldiz    _DF_STR+3     
		call   _SSD1306_PutS
			 
		call	_SSD1306_Refresh

	    //RESTORE REGISTERS
		//
		_M_POP_UPPER_REGS								;restore r16..r31
		_M_POP_LOWER_REGS
		//
		//RESTORE SREG
		//
		out		SREG,Acc
		pop		Acc	
		ret


MSG_PONTUACAO:
        .DB "Resumo diario",0 

MSG_PONTUACAO_BAIXO:
        .DB "Vel.acima    Radares",0,_nc

 MSG_TITULO_LOGO:
        .DB "Velocimetro GPS",0   

;
;
;
_TELA_TIME:
         //
	     //SAVE SREG
	     //
	    push	Acc
        in		Acc,SREG

        _M_PUSH_LOWER_REGS						        ;save r0..r15
        _M_PUSH_UPPER_REGS						        ;save r16..r31 

		
		call fn_parsing_time
		call fn_ShowTime

		 //RESTORE REGISTERS
		 //
	     _M_POP_UPPER_REGS						;restore r16..r31
	     _M_POP_LOWER_REGS
	     //
	     //RESTORE SREG
	     //
         out		SREG,Acc
	     pop		Acc	

		 RET