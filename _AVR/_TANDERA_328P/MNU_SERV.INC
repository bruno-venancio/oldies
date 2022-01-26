
_MENU_SERVICO:
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
	 
_MENU_SERV_INI:

	// Inicia na posicao 1 //
	ldi     Acc,5
	sts     _GUARDA_YL,Acc                          ;guarda posical Y

	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	TempH:Temp,AccH:Acc
	

MENU_SERV_LOOP:

    ldi		Acc,_INVERT							    ;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,1								    ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
	call	_SSD1306_Cls

	call	fn_ShowScreenBorder						;show border
	
	; MENU ITEM 1
	ldiw	Z,MENU_SERV_ITEM1*2	                       
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,5
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
	

	; MENU ITEM 2
	ldiw	Z,MENU_SERV_ITEM2*2	                      
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,18
					
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF


    ; MENU ITEM 3
	ldiw	Z,MENU_SERV_ITEM3*2	                      
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,31
	
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
MENU_SERV_KEY:

    call _KEYBOARD_READ_STATIC

	call _BUZZER_ON
	_M_DELAY_MILLISECONDS 20
	call _BUZZER_OFF
		
MNU_SERV_VER_KEY1:
	cpi Acc,_KEY_DOWN
	brne MNU_SERV_VER_KEY_UP
	// DOWN
	lds Acc,_GUARDA_YL
	ldi AccH,13
	add Acc,AccH

	cpi Acc,32
	brlo MNU_SERV_VER_L1
	ldi Acc,31
MNU_SERV_VER_L1:
	sts _GUARDA_YL,Acc
	rjmp MENU_SERV_LOOP


MNU_SERV_VER_KEY_UP:
    cpi Acc,_KEY_UP
    brne MNU_SERV_VER_KEY_ENTER
	// UP
	lds Acc,_GUARDA_YL
	subi Acc,13
	brpl MNU_SERV_VER_L2
	ldi Acc,5
MNU_SERV_VER_L2:
	sts _GUARDA_YL,Acc
	rjmp MENU_SERV_LOOP
	
    // ENTER
MNU_SERV_VER_KEY_ENTER:
     
	lds Acc,_GUARDA_YL
	cpi Acc,5

	breq MNU_SERV_ITEM_FORMATAR          //   VOLUME
	
	cpi Acc,18
	breq MNU_SERV_ITEM_BAIXAR_TUDO      //  TEMPO DE AVISO

	cpi Acc,31
	breq MNU_SERV_ITEM_SAIR            // SAIR

	rjmp  MENU_SERV_LOOP


MNU_SERV_ITEM_FORMATAR:
    
	
	
	ldi Acc,'1'
	sts _VAR_TELA,Acc                
	call FRASE_FORMATANDO_FORM          ; LCD <- formatando...

	call _APAGA_TUDO
	
	ldi Acc,'2'
	sts _VAR_TELA,Acc                
	call FRASE_FORMATANDO_FORM          ; LCD <- mem. formatada!
	call _BEEP
		 
	_M_DELAY_MILLISECONDS 3000
	
		    
	rjmp  MENU_SERV_LOOP

MNU_SERV_ITEM_BAIXAR_TUDO:
   
//	rjmp  MENU_SERV_LOOP

MNU_SERV_ITEM_SAIR:

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
	


MENU_SERV_ITEM1:
    .DB "FORMATAR MEMORIA",_NC,0
MENU_SERV_ITEM2:
	.DB "BAIXAR MEM.INTEIRA",_NC,0
MENU_SERV_ITEM3:
	.DB "SAIR",0,_NC



		