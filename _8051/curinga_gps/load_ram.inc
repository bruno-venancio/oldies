;
;
;
_CARREGA_SRAM:

         mov RAM_PONTOS_CONT,#0

CARREGA_SRAM_GO:

         ; Guarda as variaveis de latitude

         mov  TEMP_VAR_LAT_L,LATITUDE_L
         mov  TEMP_VAR_LAT_ML,LATITUDE_ML
         mov  TEMP_VAR_LAT_MH,LATITUDE_MH
         mov  TEMP_VAR_LAT_H,LATITUDE_H

         ; +----------------------------+
         ; | CARREGA LATITUDE DA EEPROM |
         ; +----------------------------+

         mov BUFFER_BYTE,#0
         mov dptr,#0
         lcall _RODA_ADDRESS          ; Faz DPTR +(CONTADOR16 * 10)

        ; Carrega variavel EE_PARDAL_POINT (32BITS) com dados da eeprom

         lcall _AT45_MAIN_PAGE_READ

         cjne a,LATITUDE_H,T0_PARDAL_CONT_SAI ; Byte da eeprom eh igual ao MSB da LAT.?

         sjmp T0_PARDAL_CONT_PULA             ; Se o MSB da LATITUDE for igual,continua                       ; ao MSB da EEPROM, continua

T0_PARDAL_CONT_SAI:

         ljmp CARREGA_SRAM_CONT     ; Senao, sai da interrupcao

T0_PARDAL_CONT_PULA:

         mov EE_PARDAL_POINT_H,a

         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ

         mov EE_PARDAL_POINT_MH,a

         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ

         mov EE_PARDAL_POINT_ML,a

         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ

         mov EE_PARDAL_POINT_L,a

         ; Decrementa 190 (~340m) em 32 bits

         clr C

         mov a,TEMP_VAR_LAT_L
         subb a,#low(1900)
         mov TEMP_VAR_LAT_L,a

         mov a,TEMP_VAR_LAT_ML
         subb a,#high(1900)
         mov TEMP_VAR_LAT_ML,a

         mov a,TEMP_VAR_LAT_MH
         subb a,#0
         mov TEMP_VAR_LAT_MH,a

         mov a,TEMP_VAR_LAT_H
         subb a,#0
         mov TEMP_VAR_LAT_H,a


;+--------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA > POSICAO LATITUDE DO CARRO - 1 |
;+--------------------------------------------------------------------+

T0_TESTA_LATITUDE1:

         clr c
         mov a,EE_PARDAL_POINT_L
         subb a,TEMP_VAR_LAT_L

         mov a,EE_PARDAL_POINT_ML
         subb a,TEMP_VAR_LAT_ML

         mov a,EE_PARDAL_POINT_MH
         subb a,TEMP_VAR_LAT_MH

         mov a,EE_PARDAL_POINT_H
         subb a,TEMP_VAR_LAT_H

         jnc T0_TESTA_LATITUDE2        ; Se latitude pardal > latitude carro -1, continua

         ljmp CARREGA_SRAM_CONT     ; Senao, sai da interrupcao

;+--------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA < POSICAO LATITUDE DO CARRO + 1 |
;+--------------------------------------------------------------------+

T0_TESTA_LATITUDE2:

; CARREGA TEMP_VAR_LAT

         mov  TEMP_VAR_LAT_L,LATITUDE_L
         mov  TEMP_VAR_LAT_ML,LATITUDE_ML
         mov  TEMP_VAR_LAT_MH,LATITUDE_MH
         mov  TEMP_VAR_LAT_H,LATITUDE_H

; INCREMENTA O TEMP_VAR_LAT em 190 (~340m) ( 32 BITS)

         clr C
         mov a,TEMP_VAR_LAT_L
         add  a,#low(1900)
         mov TEMP_VAR_LAT_L,a

         mov a,TEMP_VAR_LAT_ML
         addc a,#high(1900)
         mov TEMP_VAR_LAT_ML,a

         mov a,TEMP_VAR_LAT_MH
         addc a,#0
         mov TEMP_VAR_LAT_MH,a

         mov a,TEMP_VAR_LAT_H
         addc a,#0
         mov TEMP_VAR_LAT_H,a

;
; Agora testa
;
         clr c
         mov a,EE_PARDAL_POINT_L
         subb a,TEMP_VAR_LAT_L

         mov a,EE_PARDAL_POINT_ML
         subb a,TEMP_VAR_LAT_ML

         mov a,EE_PARDAL_POINT_MH
         subb a,TEMP_VAR_LAT_MH

         mov a,EE_PARDAL_POINT_H
         subb a,TEMP_VAR_LAT_H

         jc T0_TESTA_LONGITUDE1         ; Se latitude pardal < latitude carro +1,continua

         ljmp CARREGA_SRAM_CONT         ; Senao, sai da interrupcao

T0_TESTA_LONGITUDE1:

         ; +-----------------------------+
         ; | CARREGA LONGITUDE DA EEPROM |
         ; +-----------------------------+


         mov BUFFER_BYTE,#4
         mov dptr,#0
         lcall _RODA_ADDRESS

         lcall _AT45_MAIN_PAGE_READ

         cjne a,LONGITUDE_H,T0_TESTA_LONG_SAI
         sjmp T0_TESTA_LONGITUDE_PULA

T0_TESTA_LONG_SAI:

         ljmp CARREGA_SRAM_CONT     ; Sai da interrupcao

T0_TESTA_LONGITUDE_PULA:

         mov EE_PARDAL_POINT_H,a

         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ

         cjne a,LONGITUDE_MH,T0_TESTA_LONG_SAI   ; Se o 2o byte for diferente, sai

         mov EE_PARDAL_POINT_MH,a

         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ

         mov EE_PARDAL_POINT_ML,a

         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ

         mov EE_PARDAL_POINT_L,a

; CARREGA TEMP_VAR_LON

         mov  TEMP_VAR_LON_L,LONGITUDE_L
         mov  TEMP_VAR_LON_ML,LONGITUDE_ML
         mov  TEMP_VAR_LON_MH,LONGITUDE_MH
         mov  TEMP_VAR_LON_H,LONGITUDE_H

;+---------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA > POSICAO LONGITUDE DO CARRO - 1 |
;+---------------------------------------------------------------------+

         ; Decrementa em 190 (~340m) 32 bits

         clr C

         mov a,TEMP_VAR_LON_L
         subb a,#low(1900)
         mov TEMP_VAR_LON_L,a

         mov a,TEMP_VAR_LON_ML
         subb a,#high(1900)
         mov TEMP_VAR_LON_ML,a

         mov a,TEMP_VAR_LON_MH
         subb a,#0
         mov TEMP_VAR_LON_MH,a

         mov a,TEMP_VAR_LON_H
         subb a,#0
         mov TEMP_VAR_LON_H,a


;  Agora testa
;
         clr c
         mov a,EE_PARDAL_POINT_L
         subb a,TEMP_VAR_LON_L

         mov a,EE_PARDAL_POINT_ML
         subb a,TEMP_VAR_LON_ML

         mov a,EE_PARDAL_POINT_MH
         subb a,TEMP_VAR_LON_MH

         mov a,EE_PARDAL_POINT_H
         subb a,TEMP_VAR_LON_H

         jnc T0_TESTA_LONGITUDE2    ; Se longitude PARDAL > LONGITUDE CARRO -1,CONTINUA

         sjmp CARREGA_SRAM_CONT     ; Sai da interrupcao

T0_TESTA_LONGITUDE2:

; CARREGA TEMP_VAR_LON

         mov  TEMP_VAR_LON_L,LONGITUDE_L
         mov  TEMP_VAR_LON_ML,LONGITUDE_ML
         mov  TEMP_VAR_LON_MH,LONGITUDE_MH
         mov  TEMP_VAR_LON_H,LONGITUDE_H


;+--------------------------------------------------------------------+
;|TESTA SE PONTO DO PARDAL DA MEMORIA < POSICAO LATITUDE DO CARRO + 1 |
;+--------------------------------------------------------------------+

; INCREMENTA O TEMP_VAR_LON 190 (~340m)( 32 BITS)

         clr C
         mov a,TEMP_VAR_LON_L
         add  a,#low(1900)
         mov TEMP_VAR_LON_L,a

         mov a,TEMP_VAR_LON_ML
         addc a,#high(1900)
         mov TEMP_VAR_LON_ML,a

         mov a,TEMP_VAR_LON_MH
         addc a,#0
         mov TEMP_VAR_LON_MH,a

         mov a,TEMP_VAR_LON_H
         addc a,#0
         mov TEMP_VAR_LON_H,a

;
; Agora testa
;
         clr c
         mov a,EE_PARDAL_POINT_L
         subb a,TEMP_VAR_LON_L

         mov a,EE_PARDAL_POINT_ML
         subb a,TEMP_VAR_LON_ML

         mov a,EE_PARDAL_POINT_MH
         subb a,TEMP_VAR_LON_MH

         mov a,EE_PARDAL_POINT_H
         subb a,TEMP_VAR_LON_H

         jc T0_TESTA_ACHOU_PARDAL  ; Se longitude pardal < longitude carro +1, continua

         sjmp CARREGA_SRAM_CONT    ; Senao, sai da interrupcao


; ****** CHEGANDO AQUI, O PONTO ENCONTRA-SE NO BIG SQUARE, AGORA TRANSFERE P/ RAM ***

T0_TESTA_ACHOU_PARDAL:

         ; Transfere ponto encontrado para SRAM

         mov BUFFER_BYTE,#0
         mov dptr,#0
         lcall _RODA_ADDRESS          ; Faz DPTR +(CONTADOR16 * 10)


         inc AUXR1                 ; Troca para DPTR2

         mov dptr,#SRAM_PONTOS     ; Aponta para buffer de pontos na SRAM na dptr2
         lcall _RODA_DPTR_SRAM

         inc AUXR1                 ; Troca para DPTR1

         mov R7,#11

TESTA_ACHOU_LOOP:

         lcall _AT45_MAIN_PAGE_READ            ; Le byte da eeprom

         push acc                              ; guarda

         lcall _INCREMENTA_ENDERECO            ; incrementa DPTR1

         inc AUXR1                 ; Troca para DPTR2


         mov a,AUXR                ; Seta processador para acessar RAM interna
         clr acc.1
         mov AUXR,a

         pop acc                   ; recupera

         mov EECON,#0

         movx @dptr,a              ; Grava na SRAM (DPTR2)
         inc dptr                  ; incrementa dptr2

         inc AUXR1                 ; Troca para DPTR1

         djnz R7,TESTA_ACHOU_LOOP  ; Envia os 11 bytes correspondentes ao ponto

         inc RAM_PONTOS_CONT       ; Adiciona 1 no contador de pontos na SRAM


CARREGA_SRAM_CONT:

 ; Incrementa o contador de 16 bits

         clr C
         mov a,CONTADOR16_L
         add a,#1
         mov CONTADOR16_L,a

         mov a,CONTADOR16_H
         addc a,#0
         mov CONTADOR16_H,a

         mov a,CONTADOR16_L
         cjne a,CONT_PONTOS_L,CARREGA_SRAM_VOLTA

         mov a,CONTADOR16_H
         cjne a,CONT_PONTOS_H,CARREGA_SRAM_VOLTA

         mov CONTADOR16_L,#0
         mov CONTADOR16_H,#0

         RET

CARREGA_SRAM_VOLTA:
         ljmp CARREGA_SRAM_GO

