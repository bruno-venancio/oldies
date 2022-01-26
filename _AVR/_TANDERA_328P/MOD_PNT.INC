
;
;
_MODIFICA_PONTO:
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

	cli
	call _INT0_DISABLE
	call _BUZZER_OFF
		
MODIF_LOOP:

    ldi		Acc,_INVERT							    ;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,1								    ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
	call	_SSD1306_Cls

	call	fn_ShowScreenBorder						;show border
	
	; MENU ITEM 1
	ldiw	Z,MSG_MODIF_VEL*2	                    ;LCD <- MOD. VEL
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
		
	;km/h

	ldiw	Z,MARCA_ITEM2*2	                        ;LCD <- KM/H
	
	ldiw    X,82
	ldiw	Y,18
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
		
	ldi		Acc,2								    ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale	

	; VELOCIDADE 
	
	lds Acc,_VELOCIDADE
	ldi AccH,0
    ldi AccT,0
	ldi AccTH,0

	call    _ARREDONDA_VEL

	call    _ULTOS                                  ;Converte em ASCII
	
 	ldiw    X,44
	ldiw	Y,13
					
	call	_SSD1306_Locate							;locate msg

	ldiw    Z,_DF_STR+8  
	
MODIF_LEFT_ZERO:
    ld Acc,Z
	cpi Acc,'0'
	brne MODIF_LEFT_L1
    ldi Acc,' '
	st Z,Acc

MODIF_LEFT_L1:

	call	_SSD1306_PutS
					
	call _SSD1306_Refresh

/*
+------------+
| LE TECLADO |
+------------+
*/
MODIF_KEY:

    sbis _INT0_PORT_INPUT,_INT0_BIT
	rjmp MODIF_KEY_CONT

	lds Acc,_GPS_TICKS_COUNT
	cpi Acc,8
	breq MODIF_ITEM_SAIR
	 
	rjmp MODIF_KEY
		
MODIF_KEY_CONT:
    call _KEYBOARD_READ_STATIC

	push Acc
	call _BUZZER_ON
	_M_DELAY_MILLISECONDS 20
	call _BUZZER_OFF
	pop Acc

MODIF_VER_KEY1:
	cpi Acc,_KEY_ENTER
	breq MODIF_VER_KEY_ENTER

// UP
	ldi Acc,0
	sts _GPS_TICKS_COUNT,Acc

	lds Acc,_VELOCIDADE
	ldi AccH,6
	add Acc,AccH

	cpi Acc,66
	brlo MODIF_VER_L1
	ldi Acc,6
MODIF_VER_L1:
	sts _VELOCIDADE,Acc
	rjmp MODIF_LOOP

    // ENTER
MODIF_VER_KEY_ENTER:
    
	call _BEEP
		

MODIF_ITEM_SAIR:

        _M_DELAY_MILLISECONDS 1500

		call _INT0_INI
			      
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


MSG_MODIF_VEL:
  .DB "MOD.VEL",0