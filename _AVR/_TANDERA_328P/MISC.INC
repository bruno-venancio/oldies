;
;
;
_RODA_ADDRESS: 
        //
		//SAVE SREG
		//
		push	Acc
		in		Acc,SREG
		//
		//SAVE REGISTERS
		//
		_M_PUSH_LOWER_REGS						;save r0..r15
		;_M_PUSH_UPPER_REGS						;save r16..r31
        push AccT

		lds Acc,_CONTADOR16_L
		mov R0,Acc

		lds Acc,_CONTADOR16_H
		mov R1,Acc

		ldi Acc,11
		mov R2,Acc

		ldi Acc,0
		mov R3,Acc

		call _MUL_U16U16U32
		
		clc
		mov Acc,R4
		lds AccT,_BUFFER_BYTE
		add Acc,AccT
		sts _BUFFER_BYTE,Acc

		mov Acc,R5
		adc Acc,XL
		mov XL,Acc

		mov Acc,R6
		adc Acc,XH
		mov XH,Acc
		    
		//
		//RESTORE REGISTERS
		//
		pop AccT
		;_M_POP_UPPER_REGS						;restore r16..r31
		_M_POP_LOWER_REGS
		//
		//RESTORE SREG
		//
		out		SREG,Acc
		pop		Acc

		ret



;
; Roda DPTR para Rotina principal
; Faz DPTR <- DPTR +(CONTADOR8 * 11)
;

_RODA_DPTR_SRAM:

        //
     	//SAVE SREG
		//
		push	Acc
		in		Acc,SREG
		//
		//SAVE REGISTERS
		//
		_M_PUSH_LOWER_REGS						;save r0..r15
	;	_M_PUSH_UPPER_REGS						;save r16..r31
		push AccT


		 lds Acc,_RAM_PONTOS_CONT
         ldi AccT,11

         mul Acc,AccT
		
         clc                   ; agora adiciona RESULTADO com x
         add R0,YL
         mov YL,R0
		          
         adc R1,YH
         mov YH,R1

        //
		//RESTORE REGISTERS
		//
		pop AccT
	;	_M_POP_UPPER_REGS						;restore r16..r31
		_M_POP_LOWER_REGS
		//
		//RESTORE SREG
		//
		out		SREG,Acc
		pop		Acc

        ret






;
; Roda DPTR para Rotina principal
; Faz DPTR <- DPTR +(CONTADOR8 * 11)
;

_RODA_DPTR_SRAM2:

        //
     	//SAVE SREG
		//
		push	Acc
		in		Acc,SREG
		//
		//SAVE REGISTERS
		//
		_M_PUSH_LOWER_REGS						;save r0..r15
		;_M_PUSH_UPPER_REGS						;save r16..r31
		 push AccT

		 lds Acc,_CONTADOR8
         ldi AccT,11

         mul Acc,AccT
		
         clc                   ; agora adiciona RESULTADO com x
         add R0,XL
         mov XL,R0
		          
         adc R1,XH
         mov XH,R1

        //
		//RESTORE REGISTERS
		//
		pop AccT
		;_M_POP_UPPER_REGS						;restore r16..r31
		_M_POP_LOWER_REGS
		//
		//RESTORE SREG
		//
		out		SREG,Acc
		pop		Acc

        ret


/*
  +--------------------------+ 
  | inicializa interrupcao 0 |
  +--------------------------+
*/
_INT0_ENABLE:
	 
	    in Acc,EIMSK
		ori Acc,(1<<INT0) 
        out EIMSK,Acc

        lds Acc,EICRA
		ori Acc,(1<<ISC01)
		andi Acc,~(1<<ISC00)
		sts EICRA,Acc  
		
		ret



/*
  +--------------------------+ 
  | inicializa interrupcao 0 |
  +--------------------------+
*/
_INT0_INI:
	 	cbi  _INT0_PORT_DIR,_INT0_BIT       ; BIT como entrada
		sbi _INT0_PORT_OUTPUT,_INT0_BIT
		
	    in Acc,EIMSK
		ori Acc,(1<<INT0) 
        out EIMSK,Acc

        lds Acc,EICRA
		ori Acc,(1<<ISC01)
		andi Acc,~(1<<ISC00)
		sts EICRA,Acc  
		
		ret
/*
  +------------------------+ 
  | Habilita interrupcao 1 |
  +------------------------+

_INT1_INI:
	 	cbi  _INT1_PORT_DIR,_INT1_BIT       ; BIT como entrada
		sbi _INT1_PORT_OUTPUT,_INT1_BIT
		
	    in Acc,EIMSK
		ori Acc,(1<<INT1) 
        out EIMSK,Acc

        lds Acc,EICRA
		ori Acc,(1<<ISC11)
		andi Acc,~(1<<ISC10)
		sts EICRA,Acc  
		
		ret
*/

/*
  +--------------------------+ 
  | Desabilita interrupcao 0 |
  +--------------------------+
*/
_INT0_DISABLE:
	 	push Acc		
	    in Acc,EIMSK
		andi Acc,~(1<<INT0) 
        out EIMSK,Acc
		pop Acc

		ret
/*
//  +--------------------------+ 
//  | Desabilita interrupcao 0 |
//  +--------------------------+

_INT1_DISABLE:
	 	push Acc		
	    in Acc,EIMSK
		andi Acc,~(1<<INT1) 
        out EIMSK,Acc
		pop Acc

		ret
*/
xoxo:
ret

;
;
_TESTA_MEMORIA:

        LDIX 0
		ldi AccT,22
        sts _BUFFER_BYTE,AccT
 
        call _AT45_MAIN_PAGE_READ
		 			
		swap Acc
		
        mov AccT,Acc

        andi Acc,0b11110000 

        swap Acc
		           
        call _HEX_TO_ASCII
		
		cpi Acc,'0'
		brne TESTA_ERRO_SAI

        mov Acc,AccT

        andi Acc,0b00001111

        call _HEX_TO_ASCII

		cpi Acc,'0'
		brne TESTA_ERRO_SAI
        
		call _INCREMENTA_ENDERECO

        call _AT45_MAIN_PAGE_READ
		
		call _HEX_TO_ASCII
		
		cpi Acc,'5'
		brne TESTA_ERRO_SAI

        call _AT45_MAIN_PAGE_READ

		andi Acc,0b11110000 

        swap Acc
		
		call _HEX_TO_ASCII
		
		cpi Acc,'9'
		brne TESTA_ERRO_SAI
        
        _CLEAR _FLAG_TESTA_OK
        
        RET

TESTA_ERRO_SAI:

        _SET _FLAG_TESTA_OK
        
        RET
        

        
_RL: 
       bst r0,7
       rol r0
       bld r0,0
	   RET
_RR:
       bst r0,0
       ror r0
       bld r0,7
	   RET



;    |---------------------------------------------------|
;  -=| Rotina de coversao de HEX-> ASCII (somente NIBBLE)|=-
;    |---------------------------------------------------|

_HEX_TO_ASCII:
            push AccT

            andi    Acc,0x0F           ; Filtra byte para passar somente 1 nibble
            cpi     Acc,0x0A
			brlo    HAVal09            ; Valor de 0 a 9.

            ldi AccT,7                 ; valor e de A a F, entao da um ajuste extra
			add Acc,AccT
			
HAVal09:
            ldi AccT,'0'
			add Acc,AccT

			pop AccT
			ret





;  _ASC_TO_HEX - See if char in ACC is ASCII-hex and if so convert to a hex nibble.
;   Returns nibble in A, HexFlag tells if char was really hex. The ACC is not
;   altered if the character is not ASCII hex. Upper and lower case letters
;   are recognized.

_ASC_TO_HEX:
            cpi      Acc,'0'            ; Test for ASCII numbers.

            brlo       AHBad            ; Is character is less than a '0'?
		
            cpi      Acc,'9'+1          ; Test value range. 
			brlo     AHVal09            ; Is character is between '0' and '9'?  
          
            cpi      Acc,'A'            ; Test for upper case hex letters.
			brlo     AHBad              ; Is character is less than an 'A'?

			cpi      Acc,'F'+1          ; Test value range.
			brlo     AHValAF            ; Is character is between 'A' and 'F'?
            
			cpi      Acc,'a'            ; Test for lower case hex letters.
			brlo     AHBad              ; Is character is less than an 'a'? 

			cpi      Acc,'f'+1          ; Test value range. 
			brge     AHBad              ; Is character is between 'a' and 'f'?
                    
                  
            clc
            subi     Acc,0x27           ; Pre-adjust character to get a value.
            rjmp     AHVal09            ; Now treat as a number.

AHBad:
            _CLEAR   _HexFlag           ; Flag char as non-hex, don't alter.
            rjmp      AHEX              ; Exit
AHValAF:
            clc
            subi     Acc,7              ; Pre-adjust character to get a value.
AHVal09:
            clc
            subi     Acc,'0'            ; Adjust character to get a value.
            _SET    _HexFlag            ; Flag character as 'good' hex.
AHEX:
            ret




/*


      PUSH ACC

	    call _USART_SET_NORMAL_SPEED
        LDIAW 4800
        call _USART_SET_BAUD_RATE
	
		ldi Acc,_OFF
		call _USART_TX_INT_ENABLE
				
        ldi Acc,_ON
		call _USART_TX_ENABLE
        POP ACC
       
debug:
      push Acc
	  ldi Acc,'-'
	  call _USART_TX
      pop Acc
      
	  push acc
	   swap Acc
	  
      call _HEX_TO_ASCII
	  call _USART_TX

	    pop acc

	  

      call _HEX_TO_ASCII
	  call _USART_TX
	  RET

debug_para:
      jmp debug_para



        LDI ACC,0
	   MOV AccTH,ACC
		MOV AccT,XH
		MOV AccH,XL
		lds Acc,_BUFFER_BYTE      ; LSB
debuga:		
		call _ULTOS
		ldiz _DF_STR
xuxuta:		
		ld Acc,Z+
		call _USART_TX

		cpi Acc,0
		brne xuxuta

jmpa:   
        jmp jmpa




dede:		
		  
 ldi TempH,11
UPLOAD_LX:
		
       ldix 0
	   ldi acc,0
	   sts _BUFFER_BYTE,acc

	   call _RODA_ADDRESS
	  
UPLOAD_CONTINUA_LOOP:

       call _AT45_MAIN_PAGE_READ
	   
	   swap Acc
	  
       mov TEMP,Acc

	   swap Acc

	   call _HEX_TO_ASCII
	   call _USART_TX

	   mov Acc,TEMP

	   
	   call _HEX_TO_ASCII
	   call _USART_TX
	   
	   call _INCREMENTA_ENDERECO

	   dec TempH
	   tst TempH
	   brne UPLOAD_CONTINUA_LOOP
	   
	   ldi TempH,11

	  ; call _CPULED_ALTERNATE

       lds Acc,_CONTADOR16_L
	   ldi AccT,1
	   add Acc,AccT
	   sts _CONTADOR16_L,Acc

	   lds Acc,_CONTADOR16_H
	   ldi AccT,0
	   adc Acc,AccT
	   sts _CONTADOR16_H,Acc


	   lds Acc,_CONTADOR16_H
	   lds AccT,_CONT_PONTOS_H

	   cp Acc,AccT

	   brne UPLOAD_LX
     
	   lds Acc,_CONTADOR16_L
	   lds AccT,_CONT_PONTOS_L

	   cp Acc,AccT

	   brne UPLOAD_LX
  

ddd:
;      call _CPULED2_ON
	   jmp ddd
*/

;
;  Entrada em knots e saida em km/h
;
_ARREDONDA_VEL:
               
         ;Testa se eh menor que 20Km/h
		 push AccH 
             
         ldi AccH,11
         cp Acc,AccH

         brge ARREDONDA_40KM
         
         ; If < or = do:

         ldi Acc,20
         rjmp ARREDONDA_VEL_SAIDA

ARREDONDA_40KM:

         
         ldi AccH,22
         cp Acc,AccH
         brge ARREDONDA_50KM
		 
         ; If < ou = do:

         ldi Acc,40
         rjmp ARREDONDA_VEL_SAIDA

ARREDONDA_50KM:

         ldi AccH,28
         cp Acc,AccH
         brge ARREDONDA_60KM
		 
		 ; If < ou = do:

         ldi Acc,50
         rjmp ARREDONDA_VEL_SAIDA

ARREDONDA_60KM:

         ldi AccH,33
         cp Acc,AccH
         brge ARREDONDA_70KM
		          
         ; If < ou = do:

         ldi Acc,60
         rjmp ARREDONDA_VEL_SAIDA

ARREDONDA_70KM:

         ldi AccH,39
         cp Acc,AccH
         brge ARREDONDA_80KM
		 
         ; If < ou = do:

         ldi Acc,70
         rjmp ARREDONDA_VEL_SAIDA


ARREDONDA_80KM:

         ldi AccH,44
         cp Acc,AccH
         brge ARREDONDA_90KM

         ; If < ou = do:

         ldi Acc,80
         rjmp ARREDONDA_VEL_SAIDA


ARREDONDA_90KM:

         ldi AccH,49
         cp Acc,AccH
         brge ARREDONDA_100KM
		 
         ; If < ou = do:

         ldi Acc,90
         rjmp ARREDONDA_VEL_SAIDA

ARREDONDA_100KM:
         ldi AccH,53
         cp Acc,AccH
         brge ARREDONDA_110KM
         ldi Acc,100

		 rjmp ARREDONDA_VEL_SAIDA
ARREDONDA_110KM:
         ldi AccH,64
         cp Acc,AccH
         brge ARREDONDA_120KM
         ldi Acc,110

		 rjmp ARREDONDA_VEL_SAIDA

ARREDONDA_120KM:
         ldi Acc,120 
ARREDONDA_VEL_SAIDA:

         pop AccH

         RET




;
;  Entrada em knots e saida em knots
;
_ARREDONDA_VEL2:
               
         ;Testa se eh menor que 20Km/h
		 push AccH 
             
         ldi AccH,11
         cp Acc,AccH

         brge ARREDONDA_40KM2
         
         ; If < or = do:

         ldi Acc,16
         rjmp ARREDONDA_VEL_SAIDA2

ARREDONDA_40KM2:

         
         ldi AccH,22
         cp Acc,AccH
         brge ARREDONDA_50KM2
		 
         ; If < ou = do:

         ldi Acc,21
         rjmp ARREDONDA_VEL_SAIDA2

ARREDONDA_50KM2:

         ldi AccH,28
         cp Acc,AccH
         brge ARREDONDA_60KM2
		 
		 ; If < ou = do:

         ldi Acc,27
         rjmp ARREDONDA_VEL_SAIDA2

ARREDONDA_60KM2:

         ldi AccH,33
         cp Acc,AccH
         brge ARREDONDA_70KM2
		          
         ; If < ou = do:

         ldi Acc,32
         rjmp ARREDONDA_VEL_SAIDA2

ARREDONDA_70KM2:

         ldi AccH,39
         cp Acc,AccH
         brge ARREDONDA_80KM2
		 
         ; If < ou = do:

         ldi Acc,38
         rjmp ARREDONDA_VEL_SAIDA2


ARREDONDA_80KM2:

         ldi AccH,44
         cp Acc,AccH
         brge ARREDONDA_90KM2

         ; If < ou = do:

         ldi Acc,43
         rjmp ARREDONDA_VEL_SAIDA2


ARREDONDA_90KM2:

         ldi AccH,49
         cp Acc,AccH
         brge ARREDONDA_100KM2
		 
         ; If < ou = do:

         ldi Acc,48
         rjmp ARREDONDA_VEL_SAIDA2

ARREDONDA_100KM2:

         ldi AccH,54
         cp Acc,AccH
         brge ARREDONDA_110KM2

         ldi Acc,53
		 rjmp ARREDONDA_VEL_SAIDA2

ARREDONDA_110KM2:
         ldi AccH,59
         cp Acc,AccH
         brge ARREDONDA_120KM2 
		 ldi Acc,59
		 rjmp ARREDONDA_VEL_SAIDA2

ARREDONDA_120KM2:
         ldi AccH,64
         cp Acc,AccH
         brge ARREDONDA_ZERA 
		 ldi Acc,64
		 rjmp ARREDONDA_VEL_SAIDA2

ARREDONDA_ZERA:
         ldi Acc,0

ARREDONDA_VEL_SAIDA2:
         pop AccH

         RET









 
;
;
;
_ENVIA_PONTO_CARDEAL:

         lds AccTH,_DIRECAO_H
         lds AccT,_DIRECAO_L

		 lds Acc,_FLAG_CARRO_PARADO
		 cpi Acc,_ON
		 brne CARDEAL_NORTE

		 ldi AccTH,' '
		 ldi AccT,' '
         jmp CARDEAL_SAIDA


CARDEAL_NORTE:

         ldi TempH,0
         ldi Temp,31

         cp AccT,Temp
		 cpc AccTH,TempH

		 brge CARDEAL_NORDESTE
		 
PONTO_NORTE:
          
		 ldi AccTH,'N'
		 ldi AccT,' '
         jmp CARDEAL_SAIDA

         ;---

CARDEAL_NORDESTE:

         ldi TempH,0
         ldi Temp,32

         cp AccT,Temp
		 cpc AccTH,TempH
		 		  
         brge CARDEAL_NORDESTE_2

         rjmp PONTO_NORDESTE

CARDEAL_NORDESTE_2:

         lds AccTH,_DIRECAO_H
         lds AccT,_DIRECAO_L

         ldi TempH,0
         ldi Temp,61

         cp AccT,Temp
		 cpc AccTH,TempH

         brge CARDEAL_LESTE

PONTO_NORDESTE:

         ldi AccTH,'N'
		 ldi AccT,'E'
         jmp CARDEAL_SAIDA

         ;----
CARDEAL_LESTE:

         lds AccTH,_DIRECAO_H
         lds AccT,_DIRECAO_L

         ldi TempH,0
         ldi Temp,62

         cp AccT,Temp
		 cpc AccTH,TempH

         brge CARDEAL_LESTE_2

         rjmp PONTO_LESTE

CARDEAL_LESTE_2:

         lds AccTH,_DIRECAO_H
         lds AccT,_DIRECAO_L

         ldi TempH,0
         ldi Temp,106

         cp AccT,Temp
		 cpc AccTH,TempH

         brge CARDEAL_SUDESTE

PONTO_LESTE:

         ldi AccTH,'L'
		 ldi AccT,' '
         jmp CARDEAL_SAIDA

         ;---
CARDEAL_SUDESTE:

         lds AccTH,_DIRECAO_H
         lds AccT,_DIRECAO_L

         ldi TempH,0
         ldi Temp,107

         cp AccT,Temp
		 cpc AccTH,TempH

         brge CARDEAL_SUDESTE_2

         rjmp PONTO_SUDESTE

CARDEAL_SUDESTE_2:

         lds AccTH,_DIRECAO_H
         lds AccT,_DIRECAO_L

         ldi TempH,0
         ldi Temp,151

         cp AccT,Temp
		 cpc AccTH,TempH

         brge CARDEAL_SUL

PONTO_SUDESTE:

         ldi AccTH,'S'
		 ldi AccT,'E'
         rjmp CARDEAL_SAIDA


         ;---

CARDEAL_SUL:

         lds AccTH,_DIRECAO_H
         lds AccT,_DIRECAO_L

         ldi TempH,0
         ldi Temp,152

         cp AccT,Temp
		 cpc AccTH,TempH

         brge CARDEAL_SUL_2
         
         rjmp PONTO_SUL

CARDEAL_SUL_2:

         lds AccTH,_DIRECAO_H
         lds AccT,_DIRECAO_L

         ldi TempH,0
         ldi Temp,196

         cp AccT,Temp
		 cpc AccTH,TempH

         brge CARDEAL_SUDOESTE

PONTO_SUL:

         ldi AccTH,'S'
		 ldi AccT,' '
         rjmp CARDEAL_SAIDA
         
         ;---

CARDEAL_SUDOESTE:

         lds AccTH,_DIRECAO_H
         lds AccT,_DIRECAO_L

         ldi TempH,0
         ldi Temp,197

         cp AccT,Temp
		 cpc AccTH,TempH

         brge CARDEAL_SUDOESTE_2

         rjmp PONTO_SUDOESTE

CARDEAL_SUDOESTE_2:

         lds AccTH,_DIRECAO_H
         lds AccT,_DIRECAO_L

         ldi TempH,0
         ldi Temp,241

         cp AccT,Temp
		 cpc AccTH,TempH

         brge CARDEAL_OESTE

PONTO_SUDOESTE:

         ldi AccTH,'S'
		 ldi AccT,'O'
         rjmp CARDEAL_SAIDA
		 
  ;---

CARDEAL_OESTE:

         lds AccTH,_DIRECAO_H
         lds AccT,_DIRECAO_L

         ldi TempH,0
         ldi Temp,242

         cp AccT,Temp
		 cpc AccTH,TempH

         brge CARDEAL_OESTE_2

         rjmp PONTO_OESTE

CARDEAL_OESTE_2:

         lds AccTH,_DIRECAO_H
         lds AccT,_DIRECAO_L

         ldi TempH,high(301)
         ldi Temp,low(301)

         cp AccT,Temp
		 cpc AccTH,TempH

         brge CARDEAL_NOROESTE

PONTO_OESTE:

         ldi AccTH,'O'
		 ldi AccT,' '
         rjmp CARDEAL_SAIDA


  ;---

CARDEAL_NOROESTE:

         lds AccTH,_DIRECAO_H
         lds AccT,_DIRECAO_L

         ldi TempH,high(302)
         ldi Temp,low(302)

         cp AccT,Temp
		 cpc AccTH,TempH

         brge CARDEAL_NOROESTE_2

         rjmp PONTO_NOROESTE

CARDEAL_NOROESTE_2:

         lds AccTH,_DIRECAO_H
         lds AccT,_DIRECAO_L

         ldi TempH,high(331)
         ldi Temp,low(331)

         cp AccT,Temp
		 cpc AccTH,TempH
		 
         brge CARDEAL_NORTE2

PONTO_NOROESTE:

         ldi AccTH,'N'
		 ldi AccT,'O'
         rjmp CARDEAL_SAIDA
  ;---

CARDEAL_NORTE2:
        
        ldi AccTH,'N'
		ldi AccT,' '
         
        
CARDEAL_SAIDA:

         RET








// DOIS BEEPS CURTOS
_BEEP:
		push	Acc
		in		Acc,SREG
		//
		//SAVE REGISTERS
		//
		_M_PUSH_LOWER_REGS						;save r0..r15
		 
         call _BUZZER_ALTERNATE
		 _M_DELAY_MILLISECONDS 200
         call _BUZZER_ALTERNATE
		 _M_DELAY_MILLISECONDS 200
         call _BUZZER_ALTERNATE
		 _M_DELAY_MILLISECONDS 200
         call _BUZZER_ALTERNATE

         //
	     //RESTORE REGISTERS
    	 //
		 _M_POP_LOWER_REGS
		 //
		 //RESTORE SREG
		 //
		 out		SREG,Acc
		 pop		Acc

         ret

// DOIS BEEPS CURTOS
_BEEP2:
		push	Acc
		in		Acc,SREG
		//
		//SAVE REGISTERS
		//
		_M_PUSH_LOWER_REGS						;save r0..r15
		 
         call _BUZZER_ALTERNATE
		 _M_DELAY_MILLISECONDS 20
         call _BUZZER_ALTERNATE
		 _M_DELAY_MILLISECONDS 20
         call _BUZZER_ALTERNATE
		 _M_DELAY_MILLISECONDS 20
         call _BUZZER_ALTERNATE

         //
	     //RESTORE REGISTERS
    	 //
		 _M_POP_LOWER_REGS
		 //
		 //RESTORE SREG
		 //
		 out		SREG,Acc
		 pop		Acc

         ret

;
;  APRESENTA UM FRASE PARA USUARIO AO INICIAR O SYSTEMA
;
_FRASE_INICIAL:

    //
	//SAVE SREG
	//
	push	Acc
    in		Acc,SREG

    _M_PUSH_LOWER_REGS						        ;save r0..r15
    _M_PUSH_UPPER_REGS						        ;save r16..r31
	

    ldi		Acc,_WHITE							    ;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,1								    ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
	call	_SSD1306_Cls

	;call	fn_ShowScreenBorder						;show border
	
	
	ldiw	Z,MSG_FRASE_INICIAL*2	                        
	;call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	;movw	XH:XL,AccH:Acc
	ldiw    X,5
	ldiw	Y,5
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
	call    _SSD1306_Refresh

	ldi AccH,LOW(_EE_FRASE_TEMPO)
    ldi AccT,HIGH(_EE_FRASE_TEMPO)
	call _EEPROM_READ   
	
	cpi Acc,0xFF
	
	brne FRASE_TEMPO_TESTA    

	ldi Acc,3

FRASE_TEMPO_TESTA:
     cpi Acc,7
	 brlo FRASE_TEMPO_L1
	 
	 ldi Acc,7

FRASE_TEMPO_L1:
     
    push Acc
    _M_DELAY_MILLISECONDS 1000
	pop Acc

	dec Acc
	brne FRASE_TEMPO_L1


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


MSG_FRASE_INICIAL:
   
   .DB " * Use o cinto  ",_CR,_LF,_CR,_LF
   .DB "  * Boa viagem!  ",0











/*
;
;  APRESENTA UM FRASE PARA USUARIO AO INICIAR O SYSTEMA
;
_FRASE_CONFIRMACAO_FORMATAR:

    //
	//SAVE SREG
	//
	push	Acc
    in		Acc,SREG

    _M_PUSH_LOWER_REGS						        ;save r0..r15
    _M_PUSH_UPPER_REGS						        ;save r16..r31
	

    ldi		Acc,_WHITE							    ;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,1								    ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
	call	_SSD1306_Cls
			
	
	ldiw	Z,MSG_FRASE_FORMATAR*2	                        
	
	ldiw    X,5
	ldiw	Y,5
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
	call    _SSD1306_Refresh

	call _KEYBOARD_READ_STATIC

	;cpi Acc,_KEY_UP
	;brne FRASE_FMT_SAIDA

	call _APAGA_tUDO

FRASE_FMT_SAIDA:

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


MSG_FRASE_FORMATAR:
   
   .DB " TODA A MEMORIA ",_CR,_LF,_CR,_LF
   .DB " SERA APAGADA!  ",_CR,_LF,_CR,_LF
   .DB " CONFIRMA (-/+)? ",0
*/
;
; 
;
/*
FRASE_FORMATANDO_FORM:

    push	Acc
    in		Acc,SREG

    _M_PUSH_LOWER_REGS						        ;save r0..r15
    _M_PUSH_UPPER_REGS						        ;save r16..r31
	

    ldi		Acc,_WHITE							    ;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,1								    ;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
	call	_SSD1306_Cls

	lds     Acc,_VAR_TELA
	cpi     Acc,'1'
	brne FRASE_SELEC2 
	ldiw	Z,MSG_FRASE_FORMATANDO*2	                        
	rjmp FRASE_SELEC_SAIDA

FRASE_SELEC2:
	ldiw	Z,MSG_FRASE_FORMATADO*2	
			
FRASE_SELEC_SAIDA:	
	ldiw    X,5
	ldiw	Y,5
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
	call    _SSD1306_Refresh
	

FRASE_FMTADA_SAIDA:

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


MSG_FRASE_FORMATANDO:
   .DB "FORMATANDO...",0

MSG_FRASE_FORMATADO:
   .DB "MEM.FORMATADA!",0,_NC


   */





;
; TELAS PARA INFORMACAO DE PONTO MARCADO, APAGADO E MODIFICADO
;
TELA_PONTO:
         
    ldi		Acc,_WHITE							;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,2								;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			;Set Screen Normal 
	call	_SSD1306_Cls
	//
	//draw start message
	//
	ldiw	Z,MSG_PONTO_TELA1*2	 
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,_SSD1306_LCD_HEIGHT/2-_SSD1306_FONT_HEIGHT*2
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
    
	push    Acc
	lds     Acc,_VAR_TELA
	cpi		Acc,'A'
	breq    TELA_MENS1
	cpi     Acc,'M'
	breq    TELA_MENS2
	cpi     Acc,'I'
	breq    TELA_MENS3
		
TELA_MENS1:
    ldiw    Z,MSG_PONTO_APAGADO*2
	rjmp TELA_MENS_PULA
TELA_MENS2:
    ldiw    Z,MSG_PONTO_MODIFICADO*2
	rjmp TELA_MENS_PULA

TELA_MENS3:
    ldiw    Z,MSG_PONTO_MARCADO*2 

TELA_MENS_PULA:
    pop Acc
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,_SSD1306_LCD_HEIGHT/2
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
	;call	fn_ShowScreenBorder						;show border
	call    _SSD1306_Refresh
	
	ret

MSG_PONTO_MARCADO:
	.DB "MARCADO!",0,_NC

MSG_PONTO_MODIFICADO:
	.DB "MODIFICADO",0,_NC

MSG_PONTO_APAGADO:
	.DB "APAGADO!",0,_NC

MSG_PONTO_TELA1:
    .DB "PONTO",0



	 

 

_GRAVA_PONTOS_FABRICA:

        ldi AccH,low(_EE_CONT_PONTOS_L)
		ldi AccT,high(_EE_CONT_PONTOS_L)
		ldi Acc,low(21616)
		call _EEPROM_WRITE

		ldi AccH,low(_EE_CONT_PONTOS_H)
		ldi AccT,high(_EE_CONT_PONTOS_H)
		ldi Acc,high(21616)
		call _EEPROM_WRITE
		
		ldi Acc,low(21616)
		sts _CONT_PONTOS_L,Acc

		ldi Acc,high(21616)
		sts _CONT_PONTOS_H,Acc

		RET


;
; Mostra quantidade de pontos na tela
;
_tela_qty_pontos:

    ldi		Acc,_WHITE							    ;Set White Logical Mode
	call	_SSD1306_Set_Operator			
 	ldi		Acc,1								    ;Set Scale to 2
 	call	_SSD1306_Set_Text_Scale			
 	call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	 
	call	_SSD1306_Cls

	call	fn_ShowScreenBorder						;show border
	
    ldiw	Z,MSG_GPS_INF_QTY*2	                      
    ldiw    X,5
	ldiw	Y,8
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF	
		   	   

 // N. PONTOS
		
	ldiw    Z,_DF_STR                               ; Aponta para o Buffer
    ldiw    X,66
	ldiw	Y,8
	call	_SSD1306_Locate							; locate msg
	
	ldi Acc,0
	mov AccTH,Acc
	mov AccT,Acc
	lds AccH,_CONT_PONTOS_H
	lds Acc,_CONT_PONTOS_L      ; LSB
    
	call _ULTOS
    
	ldiw    Z,_DF_STR+6                             ; Aponta para o Buffer
	call	_SSD1306_PutS
		
	call _SSD1306_Refresh

	ret

	MSG_GPS_INF_QTY:
		.DB "QT.PONTOS:",0,_NC



.EQU	_KEY_NONE	= 0X00
.EQU	_KEY_ENTER	= 0X01
.EQU    _KEY_TIMEOUT = 0X02

;
; one key keyboard
;
_KEYBOARD_READ_STATIC:
        push Temp

		ldi Acc,0
		sts _GPS_TICKS_COUNT,Acc

        ldi Temp,0

_KEYBOARD_READ_10:  
        lds Acc,_GPS_TICKS_COUNT
	    cpi Acc,20 
		breq _KEYBOARD_READ_EXIT_TIMEOUT
	
	    sbic _INT0_PORT_INPUT,_INT0_BIT
		rjmp _KEYBOARD_READ_10

		call _BUZZER_ON

_KEYBOARD_READ_20:
        _M_DELAY_MILLISECONDS 60
		inc Temp
		cpi Temp,6
		brlo _KEYBOARD_READ_20_CONT
		
		call _BUZZER_OFF

_KEYBOARD_READ_20_CONT:
		sbis _INT0_PORT_INPUT,_INT0_BIT
		rjmp _KEYBOARD_READ_20

		call _BUZZER_OFF

		cpi Temp,5
		brge _KEYBOARD_READ_30
		
		ldi Acc,_KEY_NONE
		rjmp _KEYBOARD_READ_EXIT

_KEYBOARD_READ_30:
        ldi Acc,_KEY_ENTER
        
_KEYBOARD_READ_EXIT:
        pop Temp

        RET



_KEYBOARD_READ_EXIT_TIMEOUT:
        
		ldi Acc,_KEY_TIMEOUT
		pop Temp
		ret


     
 ;
 ;
 _BUZZER_TEST:

        _M_DELAY_MILLISECONDS 10

	    sbic _INT0_PORT_INPUT,_INT0_BIT
	  ;  jmp MAIN_INI_CONT

	    ldi		Acc,_WHITE							    ;Set White Logical Mode
		call	_SSD1306_Set_Operator			
		ldi		Acc,2								    ;Set Scale to 2
		call	_SSD1306_Set_Text_Scale			
		call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
		call	_SSD1306_Cls
	
		// LCD <- ERRO DE MEMORIA!
		ldiw	Z,DISP_TEST*2	                      
		ldiw    X,13
		ldiw	Y,16
		call	_SSD1306_Locate							;locate msg
		call	_SSD1306_PutSF
        call    _SSD1306_Refresh


BUZZER_T1:

       ldi TempH,5
buz_l1:
      call _BUZZER_ON
	  _M_DELAY_MILLISECONDS 400
	  call _BUZZER_OFF

	  _M_DELAY_MILLISECONDS 100

	  call _BUZZER_ON
	  _M_DELAY_MILLISECONDS 100
	  call _BUZZER_OFF

	  _M_DELAY_MILLISECONDS 100

	  call _BUZZER_ON
	  _M_DELAY_MILLISECONDS 100
	  call _BUZZER_OFF

	  dec TempH
	  tst TempH
	  lbrne buz_l1

      ldi TempH,20

buz_l2:
      call _BUZZER_ON
	  _M_DELAY_MILLISECONDS 10
	  call _BUZZER_OFF

	  _M_DELAY_MILLISECONDS 50

	  call _BUZZER_ON
	  _M_DELAY_MILLISECONDS 10
	  call _BUZZER_OFF

	  _M_DELAY_MILLISECONDS 50

	  call _BUZZER_ON
	  _M_DELAY_MILLISECONDS 10
	  call _BUZZER_OFF

	  dec TempH
	  tst TempH
	  lbrne buz_l2

	  jmp BUZZER_T1
     

DISP_TEST:
      .DB "TESTANDO...",0

	  ;
;
;
_COMPARA_DATA_EE:

        LDIW  Y,_SRAM_DATA

        ldi AccH,low(_EE_PONTUACAO_DATA)
		ldi AccT,high(_EE_PONTUACAO_DATA)
		call _EEPROM_READ
		ld Temp,Y+
		cp Acc,Temp
		brne COMPARA_DATA_EE_ERRO
		
        ldi AccH,low(_EE_PONTUACAO_DATA+1)
		ldi AccT,high(_EE_PONTUACAO_DATA+1)
		call _EEPROM_READ
		ld Temp,Y+
		cp Acc,Temp
		brne COMPARA_DATA_EE_ERRO

		adiw YH:YL,1
        ldi AccH,low(_EE_PONTUACAO_DATA+2)
		ldi AccT,high(_EE_PONTUACAO_DATA+2)
		call _EEPROM_READ
		ld Temp,Y+
		cp Acc,Temp
		brne COMPARA_DATA_EE_ERRO

        ldi AccH,low(_EE_PONTUACAO_DATA+3)
		ldi AccT,high(_EE_PONTUACAO_DATA+3)
		call _EEPROM_READ
		ld Temp,Y
		cp Acc,Temp
		brne COMPARA_DATA_EE_ERRO

		_CLEAR _FLAG_ERRO
		RET
		
		
COMPARA_DATA_EE_ERRO:
        _SET _FLAG_ERRO
		RET

