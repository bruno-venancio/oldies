;
;
;

_PROCURA_PONTO_RAM:

     ; Guarda as variaveis de latitude

		 lds Acc,_LATITUDE_L
		 sts _X2_L,Acc

		 lds Acc,_LATITUDE_ML
		 sts _X2_ML,Acc

		 lds Acc,_LATITUDE_MH
		 sts _X2_MH,Acc

	     lds Acc,_LATITUDE_H
		 sts _X2_H,Acc

		 ; +-------------------------+
         ; | CARREGA LATITUDE DA RAM |
         ; +-------------------------+

         LDIX _SRAM_PONTOS           ; Aponta para buffer de pontos na SRAM
         call _RODA_DPTR_SRAM2       ; Faz DPTR +(CONTADOR8 * 11)
 
         ld Acc,X+

         sts _SRAM_PONTO_H,Acc

         ld Acc,X+
          
		 sts _SRAM_PONTO_MH,Acc
		 
         ld Acc,X+
		   
		 sts _SRAM_PONTO_ML,Acc

         ld Acc,X

		 sts _SRAM_PONTO_L,Acc

         ; Decrementa 2200 (~350m) em 32 bits

    	 clc

         lds Acc,_X2_L
         subi Acc,low(2200)
		 sts _X2_L,Acc

		 lds Acc,_X2_ML
		 sbci Acc,high(2200)
		 sts _X2_ML,Acc
		 
         lds Acc,_X2_MH
         sbci Acc,0
         sts _X2_MH,Acc

         lds Acc,_X2_H
         sbci Acc,0
         sts _X2_H,Acc
		 
;+--------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA > POSICAO LATITUDE DO CARRO - 1 |
;+--------------------------------------------------------------------+

TESTA_LATITUDE1:
		 		 
         clc
         lds Acc,_SRAM_PONTO_L
		 lds AccT,_X2_L
         cp Acc,AccT

         lds Acc,_SRAM_PONTO_ML
		 lds AccT,_X2_ML
         cpc Acc,AccT

         lds Acc,_SRAM_PONTO_MH
		 lds AccT,_X2_MH
         cpc Acc,AccT

         lds Acc,_SRAM_PONTO_H
		 lds AccT,_X2_H
         cpc Acc,AccT

		 brsh TESTA_LATITUDE2        ; Se latitude pardal > latitude carro -1, continua

		 jmp PROCURA_PONTO_SAIDA

;+--------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA < POSICAO LATITUDE DO CARRO + 1 |
;+--------------------------------------------------------------------+
TESTA_LATITUDE2:

; CARREGA TEMP_VAR_LAT

		 lds Acc,_LATITUDE_L
		 sts _X2_L,Acc

		 lds Acc,_LATITUDE_ML
		 sts _X2_ML,Acc

		 lds Acc,_LATITUDE_MH
		 sts _X2_MH,Acc

	     lds Acc,_LATITUDE_H
		 sts _X2_H,Acc

; INCREMENTA O TEMP_VAR_LAT em 190 (~350m) ( 32 BITS)

         clc 
		 lds Acc,_X2_L
		 ldi AccT,low(2200)
		 add Acc,AccT
		 sts _x2_L,Acc

		 lds Acc,_X2_ML
		 ldi AccT,high(2200)
		 adc Acc,AccT
		 sts _X2_ML,Acc

		 lds Acc,_X2_MH
		 ldi AccT,0
		 adc Acc,AccT
		 sts _X2_MH,Acc

		 lds Acc,_X2_H
		 ldi AccT,0
		 adc Acc,AccT
		 sts _X2_H,Acc

;
; Agora testa
;
         clc
		 lds Acc,_SRAM_PONTO_L
		 lds AccT,_X2_L
		 cp Acc,AccT
		 
		 lds Acc,_SRAM_PONTO_ML
		 lds AccT,_X2_ML
		 cpc Acc,AccT

		 lds Acc,_SRAM_PONTO_MH
		 lds AccT,_X2_MH
		 cpc Acc,AccT

		 lds Acc,_SRAM_PONTO_H
		 lds AccT,_X2_H
		 cpc Acc,AccT

		 brlo TESTA_LONGITUDE1         ; Se latitude pardal < latitude carro +1,continua

		 jmp PROCURA_PONTO_SAIDA

TESTA_LONGITUDE1:
         ; +-----------------------------+
         ; | CARREGA LONGITUDE DA EEPROM |
         ; +-----------------------------+

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

; CARREGA TEMP_VAR_LON
         
		 lds Acc,_LONGITUDE_L  
		 sts _X2_L,Acc

		 lds Acc,_LONGITUDE_ML  
		 sts _X2_ML,Acc

		 lds Acc,_LONGITUDE_MH  
		 sts _X2_MH,Acc

		 lds Acc,_LONGITUDE_H  
		 sts _X2_H,Acc

;+---------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA > POSICAO LONGITUDE DO CARRO - 1 |
;+---------------------------------------------------------------------+

         ; Decrementa em 190 (~350m) 32 bits

		 clc

		 lds Acc,_X2_L
		 ldi AccT,low(2200)
		 sub Acc,AccT
		 sts _X2_L,Acc

		 lds Acc,_X2_ML
		 ldi AccT,high(2200)
		 sbc Acc,AccT
		 sts _X2_ML,Acc

		 lds Acc,_X2_MH
		 ldi AccT,0
		 sbc Acc,AccT
		 sts _X2_MH,Acc

		 lds Acc,_X2_H
		 ldi AccT,0
		 sbc Acc,AccT
		 sts _X2_H,Acc

		 ; Agora testa

		 clc
		 lds Acc,_SRAM_PONTO_L
		 lds AccT,_X2_L
		 cp Acc,AccT

		 lds Acc,_SRAM_PONTO_ML
		 lds AccT,_X2_ML
		 cpc Acc,AccT

		 lds Acc,_SRAM_PONTO_MH
		 lds AccT,_X2_MH
		 cpc Acc,AccT

		 lds Acc,_SRAM_PONTO_H
		 lds AccT,_X2_H
		 cpc Acc,AccT

		 brsh TESTA_LONGITUDE2    ; Se longitude PARDAL > LONGITUDE CARRO -1,CONTINUA

		 jmp PROCURA_PONTO_SAIDA

TESTA_LONGITUDE2:
; CARREGA TEMP_VAR_LON
         
		 lds Acc,_LONGITUDE_L  
		 sts _X2_L,Acc

		 lds Acc,_LONGITUDE_ML  
		 sts _X2_ML,Acc

		 lds Acc,_LONGITUDE_MH  
		 sts _X2_MH,Acc

		 lds Acc,_LONGITUDE_H  
		 sts _X2_H,Acc
         
;+--------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA < POSICAO LONGITUDE DO CARRO + 1|
;+--------------------------------------------------------------------+

; INCREMENTA O TEMP_VAR_LON 190 (~350m)( 32 BITS)

         clc 
		 lds Acc,_X2_L
		 ldi AccT,low(2200)
		 add Acc,AccT
		 sts _X2_L,Acc

		 lds Acc,_X2_ML
		 ldi AccT,high(2200)
		 adc Acc,AccT
		 sts _X2_ML,Acc

		 lds Acc,_X2_MH
		 ldi AccT,0
		 adc Acc,AccT
		 sts _X2_MH,Acc

		 lds Acc,_X2_H
		 ldi AccT,0
		 adc Acc,AccT
		 sts _X2_H,Acc

;
; Agora testa
;
         clc
		 lds Acc,_SRAM_PONTO_L
		 lds AccT,_X2_L
		 cp Acc,AccT

		 lds Acc,_SRAM_PONTO_ML
		 lds AccT,_X2_ML
		 cpc Acc,AccT

		 lds Acc,_SRAM_PONTO_MH
		 lds AccT,_X2_MH
		 cpc Acc,AccT

		 lds Acc,_SRAM_PONTO_H
		 lds AccT,_X2_H
		 cpc Acc,AccT      

		 brsh PROCURA_PONTO_SAIDA      ; Se longitude pardal > longitude carro +1, continua

		 _SET _FLAG_PONTO_ENCONTRADO
		 
		 jmp PROCURA_PONTO_END 

PROCURA_PONTO_SAIDA:

		 _CLEAR _FLAG_PONTO_ENCONTRADO

PROCURA_PONTO_END:
          
		 RET


    

















		  
      
		 








