;
;
;

_PROCURA_PONTO_RAM:

     ; Guarda as variaveis de latitude

         mov  X2_L,LATITUDE_L
         mov  X2_ML,LATITUDE_ML
         mov  X2_MH,LATITUDE_MH
         mov  X2_H,LATITUDE_H

         ; +----------------------------+
         ; | CARREGA LATITUDE DA EEPROM |
         ; +----------------------------+

         mov dptr,#SRAM_PONTOS       ; Aponta para buffer de pontos na SRAM
         lcall _RODA_DPTR_SRAM2      ; Faz DPTR +(CONTADOR8 * 11)
         lcall _SRAM_INTERNA
         lcall _SRAM_READ

         mov SRAM_PONTO_H,a

         inc dptr

         lcall _SRAM_READ

         mov SRAM_PONTO_MH,a

         inc dptr

         lcall _SRAM_READ

         mov SRAM_PONTO_ML,a

         inc dptr

         lcall _SRAM_READ

         mov SRAM_PONTO_L,a

         ; Decrementa 1900 (~350m) em 32 bits

         clr C

         mov a,X2_L
         subb a,#low(1900)
         mov X2_L,a

         mov a,X2_ML
         subb a,#high(1900)
         mov X2_ML,a

         mov a,X2_MH
         subb a,#0
         mov X2_MH,a

         mov a,X2_H
         subb a,#0
         mov X2_H,a


;+--------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA > POSICAO LATITUDE DO CARRO - 1 |
;+--------------------------------------------------------------------+

TESTA_LATITUDE1:

         clr c
         mov a,SRAM_PONTO_L
         subb a,X2_L

         mov a,SRAM_PONTO_ML
         subb a,X2_ML

         mov a,SRAM_PONTO_MH
         subb a,X2_MH

         mov a,SRAM_PONTO_H
         subb a,X2_H

         jnc TESTA_LATITUDE2        ; Se latitude pardal > latitude carro -1, continua

         ljmp PROCURA_PONTO_SAIDA

;+--------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA < POSICAO LATITUDE DO CARRO + 1 |
;+--------------------------------------------------------------------+


TESTA_LATITUDE2:

; CARREGA TEMP_VAR_LAT

         mov  X2_L,LATITUDE_L
         mov  X2_ML,LATITUDE_ML
         mov  X2_MH,LATITUDE_MH
         mov  X2_H,LATITUDE_H

; INCREMENTA O TEMP_VAR_LAT em 190 (~350m) ( 32 BITS)

         clr C
         mov a,X2_L
         add  a,#low(1900)
         mov X2_L,a

         mov a,X2_ML
         addc a,#high(1900)
         mov X2_ML,a

         mov a,X2_MH
         addc a,#0
         mov X2_MH,a

         mov a,X2_H
         addc a,#0
         mov X2_H,a

;
; Agora testa
;

         clr c
         mov a,SRAM_PONTO_L
         subb a,X2_L

         mov a,SRAM_PONTO_ML
         subb a,X2_ML

         mov a,SRAM_PONTO_MH
         subb a,X2_MH

         mov a,SRAM_PONTO_H
         subb a,X2_H

         jc TESTA_LONGITUDE1         ; Se latitude pardal < latitude carro +1,continua

         ljmp PROCURA_PONTO_SAIDA


TESTA_LONGITUDE1:

         ; +-----------------------------+
         ; | CARREGA LONGITUDE DA EEPROM |
         ; +-----------------------------+

         mov dptr,#SRAM_PONTOS+4     ; Aponta para buffer de pontos na SRAM
         lcall _RODA_DPTR_SRAM2      ; Faz DPTR +(CONTADOR8 * 11)
         lcall _SRAM_INTERNA
         lcall _SRAM_READ

         mov SRAM_PONTO_H,a

         inc dptr

         lcall _SRAM_READ

         mov SRAM_PONTO_MH,a

         inc dptr

         lcall _SRAM_READ

         mov SRAM_PONTO_ML,a

         inc dptr

         lcall _SRAM_READ

         mov SRAM_PONTO_L,a

; CARREGA TEMP_VAR_LON

         mov  X2_L,LONGITUDE_L
         mov  X2_ML,LONGITUDE_ML
         mov  X2_MH,LONGITUDE_MH
         mov  X2_H,LONGITUDE_H


;+---------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA > POSICAO LONGITUDE DO CARRO - 1 |
;+---------------------------------------------------------------------+

         ; Decrementa em 190 (~350m) 32 bits

         clr C

         mov a,X2_L
         subb a,#low(1900)
         mov X2_L,a

         mov a,X2_ML
         subb a,#high(1900)
         mov X2_ML,a

         mov a,X2_MH
         subb a,#0
         mov X2_MH,a

         mov a,X2_H
         subb a,#0
         mov X2_H,a

;  Agora testa
;
         clr c
         mov a,SRAM_PONTO_L
         subb a,X2_L

         mov a,SRAM_PONTO_ML
         subb a,X2_ML

         mov a,SRAM_PONTO_MH
         subb a,X2_MH

         mov a,SRAM_PONTO_H
         subb a,X2_H

         jnc TESTA_LONGITUDE2    ; Se longitude PARDAL > LONGITUDE CARRO -1,CONTINUA


         sjmp PROCURA_PONTO_SAIDA


TESTA_LONGITUDE2:

; CARREGA TEMP_VAR_LON

         mov  X2_L,LONGITUDE_L
         mov  X2_ML,LONGITUDE_ML
         mov  X2_MH,LONGITUDE_MH
         mov  X2_H,LONGITUDE_H

;+--------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA < POSICAO LATITUDE DO CARRO + 1 |
;+--------------------------------------------------------------------+

; INCREMENTA O TEMP_VAR_LON 190 (~350m)( 32 BITS)

         clr C
         mov a,X2_L
         add  a,#low(1900)
         mov X2_L,a

         mov a,X2_ML
         addc a,#high(1900)
         mov X2_ML,a

         mov a,X2_MH
         addc a,#0
         mov X2_MH,a

         mov a,X2_H
         addc a,#0
         mov X2_H,a

;
; Agora testa
;
         clr c
         mov a,SRAM_PONTO_L
         subb a,X2_L

         mov a,SRAM_PONTO_ML
         subb a,X2_ML

         mov a,SRAM_PONTO_MH
         subb a,X2_MH

         mov a,SRAM_PONTO_H
         subb a,X2_H

         jnc PROCURA_PONTO_SAIDA      ; Se longitude pardal < longitude carro +1, continua

         setb FLAG_PONTO_ENCONTRADO

         sjmp  PROCURA_PONTO_END

PROCURA_PONTO_SAIDA:

         clr FLAG_PONTO_ENCONTRADO

PROCURA_PONTO_END:

          RET



