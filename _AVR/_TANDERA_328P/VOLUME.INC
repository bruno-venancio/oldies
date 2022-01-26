
 
;
;
_VOLUME_CFG:

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
		
VOLUME_LOOP:
		;  call _TIMER0_PWM_ON
		
		ldi		Acc,_INVERT							;Set White Logical Mode
		call	_SSD1306_Set_Operator	 		
		ldi		Acc,1								;Set Scale to 2
		call	_SSD1306_Set_Text_Scale			
		call	_SSD1306_Set_Display_Normal			;Set Screen Normal 
		call	_SSD1306_Cls
   
		ldiw	Z,MSG_VOLUME*2	                    ;LCD <- VOLUME
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
	 
		lds		Acc,_FLAG_VOLUME
		ldi		AccH,0							 
		call	_SSD1306_Set_Progress_Bar_Value			;set value
		call	_SSD1306_Draw_Progress_Bar				;draw bar
	
		; VOLUME
	 
		lds Acc,_FLAG_VOLUME
	
		ldi AccH,0
		ldi AccT,0
		ldi AccTH,0
	
		call    _ULTOS                                  ;Converte em ASCII
	
		ldiw	x,54
		ldiw	y,18
		call	_SSD1306_Locate							;set value location
	
		ldiw    Z,_DF_STR+8  
	
VOLUME_LEFT_ZERO:
		ld Acc,Z
		cpi Acc,'0'
		brne VOLUME_LEFT_L1
		ldi Acc,' '
		st Z,Acc

VOLUME_LEFT_L1:
		call _SSD1306_PutS
		call _SSD1306_Refresh
/*
+------------+
| LE TECLADO |
+------------+
*/ 
VOLUME_KEY:
		call _KEYBOARD_READ_STATIC
		 		 
		push Acc
		call _BUZZER_ON
		_M_DELAY_MILLISECONDS 20
		call _BUZZER_OFF
		pop Acc
		
VOLUME_VER_KEY1:
		cpi Acc,_KEY_NONE
		brne VOLUME_VER_KEY_ENTER
	
		// UP
		lds Acc,_FLAG_VOLUME
		ldi AccH,5
		add Acc,AccH
		cpi Acc,11
		brlo VOLUME_VER_L1
	
		cpi Acc,13
		brge VOLUME_VER_L2
		ldi Acc,10
VOLUME_VER_L1:
		sts _FLAG_VOLUME,Acc
		rjmp VOLUME_LOOP

VOLUME_VER_L2:
		ldi Acc,0
		sts _FLAG_VOLUME,Acc
		jmp VOLUME_LOOP
			
		// ENTER
VOLUME_VER_KEY_ENTER:
    
		ldi AccH,LOW(_EE_VOLUME)
		ldi AccT,HIGH(_EE_VOLUME)
		lds Acc,_FLAG_VOLUME
		call _EEPROM_WRITE 
	

		;call _TIMER0_PWM_STOP

		call _BEEP

VOLUME_ITEM_SAIR:
        
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


MSG_VOLUME:
  .DB "VOLUME",0,_NC
 
  
/*
_TIMER0_PWM_ON:

		push	Acc					;save Acc
		in		Acc,SREG			;save SREG
		pushaw
				
		//
		//Set wgm = 010 CTC mode
		in		Acc,TCCR0A
		andi	Acc,~((1<<WGM01)|(1<<WGM00))
		ori		Acc,(1<<WGM01)
		ori		Acc,(1<<WGM00)
        ori     Acc,(1<<COM0A1)   
        andi    Acc,~(1<<COM0A0)
		ori     Acc,(1<<COM0B1)
		ori     Acc,(1<<COM0B0)
		out     TCCR0A,Acc
		
		in	    Acc,TCCR0B
		ori 	Acc,~(1<<WGM02)
		//
		//Program Timer0 divisor to 1024x
		//
	  	
	  	andi	Acc,~((1<<CS02)|(1<<CS01)|(1<<CS00))
	  ;	ori		Acc,(1<<CS00)
	  	ori		Acc,(1<<CS02)

        out		TCCR0B,Acc
		//
		//Program OCR0A to generate _TIMER_INTERRUPT_FREQUENCY interrupt frequency
		//
		
	   ; lds Acc,_FLAG_VOLUME
		ldi Acc,220
		out		OCR0A,Acc
		
		ldi     Acc,0
		sts		TIMSK0,Acc   ; sem interrupcao

		popaw						;Restore SREG
		out		SREG,Acc
		pop		Acc					;restore Acc
		ret


*/
/*
_TIMER0_PWM_ON:
		push	Acc					;save Acc
		in		Acc,SREG			;save SREG
		pushaw
				
        cbi	_BUZZER_PWM_OUTPUT,_BUZZER_PWM_BIT	;Output pull up

		popaw						;Restore SREG
		out		SREG,Acc
		pop		Acc					;restore Acc
		ret

 

_TIMER0_PWM_STOP:
		push	Acc					;save Acc
		in		Acc,SREG			;save SREG
		pushaw
		
		
		//
		//Set wgm = 010 CTC mode
		in		Acc,TCCR0A
		andi	Acc,~((1<<WGM01)|(1<<WGM00))
		ori		Acc,(1<<WGM01)
		ori		Acc,(1<<WGM00)
        ori     Acc,(1<<COM0A1)   
        andi    Acc,~(1<<COM0A0)
		andi    Acc,~(1<<COM0B1)
		andi    Acc,~(1<<COM0B0)
		out     TCCR0A,Acc
		
		in	    Acc,TCCR0B
		ori 	Acc,~(1<<WGM02)
		

		popaw						;Restore SREG
		out		SREG,Acc
		pop		Acc					;restore Acc
		ret

		*/
/*

_TIMER0_PWM_STOP:
    	push	Acc					;save Acc
		in		Acc,SREG			;save SREG
		pushaw
				
        sbi	_BUZZER_PWM_OUTPUT,_BUZZER_PWM_BIT	;Output pull up

		popaw						;Restore SREG
		out		SREG,Acc
		pop		Acc					;restore Acc
		ret
*/