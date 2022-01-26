
_PROCURA_WIFI:

      ;LISTA REDES E GRAVA OS DADOS NA SRAM INTERNA
      mov dptr,#SRAM_BUFFER
      mov Work2L,#0
      mov Work2H,#0
      mov CONTADOR_BUF,#0
      lcall _SRAM_INTERNA

      mov dptr,#AT_CWLAP
      lcall _SEND_STRING_ESP

      mov dptr,#SRAM_BUFFER

PROC_WI_L1:

      lcall _RXD_ESP      ; Recebe dado da ESP8266

      cjne a,#'+',PROC_WI_L2

      inc CONTADOR_BUF    ; conta numero de redes disponiveis

      movx @dptr,a
      inc dptr

      lcall INC_COUNT     ; Incrementa contador de bytes  (WORK)

      sjmp PROC_WI_L1


PROC_WI_L2:

      movx @dptr,a
      inc dptr

      lcall INC_COUNT     ; Incrementa contador de bytes

      cjne a,#'O',PROC_WI_TST

      lcall _RXD_ESP

      movx @dptr,a
      inc dptr

      lcall INC_COUNT     ; Incrementa contador de bytes


      cjne a,#'K',PROC_WI_TST

      lcall _RXD_ESP

      movx @dptr,a
      inc dptr


      cjne a,#CR,PROC_WI_TST
      
      ; Testa se nao ha redes disponiveis

      mov a,CONTADOR_BUF
      jnz PROC_ACHA_REDE1
      
      clr BUZZER

      lcall _PISCA_LED_VM_3X

      setb BUZZER

      setb FLAG_OK_ERRO

      ret


PROC_WI_TST:

      mov b,#HIGH(1700)
      mov a,Work2H
      cjne a,b,PROC_WI_L1

      mov b,#LOW(1700)
      mov a,Work2L
      cjne a,b,PROC_WI_L1


      clr BUZZER
      lcall _ALL_LEDS_ON

      mov a,#250
      lcall _delay_ms

      mov a,#250
      lcall _delay_ms

      lcall _ALL_LEDS_OFF

      setb BUZZER

      RET

;
; PROCURA NA SRAM A REDE COM SINAL MAIS FORTE
;


      ;+--------------+
      ;| CARREGA REDE1|
      ;+--------------+

PROC_ACHA_REDE1:

      clr LED1

      mov dptr,#SRAM_BUFFER

PROC_ACHA_R_L1:

      movx a,@dptr
      inc dptr

      cjne a,#':',PROC_ACHA_R_L1

PROC_ACHA_R_L2:

      movx a,@dptr
      inc dptr

      cjne a,#',',PROC_ACHA_R_L2

      inc dptr         ; pula "


      mov R0,#BUFFER_REDE1


PROC_ACHA_R_CPY1:

      movx a,@dptr
      mov @R0,a

      inc dptr
      inc R0

      cjne a,#'"',PROC_ACHA_R_CPY1

      movx a,@dptr
      mov @R0,a

      inc dptr         ; pula ,
      inc R0

      movx a,@dptr
      mov @R0,a

      inc dptr         ; pula -
      inc R0

      movx a,@dptr     ; GUARDA VALOR INTESIDADE DO SINAL
      mov WorkH,a
      mov @R0,a

      inc dptr
      inc R0

      movx a,@dptr
      mov Work,a
      mov @R0,a


PROC_ACHA_REDE2:

      movx a,@dptr
      inc dptr

      cjne a,#LF,PROC_ACHA_REDE2

      mov a,CONTADOR_BUF                ; Se houver apenas uma rede, grava direto no buffer resultado

      CJNE A,#1,PROC_ACHA2_R_L1

      ajmp PROC_ACHA_RESULTADO_L1

      ;+--------------+
      ;| CARREGA REDE2|
      ;+--------------+

PROC_ACHA2_R_L1:

      movx a,@dptr
      inc dptr

      cjne a,#':',PROC_ACHA2_R_L1

PROC_ACHA2_R_L2:

      movx a,@dptr
      inc dptr

      cjne a,#',',PROC_ACHA2_R_L2

      inc dptr ; pula "

      mov R0,#BUFFER_REDE2

PROC_ACHA2_R_CPY1:

      movx a,@dptr
      mov @R0,a

      inc dptr
      inc R0

      cjne a,#'"',PROC_ACHA2_R_CPY1

      movx a,@dptr
      mov @R0,a

      inc dptr       ; pula ,
      inc R0

      movx a,@dptr
      mov @R0,a

      inc dptr       ; pula -
      inc R0

      movx a,@dptr   ; carrega valor da intensidade do sinal
      mov R6,a
      mov @R0,a

      inc dptr
      inc R0

      movx a,@dptr
      mov R5,a
      mov @R0,a

      mov b,Work     ; Compara com sinal anterior

      mov a,WorkH

      lcall _TESTA16

      jb FLAG_EH_MENOR,PROC_ACHA_RESULTADO_L1

      mov R0,#BUFFER_RESULTADO

      mov R1,#BUFFER_REDE2
      
      sjmp  PROC_ACHA_RESULTADO_CPY


PROC_ACHA_RESULTADO_L1:

      mov R0,#BUFFER_RESULTADO

      mov R1,#BUFFER_REDE1


PROC_ACHA_RESULTADO_CPY:

      mov a,@R1
      mov @R0,a

      inc R0
      inc R1

      cjne a,#'"',PROC_ACHA_RESULTADO_CPY

      mov a,@R1    ; copia o ,
      mov @R0,a

      inc R0
      inc R1

      mov a,@R1    ; copia o -
      mov @R0,a

      inc R0
      inc R1

      mov a,@R1    ; copia o H do nivel sinal
      mov @R0,a
      mov WORKH,a  ; copia para futura comparacao

      inc R0
      inc R1

      mov a,@R1    ; copia o L do nivel sinal
      mov @R0,a
      mov WORK,a


      ; Verifica se tem mais redes

      mov a,CONTADOR_BUF
      mov b,#2
      lcall _TESTA

      jb FLAG_EH_IGUAL,PROCURA_REDE_SAIDA
      jb FLAG_EH_MENOR,PROCURA_REDE_SAIDA

      DEC CONTADOR_BUF

      ; final da segunda rede

PROC_ACHA_REDE3:

      movx a,@dptr
      inc dptr

      cjne a,#CR,PROC_ACHA_REDE3

      mov R1,#BUFFER_RESULTADO

      mov R0,#BUFFER_REDE1


PROC_COPIA_RES_REDE1:

      mov a,@R1
      mov @R0,a

      inc R0
      inc R1

      cjne a,#'"',PROC_COPIA_RES_REDE1

      mov a,@R1    ; copia o ,
      mov @R0,a

      inc R0
      inc R1

      mov a,@R1    ; copia o -
      mov @R0,a

      inc R0
      inc R1

      mov a,@R1    ; copia o H do nivel sinal
      mov @R0,a

      inc R0
      inc R1

      mov a,@R1    ; copia o L do nivel sinal
      mov @R0,a

      ljmp PROC_ACHA2_R_L1


PROCURA_REDE_SAIDA:

       ;+--------------+
       ;| Conecta WIFI |
       ;+--------------+

       mov dptr,#AT_CWJAP
       lcall _SEND_STRING_ESP

       mov R1,#BUFFER_RESULTADO

_CONECTA_WIFI_L1:

       mov a,@R1
       inc R1

       lcall _TXD_ESP

       cjne a,#'"',_CONECTA_WIFI_L1

       mov a,#','
       lcall _TXD_ESP

       mov dptr,#AT_CWJAP_PW3
       lcall _SEND_STRING_ESP

       lcall _GET_RESP_OK2
       
       jnb FLAG_OK_ERRO,_CONECTA_WIFI_L2
       
       clr BUZZER

       lcall _PISCA_LED_1_3X

       setb BUZZER

_CONECTA_WIFI_L2:

       ret



;
;   BAIXA PACOTE PRINCIPAL
;
_DATA_DOWNLOAD:

       mov a,#100
       lcall _delay_ms

       mov Work2L,#0
       mov Work2H,#0

       mov dptr,#AT_CIPSTART
       lcall _SEND_STRING_ESP

       lcall _GET_RESP_OK4

       jnb FLAG_OK_ERRO,DATA_DOWNLOAD_CONT
       
       djnz R8,_DATA_DOWNLOAD

       clr BUZZER

       lcall _PISCA_LED_2_3X
       
       setb BUZZER

       ret


DATA_DOWNLOAD_CONT:

       mov a,#10
       lcall _delay_ms

       clr LED3

       mov dptr,#AT_CIPSEND
       lcall _SEND_STRING_ESP

DATA_D_REC:

       lcall _RXD_ESP

       cjne a,#'>',DATA_REC_ERR

       mov dptr,#MSG_HTTP1
       lcall _SEND_STRING_ESP
       
       lcall _GET_RESP_OK4

       jnb FLAG_OK_ERRO,DATA_D_REC_PULA
D_ENTRY:
       clr BUZZER

       lcall _PISCA_LED_3_3X

       setb BUZZER

       ret

DATA_REC_ERR:
       cjne a,#'l',DATA_D_REC
       sjmp D_ENTRY

DATA_D_REC_PULA:

       clr LED4

       ; RECEBE E TRATA OS DADOS

DATA_D_REC_INI:

       lcall _RXD_ESP

       cjne a,#'!',DATA_D_REC_INI

       mov dptr,#SRAM_NRO_PACOTE              ; Pega numero do pacote
       mov R7,#5
       lcall _SRAM_INTERNA

PEGA_NRO_PACOTE:

       lcall _RXD_ESP

       movx @dptr,a
       inc dptr

       djnz R7,PEGA_NRO_PACOTE

DATA_GET_I:
       lcall _RXD_ESP
       cjne a,#'I',DATA_GET_I

       lcall _RXD_ESP

       mov R0,#BUFFER_MINI

       mov @R0,a
       inc R0

       lcall _RXD_ESP

       mov @R0,a
       inc R0

       mov a,#0
       mov @R0,a

       mov R0,#BUFFER_MINI
       lcall _DECBIN

       mov CONT_I,R4

       lcall _RXD_ESP     ; PULA X

       lcall _RXD_ESP

       lcall _ASC_TO_HEX
       anl a,#00001111b
       mov R6,A

       lcall _RXD_ESP

       lcall _ASC_TO_HEX
       anl a,#00001111b
       mov R7,A

       mov a,r6
       rl a
       rl a
       rl a
       rl a
       mov r6,a

       orl a,r7

       mov CONT_X,a


DATA_D_REC_00:

       lcall _RXD_ESP

       cjne a,#':',DATA_D_REC_00

       mov dptr,#SRAM_BUFFER
       lcall _SRAM_INTERNA


DATA_COPIA_SRAM_00:

       lcall _RXD_ESP

       cjne a,#CR,DATA_COPIA_SRAM_01

       sjmp DATA_COPIA_SRAM_00

DATA_COPIA_SRAM_01:

       cjne a,#LF,DATA_COPIA_SRAM_02

       sjmp DATA_COPIA_SRAM_00

DATA_COPIA_SRAM_02:

       cjne a,#'O',DATA_COPIA_SRAM_05

DATA_PULA_IPD:
       lcall _RXD_ESP

       cjne A,#':',DATA_PULA_IPD

       sjmp DATA_COPIA_SRAM_00

DATA_COPIA_SRAM_05:

       movx @dptr,a
       inc dptr

       lcall INC_COUNT      ; Incrementa contador de bytes

       mov R6,a

       mov a,Work2H
       cjne A,#HIGH(1700),DATA_COPIA_SRAM_06

       mov a,Work2L
       cjne A,#LOW(1700),DATA_COPIA_SRAM_06

       clr BUZZER
       lcall _PISCA_LED_4_3X
       setb BUZZER

       ret


DATA_COPIA_SRAM_06:

       mov a,R6

       cjne a,#'P',DATA_COPIA_SRAM_00

       CLR LED5

      ; lcall _SELECIONA_PACOTE

       ; Verifica versão do pacote

       lcall VERIFICA_VERSAO

       jb FLAG_OK_ERRO,DATA_INCLUI_CONTINUA

       lcall _ALL_LEDS_ON
       lcall _snd_beep1
       lcall _ALL_LEDS_OFF

       mov a,#250
       lcall _delay_ms

       lcall _ALL_LEDS_ON
       lcall _snd_beep1
       lcall _ALL_LEDS_OFF

       mov a,#250
       lcall _delay_ms

       lcall _ALL_LEDS_ON
       lcall _snd_beep1
       lcall _ALL_LEDS_OFF

       mov a,#250
       lcall _delay_ms

       RET




DATA_INCLUI_CONTINUA:

       ; +--------------------+
       ; | Processa protocolo |
       ; +--------------------+

       mov dptr,#SRAM_BUFFER
       lcall _SRAM_INTERNA
;
;  Adiciona ponto
;
DATA_INCLUI:

         inc AUXR1               ; *** Troca para DPTR2 ***

         mov a,CONT_PONTOS_L     ; Faz ENDERECO <- ( CRD_COUNTER * 11)
         mov r3,a

         mov a,CONT_PONTOS_H
         mov r2,a

         mov r0,#0
         mov r1,#11

         lcall _MUL16

       ;  clr C                ; agora adiciona RESULTADO com dptr
         mov a,R4
         mov BUFFER_BYTE,a

         mov a,R5
         mov dpl,a

         mov a,R6
         mov dph,a

         mov CONT2,#11

         lcall _AT45_MAIN_PAGE_TO_BUFFER1_TRANSF

DATA_INCLUI_LOOP:

         inc AUXR1               ; *** Troca para DPTR1 ***
DATA_L1:

         lcall BYTE_TEXT2HEX     ; Converte dois bytes em formato texto na memoria para um hex

         inc AUXR1               ; *** Troca para DPTR2 ***

         lcall _AT45_MAIN_PAGE_WRITE_B1
         lcall _INCREMENTA_ENDERECO
        ;-
         djnz CONT2,DATA_INCLUI_LOOP

         inc AUXR1               ; *** Troca para DPTR1 ***

         ; Incrementa CONT_PONTOS

         clr C
         mov a,CONT_PONTOS_L
         add a,#1
         mov CONT_PONTOS_L,a

         mov a,CONT_PONTOS_H
         addc a,#0
         mov CONT_PONTOS_H,a

        ; Salva CONT_PONTOS na EEPROM
        
         push DPL
         push DPH

         mov dptr,#EE_CONT_PONTOS_L
         mov a,CONT_PONTOS_L
         mov DADO,a
         lcall _EE_WRITE

         mov dptr,#EE_CONT_PONTOS_H
         mov a,CONT_PONTOS_H
         mov DADO,a
         lcall _EE_WRITE

         pop DPH
         pop DPL
         
         djnz CONT_I,DATA_INCLUI
         
         lcall GRAVA_VERSAO   ;  grava versao do pacote

         
         mov a,CONT_X         ; Se nao houver dados para apagar, sai
         jz DATA_SAIDA

        ;+--------------+
        ;| APAGA PONTOS |
        ;+--------------+
APAGA_PONTO_FLASH:

         lcall DATA_APAGA_FLASH
         
         djnz CONT_X,APAGA_PONTO_FLASH
        
DATA_SAIDA:

        RET
        


         



; +---------------------+
; | APAGA DADO DA FLASH |
; +---------------------+

DATA_APAGA_FLASH:

          ;Carrega LATITUDE DA SRAM

         lcall BYTE_TEXT2HEX          ; dptr 1
         mov TEMP_VAR_LAT_H,a

         lcall BYTE_TEXT2HEX
         mov TEMP_VAR_LAT_MH,a

         lcall BYTE_TEXT2HEX
         mov TEMP_VAR_LAT_ML,a

         lcall BYTE_TEXT2HEX
         mov TEMP_VAR_LAT_L,a

        ;Carrega LONGITUDE DA SRAM

         lcall BYTE_TEXT2HEX
         mov TEMP_VAR_LON_H,a


         lcall BYTE_TEXT2HEX
         mov TEMP_VAR_LON_MH,a


         lcall BYTE_TEXT2HEX
         mov TEMP_VAR_LON_ML,a

         lcall BYTE_TEXT2HEX
         mov TEMP_VAR_LON_L,a


         mov CONTADOR16_L,#0
         mov CONTADOR16_H,#0

         inc AUXR1               ; *** Troca para DPTR2 ***

         sjmp DATA_APAGA_PARDAL_CONTINUA


DATA_APAGA_INCREMENTA:

        ; INCREMENTA CONTADOR16 E COMPARA COM O TOTAL DA MEMORIA

         clr C
         mov a,CONTADOR16_L
         add a,#1
         mov CONTADOR16_L,a

         mov a,CONTADOR16_H
         addc a,#0
         mov CONTADOR16_H,a

         mov a,CONTADOR16_L
         cjne a,CONT_PONTOS_L,DATA_APAGA_PARDAL_CONTINUA

         mov a,CONTADOR16_H
         cjne a,CONT_PONTOS_H,DATA_APAGA_PARDAL_CONTINUA

         ljmp DATA_APAGA_PARDAL_SAIDA



DATA_APAGA_PARDAL_CONTINUA:

         mov BUFFER_BYTE,#0
         mov dptr,#0                ; DPTR 2

         lcall _RODA_ADDRESS

DATA_APAGA_PARDAL_CONTINUA_LOOP:

        ; Carrega LATITUDE (32BITS)

         lcall _AT45_MAIN_PAGE_READ
         mov LATITUDE_H,a

         cjne a,TEMP_VAR_LAT_H,DATA_APAGA_INCREMENTA
       ;-
         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ
         mov LATITUDE_MH,a

         cjne a,TEMP_VAR_LAT_MH,DATA_APAGA_INCREMENTA

       ;-
         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ
         mov LATITUDE_ML,a

         cjne a,TEMP_VAR_LAT_ML,DATA_APAGA_INCREMENTA

       ;-
         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ
         mov LATITUDE_L,a

         cjne a,TEMP_VAR_LAT_L,DATA_APAGA_INCREMENTA


         lcall _INCREMENTA_ENDERECO

       ; Carrega LONGITUDE (32BITS)

         lcall _AT45_MAIN_PAGE_READ
         mov LONGITUDE_H,a

         cjne a,TEMP_VAR_LON_H,DATA_APAGA_INCREMENTA


         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ
         mov LONGITUDE_MH,a

         cjne a,TEMP_VAR_LON_MH,DATA_APAGA_INCREMENTA

       ;-
         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ
         mov LONGITUDE_ML,a

         cjne a,TEMP_VAR_LON_ML,DATA_APAGA_INCREMENTA
       ;-
         lcall _INCREMENTA_ENDERECO

         lcall _AT45_MAIN_PAGE_READ
         mov LONGITUDE_L,a

         cjne a,TEMP_VAR_LON_L,DATA_APAGA_INCREMENTA

         ; Se o ponto foi encontrado, apaga

         mov BUFFER_BYTE,#0
         mov dptr,#0                ; DPTR 2

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

        ;-

DATA_APAGA_PARDAL_SAIDA:

         inc AUXR1               ; *** Troca para DPTR1 ***

         ret




;
;
;
VERIFICA_VERSAO:

         mov GUARDA_DPL1,DPL
         mov GUARDA_DPH1,DPH

         inc AUXR1               ; *** Troca para DPTR2 ***

         mov GUARDA_DPL2,DPL
         mov GUARDA_DPH2,DPH

         inc AUXR1               ; *** Troca para DPTR1 ***

         mov dptr,#SRAM_NRO_PACOTE

         inc AUXR1               ; *** Troca para DPTR2 ***

         mov dptr,#EE_NRO_PACOTE

         inc AUXR1               ; *** Troca para DPTR1 ***

         mov R7,#5

VERSAO_L1:

         lcall _SRAM_INTERNA

         movx a,@dptr
         mov b,a
         inc dptr

         inc AUXR1               ; *** Troca para DPTR2 ***

         lcall _EE_READ
         inc dptr

         inc AUXR1               ; *** Troca para DPTR1 ***

         cjne a,b,VERSAO_SAIDA

         djnz R7,VERSAO_L1

         clr FLAG_OK_ERRO

         mov DPL,GUARDA_DPL1
         mov DPH,GUARDA_DPH1

         inc AUXR1               ; *** Troca para DPTR2 ***

         mov DPL,GUARDA_DPL2
         mov DPH,GUARDA_DPH2

         inc AUXR1               ; *** Troca para DPTR1 ***

         RET


VERSAO_SAIDA:

         setb FLAG_OK_ERRO

         mov DPL,GUARDA_DPL1
         mov DPH,GUARDA_DPH1

         inc AUXR1               ; *** Troca para DPTR2 ***

         mov DPL,GUARDA_DPL2
         mov DPH,GUARDA_DPH2

         inc AUXR1               ; *** Troca para DPTR1 ***

         RET








;
;
;
GRAVA_VERSAO:

         mov GUARDA_DPL1,DPL
         mov GUARDA_DPH1,DPH

         inc AUXR1               ; *** Troca para DPTR2 ***

         mov GUARDA_DPL2,DPL
         mov GUARDA_DPH2,DPH

         inc AUXR1               ; *** Troca para DPTR1 ***

         mov dptr,#SRAM_NRO_PACOTE

         inc AUXR1               ; *** Troca para DPTR2 ***

         mov dptr,#EE_NRO_PACOTE

         inc AUXR1               ; *** Troca para DPTR1 ***

         mov R7,#5

GRAVA_VERSAO_L1:

         lcall _SRAM_INTERNA

         movx a,@dptr
         mov b,a
         inc dptr

         inc AUXR1               ; *** Troca para DPTR2 ***

         mov DADO,a
         lcall _EE_WRITE
         inc dptr

         inc AUXR1               ; *** Troca para DPTR1 ***


         djnz R7,GRAVA_VERSAO_L1

         mov DPL,GUARDA_DPL1
         mov DPH,GUARDA_DPH1

         inc AUXR1               ; *** Troca para DPTR2 ***

         mov DPL,GUARDA_DPL2
         mov DPH,GUARDA_DPH2

         inc AUXR1               ; *** Troca para DPTR1 ***

         RET





;
;
;
BYTE_TEXT2HEX:

         movx a,@dptr
         inc dptr       ; dptr 1


         lcall _ASC_TO_HEX
         anl a,#00001111b
         mov R6,A

         movx a,@dptr

         inc dptr       ; dptr 1

         lcall _ASC_TO_HEX
         anl a,#00001111b
         mov R7,A

         mov a,r6
         rl a
         rl a
         rl a
         rl a
         mov r6,a

         orl a,r7

         swap a

         RET








;
; Inicializa modulo ESP8266
;

_WIFI_INI:

      mov dptr,#AT_ATE0
      lcall _SEND_STRING_ESP

      mov a,#70
      lcall _delay_ms


      mov dptr,#AT_CWMODE3
      lcall _SEND_STRING_ESP

      mov a,#60
      lcall _delay_ms

AT_CWQAP_ENTRY:

      mov dptr,#AT_CWQAP
      lcall _SEND_STRING_ESP

      lcall _GET_RESP_OK3

      RET



; Incrementa contador de bytes
INC_COUNT:
      push acc

      clr C

      mov a,Work2L
      add a,#1
      mov Work2L,a

      mov a,Work2H
      addc a,#0
      mov Work2H,a

      pop acc

      RET
      



;
;
;
_SEND_STRING_ESP:

      clr a
      movc a,@a+DPTR
      jz SEND_ESP_SAIDA

      lcall _TXD_ESP

      inc DPTR
      sjmp _SEND_STRING_ESP

SEND_ESP_SAIDA:

      RET


;
;
;
_GET_RESP_OK:

        lcall _RXD_ESP  ; Pula CR
        lcall _RXD_ESP  ; Pula LF


GET_L1:
        lcall _RXD_ESP

        cjne a,#LF,GET_L1

        lcall _RXD_ESP

        cjne a,#'O',GET_SAIDA

        clr FLAG_OK_ERRO

        RET

GET_SAIDA:

        setb FLAG_OK_ERRO

        RET



;
;
_GET_RESP_OK2:

        lcall _RXD_ESP

        cjne a,#'K',_GET_RESP_OK23

        clr FLAG_OK_ERRO
        
        RET

_GET_RESP_OK23:

        cjne a,#'L',_GET_RESP_OK2

        setb FLAG_OK_ERRO

        RET


;
;
_GET_RESP_OK3:

        lcall _RXD_ESP

        cjne a,#'K',_GET_RESP_OK33

        clr FLAG_OK_ERRO

        RET

_GET_RESP_OK33:

        cjne a,#'R',_GET_RESP_OK3

        setb FLAG_OK_ERRO

        RET



;
;
_GET_RESP_OK4:

        lcall _RXD_ESP

        cjne a,#'K',GETRESP_L1

        clr FLAG_OK_ERRO

        ret

GETRESP_L1:

        cjne a,#'T',GETRESP_L2

        clr FLAG_OK_ERRO

        ret


GETRESP_L2:

        cjne a,#'R',GETRESP_L3

        setb FLAG_OK_ERRO
        ret

GETRESP_L3:

        cjne a,#'F',GETRESP_L4
        setb FLAG_OK_ERRO
        ret

GETRESP_L4:

        cjne a,#'t',_GET_RESP_OK4
        setb FLAG_OK_ERRO

        ret














APITA_ERRO:

         ; Se deu erro de conexao, apita
         push acc

         clr LED1
         clr LED2
         setb LED3
         clr LED4
         clr LED5
         clr BUZZER

         mov a,#1
         lcall _DELAY_SEC

         setb BUZZER
         setb LED1
         setb LED2
         setb LED3
         setb LED4
         setb LED5

         pop acc

         RET

; COMANDOS AT


AT_GMR:
      DB 'AT+GMR',CR,LF,0

AT_ATE0:
      DB 'ATE0',CR,LF,0

AT_CIFSR:
      DB 'AT+CIFSR',CR,0

AT_RST:
      DB 'AT+RST',CR,0

AT_CWQAP:
      DB 'AT+CWQAP',CR,LF,0

AT_CWLAP:
      DB 'AT+CWLAP',CR,LF,0

AT_CWJAP:
      DB 'AT+CWJAP="',0

AT_CWMODE3:
      DB 'AT+CWMODE=3',CR,LF,0





AT_CWJAP_PW2:
      DB '012345678"',CR,0

AT_CWJAP_PW3:
      DB '"12345678"',CR,LF,0


AT_CIPSTART:
      DB 'AT+CIPSTART="TCP","curinga.ind.br",80',CR,LF,0


AT_CIPSEND:
      DB 'AT+CIPSEND=59',CR,LF,0



MSG_HTTP1:
      DB 'GET /teste/pack1.cab HTTP/1.1',CR,LF
      DB 'Host: www.curinga.ind.br',CR,LF,CR,LF,0







