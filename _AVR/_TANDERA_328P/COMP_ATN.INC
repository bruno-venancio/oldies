;
;
;
_LOAD_DELTA_XY:
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

        ; Calcula SRAM_PONTO - LATITUDE  resultado em X2
	    ; Carrega variaveis de LATITUDE

		lds Acc,_LATITUDE_L
		sts _XL,Acc

		lds Acc,_LATITUDE_ML
		sts _XML,Acc

		lds Acc,_LATITUDE_MH
		sts _XMH,Acc
		 
		lds Acc,_LATITUDE_H
		sts _XH,Acc
				 
		; +---------------------------+
        ; | CARREGA LATITUDE DA SRAM  |
        ; +---------------------------+
		
		LDIX _SRAM_PONTOS             ; Aponta para buffer de pontos na SRAM
		call _RODA_DPTR_SRAM2         ; Faz DPTR +(CONTADOR8 * 11)

		ld Acc,X+
		sts _SRAM_PONTO_H,Acc

		ld Acc,X+
		sts _SRAM_PONTO_MH,Acc

		ld Acc,X+
		sts _SRAM_PONTO_ML,Acc

		ld Acc,X
		sts _SRAM_PONTO_L,Acc 
		
SUBTRAI_CPL_X:

        ; Subtrai em 32 bits
		clc

		lds Acc,_SRAM_PONTO_L
		lds AccT,_XL
		sub Acc,AccT
		sts _XL,Acc

		lds Acc,_SRAM_PONTO_ML
		lds AccT,_XML
		sbc Acc,AccT
		sts _XML,Acc

		lds Acc,_SRAM_PONTO_MH 
		lds AccT,_XMH
		sbc Acc,AccT
		sts _XMH,Acc

        lds Acc,_SRAM_PONTO_H
		lds AccT,_XH
		sbc Acc,AccT
		sts _XH,Acc
		
		; inverte
		
		lds Acc,_XL
		com Acc
		sts _XL,Acc

		lds Acc,_XML
		com Acc
		sts _XML,Acc

		lds Acc,_XMH
		com Acc
		sts _XMH,Acc

		lds Acc,_XH
		com Acc
		sts _XH,Acc

		; add +1
        lds Acc,_XL
		ldi AccT,1
		add Acc,AccT
		sts _XL,Acc

		lds Acc,_XML
        ldi AccT,0
		adc Acc,AccT
		sts _XML,Acc

		lds Acc,_XMH
		ldi AccT,0
		adc Acc,AccT
		sts _XMH,Acc

		lds Acc,_XH
		ldi AccT,0
		adc Acc,AccT
		sts _XH,Acc
				
AGORA_Y:
        ; Carrega variaveis de LONGITUDE

		lds Acc,_LONGITUDE_L
		sts _YL,Acc

		lds Acc,_LONGITUDE_ML
		sts _YML,Acc

		lds Acc,_LONGITUDE_MH
		sts _YMH,Acc

		lds Acc,_LONGITUDE_H
		sts _YH,Acc

        ; +---------------------------+
        ; | CARREGA LONGITUDE DA SRAM |
        ; +---------------------------+

		LDIX _SRAM_PONTOS+4           ; Aponta para buffer de pontos na SRAM
		call _RODA_DPTR_SRAM2         ; Faz DPTR +(CONTADOR8 * 11)

		ld Acc,X+
		sts _SRAM_PONTO_H,Acc

		ld Acc,X+
		sts _SRAM_PONTO_MH,Acc

		ld Acc,X+
		sts _SRAM_PONTO_ML,Acc

		ld Acc,X
		sts _SRAM_PONTO_L,Acc

SUBTRAI_CPL_Y:
        ; Subtrai em 32 bits
		clc
		lds Acc,_SRAM_PONTO_L
		lds AccT,_YL
		sub Acc,AccT
		sts _YL,Acc

		lds Acc,_SRAM_PONTO_ML
		lds AccT,_YML
		sbc Acc,AccT
		sts _YML,Acc

		lds Acc,_SRAM_PONTO_MH
		lds AccT,_YMH
    	sbc Acc,AccT
		sts _YMH,Acc

		lds Acc,_SRAM_PONTO_H
		lds AccT,_YH
		sbc Acc,AccT
		sts _YH,Acc

		; inverte
		
		lds Acc,_YL
		com Acc
		sts _YL,Acc

		lds Acc,_YML
		com Acc
		sts _YML,Acc

		lds Acc,_YMH
		com Acc
		sts _YMH,Acc

		lds Acc,_YH
		com Acc
		sts _YH,Acc

		; add +1
        lds Acc,_YL
		ldi AccT,1
		add Acc,AccT
		sts _YL,Acc

		lds Acc,_YML
        ldi AccT,0
		adc Acc,AccT
		sts _YML,Acc

		lds Acc,_YMH
		ldi AccT,0
		adc Acc,AccT
		sts _YMH,Acc

		lds Acc,_YH
		ldi AccT,0
		adc Acc,AccT
		sts _YH,Acc
				
SUBTRAI_SAIDA:
		        		
		//
		//RESTORE REGISTERS
		//
		;_M_POP_UPPER_REGS						;restore r16..r31
		_M_POP_LOWER_REGS
		//
		//RESTORE SREG
		//
		out		SREG,Acc
		pop		Acc

		ret

		     
;
;
;
_COMPARA_ATAN:

        lds Temp,_DIR_COMP_L                ; Primeiro Guarda DIR_COMP
		lds TempH,_DIR_COMP_H

		; INCREMENTA O DIRCOMP EM 30 ( 16 BITS)
 
        clc

		lds Acc,_DIR_COMP_L
		ldi AccT,12
		add Acc,AccT
		sts _DIR_COMP_L,Acc

		lds Acc,_DIR_COMP_H
		ldi AccT,0
		adc Acc,AccT
		sts _DIR_COMP_H,Acc

		; Testa se da mais que 360

		lds Acc,_DIR_COMP_H
		ldi AccT,HIGH(360)

		cp Acc,AccT

		breq ATN_TESTA_360_L
		brsh ATN_TESTA_360_MAIOR
		brlo ATN_COMP1_DIR_H           ; CONTINUA NORMALMENTE

ATN_TESTA_360_L:

	    lds Acc,_DIR_COMP_L
		ldi AccT,LOW(360)

		cp Acc,AccT

		breq ATN_COMP1_DIR_H           ; CONTINUA NORMALMENTE
		brsh ATN_TESTA_360_MAIOR
		brlo ATN_COMP1_DIR_H           ; CONTINUA NORMALMENTE

ATN_TESTA_360_MAIOR:
        ; PEGA DIRCOMP(> 360) - 360
		clc
		lds Acc,_DIR_COMP_L
		ldi AccT,low(360)
		sub Acc,AccT
		sts _DIR_COMP_L,Acc

		lds Acc,_DIR_COMP_H
		ldi AccT,high(360)
		sbc Acc,AccT
		sts _DIR_COMP_H,Acc

		; Compara DIRECAO com DIR_COMP
ATN_COMP1_DIR_H:
        lds Acc,_DIRECAO_H
		lds AccT,_DIR_COMP_H

		cp Acc,AccT

		breq ATN_COMP1_DIR_L
		brsh ATN_TESTA_MAIOR_30H
		brlo ATN_COMP_CONTINUA

ATN_COMP1_DIR_L:
	    lds Acc,_DIRECAO_L
		lds AccT,_DIR_COMP_L

		cp Acc,AccT

		breq ATN_COMP_CONTINUA
		brsh ATN_TESTA_MAIOR_30H
		brlo ATN_COMP_CONTINUA

;---------------------
ATN_TESTA_MAIOR_30H:
        
		mov Acc,TEMPH             ; DIR_COMP_H
		ldi AccT,high(12)

		cp Acc,AccT

		breq ATN_TESTA_MAIOR_30L
		brsh ATN_TESTA_MAIOR_330H
		brlo ATN_COMP_CONTINUA

ATN_TESTA_MAIOR_30L:
	    mov Acc,TEMP              ; DIR_COMP_L
		ldi AccT,low(12)

		cp Acc,AccT

		breq ATN_COMP_CONTINUA
		brsh ATN_TESTA_MAIOR_330H
		brlo ATN_COMP_CONTINUA

ATN_TESTA_MAIOR_330H:
        mov Acc,TEMPH             ; DIR_COMP_H
		ldi AccT,high(348)

		cp Acc,AccT

		breq ATN_TESTA_MAIOR_330L
		brsh ATN_COMP_CONTINUA
		lbrlo ATN_COMP_ERRO2

ATN_TESTA_MAIOR_330L:

	    mov Acc,TEMP              ; DIR_COMP_L
		ldi AccT,low(348)

		cp Acc,AccT

		lbrlo ATN_COMP_ERRO2

;---------------------
ATN_COMP_CONTINUA:
		; DECREMENTA O DIRCOMP EM 30 ( 16 BITS)
        ; DIR_COMP <- DIR_COMP - 60
		clc
		lds Acc,_DIR_COMP_L
		ldi AccT,24
		sub Acc,AccT
		sts _DIR_COMP_L,Acc

		lds Acc,_DIR_COMP_H
		ldi AccT,0
		sbc Acc,AccT
		sts _DIR_COMP_H,Acc

        ;*****************
        brcc ATN_COMP2_DIR_H
        ;*****************
	    ;Se deu carry, a subtracao deu < 0
	    _SET _FLAG_MENOR_ZERO

		clc
		; Complementa 1 em 16 bits
		ldi Acc,low(0xffff)
		lds AccT,_DIR_COMP_L
		sub Acc,AccT
		sts _DIR_COMP_L,Acc

		ldi Acc,high(0xffff)
		lds AccT,_DIR_COMP_H
		sbc Acc,AccT
		sts _DIR_COMP_H,Acc

        ;-- Guarda Diferenca

        lds Acc,_DIR_COMP_L
        sts _ACIMA_360,Acc

		;---
		clc
		ldi Acc,low(360)
		lds AccT,_DIR_COMP_L
		sub Acc,AccT
		sts _DIR_COMP_L,Acc

		ldi Acc,high(360)
		lds AccT,_DIR_COMP_H
		sbc Acc,AccT
		sts _DIR_COMP_L,Acc
		
ATN_COMP2_DIR_H:

        lds Acc,_DIRECAO_H
		lds AccT,_DIR_COMP_H
		 
		cp Acc,AccT
		 
		breq ATN_COMP2_DIR_L
		brsh ATN_COMP_OK
		brlo ATN_COMP_ERRO

ATN_COMP2_DIR_L:
        
        lds Acc,_DIRECAO_L
		lds AccT,_DIR_COMP_L

		cp Acc,AccT

		brsh ATN_COMP_OK 

ATN_COMP_ERRO:
        
		lds Acc,_FLAG_MENOR_ZERO 

		cpi Acc,_ON
		brne ATN_COMP_ERRO2

		; Agora testa a sobra que vem depois do 360
		clc
		ldi Acc,24
		lds AccT,_ACIMA_360
		sub Acc,AccT
		sts _ACIMA_360,Acc

		lds Acc,_DIRECAO_L
		lds AccT,_ACIMA_360

		cp Acc,AccT

		breq ATN_COMP_OK
		brlo ATN_COMP_OK

ATN_COMP_ERRO2:

        _SET _FLAG_ANG_ERRO
		jmp ATN_COMP_DIR_SAIDA

ATN_COMP_OK:
        _CLEAR _FLAG_ANG_ERRO
		  
ATN_COMP_DIR_SAIDA:
        _CLEAR _FLAG_MENOR_ZERO

		RET







        

		  
         








		 














