

_GRAVA_EEPROM:

         clr FLAG_START
         clr FLAG_POSSUI_DADOS_GPS
         clr ES


         push acc
         push psw
         push dpl
         push dph
         push 00h
         push 01h
         push 02h
         push 03h
         push 04h
         push 05h
         push 06h
         push 07h
         push b

         mov a,CONT_PONTOS_L     ; Faz ENDERECO <- ( CRD_COUNTER * 11)
         mov r3,a

         mov a,CONT_PONTOS_H
         mov r2,a

         mov r0,#0
         mov r1,#11

         lcall _MUL16

       ;  clr C                ; agora adiciona RESULTADO com dptr
         mov a,R4
         ;add a,BUFFER_BYTE
        ; add a,#0
         mov BUFFER_BYTE,a

         mov a,R5
         ;addc a,dpl
        ; addc a,#0
         mov dpl,a

         mov a,R6
         ;addc a,dph
        ; addc a,#0
         mov dph,a

GRAVA_LOOP:

         lcall _AT45_MAIN_PAGE_TO_BUFFER1_TRANSF

         ;GRAVA LATITUDE
         mov a,LATITUDE_H
         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         mov a,LATITUDE_MH

         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         mov a,LATITUDE_ML

         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         mov a,LATITUDE_L
         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         ;GRAVA LONGITUDE
         mov a,LONGITUDE_H
         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         mov a,LONGITUDE_MH
         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         mov a,LONGITUDE_ML
         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         mov a,LONGITUDE_L
         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         lcall _ARREDONDA_VEL  ; "Arredonda" a velocidade atual para cima

         mov a,VARIAVEL
         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO

         mov a,DIRECAO_H

         setb acc.7                     ; Seta o bit MSB para indicar que é ponto do usuário

         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO

         mov a,DIRECAO_L
         lcall _AT45_MAIN_PAGE_WRITE_B1

         ; Incrementa CONT_PONTOS

         clr C
         mov a,CONT_PONTOS_L
         add a,#1
         mov CONT_PONTOS_L,a

         mov a,CONT_PONTOS_H
         addc a,#0
         mov CONT_PONTOS_H,a

        ; Salva CONT_PONTOS na EEPROM

         mov dptr,#EE_CONT_PONTOS_L
         mov a,CONT_PONTOS_L
         mov DADO,a
         lcall _EE_WRITE

         mov dptr,#EE_CONT_PONTOS_H
         mov a,CONT_PONTOS_H
         mov DADO,a
         lcall _EE_WRITE

         mov a,#2
         lcall _delay_ms

         lcall _BUZZER_APITA

         pop b
         pop 07h
         pop 06h
         pop 05h
         pop 04h
         pop 03h
         pop 02h
         pop 01h
         pop 00h
         pop dph
         pop dpl
         pop psw
         pop acc

         setb ES

         ret




;
; Grava POS (posicao atual) na eeprom
;

_GRAVA_POSICAO:

         clr FLAG_START
         clr FLAG_POSSUI_DADOS_GPS
         setb LED5

         push acc
         push psw
         push dpl
         push dph
         push b
         
         ; Se carro estiver parado, detecta se botao esta pressionado por 1seg. para alterar volume
         jnb FLAG_CARRO_PARADO,GRAVA_CONTINUA

         mov a,#1
         lcall _delay_sec

         jb BOTAO_GRAVA,GRAVA_LOOP_SAIDA

         lcall _SND_BEEP2
         lcall _SND_BEEP2
         lcall _SND_BEEP2
         
         setb FLAG_WIFI
         clr LED_VM

         sjmp GRAVA_LOOP_SAIDA

GRAVA_CONTINUA:

         jb FLAG_PARDAL_LOCALIZADO,APAGA_PARDAL

         mov a,#80
         lcall _DELAY_MS

         jb BOTAO_GRAVA,GRAVA_LOOP_SAIDA

         setb FLAG_GRAVA_EEPROM
         clr  FLAG_APAGA_EEPROM
         clr  FLAG_WIFI

         lcall _SND_BEEP1

         sjmp GRAVA_LOOP_SAIDA


APAGA_PARDAL:

         ; Testa se requisicao para apagar ponto foi acionado

         mov dptr,#SRAM_PONTOS+9     ; Aponta para buffer de pontos na SRAM
         lcall _RODA_DPTR_SRAM2      ; Faz DPTR +(CONTADOR8 * 11)
         lcall _SRAM_INTERNA
         lcall _SRAM_READ

        ; jnb acc.7,GRAVA_LOOP_SAIDA  ; Se nao for ponto manual, sai
         ;jb acc.7,GRAVA_LOOP_SAIDA  ;  xoxo

         mov a,#250
         lcall _DELAY_MS

         mov a,#250
         lcall _DELAY_MS


         jb BOTAO_GRAVA,GRAVA_LOOP_SAIDA

         lcall _SND_BEEP2
         lcall _SND_BEEP2

         setb FLAG_APAGA_EEPROM
         clr FLAG_GRAVA_EEPROM
         clr FLAG_WIFI

GRAVA_LOOP_SAIDA:

         jnb BOTAO_GRAVA,$

         pop b
         pop dph
         pop dpl
         pop psw
         pop acc

         reti









_APAGA_PARDAL_EEPROM:

         clr ES
         clr FLAG_START
         clr FLAG_POSSUI_DADOS_GPS

          ;Carrega LATITUDE DA SRAM

         mov dptr,#SRAM_PONTOS       ; Aponta para buffer de pontos na SRAM
         lcall _RODA_DPTR_SRAM2      ; Faz DPTR +(CONTADOR8 * 11)
         lcall _SRAM_INTERNA
         lcall _SRAM_READ

         mov TEMP_VAR_LAT_H,a

         inc dptr

         lcall _SRAM_READ

         mov TEMP_VAR_LAT_MH,a

         inc dptr

         lcall _SRAM_READ

         mov TEMP_VAR_LAT_ML,a

         inc dptr

         lcall _SRAM_READ

         mov TEMP_VAR_LAT_L,a

        ;Carrega LONGITUDE DA SRAM

         inc dptr

         lcall _SRAM_READ

         mov TEMP_VAR_LON_H,a

         inc dptr

         lcall _SRAM_READ

         mov TEMP_VAR_LON_MH,a

         inc dptr

         lcall _SRAM_READ

         mov TEMP_VAR_LON_ML,a

         inc dptr

         lcall _SRAM_READ

         mov TEMP_VAR_LON_L,a


         mov CONTADOR16_L,#0
         mov CONTADOR16_H,#0


APAGA_PARDAL_L1:


       ;  mov BUFFER_BYTE,#9
       ;  mov dptr,#0

       ;  lcall _RODA_ADDRESS

       ;  lcall _AT45_MAIN_PAGE_READ

         ;jb acc.7,APAGA_PARDAL_CONTINUA
         ;jnb acc.7,APAGA_PARDAL_CONTINUA     ; xoxo
         sjmp APAGA_PARDAL_CONTINUA



APAGA_INCREMENTA:

        ; INCREMENTA CONTADOR16 E COMPARA COM O TOTAL DA MEMORIA

         clr C
         mov a,CONTADOR16_L
         add a,#1
         mov CONTADOR16_L,a

         mov a,CONTADOR16_H
         addc a,#0
         mov CONTADOR16_H,a

         mov a,CONTADOR16_L
         cjne a,CONT_PONTOS_L,APAGA_PARDAL_L1

         mov a,CONTADOR16_H
         cjne a,CONT_PONTOS_H,APAGA_PARDAL_L1

         ajmp APAGA_PARDAL_SAIDA

APAGA_PARDAL_CONTINUA:

         mov BUFFER_BYTE,#0
         mov dptr,#0

         lcall _RODA_ADDRESS

APAGA_PARDAL_CONTINUA_LOOP:

        ; Carrega LATITUDE (32BITS)

         lcall _AT45_MAIN_PAGE_READ
         mov LATITUDE_H,a

         cjne a,TEMP_VAR_LAT_H,APAGA_INCREMENTA
       ;-
         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ
         mov LATITUDE_MH,a

         cjne a,TEMP_VAR_LAT_MH,APAGA_INCREMENTA

       ;-
         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ
         mov LATITUDE_ML,a

         cjne a,TEMP_VAR_LAT_ML,APAGA_INCREMENTA

       ;-
         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ
         mov LATITUDE_L,a

         cjne a,TEMP_VAR_LAT_L,APAGA_INCREMENTA


         lcall _INCREMENTA_ENDERECO

       ; Carrega LONGITUDE (32BITS)

         lcall _AT45_MAIN_PAGE_READ
         mov LONGITUDE_H,a

         cjne a,TEMP_VAR_LON_H,APAGA_INCREMENTA


         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ
         mov LONGITUDE_MH,a

         cjne a,TEMP_VAR_LON_MH,APAGA_INCREMENTA

       ;-
         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ
         mov LONGITUDE_ML,a

         cjne a,TEMP_VAR_LON_ML,APAGA_INCREMENTA
       ;-
         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ
         mov LONGITUDE_L,a

         cjne a,TEMP_VAR_LON_L,APAGA_INCREMENTA

         ; Se o ponto foi encontrado, apaga

         mov BUFFER_BYTE,#0
         mov dptr,#0

         lcall _RODA_ADDRESS

         lcall _AT45_MAIN_PAGE_TO_BUFFER1_TRANSF

         ;GRAVA LATITUDE
         mov a,#0ffh
         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         mov a,#0ffh
         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         mov a,#0ffh
         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         mov a,#0ffh
         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         ;GRAVA LONGITUDE
         mov a,#0ffh
         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         mov a,#0ffh
         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         mov a,#0ffh
         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         mov a,#0ffh
         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO

         lcall _INCREMENTA_ENDERECO   ; aponta para o direcao_h
         mov a,#00h
         lcall _AT45_MAIN_PAGE_WRITE_B1


         lcall _BUZZER_NEGADO

        ;-

APAGA_PARDAL_SAIDA:

         setb ES

         ret




