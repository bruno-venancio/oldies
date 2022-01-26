
_MENU_CONFIG:
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
	 
_MENU_CFG_INI:

	// Inicia na posicao 1 //
	ldi     Acc,5
	sts     _GUARDA_YL,Acc                          ;guarda posical Y

	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	TempH:Temp,AccH:Acc
	

MENU_CFG_LOOP:

    ldi		Acc,_INVERT							    ;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,1								    ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
	call	_SSD1306_Cls

	call	fn_ShowScreenBorder						;show border
	
	; MENU ITEM 1
	ldiw	Z,MENU_CFG_ITEM1*2	                        ;Progress
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,5
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
	

	; MENU ITEM 2
	ldiw	Z,MENU_CFG_ITEM2*2	                        ;Progress
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,18
					
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF


    ; MENU ITEM 3
	ldiw	Z,MENU_CFG_ITEM3*2	                        ;Progress
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,31
	
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
		

	; MENU ITEM 4
	ldiw	Z,MENU_CFG_ITEM4*2	                        ;Progress
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,44
		
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
MENU_CFG_KEY:

    call _KEYBOARD_READ_STATIC

	call _BUZZER_ON
	_M_DELAY_MILLISECONDS 20
	call _BUZZER_OFF
		
MNU_CFG_VER_KEY1:
	cpi Acc,_KEY_DOWN
	brne MNU_CFG_VER_KEY_UP
	// DOWN
	lds Acc,_GUARDA_YL
	ldi AccH,13
	add Acc,AccH

	cpi Acc,45
	brlo MNU_CFG_VER_L1
	ldi Acc,44
MNU_CFG_VER_L1:
	sts _GUARDA_YL,Acc
	rjmp MENU_CFG_LOOP


MNU_CFG_VER_KEY_UP:
    cpi Acc,_KEY_UP
    brne MNU_CFG_VER_KEY_ENTER
	// UP
	lds Acc,_GUARDA_YL
	subi Acc,13
	brpl MNU_CFG_VER_L2
	ldi Acc,5
MNU_CFG_VER_L2:
	sts _GUARDA_YL,Acc
	rjmp MENU_CFG_LOOP
	
    // ENTER
MNU_CFG_VER_KEY_ENTER:
     
	lds Acc,_GUARDA_YL
	cpi Acc,5

	breq MNU_CFG_ITEM_VOLUME          //   VOLUME
	
	cpi Acc,18
	breq MNU_CFG_ITEM_TEMP_AVISO      //  TEMPO DE AVISO

	cpi Acc,31
	breq MNU_CFG_ITEM_BRILHO          //  BRILHO DA TELA

	cpi Acc,44
	breq MNU_CFG_ITEM_SAIR            // SAIR

	rjmp  MENU_CFG_LOOP


MNU_CFG_ITEM_VOLUME:
    //call _GPS_INFO
	rjmp  MENU_CFG_LOOP

MNU_CFG_ITEM_TEMP_AVISO:
   // call _ATUALIZA_PONTOS
	rjmp  MENU_CFG_LOOP

MNU_CFG_ITEM_BRILHO:
    //call _ATUALIZA_PONTOS
	rjmp  MENU_CFG_LOOP

MNU_CFG_ITEM_SAIR:

    _M_DELAY_MILLISECONDS 100
	      
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
	


MENU_CFG_ITEM1:
    .DB "VOLUME",_NC,0
MENU_CFG_ITEM2:
	.DB "TEMPO DE AVISO",_NC,0
MENU_CFG_ITEM3:
	.DB "BRILHO DA TELA",0,_NC
MENU_CFG_ITEM4:
	.DB "SAIR",_NC,0	            



		