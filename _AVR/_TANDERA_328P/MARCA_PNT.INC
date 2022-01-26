
_MARCA_PONTO:
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

	;cli
	call _INT0_DISABLE
	call _BUZZER_OFF
	
	ldi Acc,0
	sts _GPS_TICKS_COUNT,Acc
	
	
MARCA_LOOP:

    _M_DELAY_MILLISECONDS 80  
    
	 ldi		Acc,_INVERT							    ;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,1								    ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
	call	_SSD1306_Cls

	call	fn_ShowScreenBorder						;show border
	
	; MENU ITEM 1
	ldiw	Z,MARCA_ITEM1*2	                        ;LCD <- MARCAR
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,4
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF

	; +  
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
	
MARCA_LEFT_ZERO:
    ld Acc,Z
	cpi Acc,'0'
	brne MARCA_LEFT_L1
    ldi Acc,' '
	st Z,Acc

MARCA_LEFT_L1:
	call	_SSD1306_PutS
	call _SSD1306_Refresh

/*
+------------+
| LE TECLADO |
+------------+
*/
MARCA_KEY:

   	sbis _INT0_PORT_INPUT,_INT0_BIT
	rjmp MARCA_KEY_CONT

	lds Acc,_GPS_TICKS_COUNT
	cpi Acc,7
	breq MARCA_ITEM_SAIR
	 
	rjmp MARCA_KEY

MARCA_KEY_CONT:
    call _KEYBOARD_READ_STATIC

	push Acc
	call _BUZZER_ON
	_M_DELAY_MILLISECONDS 20
	call _BUZZER_OFF
	pop Acc
		
MARCA_VER_KEY1:
	cpi Acc,_KEY_ENTER
	breq MARCA_VER_KEY_ENTER

	// UP
	ldi Acc,0
	sts _GPS_TICKS_COUNT,Acc

	lds Acc,_VELOCIDADE
	ldi AccH,6
	add Acc,AccH

	cpi Acc,66
	brlo MARCA_VER_L1
	ldi Acc,6
MARCA_VER_L1:
	sts _VELOCIDADE,Acc
	rjmp MARCA_LOOP
	
    // ENTER
MARCA_VER_KEY_ENTER:

    cli
     
	call GRAVA_WAYPOINT
	
	ldi Acc,'I'
    sts _VAR_TELA,Acc
 	call TELA_PONTO

    call _BEEP
	
MARCA_ITEM_SAIR:

        _M_DELAY_MILLISECONDS 1500

		call _INT0_INI
		sei
			      
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
	


MARCA_ITEM1:
    .DB "MARCAR",_NC,0
MARCA_ITEM2:
	.DB "km/h",0,_NC


  
;
;
GRAVA_WAYPOINT:

         lds Acc,_CONT_PONTOS_H     ; Faz ENDERECO <- ( CRD_COUNTER * 11)
         mov R3,Acc

         lds Acc,_CONT_PONTOS_L
         mov r2,Acc

         ldi Acc,11
		 mov R0,Acc

		 ldi Acc,0
		 mov R1,Acc
         
         call _MUL_U16U16U32

         ; agora adiciona RESULTADO com dptr
         mov Acc,R4
         sts _BUFFER_BYTE,Acc

         mov Acc,R5
         mov XL,Acc

         mov Acc,R6
         mov XH,Acc
                  
GRAVA_LOOP:

         call _AT45_MAIN_PAGE_TO_BUFFER1_TRANSF

         ;GRAVA LATITUDE
         lds Acc,_LATITUDE_H
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         lds Acc,_LATITUDE_MH

         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         lds Acc,_LATITUDE_ML

         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         lds Acc,_LATITUDE_L
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         ;GRAVA LONGITUDE
         lds Acc,_LONGITUDE_H
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         lds Acc,_LONGITUDE_MH
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         lds Acc,_LONGITUDE_ML
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         lds Acc,_LONGITUDE_L
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
		 lds Acc,_VELOCIDADE
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO

         lds Acc,_DIRECAO_H

		 ;sbr Acc,7                    ; Seta o bit MSB para indicar que é ponto do usuário
         ori Acc,0b10000000

         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO

         lds Acc,_DIRECAO_L
         call _AT45_MAIN_PAGE_WRITE_B1

         ; Incrementa CONT_PONTOS
		         
         lds Acc,_CONT_PONTOS_L
         ldi AccH,1
		 add Acc,AccH
		 sts _CONT_PONTOS_L,Acc

         lds Acc,_CONT_PONTOS_H
		 ldi AccH,0
		 adc Acc,AccH
         sts _CONT_PONTOS_H,Acc
		        
		; Salva CONT_PONTOS na EEPROM
        
         ldi AccH,low(_EE_CONT_PONTOS_L)
         ldi AccT,high(_EE_CONT_PONTOS_L)
    	 lds Acc,_CONT_PONTOS_L
    	 call _EEPROM_WRITE

         ldi AccH,low(_EE_CONT_PONTOS_H)
         ldi AccT,high(_EE_CONT_PONTOS_H)
   	     lds Acc,_CONT_PONTOS_H
	     call _EEPROM_WRITE
		 
	     ret
 


