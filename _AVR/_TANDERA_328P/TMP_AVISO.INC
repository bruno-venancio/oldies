
;
;
_TEMPO_AVISO_CFG:
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
	
TEMPO_LOOP:

    ldi		Acc,_INVERT							    ;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,2								    ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
	call	_SSD1306_Cls

	call	fn_ShowScreenBorder						;show border
	
	; MENU ITEM 1
	ldiw	Z,MSG_TEMPO_AVISO*2	                    ;LCD <- TEMPO AVISO
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,5
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF

	; + -
	ldiw    X,111
	ldiw    Y,14
	call	_SSD1306_Locate							;locate msg

	ldi Acc,'+'
	call _SSD1306_PutC
	

	; TEMPO DE CHEGADA 
	
	lds Acc,_TEMPO_CHEGADA
	ldi AccH,0
    ldi AccT,0
	ldi AccTH,0
	
	call    _ULTOS                                  ;Converte em ASCII
	
 	ldiw    X,31
	ldiw	Y,8
					
	call	_SSD1306_Locate							;locate msg

	ldiw    Z,_DF_STR+9  
	
TEMPO_LEFT_ZERO:
    ld Acc,Z
	cpi Acc,'0'
	brne TEMPO_LEFT_L1
    ldi Acc,' '
	st Z,Acc

TEMPO_LEFT_L1:

	call	_SSD1306_PutS
	
    ldiw	Z,MSG_TEMPO_SEG*2	                    ;LCD <- SEGUNDOS
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	ldiw    X,60
	ldiw	Y,8
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
						
	call _SSD1306_Refresh

/*
+------------+
| LE TECLADO |
+------------+
*/
TEMPO_KEY:

    call _KEYBOARD_READ_STATIC

	push Acc
	
	call _BUZZER_ON
	_M_DELAY_MILLISECONDS 20
	call _BUZZER_OFF
	
	pop Acc
		
TEMPO_VER_KEY1:
	cpi Acc,_KEY_ENTER
	brne TEMPO_VER_KEY_UP
	
	rjmp TEMPO_VER_KEY_ENTER
	
TEMPO_VER_KEY_UP:
   
	// UP
	lds Acc,_TEMPO_CHEGADA
	ldi AccH,1
	add Acc,AccH

	cpi Acc,21
	brlo TEMPO_VER_L1
	ldi Acc,11
TEMPO_VER_L1:
	sts _TEMPO_CHEGADA,Acc
	rjmp TEMPO_LOOP
	
    // ENTER
TEMPO_VER_KEY_ENTER:
    
	ldi AccH,LOW(_EE_TEMPO_CHEGADA)
	ldi AccT,HIGH(_EE_TEMPO_CHEGADA)
    lds Acc,_TEMPO_CHEGADA
	call _EEPROM_WRITE 

	call _BEEP
	
TEMPO_ITEM_SAIR:

        _M_DELAY_MILLISECONDS 1500
	      
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


MSG_TEMPO_AVISO:
  .DB "TEMPO AVISO",0

MSG_TEMPO_SEG:
  .DB "SEG",0 