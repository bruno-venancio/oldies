 
_APAGA_PONTO:

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
		 
_APAGA_PNT_INI:

		// Inicia na posicao 1 //
		ldi     Acc,5
		sts     _GUARDA_YL,Acc                          ;guarda posical Y

		call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
		movw	TempH:Temp,AccH:Acc
	
APAGA_PNT_LOOP:

		ldi		Acc,_INVERT							    ;Set White Logical Mode
		call	_SSD1306_Set_Operator			
		ldi		Acc,1								    
		call	_SSD1306_Set_Text_Scale			
		call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
		call	_SSD1306_Cls

		call	fn_ShowScreenBorder						;show border
	
		; MENU ITEM 1
		ldiw	Z,MSG_APAGA_PNT_ITEM1*2	   //apaga ponto              
		call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
		movw	XH:XL,AccH:Acc
		ldiw	Y,5
		call	_SSD1306_Locate							;locate msg
		call	_SSD1306_PutSF
	

		; MENU ITEM 2
		ldiw	Z,MSG_APAGA_PNT_ITEM2*2	    // modifica vel                    
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
APAGA_PNT_KEY:

		call _KEYBOARD_READ_STATIC

		call _BUZZER_ON
		_M_DELAY_MILLISECONDS 20
		call _BUZZER_OFF
		
APAGA_PNT_VER_KEY1:
		cpi Acc,_KEY_ENTER
		breq APAGA_PNT_VER_KEY_ENTER
			// DOWN
		lds Acc,_GUARDA_YL
		ldi AccH,13
		add Acc,AccH

		cpi Acc,32
		brlo APAGA_PNT_VER_L1
		ldi Acc,5
APAGA_PNT_VER_L1:
		sts _GUARDA_YL,Acc
		rjmp APAGA_PNT_LOOP
		
		// ENTER
APAGA_PNT_VER_KEY_ENTER:
     
		lds Acc,_GUARDA_YL
		cpi Acc,5
		breq APAGA_SELEC_ITEM1
	
		cpi Acc,18
		breq APAGA_SELEC_ITEM2

		cpi Acc,31
		call _BEEP
		breq APAGA_PNT_ITEM_SAIR
	
		rjmp  APAGA_PNT_LOOP

APAGA_SELEC_ITEM1:
		rcall APAGA_WAYPOINT

		ldi Acc,'A'
		sts _VAR_TELA,Acc
 		call TELA_PONTO

		call _BEEP	
		_M_DELAY_MILLISECONDS 2500
	
		rjmp APAGA_PNT_ITEM_SAIR

APAGA_SELEC_ITEM2:
		call _MODIFICA_PONTO 
		call MOD_VEL_PONTO

		ldi Acc,'M'
		sts _VAR_TELA,Acc
 		call TELA_PONTO

		call _BEEP
		_M_DELAY_MILLISECONDS 2500
	
APAGA_PNT_ITEM_SAIR:

        _M_DELAY_MILLISECONDS 100

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
	


MSG_APAGA_PNT_ITEM1:
    .DB "APAGA PONTO",0
MSG_APAGA_PNT_ITEM2:
	.DB "MODIFICA VEL",_NC,0
MSG_APAGA_PNT_ITEM3:
	.DB "SAIR",0,_NC
           
        
APAGA_WAYPOINT:
    
         ;Carrega LATITUDE DA SRAM

         ldiw X,_SRAM_PONTOS       ; Aponta para buffer de pontos na SRAM
         call _RODA_DPTR_SRAM2     ; Faz DPTR +(CONTADOR8 * 11)
     
         ld Acc,X+	 
         sts _TEMP_VAR_LAT_H,Acc

         ld Acc,X+	 
         
         sts _TEMP_VAR_LAT_MH,Acc

         ld Acc,X+	 

         sts _TEMP_VAR_LAT_ML,Acc

         ld Acc,X+	 

         sts _TEMP_VAR_LAT_L,Acc

        ;Carrega LONGITUDE DA SRAM

		 ld Acc,X+	 

         sts _TEMP_VAR_LON_H,Acc

         ld Acc,X+

		 sts _TEMP_VAR_LON_MH,Acc
         
         ld Acc,X+

		 sts _TEMP_VAR_LON_ML,Acc

         ld Acc,X

         sts _TEMP_VAR_LON_L,Acc
		 
         ldi Acc,0
		 sts _CONTADOR16_L,Acc
         sts _CONTADOR16_H,Acc
	   
APAGA_PARDAL_CONTINUA:

         ldi Acc,0
         sts _BUFFER_BYTE,Acc
         ldiw X,0

         call _RODA_ADDRESS

APAGA_PARDAL_CONTINUA_LOOP:

        ; Carrega LATITUDE (32BITS)

         call _AT45_MAIN_PAGE_READ
         sts _LATITUDE_H,Acc

		 lds AccH,_TEMP_VAR_LAT_H
		 cp Acc,AccH
		 lbrne APAGA_INC_LOOP
       ;-
         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
		 sts _LATITUDE_MH,Acc

		 lds AccH,_TEMP_VAR_LAT_MH

		 cp Acc,AccH
		 lbrne APAGA_INC_LOOP
		   
       ;-
         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
         sts _LATITUDE_ML,Acc

		 lds Acch,_TEMP_VAR_LAT_ML

		 cp Acc,AccH
	     lbrne APAGA_INC_LOOP

       ;-
         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ

         sts _LATITUDE_L,Acc

		 lds Acch,_TEMP_VAR_LAT_L

		 cp Acc,AccH
		 lbrne APAGA_INC_LOOP
		 
         call _INCREMENTA_ENDERECO

       ; Carrega LONGITUDE (32BITS)

         call _AT45_MAIN_PAGE_READ
         sts _lONGITUDE_H,Acc

		 lds AccH,_TEMP_VAR_LON_H

		 cp Acc,AccH
		 lbrne APAGA_INC_LOOP

         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
         sts _lONGITUDE_MH,Acc

		 lds AccH,_TEMP_VAR_LON_MH

		 cp Acc,AccH
		 lbrne APAGA_INC_LOOP
		 
       ;-
         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
         sts _lONGITUDE_ML,Acc

		 lds AccH,_TEMP_VAR_LON_ML

		 cp Acc,AccH
		 lbrne APAGA_INC_LOOP

       ;-
         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
         sts _lONGITUDE_L,Acc

		 lds AccH,_TEMP_VAR_LON_L

		 cp Acc,AccH
		 lbrne APAGA_INC_LOOP
		 
         ; Se o ponto foi encontrado, apaga
         ldi Acc,0
         sts _BUFFER_BYTE,Acc
         ldiw X,0

         call _RODA_ADDRESS

         call _AT45_MAIN_PAGE_TO_BUFFER1_TRANSF

         ;GRAVA LATITUDE
         ldi Acc,0xff
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         ldi Acc,0xff 
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         ldi Acc,0xff 
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         ldi Acc,0xff 
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         ;GRAVA LONGITUDE
         ldi Acc,0xff  
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         ldi Acc,0xff 
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         ldi Acc,0xff 
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         ldi Acc,0xff 
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
   
         call _INCREMENTA_ENDERECO   ; aponta para o direcao_h
         ldi Acc,0
         call _AT45_MAIN_PAGE_WRITE_B1

		 ; Apaga da SRAM
		
		 ldix _SRAM_PONTOS			 ; Aponta para buffer de pontos na SRAM
		 call _RODA_DPTR_SRAM2       ; Faz DPTR +(CONTADOR8 * 11)

         ldi Acc,0
		 ldi AccT,8

APAGA_SRAM_INTERNA:
         
		 st X+,Acc
		
		 subi AccT,1
		 tst AccT
		 brne APAGA_SRAM_INTERNA
		
		; ldi Acc,0
		; sts _CONTADOR8,Acc
		; ldi Acc,0
		 ;sts _RAM_PONTOS_CONT,Acc

		 ;Prepara nova carga Sram
		 ldi Acc,23
		 sts _GPS_TICKS_COUNT,Acc
		
		 RET
		 
APAGA_INC_LOOP:
       
	     lds Acc,_CONTADOR16_L
		 ldi AccH,1
		 add Acc,AccH
         sts _CONTADOR16_L,Acc

         lds Acc,_CONTADOR16_H
		 ldi AccH,0
         adc Acc,AccH
         sts _CONTADOR16_H,Acc

         lds Acc,_CONTADOR16_L
         lds AccH,_CONT_PONTOS_L
         cp Acc,AccH

         lbrne APAGA_PARDAL_CONTINUA

         lds Acc,_CONTADOR16_H
         lds AccH,_CONT_PONTOS_H
         cp Acc,AccH

         lbrne APAGA_PARDAL_CONTINUA

		 call _BEEP
		 call _BEEP
		 call _BEEP
		 call _BEEP
		  
         ret






;
;

MOD_VEL_PONTO:

 ;Carrega LATITUDE DA SRAM

         ldiw X,_SRAM_PONTOS       ; Aponta para buffer de pontos na SRAM
         call _RODA_DPTR_SRAM2     ; Faz DPTR +(CONTADOR8 * 11)
     
         ld Acc,X+	 
         sts _TEMP_VAR_LAT_H,Acc

         ld Acc,X+	 
         
         sts _TEMP_VAR_LAT_MH,Acc

         ld Acc,X+	 

         sts _TEMP_VAR_LAT_ML,Acc

         ld Acc,X+	 

         sts _TEMP_VAR_LAT_L,Acc

        ;Carrega LONGITUDE DA SRAM

		 ld Acc,X+	 

         sts _TEMP_VAR_LON_H,Acc

         ld Acc,X+

		 sts _TEMP_VAR_LON_MH,Acc
         
         ld Acc,X+

		 sts _TEMP_VAR_LON_ML,Acc

         ld Acc,X

         sts _TEMP_VAR_LON_L,Acc
		 
         ldi Acc,0
		 sts _CONTADOR16_L,Acc
         sts _CONTADOR16_H,Acc
	   
MOD_PARDAL_CONTINUA:

         ldi Acc,0
         sts _BUFFER_BYTE,Acc
         ldiw X,0

         call _RODA_ADDRESS

MOD_PARDAL_CONTINUA_LOOP:

        ; Carrega LATITUDE (32BITS)

         call _AT45_MAIN_PAGE_READ
         sts _LATITUDE_H,Acc

		 lds AccH,_TEMP_VAR_LAT_H
		 cp Acc,AccH
		 lbrne MOD_INC_LOOP
       ;-
         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
		 sts _LATITUDE_MH,Acc

		 lds AccH,_TEMP_VAR_LAT_MH

		 cp Acc,AccH
		 lbrne MOD_INC_LOOP
		   
       ;-
         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
         sts _LATITUDE_ML,Acc

		 lds Acch,_TEMP_VAR_LAT_ML

		 cp Acc,AccH
		 lbrne MOD_INC_LOOP

       ;-
         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ

         sts _LATITUDE_L,Acc

		 lds Acch,_TEMP_VAR_LAT_L

		 cp Acc,AccH
		 lbrne MOD_INC_LOOP
		 
         call _INCREMENTA_ENDERECO

       ; Carrega LONGITUDE (32BITS)

         call _AT45_MAIN_PAGE_READ
         sts _lONGITUDE_H,Acc

		 lds AccH,_TEMP_VAR_LON_H

		 cp Acc,AccH
		 lbrne MOD_INC_LOOP

         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
         sts _lONGITUDE_MH,Acc

		 lds AccH,_TEMP_VAR_LON_MH

		 cp Acc,AccH
		 lbrne MOD_INC_LOOP
		 
       ;-
         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
         sts _lONGITUDE_ML,Acc

		 lds AccH,_TEMP_VAR_LON_ML

		 cp Acc,AccH
		 lbrne MOD_INC_LOOP

       ;-
         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
         sts _lONGITUDE_L,Acc

		 lds AccH,_TEMP_VAR_LON_L

		 cp Acc,AccH
		 lbrne MOD_INC_LOOP
		 
         ; Se o ponto foi encontrado, modifica

         ldi Acc,8   // APONTA VELOCIDADE
         sts _BUFFER_BYTE,Acc
         ldiw X,0

         call _RODA_ADDRESS

         call _AT45_MAIN_PAGE_TO_BUFFER1_TRANSF

         ;GRAVA LATITUDE
         lds Acc,_VELOCIDADE
         call _AT45_MAIN_PAGE_WRITE_B1
         
		 RET


MOD_INC_LOOP:
       
	     lds Acc,_CONTADOR16_L
		 ldi AccH,1
		 add Acc,AccH
         sts _CONTADOR16_L,Acc

         lds Acc,_CONTADOR16_H
		 ldi AccH,0
         adc Acc,AccH
         sts _CONTADOR16_H,Acc

         lds Acc,_CONTADOR16_L
         lds AccH,_CONT_PONTOS_L
         cp Acc,AccH

         lbrne MOD_PARDAL_CONTINUA

         lds Acc,_CONTADOR16_H
         lds AccH,_CONT_PONTOS_H
         cp Acc,AccH

         lbrne MOD_PARDAL_CONTINUA
 
		 call _BEEP
		 call _BEEP
  		 call _BEEP
		 call _BEEP

         ret