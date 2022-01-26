;
; Protocolo de comunicacao do sistema Curinga GPS
; Sumario de comandos:
;
; "D" - Download de pontos para o Curinga
; "I" - Upload de pontos do Curinga para o PC
; "S" - Envia numero de serie para PC
; "X" - Apaga FLASH inteira
; "O" - Inicia teste de buzzer e LED
; "X" - Apaga FLASH inteira
; "P" - Upload de POI no formato .ASC para PC
; "L" - Download de POI no formato .ASC para o Curinga
; "T" - Download do tempo de chegada ao ponto para o Curinga
; "R" - Upload do tempo de chegada ao ponto para o PC
; "A" - LER quilometragem para troca de oleo
; "H" - GRAVAR quilometragem para troca de oleo
; "Z" - Zera acumulador de quilometragem do oleo
; "K" - LER acumulador de quilometragem do oleo
; "F" - LER quantidade de pontos


;
;
;
PROTOCOLO:

         clr ES

         lcall _BUZZER_APITA
         lcall _BUZZER_APITA

RECEPCAO:
       ;   mov a,#CR
       ;   lcall _TXD

       ;  mov a,#LF
       ;  lcall _TXD

         setb LED5

         clr CTS
         
         mov R8,#3
         
         jnb BOTAO_GRAVA,$

         mov a,#20
         lcall _delay_ms

         jnb BOTAO_GRAVA,$

         lcall _RXD_3

         cjne a,#'D',PROTO_EXECUTA_COMANDO3

         sjmp DOWNLOAD


PROTO_EXECUTA_COMANDO3:

         cjne a,#'S',PROTO_EXECUTA_COMANDO4

         cpl LED5

         ljmp ENVIA_NSERIE

PROTO_EXECUTA_COMANDO4:

         cjne a,#'X',PROTO_EXECUTA_COMANDO_5

         ljmp APAGA_TUDO

PROTO_EXECUTA_COMANDO_5:

         cjne a,#'I',PROTO_EXECUTA_COMANDO_6

         ljmp UPLOAD

PROTO_EXECUTA_COMANDO_6:

         cjne a,#'O',PROTO_EXECUTA_COMANDO_7

         ljmp BUZZER_TEST

PROTO_EXECUTA_COMANDO_7:

         cjne a,#'P',PROTO_EXECUTA_COMANDO_8

         ljmp POI_UPLOAD

PROTO_EXECUTA_COMANDO_8:

         cjne a,#'L',PROTO_EXECUTA_COMANDO_9

         ljmp POI_DOWNLOAD

PROTO_EXECUTA_COMANDO_9:

         cjne a,#'T',PROTO_EXECUTA_COMANDO_10

         ljmp TIME_SET_DL

PROTO_EXECUTA_COMANDO_10:

         cjne a,#'R',PROTO_EXECUTA_COMANDO_11

         ljmp TIME_SETUP

PROTO_EXECUTA_COMANDO_11:

         cjne a,#'A',PROTO_EXECUTA_COMANDO_12

        ; ljmp LER_KM_TROCA
          ljmp RECEPCAO

PROTO_EXECUTA_COMANDO_12:

         cjne a,#'Z',PROTO_EXECUTA_COMANDO_13

         ;ljmp ZERA_KM_TROCA
          ljmp RECEPCAO

PROTO_EXECUTA_COMANDO_13:

         cjne a,#'H',PROTO_EXECUTA_COMANDO_14

        ; ljmp GRAVAR_KM_TROCA
         ljmp RECEPCAO

PROTO_EXECUTA_COMANDO_14:

         cjne a,#'K',PROTO_EXECUTA_COMANDO_15

         ;ljmp LER_ACUMULADOR
          ljmp RECEPCAO

PROTO_EXECUTA_COMANDO_15:

         cjne a,#'F',PROTO_EXECUTA_COMANDO_ERRO

         ljmp LER_QTD_PONTOS

PROTO_EXECUTA_COMANDO_ERRO:

         sjmp RECEPCAO




;
; BAIXA OS DADOS PARA A EEPROM DO APARELHO
;

DOWNLOAD:
        ;---
         lcall _BUZZER_APITA
         mov DADO,#0

         setb MEM_WP     ; Desabilita o Write Protect

         clr FLAG_CARREGA_R7

         mov CONTADOR24_L,#0
         mov CONTADOR24_H,#0
         mov CONTADOR24_HH,#0

         mov dptr,#0

         mov BUFFER_BYTE,#0

         mov a,#255
         lcall _delay_ms

DOWN_REC:
         clr CTS

RXD_2:
   	jb	RXD2,$		;Wait for start bit
	mov	R0,#BITTIM/2	;Wait 1/2 bit-time
	djnz	R0,$		;To sample in middle

	jb	RXD2,RXD_2	;Insure valid
	mov	R1,#8		;Read 8 bits
getc1X:
	mov	R0,#BITTIM	;Wait full bit-time
	djnz	R0,$		;For DATA bit
	mov	C,RXD2		;Read bit
	rrc	A		;Shift it into ACC
	djnz	R1,getc1X	;read 8 bits

        setb CTS
       ;--

        cjne a,#CR,DOWN_REC_PULA1

        sjmp DOWN_REC

DOWN_REC_PULA1:

         cjne a,#LF,DOWN_REC_PULA2

         sjmp DOWN_REC

DOWN_REC_PULA2:

         cjne a,#'P',DOWN_REC_PULA3

         ljmp ENCONTROU_P

DOWN_REC_PULA3:

         jb FLAG_CARREGA_R7,CARREGA_R7

         lcall _ASC_TO_HEX
         anl a,#00001111b
         mov R6,A

         cpl FLAG_CARREGA_R7
         sjmp DOWN_REC

CARREGA_R7:

         lcall _ASC_TO_HEX
         anl a,#00001111b
         mov R7,A

         cpl FLAG_CARREGA_R7

         mov a,r6
         rl a
         rl a
         rl a
         rl a
         mov r6,a

         orl a,r7

         swap a

        ;--
        ;cpl LED5

        ;-

         lcall _WRITE_BUFFER1

        ; incrementa o contador para cada byte gravado

         clr C
         mov a,CONTADOR24_L
         add a,#1
         mov CONTADOR24_L,a

         mov a,CONTADOR24_H
         addc a,#0
         mov CONTADOR24_H,a

         mov a,CONTADOR24_HH
         addc a,#0
         mov CONTADOR24_HH,a

         mov A,BUFFER_BYTE

         cjne A,#0FFH,DOWN_REC_INC          ; Quando encher o buffer com 256 bytes, grava
         sjmp  DOWN_REC_ENCHEU

DOWN_REC_INC:

         inc BUFFER_BYTE

         sjmp DOWN_REC


DOWN_REC_ENCHEU:

         lcall _AT45_BUFFER1_TO_MAIN_MEMORY
  
         acall GIRA_BARRA_ESQUERDA

         mov BUFFER_BYTE,#0

         inc dptr

         mov A,DPH
         mov B,#HIGH(2047)
         cjne A,B,DOWN_REC

         mov A,DPL
         mov B,#LOW(2047)
         cjne A,B,DOWN_REC

ENCONTROU_P:

        lcall _AT45_BUFFER1_TO_MAIN_MEMORY   ; grava o que tiver no buffer

        mov R0,CONTADOR24_L
        mov R1,CONTADOR24_H
        mov R2,CONTADOR24_HH
        mov R3,#0

        mov R4,#11
        mov R5,#0
        mov R6,#0
        mov R7,#0

        lcall _DIV_U32U32U32

        ; Salva CONT_PONTOS na EEPROM
        mov CONT_PONTOS_L,R0
        mov dptr,#EE_CONT_PONTOS_L
        mov a,CONT_PONTOS_L
        mov DADO,a
        lcall _EE_WRITE

        mov CONT_PONTOS_H,R1
        mov dptr,#EE_CONT_PONTOS_H
        mov a,CONT_PONTOS_H
        mov DADO,a
        lcall _EE_WRITE

        mov P0,#11111111B

        lcall _BUZZER_SAT_FOUND

       ; clr MEM_WP

        ljmp RECEPCAO


;
;
;

GIRA_BARRA_ESQUERDA:

        mov a,DADO

        cjne a,#0,GIRA2
        mov P0,#11111110b
        sjmp GIRA_SAIDA1
GIRA2:
        cjne a,#1,GIRA3
        mov P0,#11111101b
        sjmp GIRA_SAIDA1

GIRA3:
        cjne a,#2,GIRA4
        mov P0,#11111011b
        sjmp GIRA_SAIDA1

GIRA4:
        cjne a,#3,GIRA5
        mov P0,#11110111b
        sjmp GIRA_SAIDA1

GIRA5:
        mov P0,#11101111b

        mov DADO,#0
        sjmp GIRA_SAIDA2

GIRA_SAIDA1:

        inc DADO

GIRA_SAIDA2:

        ret


;
; Faz  dos dados da flash para o PC
;


UPLOAD:

        mov CONTADOR16_L,#0
        mov CONTADOR16_H,#0

        mov a,#CR
        lcall _TXD

        mov a,#LF
        lcall _TXD

        mov R8,#11

UPLOAD_L1:

        mov BUFFER_BYTE,#0
        mov dptr,#0

        lcall _RODA_ADDRESS

UPLOAD_CONTINUA_LOOP:

        lcall _AT45_MAIN_PAGE_READ

        swap a

        mov b,a

        anl a,#11110000b

        rr a
        rr a
        rr a
        rr a

        lcall _HEX_TO_ASCII

        lcall _TXD

        mov a,b

        anl a,#00001111b

        lcall _HEX_TO_ASCII

        lcall _TXD

        lcall _INCREMENTA_ENDERECO

        djnz R8,UPLOAD_CONTINUA_LOOP
        mov R8,#11


           ; xoxo
          ;    mov a,#CR
          ;    lcall _TXD
          ;    mov a,#LF
          ;    lcall _TXD

       ;lcall _DELAY_50US
        lcall GIRA_BARRA_DIREITA

       ;cpl LED5


        ; INCREMENTA CONTADOR16 E COMPARA COM O TOTAL DA MEMORIA

        clr C
        mov a,CONTADOR16_L
        add a,#1
        mov CONTADOR16_L,a

        mov a,CONTADOR16_H
        addc a,#0
        mov CONTADOR16_H,a

        mov a,CONTADOR16_L
        cjne a,CONT_PONTOS_L,UPLOAD_L1

        mov a,CONTADOR16_H
        cjne a,CONT_PONTOS_H,UPLOAD_L1

UPLOAD_EXIT:

        mov a,#CR
        lcall _TXD

        mov a,#LF
        lcall _TXD


        mov a,#'P'
        lcall _TXD

        lcall _BUZZER_SAT_FOUND
        setb LED5

        mov P0,#11111111B

        ljmp RECEPCAO
        

;
;
;

GIRA_BARRA_DIREITA:

        mov a,DADO

        cjne a,#0,GIRAD2
        mov P0,#11111110b
        sjmp GIRAD_SAIDA1
GIRAD2:
        cjne a,#1,GIRAD3
        mov P0,#11111101b
        sjmp GIRAD_SAIDA1

GIRAD3:
        cjne a,#2,GIRAD4
        mov P0,#11111011b
        sjmp GIRAD_SAIDA1

GIRAD4:
        cjne a,#3,GIRAD5
        mov P0,#11110111b
        sjmp GIRAD_SAIDA1

GIRAD5:
        mov P0,#11101111b

        mov DADO,#0
        sjmp GIRAD_SAIDA2

GIRAD_SAIDA1:

        inc DADO

GIRAD_SAIDA2:

        ret




;
; ENVIA NUMERO DE SERIE
;


ENVIA_NSERIE:

         mov dptr,#N_SERIE

ENVIA_NSERIE_L1:

         clr a
         movc a,@a+dptr

         jz ENVIA_NSERIE_L1_SAIDA

         lcall _TXD

         inc dptr

         mov a,#10
         lcall _delay_ms

         sjmp ENVIA_NSERIE_L1

ENVIA_NSERIE_L1_SAIDA:

         ljmp RECEPCAO



APAGA_TUDO:

         lcall _BUZZER_APITA

         setb MEM_WP     ; Desabilita o Write Protect

         lcall _AT45_CLEAR_ALL

         lcall _BUZZER_APITA

       ;  clr MEM_WP

        ; Salva CONT_PONTOS na EEPROM
         mov a,#0ffh
         mov dptr,#EE_CONT_PONTOS_L
         mov DADO,a
         lcall _EE_WRITE

         mov a,#0ffh
         mov dptr,#EE_CONT_PONTOS_H
         mov DADO,a
         lcall _EE_WRITE

         mov CONT_PONTOS_L,#1
         Mov CONT_PONTOS_H,#0


         mov a,#'!'
         lcall _TXD

         setb led5

         ljmp RECEPCAO


; Testa o buzzer

BUZZER_TEST:

         mov R7,#60

BUZ_T1:
         cpl buzzer
         cpl led5

         mov a,#80
         lcall _delay_ms

         djnz R7,BUZ_T1

         mov R7,#5

BUZ_T2:
         cpl buzzer
         cpl led5

         mov a,#200
         lcall _delay_ms

         djnz R7,BUZ_T2

BUZ_T3:

         cpl buzzer
         cpl led5

         mov a,#20
         lcall _delay_ms

         jmp BUZ_T3

;
; Faz upload dos ponto de interesse (POI)
;


POI_UPLOAD:

        mov CONTADOR16_L,#0
        mov CONTADOR16_H,#0


POI_UPLOAD_L1:


        mov BUFFER_BYTE,#9
        mov dptr,#0

        lcall _RODA_ADDRESS

        lcall _AT45_MAIN_PAGE_READ

        jb acc.7,POI_UPLOAD_CONTINUA ; xuxu


        ; INCREMENTA CONTADOR16 E COMPARA COM O TOTAL DA MEMORIA

        clr C
        mov a,CONTADOR16_L
        add a,#1
        mov CONTADOR16_L,a

        mov a,CONTADOR16_H
        addc a,#0
        mov CONTADOR16_H,a

        mov a,CONTADOR16_L
        cjne a,CONT_PONTOS_L,POI_UPLOAD_L1

        mov a,CONTADOR16_H
        cjne a,CONT_PONTOS_H,POI_UPLOAD_L1

        ljmp POI_UPLOAD_EXIT

POI_UPLOAD_CONTINUA:

         push acc
         ; gato para apagar o bit de atributo
         lcall _AT45_MAIN_PAGE_TO_BUFFER1_TRANSF
         pop acc
         
         clr acc.7

         lcall _AT45_MAIN_PAGE_WRITE_B1




        mov BUFFER_BYTE,#0
        mov dptr,#0

        lcall _RODA_ADDRESS

POI_UPLOAD_CONTINUA_LOOP:

        ; Carrega LATITUDE (32BITS)

        lcall _AT45_MAIN_PAGE_READ
        mov LATITUDE_H,a
       ;-
        lcall _INCREMENTA_ENDERECO

        lcall _AT45_MAIN_PAGE_READ
        mov LATITUDE_MH,a
       ;-
        lcall _INCREMENTA_ENDERECO

        lcall _AT45_MAIN_PAGE_READ
        mov LATITUDE_ML,a
       ;-
        lcall _INCREMENTA_ENDERECO

        lcall _AT45_MAIN_PAGE_READ
        mov LATITUDE_L,a

        lcall _INCREMENTA_ENDERECO

       ; Carrega LONGITUDE (32BITS)

        lcall _AT45_MAIN_PAGE_READ
        mov LONGITUDE_H,a


        lcall _INCREMENTA_ENDERECO

        lcall _AT45_MAIN_PAGE_READ
        mov LONGITUDE_MH,a
       ;-
        lcall _INCREMENTA_ENDERECO

        lcall _AT45_MAIN_PAGE_READ
        mov LONGITUDE_ML,a
       ;-
        lcall _INCREMENTA_ENDERECO

        lcall _AT45_MAIN_PAGE_READ
        mov LONGITUDE_L,a

        ;
        ; Transforme LONGITUDE EM DEGREES
        ;
        mov R3,LONGITUDE_H         ; Divide LONGITUDE por 60
        mov R2,LONGITUDE_MH
        mov R1,LONGITUDE_ML
        mov R0,LONGITUDE_L

        mov R7,#0
        mov R6,#0
        mov R5,#0
        mov R4,#6

        lcall _DIV_U32U32U32

        mov a,R0   ; Transfere resultado para R4,R5,R6 e R7
        mov R4,a

        mov a,R1
        mov R5,a

        mov a,R2
        mov R6,a

        mov a,R3
        mov R7,a

        ; Carrega nas variaveis para conversao

        mov a,R4
        mov R1,a

        mov a,R5
        mov R2,a

        mov a,R6
        mov R3,a

        mov a,R7
        mov R4,a


        lcall _BINTOASC32    ; Converte para ASCII

        ; Imprime

        mov a,#'-'
        lcall _TXD

        lcall _DELAY_50US

        mov a,58h
        lcall _TXD

        lcall _DELAY_50US

        mov a,59h
        lcall _TXD

        lcall _DELAY_50US

        mov a,5Ah
        lcall _TXD

        lcall _DELAY_50US

        mov a,#'.'
        lcall _TXD

        mov a,5Bh
        lcall _TXD

        lcall _DELAY_50US

        mov a,5Ch
        lcall _TXD

        lcall _DELAY_50US

        mov a,5Dh
        lcall _TXD

        lcall _DELAY_50US

        mov a,5Eh
        lcall _TXD

        lcall _DELAY_50US

        mov a,5Fh
        lcall _TXD

        lcall _DELAY_50US

        mov a,#','
        lcall _TXD

        ;
        ; Transforme LATITUDE EM DEGREES
        ;
        
        mov R3,LATITUDE_H         ; Divide latitude por 60
        mov R2,LATITUDE_MH
        mov R1,LATITUDE_ML
        mov R0,LATITUDE_L

        mov R7,#0
        mov R6,#0
        mov R5,#0
        mov R4,#6

        lcall _DIV_U32U32U32

        mov a,R0   ; Transfere resultado para R4,R5,R6 e R7
        mov R4,a

        mov a,R1
        mov R5,a

        mov a,R2
        mov R6,a

        mov a,R3
        mov R7,a

        ; Carrega nas variaveis para conversao

        mov a,R4
        mov R1,a

        mov a,R5
        mov R2,a

        mov a,R6
        mov R3,a

        mov a,R7
        mov R4,a


        lcall _BINTOASC32    ; Converte para ASCII

        ; Imprime

        mov a,#'-'
        lcall _TXD

        lcall _DELAY_50US

        mov a,58h
        lcall _TXD

        lcall _DELAY_50US

        mov a,59h
        lcall _TXD

        lcall _DELAY_50US

        mov a,5Ah
        lcall _TXD

        lcall _DELAY_50US

        mov a,#'.'
        lcall _TXD

        lcall _DELAY_50US

        mov a,5Bh
        lcall _TXD

        lcall _DELAY_50US

        mov a,5Ch
        lcall _TXD

        lcall _DELAY_50US

        mov a,5Dh
        lcall _TXD

        lcall _DELAY_50US

        mov a,5Eh
        lcall _TXD

        lcall _DELAY_50US

        mov a,5Fh
        lcall _TXD

        lcall _DELAY_50US

        mov a,#','
        lcall _TXD

        lcall _DELAY_50US

        mov a,#'"'
        lcall _TXD

        lcall _DELAY_50US


        mov a,#'D'
        lcall _TXD

        lcall _DELAY_50US


        mov a,#'I'
        lcall _TXD

        lcall _DELAY_50US


        mov a,#'R'
        lcall _TXD

        lcall _DELAY_50US


        mov a,#'E'
        lcall _TXD

        lcall _DELAY_50US


        mov a,#'C'
        lcall _TXD

        lcall _DELAY_50US


        mov a,#'A'
        lcall _TXD

        lcall _DELAY_50US


        mov a,#'O'
        lcall _TXD

        lcall _DELAY_50US

        mov a,#' '
        lcall _TXD

        lcall _DELAY_50US

        ; Pega direcao

        mov BUFFER_BYTE,#9
        mov dptr,#0

        lcall _RODA_ADDRESS

        lcall _AT45_MAIN_PAGE_READ
        clr acc.7    ; apaga bit de atributo

        mov DIRECAO_H,a

        lcall _INCREMENTA_ENDERECO

        lcall _AT45_MAIN_PAGE_READ
        mov DIRECAO_L,a
        
        
        mov R1,DIRECAO_L
        mov R2,DIRECAO_H
        
        lcall _BINTOASC32

        mov a,5DH
        lcall _TXD

        lcall _DELAY_50US

        mov a,5EH
        lcall _TXD

        lcall _DELAY_50US


        mov a,5FH
        lcall _TXD

        lcall _DELAY_50US

        mov a,#' '
        lcall _TXD

        lcall _DELAY_50US

        mov a,#'-'
        lcall _TXD

        lcall _DELAY_50US

        mov a,#' '
        lcall _TXD

        lcall _DELAY_50US

        ; Pega velocidade

        mov BUFFER_BYTE,#8
        mov dptr,#0

        lcall _RODA_ADDRESS

        lcall _AT45_MAIN_PAGE_READ
        
        mov VELOCIDADE,a
        lcall _ARREDONDA_VEL
        mov a,VARIAVEL

        cjne a,#10,POI_TESTA_02

        mov a,#'2'
        lcall _TXD

        lcall _DELAY_50US

        mov a,#'0'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'k'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'m'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'/'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'h'
        lcall _TXD

        ljmp POI_TESTA_SAIDA


POI_TESTA_02:
        cjne a,#21,POI_TESTA_03

        mov a,#'4'
        lcall _TXD

        lcall _DELAY_50US

        mov a,#'0'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'k'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'m'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'/'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'h'
        lcall _TXD

        ljmp POI_TESTA_SAIDA


POI_TESTA_03:
        cjne a,#27,POI_TESTA_04
        
        mov a,#'5'
        lcall _TXD

        lcall _DELAY_50US

        mov a,#'0'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'k'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'m'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'/'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'h'
        lcall _TXD

        ljmp POI_TESTA_SAIDA

POI_TESTA_04:
        cjne a,#32,POI_TESTA_05
        
        mov a,#'6'
        lcall _TXD

        lcall _DELAY_50US

        mov a,#'0'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'k'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'m'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'/'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'h'
        lcall _TXD

        ljmp POI_TESTA_SAIDA

POI_TESTA_05:
        cjne a,#38,POI_TESTA_06

        mov a,#'7'
        lcall _TXD

        lcall _DELAY_50US

        mov a,#'0'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'k'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'m'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'/'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'h'
        lcall _TXD

        ljmp POI_TESTA_SAIDA

POI_TESTA_06:
        cjne a,#43,POI_TESTA_07

        mov a,#'8'
        lcall _TXD

        lcall _DELAY_50US

        mov a,#'0'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'k'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'m'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'/'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'h'
        lcall _TXD

        sjmp POI_TESTA_SAIDA

POI_TESTA_07:

        cjne a,#48,POI_TESTA_08
        
        mov a,#'9'
        lcall _TXD

        lcall _DELAY_50US

        mov a,#'0'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'k'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'m'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'/'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'h'
        lcall _TXD

        sjmp POI_TESTA_SAIDA

POI_TESTA_08:

        cjne a,#54,POI_TESTA_SAIDA

        mov a,#'1'
        lcall _TXD

        lcall _DELAY_50US

        mov a,#'0'
        lcall _TXD

        lcall _DELAY_50US

        mov a,#'0'
        lcall _TXD

        lcall _DELAY_50US

        mov a,#'k'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'m'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'/'
        lcall _TXD

        lcall _DELAY_50US
        mov a,#'h'
        lcall _TXD


POI_TESTA_SAIDA:

        lcall _DELAY_50US
       
        mov a,#'"'
        lcall _TXD


             mov a,#CR
             lcall _TXD

               mov a,#LF
            lcall _TXD

        lcall _DELAY_50US


        cpl LED5


        ; INCREMENTA CONTADOR16 E COMPARA COM O TOTAL DA MEMORIA

        clr C
        mov a,CONTADOR16_L
        add a,#1
        mov CONTADOR16_L,a

        mov a,CONTADOR16_H
        addc a,#0
        mov CONTADOR16_H,a

        mov a,CONTADOR16_L
        cjne a,CONT_PONTOS_L,POI_UPLOAD_L1_TRAMPOLIM

        mov a,CONTADOR16_H
        cjne a,CONT_PONTOS_H,POI_UPLOAD_L1_TRAMPOLIM

POI_UPLOAD_EXIT:


        lcall _BUZZER_SAT_FOUND
        setb LED5

        ljmp RECEPCAO

POI_UPLOAD_L1_TRAMPOLIM:
        
        Ljmp POI_UPLOAD_L1



;
;   EXECUTA DOWNLOAD DE POIs PARA FLASH
;
;


POI_DOWNLOAD:

        ;---
         lcall _BUZZER_APITA

         setb MEM_WP          ; Desabilita o Write Protect

         clr FLAG_POI_DOWNLOAD_START

         mov CONTADOR24_L,#0
         mov CONTADOR24_H,#0

         lcall _SRAM_INTERNA

         mov dptr,#SRAM_BUFFER

         mov BUFFER_BYTE,#0

         mov a,#255
         lcall _delay_ms

DOWN_REC_POI:

         lcall _rxd_2

         cpl LED5
       ;--
VERIFICA_P:

         cjne a,#'P',DOWN_REC_PULA1_POI

         ;clr MEM_WP

         lcall _BUZZER_APITA

         ljmp RECEPCAO


DOWN_REC_PULA1_POI:

         cjne a,#'-',DOWN_REC_PULA2_POI

         jb FLAG_POI_DOWNLOAD_START,DOWN_REC_PULA2_POI

         setb  FLAG_POI_DOWNLOAD_START   ;Seta flag para iniciar carregamento do buffer

         mov DPTR,#SRAM_BUFFER             ;Carrega primeiro caractere ('-') no buffer
         movx @dptr,a                      ;depois sai

         inc dptr

         sjmp DOWN_REC_POI


DOWN_REC_PULA2_POI:

         jnb FLAG_POI_DOWNLOAD_START,DOWN_REC_POI

         movx @dptr,a

         inc DPTR

         cjne a,#CR,DOWN_REC_POI  ;Quando chegar ao final da linha, converte para binario

         
         ;Envia caractere de controle para host esperar
         
       ;  mov a,#'w'
       ;  lcall _TXD


CONV_DATA_BUFFER:

         mov R7,#8    ;seta contador de bytes a serem gravados
        ;--
         clr LED5

         ; Converte LONGITUDE NA MEMORIA

         mov DPTR,#SRAM_BUFFER
         mov R0,#BUFFER_MONTAGEM

         inc DPTR    ; pula o "-"

CONV_DATA_LOOP:

         lcall _SRAM_READ

         cjne a,#'.',CONV_DATA_CONT

         inc DPTR                       ; Pula ponto "."
         lcall _SRAM_READ

CONV_DATA_CONT:

         mov @R0,a
         inc R0
         inc DPTR

         djnz R7,CONV_DATA_LOOP

         mov @R0,#0

         mov R0,#BUFFER_MONTAGEM

         lcall _DECBIN

        ;
        ; Transforme DEGREES em DEGREE,MIN
        ;

         mov R3,#0         ; Multiplica resultado de DECBIN por 60
         mov R2,#0
         mov R1,#0
         mov R0,#6

         lcall _MUL_U32U32U32

         mov LONGITUDE_L,R8       ;Carrega o registrador LONGITUDE(32bits)com o resultado
         mov LONGITUDE_ML,R9
         mov LONGITUDE_MH,R10
         mov LONGITUDE_H,R11

         ;Converte LATITUDE

         mov R7,#8

         mov DPTR,#SRAM_BUFFER+3
         mov R0,#BUFFER_MONTAGEM

         ; Procura o proximo "-"
CONV_DATA_CONT2:

         lcall _SRAM_READ

         cjne a,#'-',CONV_DATA_INC

         inc DPTR
         sjmp CONV_DATA_LOOP2

CONV_DATA_INC:

         inc DPTR
         sjmp CONV_DATA_CONT2


CONV_DATA_LOOP2:

         lcall _SRAM_READ
         cjne a,#'.',CONV_DATA_CONT3

         inc DPTR                      ; Pula ponto "."
         lcall _SRAM_READ


CONV_DATA_CONT3:

         mov @R0,a
         inc R0
         inc DPTR

         djnz R7,CONV_DATA_LOOP2

         mov @R0,#0

         mov R0,#BUFFER_MONTAGEM

         lcall _DECBIN


        ;
        ; Transforme DEGREES em DEGREE,MIN
        ;
         mov R3,#0         ; Multiplica resultado de DECBIN por 60
         mov R2,#0
         mov R1,#0
         mov R0,#6

         lcall _MUL_U32U32U32

         mov LATITUDE_L,R8       ;Carrega o registrador LATITUDE(32bits)com o resultado
         mov LATITUDE_ML,R9
         mov LATITUDE_MH,R10
         mov LATITUDE_H,R11

         ; Procura " para achar direcao

         mov DPTR,#SRAM_BUFFER

ACHA_DIR_LOOP:

         lcall _SRAM_READ

         inc DPTR

         cjne a,#'"',ACHA_DIR_LOOP

         ;Procura numeros em ASCII

ACHA_DIR_LOOP2:

         lcall _SRAM_READ
         mov b,#'9'

         lcall _TESTA

         jb FLAG_EH_MAIOR,ACHA_DIR_LOOP_INC

         lcall _SRAM_READ
         mov b,#'0'
         lcall _TESTA

         jb FLAG_EH_MENOR,ACHA_DIR_LOOP_INC

         ; Se byte for <'9' ou >'0' entao eh valido!

         sjmp TRANSF_DIR

ACHA_DIR_LOOP_INC:

         inc DPTR
         sjmp ACHA_DIR_LOOP2

TRANSF_DIR:
         mov R7,#3
         mov R0,#BUFFER_MONTAGEM

TRANSF_DIR_LOOP:

         lcall _SRAM_READ
         mov @R0,a

         inc R0
         inc DPTR

         djnz R7,TRANSF_DIR_LOOP

         mov @R0,#0

         mov R0,#BUFFER_MONTAGEM
         lcall _DECBIN

         mov DIRECAO_L,R4
         mov DIRECAO_H,R5

        ; Acha agora a velocidade
         mov DPTR,#SRAM_BUFFER

ACHA_VEL_LOOP:

         lcall _SRAM_READ

         inc DPTR

         cjne a,#'k',ACHA_VEL_LOOP

         lcall _DEC_DPTR ; Volta TRES posicoes
         lcall _DEC_DPTR
         lcall _DEC_DPTR

TRANSF_VEL:
         mov R7,#2
         mov R0,#BUFFER_MONTAGEM

TRANSF_VEL_LOOP:

         lcall _SRAM_READ
         mov @R0,a

         inc R0
         inc DPTR

         djnz R7,TRANSF_VEL_LOOP

         mov @R0,#0

         mov R0,#BUFFER_MONTAGEM
         lcall _DECBIN

         mov A,R4  ; RESULTADO EM ACC (EM Km/h)

         ; Transforma em Knot

         lcall TRANSF_KM_NKOT


         ;+-------------------+
         ;|Grava dado na FLASH|
         ;+-------------------+

         mov a,CONT_PONTOS_L     ; Faz ENDERECO <- (CRD_COUNTER * 11)
         mov r3,a

         mov a,CONT_PONTOS_H
         mov r2,a

         mov r0,#0
         mov r1,#11

         lcall _MUL16

        ; Agora coloca RESULTADO no dptr

         mov a,R4
         mov BUFFER_BYTE,a

         mov a,R5
         mov dpl,a

         mov a,R6
         mov dph,a

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
         
         ; Envie caractere de controle para prosseguir
         
         clr  FLAG_POI_DOWNLOAD_START   ;Zera flag para reiniciar carregamento do buffer

         mov DPTR,#SRAM_BUFFER

         lcall _SRAM_INTERNA

         mov a,#'g'
         lcall _TXD

         ljmp DOWN_REC_POI
         
;
; Download do tempo de chegada no ponto para o Curinga
;

TIME_SET_DL:

         clr FLAG_CARREGA_R7

TIME_SET_L1:

         clr CTS

         lcall _RXD_2

         jnb RXD2,$

         setb CTS

         jb FLAG_CARREGA_R7,TIME_SET_CARREGA_R7

         lcall _ASC_TO_HEX
         anl a,#00001111b
         mov R6,A

         cpl FLAG_CARREGA_R7
         sjmp TIME_SET_L1

TIME_SET_CARREGA_R7:

         lcall _ASC_TO_HEX
         anl a,#00001111b
         mov R7,A

         cpl FLAG_CARREGA_R7

         mov a,r6
         rl a
         rl a
         rl a
         rl a
         mov r6,a

         orl a,r7

         mov TEMPO_CHEGADA,a


         mov b,#15
         lcall _TESTA

         jb FLAG_EH_MENOR,TIME_SET_L2
         jb FLAG_EH_IGUAL,TIME_SET_L2

         ;if > 15 then TEMPO_CHEGADA=15

         mov TEMPO_CHEGADA,#15

TIME_SET_L2:

         mov b,#6
         lcall _TESTA

         jb FLAG_EH_MAIOR,TIME_SET_L3
         jb FLAG_EH_IGUAL,TIME_SET_L3

         ;if <6 then TEMPO_CHEGADA=6
         mov TEMPO_CHEGADA,#6

TIME_SET_L3:

         mov DADO,TEMPO_CHEGADA
         mov DPTR,#EE_TEMPO_CHEGADA
         lcall _EE_WRITE

         lcall _BUZZER_APITA

         ljmp RECEPCAO
         
         
;
; Upload do tempo de chegada gravado na memoria do curinga para o PC
;

TIME_SETUP:

        mov dptr,#EE_TEMPO_CHEGADA
        lcall _EE_READ

        mov b,a

        anl a,#11110000b

        rr a
        rr a
        rr a
        rr a

        lcall _HEX_TO_ASCII

        lcall _TXD

        mov a,b

        anl a,#00001111b

        lcall _HEX_TO_ASCII

        lcall _TXD

        mov a,#CR
        lcall _TXD

        mov a,#LF
        lcall _TXD

        ljmp RECEPCAO




;
; Ler quantidade de pontos
;

LER_QTD_PONTOS:

         mov R1,CONT_PONTOS_L
         mov R2,CONT_PONTOS_H
         mov R3,#0
         mov R4,#0
         
         lcall _BINTOASC32

         mov a,5bh
         lcall _TXD

         mov a,5ch
         lcall _TXD

         mov a,5dh
         lcall _TXD

         mov a,5eh
         lcall _TXD

         mov a,5fh
         lcall _TXD


         lcall _BUZZER_APITA

         ljmp RECEPCAO













