;
;
;
_CARREGA_SRAM:
        ldi Acc,0
		sts _RAM_PONTOS_CONT,Acc
		sts _TROCA_BANCO,Acc
		_CLEAR _FLAG_TROCA_BANCO

CARREGA_SRAM_GO:

	    ; Guarda as variaveis de latitude
		lds Acc,_LATITUDE_L
	    sts _TEMP_VAR_LAT_L,Acc

		lds Acc,_LATITUDE_ML
	    sts _TEMP_VAR_LAT_ML,Acc

		lds Acc,_LATITUDE_MH
	    sts _TEMP_VAR_LAT_MH,Acc

		lds Acc,_LATITUDE_H
	    sts _TEMP_VAR_LAT_H,Acc
								 		
		; +----------------------------+
        ; | CARREGA LATITUDE DA EEPROM |
        ; +----------------------------+

		 ldi Acc,0
		 sts _BUFFER_BYTE,Acc
		 ldix 0

		 call _RODA_ADDRESS

		 lds Acc,_CONTADOR16_L            ; = 0 ?
		 lds AccT,_CONTADOR16_H
		 add Acc,AccT

		 tst Acc
		 breq CARREGA_NORMAL


; Testa se e multiplo 23

         lds Acc,_CONTADOR16_L
         mov R0,Acc

         lds Acc,_CONTADOR16_H
         mov r1,Acc

		 ldi Acc,23     ; Divide por 23 
         mov R2,Acc

         ldi Acc,0
	     mov R3,Acc

		 call _DIV_U16U16U16
         
         ;Testa se deu resto
         
		 mov Acc,R4
		 mov AccT,R5
		 add Acc,AccT

		 tst Acc
		 brne CARREGA_NORMAL
         
		 _SET _FLAG_TROCA_BANCO

CARREGA_NORMAL:

         lds Acc,_FLAG_TROCA_BANCO
		 cpi Acc,_ON
		 lbreq CARREGA_23
		 		
        ; Carrega variavel EE_PARDAL_POINT (32BITS) com dados da eeprom

        ; acall _AT45_MAIN_PAGE_READ

         call _AT45_READ_CONTINUOUS

         call _AT45_MASTER_IO
         swap Acc   ; desembaralha o dado
		 
		 lds AccT,_LATITUDE_H
		 cp Acc,AccT
		 brne T0_PARDAL_CONT_SAI              ; Byte da eeprom eh igual ao MSB da LAT.?

         rjmp T0_PARDAL_CONT_PULA             ; Se o MSB da LATITUDE for igual,continua     

T0_PARDAL_CONT_SAI:

         rjmp CARREGA_SRAM_CONT                ; Senao, sai da interrupcao

T0_PARDAL_CONT_PULA:
       
		sts _EE_PARDAL_POINT_H,Acc

		call _AT45_MASTER_IO
		swap Acc
		
		sts _EE_PARDAL_POINT_MH,Acc

		call _AT45_MASTER_IO
		swap Acc

		sts _EE_PARDAL_POINT_ML,Acc

		call _AT45_MASTER_IO
		swap Acc

		sts _EE_PARDAL_POINT_L,Acc

     	;NOVA ROTINA ***

        call _AT45_MASTER_IO
        swap Acc   ; desembaralha o dado

		lds AccT,_LONGITUDE_H
		cp Acc,AccT
        brne T0_PARDAL_CONT_SAI

        sts _TEMP_I,Acc
								
    	; Decrementa 190 (~340m) em 32 bits
		
        lds Acc,_TEMP_VAR_LAT_L
		lds AccT,_VAR_QUAD_L
        ;subi Acc,low(7000)
		sub Acc,AccT
        sts _TEMP_VAR_LAT_L,Acc

        lds Acc,_TEMP_VAR_LAT_ML
		lds AccT,_VAR_QUAD_H
       ; sbci Acc,high(7000)
	    sbc Acc,AccT
        sts _TEMP_VAR_LAT_ML,Acc

        lds Acc,_TEMP_VAR_LAT_MH
        sbci Acc,0
        sts _TEMP_VAR_LAT_MH,Acc

        lds Acc,_TEMP_VAR_LAT_H
        sbci Acc,0
        sts _TEMP_VAR_LAT_H,Acc
		
						
;+--------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA > POSICAO LATITUDE DO CARRO - 1 |
;+--------------------------------------------------------------------+

T0_TESTA_LATITUDE1:
       
         lds Acc,_EE_PARDAL_POINT_L
		 lds AccT,_TEMP_VAR_LAT_L
         cp Acc,AccT

         lds Acc,_EE_PARDAL_POINT_ML
		 lds AccT,_TEMP_VAR_LAT_ML
         cpc Acc,AccT

         lds Acc,_EE_PARDAL_POINT_MH
		 lds AccT,_TEMP_VAR_LAT_MH
         cpc Acc,AccT

         lds Acc,_EE_PARDAL_POINT_H
		 lds AccT,_TEMP_VAR_LAT_H
         cpc Acc,AccT

         brsh T0_TESTA_LATITUDE2        ; Se latitude pardal > latitude carro -1, continua

		 rjmp CARREGA_SRAM_CONT          ; Senao, sai da interrupcao

;+--------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA < POSICAO LATITUDE DO CARRO + 1 |
;+--------------------------------------------------------------------+

T0_TESTA_LATITUDE2:

         ; CARREGA TEMP_VAR_LAT

         lds  Acc,_LATITUDE_L
         sts  _TEMP_VAR_LAT_L,Acc

		 lds Acc,_LATITUDE_ML
         sts  _TEMP_VAR_LAT_ML,Acc

		 lds Acc,_LATITUDE_MH
         sts  _TEMP_VAR_LAT_MH,Acc

		 lds Acc,_LATITUDE_H
         sts  _TEMP_VAR_LAT_H,Acc
		 
; INCREMENTA O TEMP_VAR_LAT em 190 (~340m) ( 32 BITS)

         clc
         lds Acc,_TEMP_VAR_LAT_L
		 lds AccT,_VAR_QUAD_L
		; ldi AccT,low(7000)
         add  Acc,AccT
         sts _TEMP_VAR_LAT_L,Acc

         lds Acc,_TEMP_VAR_LAT_ML
         ;ldi AccT,high(7000)
		 lds AccT,_VAR_QUAD_H
		 adc Acc,AccT
         sts _TEMP_VAR_LAT_ML,Acc

         lds Acc,_TEMP_VAR_LAT_MH
         ldi AccT,0
		 adc Acc,AccT
         sts _TEMP_VAR_LAT_MH,Acc

         lds Acc,_TEMP_VAR_LAT_H
         ldi AccT,0
		 adc Acc,AccT
         sts _TEMP_VAR_LAT_H,Acc

;
; Agora testa
;
         clc
         lds Acc,_EE_PARDAL_POINT_L
		 lds AccT,_TEMP_VAR_LAT_L
         cp Acc,AccT

         lds Acc,_EE_PARDAL_POINT_ML
         lds AccT,_TEMP_VAR_LAT_ML
		 cpc Acc,AccT

         lds Acc,_EE_PARDAL_POINT_MH
         lds AccT,_TEMP_VAR_LAT_MH
		 cpc Acc,AccT

         lds Acc,_EE_PARDAL_POINT_H
         lds AccT,_TEMP_VAR_LAT_H
		 cpc Acc,AccT
		 		 			 		 
         brlo T0_TESTA_LONGITUDE1       ; Se latitude pardal < latitude carro +1,continua

         rjmp CARREGA_SRAM_CONT          ; Senao, sai da interrupcao

T0_TESTA_LONGITUDE1:
         
         ; +-----------------------------+
         ; | CARREGA LONGITUDE DA EEPROM |
         ; +-----------------------------+
		 
        /* ldi Acc,4
		 sts _BUFFER_BYTE,Acc
         ldix 0
         call _RODA_ADDRESS

         call _AT45_MAIN_PAGE_READ
		  
		 lds AccT,_LONGITUDE_H
		 cp Acc,AccT

		 brne T0_TESTA_LONG_SAI
        */
         rjmp T0_TESTA_LONGITUDE_PULA

T0_TESTA_LONG_SAI:

         rjmp CARREGA_SRAM_CONT                   ; Sai da interrupcao

T0_TESTA_LONGITUDE_PULA:
          
		 lds Acc,_TEMP_I      
         sts _EE_PARDAL_POINT_H,Acc

         call _AT45_MASTER_IO
         swap Acc   ; desembaralha o dado

		 lds AccT,_LONGITUDE_MH

		 cp Acc,AccT

		 brne T0_TESTA_LONG_SAI   ; Se o 2o byte for diferente, sai
		 
         sts _EE_PARDAL_POINT_MH,Acc

         call _AT45_MASTER_IO
         swap Acc   ; desembaralha o dado
		 
         sts _EE_PARDAL_POINT_ML,Acc
		 
         call _AT45_MASTER_IO
         swap Acc   ; desembaralha o dado
		 
         sts _EE_PARDAL_POINT_L,Acc

		 sbi _AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	   ; END continuous
		 
; CARREGA TEMP_VAR_LON
       
		 lds  Acc,_LONGITUDE_L
         sts  _TEMP_VAR_LON_L,Acc

		 lds Acc,_LONGITUDE_ML
         sts  _TEMP_VAR_LON_ML,Acc

		 lds Acc,_LONGITUDE_MH
         sts  _TEMP_VAR_LON_MH,Acc

		 lds Acc,_LONGITUDE_H
         sts  _TEMP_VAR_LON_H,Acc
		 		 
;+---------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA > POSICAO LONGITUDE DO CARRO - 1 |
;+---------------------------------------------------------------------+

         ; Decrementa em 190 (~340m) 32 bits
		 
         lds Acc,_TEMP_VAR_LON_L
		 lds AccT,_VAR_QUAD_L
        ; subi Acc,low(7000)
		 sub Acc,AccT
		 sts _TEMP_VAR_LON_L,Acc

		 lds Acc,_TEMP_VAR_LON_ML
		 lds AccT,_VAR_QUAD_H
		 sbc Acc,AccT
		; sbci Acc,high(7000)
		 sts _TEMP_VAR_LON_ML,Acc
		 
         lds Acc,_TEMP_VAR_LON_MH
         sbci Acc,0
         sts _TEMP_VAR_LON_MH,Acc

         lds Acc,_TEMP_VAR_LON_H
         sbci Acc,0
         sts _TEMP_VAR_LON_H,Acc
	
	;  Agora testa
		 		 
         clc
         lds Acc,_EE_PARDAL_POINT_L
		 lds AccT,_TEMP_VAR_LON_L
         cp Acc,AccT

         lds Acc,_EE_PARDAL_POINT_ML
		 lds AccT,_TEMP_VAR_LON_ML
         cpc Acc,AccT

         lds Acc,_EE_PARDAL_POINT_MH
		 lds AccT,_TEMP_VAR_LON_MH
         cpc Acc,AccT

         lds Acc,_EE_PARDAL_POINT_H
		 lds AccT,_TEMP_VAR_LON_H
         cpc Acc,AccT

         brsh T0_TESTA_LONGITUDE2    ; Se longitude PARDAL > LONGITUDE CARRO -1,CONTINUA

         rjmp CARREGA_SRAM_CONT       ; Sai da interrupcao

T0_TESTA_LONGITUDE2:

; CARREGA TEMP_VAR_LON

         lds  Acc,_LONGITUDE_L
         sts  _TEMP_VAR_LON_L,Acc

		 lds Acc,_LONGITUDE_ML
         sts  _TEMP_VAR_LON_ML,Acc

		 lds Acc,_LONGITUDE_MH
         sts  _TEMP_VAR_LON_MH,Acc

		 lds Acc,_LONGITUDE_H
         sts  _TEMP_VAR_LON_H,Acc

;+--------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA < POSICAO LATITUDE DO CARRO + 1 |
;+--------------------------------------------------------------------+

; INCREMENTA O TEMP_VAR_LON 190 (~340m)( 32 BITS)

         clc
         lds Acc,_TEMP_VAR_LON_L
		 lds AccT,_VAR_QUAD_L
         ;ldi AccT,low(7000)
		 lds AccT,_VAR_QUAD_L
         add Acc,AccT
		 sts _TEMP_VAR_LON_L,Acc

         lds Acc,_TEMP_VAR_LON_ML
		 lds AccT,_VAR_QUAD_H
         ;ldi AccT,high(7000)
         adc Acc,AccT
         sts _TEMP_VAR_LON_ML,Acc

         lds Acc,_TEMP_VAR_LON_MH
         ldi AccT,0
		 adc Acc,AccT
		 sts _TEMP_VAR_LON_MH,Acc

         lds Acc,_TEMP_VAR_LON_H
         ldi AccT,0
		 adc Acc,AccT
         sts _TEMP_VAR_LON_H,Acc

;
; Agora testa
;
         clc
         lds Acc,_EE_PARDAL_POINT_L
		 lds AccT,_TEMP_VAR_LON_L
         cp Acc,AccT
		
         lds Acc,_EE_PARDAL_POINT_ML
		 lds AccT,_TEMP_VAR_LON_ML
         cpc Acc,AccT
		 
         lds Acc,_EE_PARDAL_POINT_MH
		 lds AccT,_TEMP_VAR_LON_MH
         cpc Acc,AccT
		
         lds Acc,_EE_PARDAL_POINT_H
		 lds AccT,_TEMP_VAR_LON_H
         cpc Acc,AccT
		
         brlo T0_TESTA_ACHOU_PARDAL  ; Se longitude pardal < longitude carro +1, continua
		 
		 rjmp CARREGA_SRAM_CONT       ; Senao, sai da interrupcao 
		 
; ****** CHEGANDO AQUI, O PONTO ENCONTRA-SE NO BIG SQUARE, AGORA TRANSFERE P/ RAM ***

T0_TESTA_ACHOU_PARDAL:
         
		; Transfere ponto encontrado para SRAM

		ldi Acc,0
		sts _BUFFER_BYTE,Acc
		ldix 0
		call _RODA_ADDRESS                  ; Faz DPTR +(CONTADOR16 * 10)

		ldiy _SRAM_PONTOS                   ; Aponta para buffer de pontos na SRAM
        call _RODA_DPTR_SRAM

        ldi TempH,11

TESTA_ACHOU_LOOP:
        
         call _AT45_MAIN_PAGE_READ            ; Le byte da eeprom

         push Acc                             ; guarda

         call _INCREMENTA_ENDERECO            ; incrementa DPTR1
		  
		 pop Acc                              ; Recupera
		 
		 st Y+,Acc                            ; Grava na SRAM
		 
		 subi TempH,1
	     tst TempH
	     brne TESTA_ACHOU_LOOP                ; Envia os 11 bytes correspondentes ao ponto

		 lds Acc,_RAM_PONTOS_CONT
		 inc Acc                              ; Adiciona 1 no contador de pontos na SRAM
		 sts _RAM_PONTOS_CONT,Acc

		 ;Verifica se chegou a 39 pontos
		 cpi Acc,39
		 brlo CARREGA_SRAM_CONT

		 sbi _AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	   ; END continuous
		 		 
		 ;Diminui o quadrante
		 ldi Acc,low(3500)
		 sts _VAR_QUAD_L,Acc

		 ldi Acc,high(3500)
		 sts _VAR_QUAD_H,Acc
		 

		; ldi Acc,25
		; sts _GPS_TICKS_COUNT,Acc

		 ldi Acc,0
		 sts _CONTADOR16_L,Acc
		 sts _CONTADOR16_H,Acc

		 _SET _FLAG_MAIOR39

		 RET


CARREGA_SRAM_CONT:
	    
		 sbi _AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	   ; END continuous
	
 ; Incrementa o contador de 16 bits

         clc
         lds Acc,_CONTADOR16_L
         ldi AccT,1
		 add Acc,AccT
         sts _CONTADOR16_L,Acc

         lds Acc,_CONTADOR16_H
         ldi AccT,0
		 adc Acc,AccT
		 sts _CONTADOR16_H,Acc

		 lds Acc,_CONTADOR16_L
		 lds AccT,_CONT_PONTOS_L
		 cp Acc,AccT

		 brne CARREGA_SRAM_VOLTA
		 
		 lds Acc,_CONTADOR16_H
		 lds AccT,_CONT_PONTOS_H
		 cp Acc,AccT

		 brne CARREGA_SRAM_VOLTA
		 
		 ldi Acc,0
		 sts _CONTADOR16_L,Acc
		 sts _CONTADOR16_H,Acc

		 ;Mantem quadrante
		 ldi Acc,low(7000)
		 sts _VAR_QUAD_L,Acc

		 ldi Acc,high(7000)
		 sts _VAR_QUAD_H,Acc

		 _CLEAR _FLAG_MAIOR39

        ;ldi Acc,0
		; sts _GPS_TICKS_COUNT,Acc

		 
		 ret

CARREGA_SRAM_VOLTA:
         rjmp CARREGA_SRAM_GO


;
;
;

CARREGA_23:

         _CLEAR _FLAG_TROCA_BANCO

         ; Carrega variavel EE_PARDAL_POINT (32BITS) com dados da eeprom
         call _AT45_MAIN_PAGE_READ

         lds AccT,_LATITUDE_H
		 cp Acc,AccT

         brne T0_PARDAL_CONT_SAI2     ; Byte da eeprom eh igual ao MSB da LAT.?

		 rjmp T0_PARDAL_CONT_PULA2     ; Se o MSB da LATITUDE for igual,continua   

T0_PARDAL_CONT_SAI2:

         rjmp CARREGA_SRAM_CONT     ; Senao, sai da interrupcao

T0_PARDAL_CONT_PULA2:

         sts _EE_PARDAL_POINT_H,Acc

		 call _INCREMENTA_ENDERECO

		 call _AT45_MAIN_PAGE_READ

		 sts _EE_PARDAL_POINT_MH,Acc

		 call _INCREMENTA_ENDERECO

		 call _AT45_MAIN_PAGE_READ

		 sts _EE_PARDAL_POINT_ML,Acc

		 call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ

		 sts _EE_PARDAL_POINT_L,Acc

         ;NOVA ROTINA ***

         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
		 
		 lds AccT,_LONGITUDE_H
		 cp Acc,AccT
		 lbrne T0_PARDAL_CONT_SAI
		          
         sts _TEMP_I,Acc
		 
         ; Decrementa 190 (~340m) em 32 bits

         lds Acc,_TEMP_VAR_LAT_L
		 lds AccT,_VAR_QUAD_L
        ; subi Acc,low(7000)
		 sub Acc,AccT
         sts _TEMP_VAR_LAT_L,Acc

         lds Acc,_TEMP_VAR_LAT_ML
		 lds AccT,_VAR_QUAD_H
         ;sbci Acc,high(7000)
		 sbc Acc,AccT
         sts _TEMP_VAR_LAT_ML,Acc
 
         lds Acc,_TEMP_VAR_LAT_MH
         sbci Acc,0
         sts _TEMP_VAR_LAT_MH,Acc

         lds Acc,_TEMP_VAR_LAT_H
         sbci Acc,0
         sts _TEMP_VAR_LAT_H,Acc


;+--------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA > POSICAO LATITUDE DO CARRO - 1 |
;+--------------------------------------------------------------------+

T0_TESTA_LATITUDE12:

        lds Acc,_EE_PARDAL_POINT_L
		lds AccT,_TEMP_VAR_LAT_L
        cp Acc,AccT

        lds Acc,_EE_PARDAL_POINT_ML
		lds AccT,_TEMP_VAR_LAT_ML
        cpc Acc,AccT

        lds Acc,_EE_PARDAL_POINT_MH
		lds AccT,_TEMP_VAR_LAT_MH
        cpc Acc,AccT

        lds Acc,_EE_PARDAL_POINT_H
		lds AccT,_TEMP_VAR_LAT_H
        cpc Acc,AccT

        brsh T0_TESTA_LATITUDE22        ; Se latitude pardal > latitude carro -1, continua

        rjmp CARREGA_SRAM_CONT           ; Senao, sai da interrupcao

;+--------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA < POSICAO LATITUDE DO CARRO + 1 |
;+--------------------------------------------------------------------+

T0_TESTA_LATITUDE22:
 
         ; CARREGA TEMP_VAR_LAT

         lds  Acc,_LATITUDE_L
         sts  _TEMP_VAR_LAT_L,Acc

		 lds Acc,_LATITUDE_ML
         sts  _TEMP_VAR_LAT_ML,Acc

		 lds Acc,_LATITUDE_MH
         sts  _TEMP_VAR_LAT_MH,Acc

		 lds Acc,_LATITUDE_H
         sts  _TEMP_VAR_LAT_H,Acc

; INCREMENTA O TEMP_VAR_LAT em 190 (~340m) ( 32 BITS)

         clc
         lds Acc,_TEMP_VAR_LAT_L
		 lds AccT,_VAR_QUAD_L
		; ldi AccT,low(7000)
         add  Acc,AccT
         sts _TEMP_VAR_LAT_L,Acc

         lds Acc,_TEMP_VAR_LAT_ML
		 lds AccT,_VAR_QUAD_H
       ;  ldi AccT,high(7000)
		 adc Acc,AccT
         sts _TEMP_VAR_LAT_ML,Acc

         lds Acc,_TEMP_VAR_LAT_MH
         ldi AccT,0
		 adc Acc,AccT
         sts _TEMP_VAR_LAT_MH,Acc

         lds Acc,_TEMP_VAR_LAT_H
         ldi AccT,0
		 adc Acc,AccT
         sts _TEMP_VAR_LAT_H,Acc
		 
;
; Agora testa
;
         clc
         lds Acc,_EE_PARDAL_POINT_L
		 lds AccT,_TEMP_VAR_LAT_L
         cp Acc,AccT

         lds Acc,_EE_PARDAL_POINT_ML
         lds AccT,_TEMP_VAR_LAT_ML
		 cpc Acc,AccT

         lds Acc,_EE_PARDAL_POINT_MH
         lds AccT,_TEMP_VAR_LAT_MH
		 cpc Acc,AccT

         lds Acc,_EE_PARDAL_POINT_H
         lds AccT,_TEMP_VAR_LAT_H
		 cpc Acc,AccT

         brlo T0_TESTA_LONGITUDE12      ; Se latitude pardal < latitude carro +1,continua

         rjmp CARREGA_SRAM_CONT         ; Senao, sai da interrupcao

T0_TESTA_LONGITUDE12:

        ; +-----------------------------+
        ; | CARREGA LONGITUDE DA EEPROM |
        ; +-----------------------------+
		 
       /*  ldi Acc,4
		 sts _BUFFER_BYTE,Acc
         ldix 0
         call _RODA_ADDRESS

         call _AT45_MAIN_PAGE_READ

		 lds AccT,_LONGITUDE_H
		 cp Acc,AccT

		 brne T0_TESTA_LONG_SAI
        */
         rjmp T0_TESTA_LONGITUDE_PULA2

T0_TESTA_LONG_SAI2:

         rjmp CARREGA_SRAM_CONT     ; Sai da interrupcao

T0_TESTA_LONGITUDE_PULA2:

         lds Acc,_TEMP_I
         sts _EE_PARDAL_POINT_H,Acc  ; Recupera

         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ

		 lds AccT,_LONGITUDE_MH

		 cp Acc,AccT

		 lbrne T0_TESTA_LONG_SAI2   ; Se o 2o byte for diferente, sai
		 		 
         sts _EE_PARDAL_POINT_MH,Acc

         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ

         sts _EE_PARDAL_POINT_ML,Acc

         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ

         sts _EE_PARDAL_POINT_L,Acc
		 
; CARREGA TEMP_VAR_LON

      	 lds  Acc,_LONGITUDE_L
         sts  _TEMP_VAR_LON_L,Acc

		 lds Acc,_LONGITUDE_ML
         sts  _TEMP_VAR_LON_ML,Acc

		 lds Acc,_LONGITUDE_MH
         sts  _TEMP_VAR_LON_MH,Acc

		 lds Acc,_LONGITUDE_H
         sts  _TEMP_VAR_LON_H,Acc
;+---------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA > POSICAO LONGITUDE DO CARRO - 1 |
;+---------------------------------------------------------------------+

         ; Decrementa em 190 (~340m) 32 bits
		 
         lds Acc,_TEMP_VAR_LON_L
		 lds AccT,_VAR_QUAD_L
         ;subi Acc,low(7000)
		 sub Acc,AccT
		 sts _TEMP_VAR_LON_L,Acc

		 lds Acc,_TEMP_VAR_LON_ML
		 lds AccT,_VAR_QUAD_H
		; sbci Acc,high(7000)
		 sbc Acc,AccT
		 sts _TEMP_VAR_LON_ML,Acc
		 
         lds Acc,_TEMP_VAR_LON_MH
         sbci Acc,0
         sts _TEMP_VAR_LON_MH,Acc

         lds Acc,_TEMP_VAR_LON_H
         sbci Acc,0
         sts _TEMP_VAR_LON_H,Acc

	;  Agora testa
		 		 
         clc
         lds Acc,_EE_PARDAL_POINT_L
		 lds AccT,_TEMP_VAR_LON_L
         cp Acc,AccT

         lds Acc,_EE_PARDAL_POINT_ML
		 lds AccT,_TEMP_VAR_LON_ML
         cpc Acc,AccT

         lds Acc,_EE_PARDAL_POINT_MH
		 lds AccT,_TEMP_VAR_LON_MH
         cpc Acc,AccT

         lds Acc,_EE_PARDAL_POINT_H
		 lds AccT,_TEMP_VAR_LON_H
         cpc Acc,AccT

         brsh T0_TESTA_LONGITUDE22    ; Se longitude PARDAL > LONGITUDE CARRO -1,CONTINUA

         rjmp CARREGA_SRAM_CONT     ; Sai da interrupcao

T0_TESTA_LONGITUDE22:

         ; CARREGA TEMP_VAR_LON

         lds  Acc,_LONGITUDE_L
         sts  _TEMP_VAR_LON_L,Acc

		 lds Acc,_LONGITUDE_ML
         sts  _TEMP_VAR_LON_ML,Acc

		 lds Acc,_LONGITUDE_MH
         sts  _TEMP_VAR_LON_MH,Acc

		 lds Acc,_LONGITUDE_H
         sts  _TEMP_VAR_LON_H,Acc
		 
;+--------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA < POSICAO LATITUDE DO CARRO + 1 |
;+--------------------------------------------------------------------+

; INCREMENTA O TEMP_VAR_LON 190 (~340m)( 32 BITS)

         clc
         lds Acc,_TEMP_VAR_LON_L
		 lds AccT,_VAR_QUAD_L
         ;ldi AccT,low(7000)
         add Acc,AccT
		 sts _TEMP_VAR_LON_L,Acc

         lds Acc,_TEMP_VAR_LON_ML
		 lds AccT,_VAR_QUAD_H
        ; ldi AccT,high(7000)
         adc Acc,AccT
         sts _TEMP_VAR_LON_ML,Acc

         lds Acc,_TEMP_VAR_LON_MH
         ldi AccT,0
		 adc Acc,AccT
		 sts _TEMP_VAR_LON_MH,Acc

         lds Acc,_TEMP_VAR_LON_H
         ldi AccT,0
		 adc Acc,AccT
         sts _TEMP_VAR_LON_H,Acc


;
; Agora testa
;
         clc
         lds Acc,_EE_PARDAL_POINT_L
		 lds AccT,_TEMP_VAR_LON_L
         cp Acc,AccT
		
         lds Acc,_EE_PARDAL_POINT_ML
		 lds AccT,_TEMP_VAR_LON_ML
         cpc Acc,AccT
		 
         lds Acc,_EE_PARDAL_POINT_MH
		 lds AccT,_TEMP_VAR_LON_MH
         cpc Acc,AccT
		
         lds Acc,_EE_PARDAL_POINT_H
		 lds AccT,_TEMP_VAR_LON_H
         cpc Acc,AccT

         brlo T0_TESTA_ACHOU_PARDAL_TRAMP2  ; Se longitude pardal < longitude carro +1, continua

         rjmp CARREGA_SRAM_CONT    ; Senao, sai da interrupcao


T0_TESTA_ACHOU_PARDAL_TRAMP2:

         rjmp T0_TESTA_ACHOU_PARDAL

 

















