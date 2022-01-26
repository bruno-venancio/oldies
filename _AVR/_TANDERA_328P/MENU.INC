
_MENU:
        //
		//SAVE SREG
		//
		push	Acc
		in		Acc,SREG
  		//
		//SAVE REGISTERS
		//

		_M_PUSH_LOWER_REGS						        ;save r0..r15
		_M_PUSH_UPPER_REGS						        ;save r16..r31
	
	    ldi Acc,0
	    sts _GPS_TICKS_COUNT,Acc

					
_MENU_INI:
    		
		// Inicia na posicao 1 //
		ldi     Acc,5
		sts     _GUARDA_YL,Acc                          ;guarda posical Y

		call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
		movw	TempH:Temp,AccH:Acc
	
MENU_LOOP:

		ldi		Acc,_WHITE							    ;Set White Logical Mode
		call	_SSD1306_Set_Operator			
		ldi		Acc,1								    ;Set Scale to 2
		call	_SSD1306_Set_Text_Scale			
		call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
		call	_SSD1306_Cls

		call	fn_ShowScreenBorder						;show border
	
		; MENU ITEM 1 (ATUALIZAR PONTOS)
		ldiw	Z,MENU_ITEM1*2	                        ;Progress
		call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
		movw	XH:XL,AccH:Acc
		ldiw	Y,5
		call	_SSD1306_Locate							;locate msg
		call	_SSD1306_PutSF

	    ; MENU ITEM 2 (TEMPO DE AVISO)
		ldiw	Z,MENU_ITEM2*2	                        ;Progress
		call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
		movw	XH:XL,AccH:Acc
		ldiw	Y,18
	
		call	_SSD1306_Locate							;locate msg
		call	_SSD1306_PutSF
				
;
; Seleciona ITEM
;
	
		ldi		Acc,_INVERT							    ;Set White Logical Mode
		call	_SSD1306_Set_Operator			
		 
		ldiaw	_SSD1306_LCD_WIDTH
		pushaw
		call	_SSD1306_Get_Char_Height
		movw	AccTH:AccT,AccH:Acc
		popaw
    	
		movw	XH:XL,TempH:Temp

		ldi     YH,0		
		lds     YL,_GUARDA_YL                           ;recupera posical Y
	
		call _SSD1306_Draw_Box_Full
		
		call _SSD1306_Refresh

/*
+------------+
| LE TECLADO |
+------------+
*/
GPS_INF_MENU_KEY:
  
		call _KEYBOARD_READ_STATIC
		cpi Acc,_KEY_TIMEOUT
		breq MNU_ITEM_SAIR
		
VER_KEY1:
		cpi Acc,_KEY_NONE
		brne VER_KEY_ENTER
		// DOWN
		lds Acc,_GUARDA_YL
		ldi AccH,13
		add Acc,AccH

		cpi Acc,19
		brlo VER_L1
    
		rjmp MENU_2
VER_L1:
		sts _GUARDA_YL,Acc
		rjmp MENU_LOOP
	
		// ENTER
VER_KEY_ENTER:
     
		lds Acc,_GUARDA_YL
		cpi Acc,5

		breq MNU_ITEM_ATUALIZA      // ATUALIZA PONTOS
	
		cpi Acc,18
		breq MNU_ITEM_CONFIGURA      // CONFIGURACOES
	
		cpi Acc,31
		breq MNU_ITEM_SAIR          // SAIR

		rjmp  MENU_LOOP 
	
MNU_ITEM_ATUALIZA:
     
		call _ATUALIZA_PONTOS
	
		ldi Acc,_ON
		call _USART_RX_INT_ENABLE

		;ldi     Acc,18
		;sts     _GUARDA_YL,Acc 

		;rjmp  MENU2_LOOP 
		rjmp MNU_ITEM_SAIR

MNU_ITEM_CONFIGURA:

		_M_DELAY_MILLISECONDS 200 
	
		ldiw Z,0
		call _TEMPO_AVISO_CFG
	
		ldi     Acc,18
		sts     _GUARDA_YL,Acc                          ;volta na posicao do "sair"
	 
		rjmp  MENU2_LOOP 


MNU_ITEM_SAIR:
      	       
	      		 
		//
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
	


MENU_ITEM1:
		.DB "ATUALIZAR PONTOS",0,_NC
		    
MENU_ITEM2:
		.DB "TEMPO DE AVISO",0,_NC 

MENU_ITEM3:
		.DB "INFO GPS",0,_NC

MENU_ITEM4:
		.DB "VOLUME",0,_NC
		
MENU_ITEM6:
		.DB "GIRAR TELA",0,_NC	  

MENU_ITEM7:
		.DB "ALERTA VELOCIDADE",0

MENU_ITEM8:
		.DB "SAIR",0,_NC	  


;
;
MENU_2:
		// Inicia na posicao 1 //
		ldi     Acc,5
		sts     _GUARDA_YL,Acc                          ;guarda posical Y

		call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
		movw	TempH:Temp,AccH:Acc
	
MENU2_LOOP:

		ldi		Acc,_WHITE							    ;Set White Logical Mode
		call	_SSD1306_Set_Operator			
		ldi		Acc,1								    ;Set Scale to 2
		call	_SSD1306_Set_Text_Scale			
		call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
		call	_SSD1306_Cls

		call	fn_ShowScreenBorder						;show border
	
		; MENU ITEM 4 (VOLUME)
		ldiw	Z,MENU_ITEM4*2	                        ;Progress
		call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
		movw	XH:XL,AccH:Acc
		ldiw	Y,5
	
		call	_SSD1306_Locate							;locate msg
		call	_SSD1306_PutSF
				               
		
	    ; MENU ITEM 4 (GIRATELA)
		ldiw	Z,MENU_ITEM6*2	                        ;Progress
		call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
		movw	XH:XL,AccH:Acc
		ldiw	Y,18
	
		call	_SSD1306_Locate							;locate msg
		call	_SSD1306_PutSF
			
		ldi		Acc,_INVERT							    ;Set White Logical Mode
		call	_SSD1306_Set_Operator			
		 
		ldiaw	_SSD1306_LCD_WIDTH
		pushaw
		call	_SSD1306_Get_Char_Height
		movw	AccTH:AccT,AccH:Acc
		popaw
    	
		movw	XH:XL,TempH:Temp

		ldi     YH,0		
		lds     YL,_GUARDA_YL                           ;recupera posical Y
	
		call _SSD1306_Draw_Box_Full
		
		call _SSD1306_Refresh

		ldi Acc,0
		sts _GPS_TICKS_COUNT,Acc

/*
+------------+
| LE TECLADO |
+------------+
*/
MENU2_L1:
		call _KEYBOARD_READ_STATIC
		cpi Acc,_KEY_TIMEOUT
		lbreq MNU_ITEM_SAIR
		
VER2_KEY1:
		cpi Acc,_KEY_NONE
		brne VER2_KEY_ENTER
		// DOWN
		lds Acc,_GUARDA_YL
		ldi AccH,13
		add Acc,AccH

		cpi Acc,19
		brlo VER2_L1
    
		ldiw Z,0
		jmp MENU_4
VER2_L1:
		sts _GUARDA_YL,Acc
		rjmp MENU2_LOOP
	
		// ENTER
VER2_KEY_ENTER:
     
		lds Acc,_GUARDA_YL
		cpi Acc,5
		brne VER2_COMP2
		call _VOLUME_CFG            //  VOLUME

		lds Acc,_FLAG_VOLUME
		tst Acc
		brne VER2_KEY_CONT1

		call _MSG_MUTE
	

VER2_KEY_CONT1:
		rjmp MENU2_LOOP
VER2_COMP2:	
		cpi Acc,18
		lbrne MENU2_LOOP
		call MNU_ITEM_GIRAR            // GIRA TELA
		rjmp MENU2_LOOP
		 
	
MNU_ITEM_INFO_GPS:
    
		call _INT0_DISABLE

		;call _GPS_INFO
	
		cli

		;call _INT0_ENABLE
	
		ldi Acc,_ON
		call _USART_RX_INT_ENABLE

		ldiw Z,0
		call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
		movw	TempH:Temp,AccH:Acc

		ldi     Acc,18
		sts     _GUARDA_YL,Acc 
		
		rjmp  MENU2_LOOP 
		
 


;
; Gira a tela
;
MNU_ITEM_GIRAR:
     
		ldi		Acc,_INVERT							    ;Set White Logical Mode
		call	_SSD1306_Set_Operator			
		ldi		Acc,2								    ;Set Scale to 2
		call	_SSD1306_Set_Text_Scale			
		call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
		call	_SSD1306_Cls

		call	fn_ShowScreenBorder						;show border
	
		ldiw	Z,MSG_GIRA*2	                        ;LCD <- GIRA TELA
		call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
		movw	XH:XL,AccH:Acc
		ldiw	Y,8
		call	_SSD1306_Locate							;locate msg
		call	_SSD1306_PutSF

		call    _SSD1306_Refresh

		ldi Acc,0
		sts _GPS_TICKS_COUNT,Acc
	
/*
+------------+
| LE TECLADO |
+------------+
*/
GIRA_KEY:
	   	call _KEYBOARD_READ_STATIC
		cpi Acc,_KEY_TIMEOUT
		lbreq MNU_ITEM_SAIR

		cpi Acc,_KEY_ENTER
		breq GIRA_KEY_ENTER

GIRA_KEY_CONT:

		call _BUZZER_ON
		_M_DELAY_MILLISECONDS 20
		call _BUZZER_OFF
		
GIRA_VER_KEY1:
		// DOWN
		lds Acc,_FLAG_GIRA_TELA
		com Acc
		sts _FLAG_GIRA_TELA,Acc

		rcall _GIRA_TELA
	
		rjmp MNU_ITEM_GIRAR


		// ENTER
GIRA_KEY_ENTER:

		ldi AccH,LOW(_EE_GIRA_TELA)
		ldi AccT,HIGH(_EE_GIRA_TELA)
		lds Acc,_FLAG_GIRA_TELA
		call _EEPROM_WRITE 

		call _BEEP

		ldi     Acc,18
		sts     _GUARDA_YL,Acc                          ;guarda posical Y
		jmp    MENU2_LOOP 


MSG_GIRA:
		.DB "GIRA TELA",0	
	 

; 
; 
_GIRA_TELA:
        lds Acc,_FLAG_GIRA_TELA
		cpi Acc,_ON
		breq GIRA_TELA_NORMAL
		
		call _SSD1306_SET_SCREEN_UPSIDEDOWN
		rjmp GIRA_TELA_EXIT

GIRA_TELA_NORMAL:
        call _SSD1306_Display_Initialize 	
GIRA_TELA_EXIT:
		RET
 
;
;  MENU 4
;
;
;
MENU_4:

		// Inicia na posicao 1 //
		ldi     Acc,5
		sts     _GUARDA_YL,Acc                          ;guarda posical Y

		call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
		movw	TempH:Temp,AccH:Acc
	
MENU4_LOOP:

		ldi		Acc,_WHITE							    ;Set White Logical Mode
		call	_SSD1306_Set_Operator			
		ldi		Acc,1								    ;Set Scale to 2
		call	_SSD1306_Set_Text_Scale			
		call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
		call	_SSD1306_Cls

		call	fn_ShowScreenBorder						;show border
	
		; MENU ITEM 7 (Alerta velocidade)
		ldiw	Z,MENU_ITEM7*2	                        ;Progress
		call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
		movw	XH:XL,AccH:Acc
		ldiw	Y,5
		call	_SSD1306_Locate							;locate msg
		call	_SSD1306_PutSF
		
		;imprime setinha
        ldiw    X,117
        ldiw	Y,5
		call	_SSD1306_Locate							;locate msg

		;pega config da eeprom
		ldi		AccH,LOW(_EE_VEL_CORRENTE)
		ldi		AccT,HIGH(_EE_VEL_CORRENTE)
		call	_EEPROM_READ
		cpi     Acc,'1'

		brne	MNU_ITEM_ALERTA_JP1

		ldi     Acc,'{'    ; imprime setinha
		call	_SSD1306_PutC
		_SET    _FLAG_VEL_CORRENTE
        rjmp MNU_ITEM_ALERTA_JP2

MNU_ITEM_ALERTA_JP1:
        ldi     Acc,' '   ; imprime setinha
		call	_SSD1306_PutC
		_CLEAR _FLAG_VEL_CORRENTE

MNU_ITEM_ALERTA_JP2:

		; MENU ITEM 8 (Sair)
		ldiw	Z,MENU_ITEM8*2	                        ;Progress
		call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
		movw	XH:XL,AccH:Acc
		ldiw	Y,18
	
		call	_SSD1306_Locate							;locate msg
		call	_SSD1306_PutSF
		
		ldi		Acc,_INVERT							    ;Set White Logical Mode
		call	_SSD1306_Set_Operator			
		 
		ldiaw	_SSD1306_LCD_WIDTH
		pushaw
		call	_SSD1306_Get_Char_Height
		movw	AccTH:AccT,AccH:Acc
		popaw
    	
		movw	XH:XL,TempH:Temp

		ldi     YH,0		
		lds     YL,_GUARDA_YL                           ;recupera posical Y
	
		call _SSD1306_Draw_Box_Full
		
		call _SSD1306_Refresh
	 
	    ldi  Acc,0
		sts _GPS_TICKS_COUNT,Acc
	
/*
+------------+
| LE TECLADO |
+------------+
*/
		call _KEYBOARD_READ_STATIC
		cpi Acc,_KEY_TIMEOUT
		lbreq MNU_ITEM_SAIR
		
VER4_KEY1:
		cpi Acc,_KEY_NONE
		brne VER4_KEY_ENTER
		// DOWN

		lds Acc,_GUARDA_YL
		ldi AccH,13
		add Acc,AccH

		cpi Acc,19
		brlo VER4_L1

		ldiw Z,0
		jmp _MENU_INI

VER4_L1:
		sts _GUARDA_YL,Acc
		rjmp MENU4_LOOP
				
     
		// ENTER
VER4_KEY_ENTER:
 
        lds Acc,_GUARDA_YL
		cpi Acc,5

		breq MNU_ITEM_ALERTA_VEL     // ALERTA VELOCIDADE ON/OFF
	
		cpi Acc,18
		lbreq MNU_ITEM_SAIR       // SAIR
        
		rjmp  MENU4_LOOP 	


MNU_ITEM_ALERTA_VEL:
      
		call    _BEEP
			
		lds     Acc,_FLAG_VEL_CORRENTE
		cpi		Acc,_ON
		
		brne    MNU_ITEM_ALERTA_VEL2
		
MNU_ITEM_ALERTA_VEL1:
        ldiw    X,117
        ldiw	Y,5
		call	_SSD1306_Locate							;locate msg
		ldi     Acc,' '   ; imprime setinha
		call	_SSD1306_PutC

		call    _SSD1306_Refresh
		
		ldi AccH,LOW(_EE_VEL_CORRENTE)
		ldi AccT,HIGH(_EE_VEL_CORRENTE)
		
		ldi Acc,0xff
		call _EEPROM_WRITE

		_CLEAR _FLAG_VEL_CORRENTE

		rjmp  MENU4_LOOP 


MNU_ITEM_ALERTA_VEL2:
        ldiw    X,117
        ldiw	Y,5
		call	_SSD1306_Locate							;locate msg
		ldi     Acc,'{'   ; imprime setinha
		call	_SSD1306_PutC

		call    _SSD1306_Refresh
		
		ldi AccH,LOW(_EE_VEL_CORRENTE)
		ldi AccT,HIGH(_EE_VEL_CORRENTE)
		
		ldi Acc,'1'
		call _EEPROM_WRITE
		_SET _FLAG_VEL_CORRENTE

		rjmp  MENU4_LOOP 


		
_MSG_MUTE:
        call	_SSD1306_Cls
        ldiw	Z,MSG_MUTE_AVISO*2	                      
		ldiw    X,13
		ldiw	Y,16
		call	_SSD1306_Locate							;locate msg
		call	_SSD1306_PutSF
        call    _SSD1306_Refresh

		_M_DELAY_MILLISECONDS 4000

		RET

MSG_MUTE_AVISO:
        .DB "APARELHO MUDO! ",0

