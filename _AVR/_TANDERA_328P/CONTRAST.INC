//_SSD1306_Set_Contrast:


 
;
;
_CONTRAST_CFG:
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
	
CONTRAST_LOOP:

		ldi		Acc,_INVERT							;Set White Logical Mode
		call	_SSD1306_Set_Operator	 		
		ldi		Acc,1								;Set Scale to 2
		call	_SSD1306_Set_Text_Scale			
		call	_SSD1306_Set_Display_Normal			;Set Screen Normal 
		call	_SSD1306_Cls
   
		ldiw	Z,MSG_CONTRAST*2	                    ;LCD <- VOLUME
		call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
 		movw	XH:XL,AccH:Acc
 		ldiw	Y,5

		call	_SSD1306_Locate							;locate msg
 		call	_SSD1306_PutSF
		
		ldiw	x,8
		ldiw	y,15
		call	_SSD1306_Set_Progress_Bar_Coords		;bar start coords

		ldiw	x,_SSD1306_LCD_WIDTH-8*2
		ldiw	y,14
		call	_SSD1306_Set_Progress_Bar_Sizes			;Bar Sizes

		ldiw	x,0										
		ldiw	y,10
		call	_SSD1306_Set_Progress_Bar_Range			;Bar Range
		ldiaw	0
		ldiawt	0
	 
		call ADJUST_CONTRAST
		mov Acc,R0
		ldi AccH,0							 
		call	_SSD1306_Set_Progress_Bar_Value			;set value
		call	_SSD1306_Draw_Progress_Bar				;draw bar
	
		; CONTRASTE
	 
	    call ADJUST_CONTRAST
		mov Acc,R0
	
		ldi AccH,0
		ldi AccT,0
		ldi AccTH,0
	
		call    _ULTOS                                  ;Converte em ASCII
	
		ldiw	x,54
		ldiw	y,18
		call	_SSD1306_Locate							;set value location
	
		ldiw    Z,_DF_STR+8  
	
CONTRAST_LEFT_ZERO:
		ld Acc,Z
		cpi Acc,'0'
		brne CONTRAST_LEFT_L1
		ldi Acc,' '
		st Z,Acc

CONTRAST_LEFT_L1:

		call _SSD1306_PutS
	 					
		call _SSD1306_Refresh

/*
+------------+
| LE TECLADO |
+------------+
*/
CONTRAST_KEY:

		call _KEYBOARD_READ_STATIC

		push Acc
		call _BUZZER_ON
		_M_DELAY_MILLISECONDS 20
		call _BUZZER_OFF
		pop Acc
		
CONTRAST_VER_KEY1:
		cpi Acc,_KEY_NONE
		brne CONTRAST_VER_KEY_ENTER
	
		// UP
		lds Acc,_VAR_CONTRAST
		ldi AccH,85
		add Acc,AccH
		brcc CONTRAST_VER_L1
		ldi Acc,85
CONTRAST_VER_L1:
		sts _VAR_CONTRAST,Acc
		call _SSD1306_Set_Contrast
		rjmp CONTRAST_LOOP
	
		// ENTER
CONTRAST_VER_KEY_ENTER:
    
		ldi AccH,LOW(_EE_CONTRAST)
		ldi AccT,HIGH(_EE_CONTRAST)
		lds Acc,_VAR_CONTRAST
		call _EEPROM_WRITE 

		call _BEEP

CONTRAST_ITEM_SAIR:
        
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


MSG_CONTRAST:
		.DB "CONTRASTE",0

  

ADJUST_CONTRAST:
        ; R0 = VAR_CONTAST * 10 / 255

        lds R0,_VAR_CONTRAST
		ldi Acc,0
		mov R1,Acc
		
		ldi Acc,10
		mov R2,Acc

		ldi Acc,0
		mov R3,Acc

		call _MUL_U16U16U32
		
		mov R0,R4
		mov R1,R5

		ldi Acc,255
		mov R2,Acc

		ldi Acc,0
		mov R3,Acc

		call _DIV_U16U16U16
					
		ret
		 
		  