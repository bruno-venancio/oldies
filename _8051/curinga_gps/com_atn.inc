;
;
;

LOAD_DELTA_XY:
                push DPL
                push DPH
                push PSW

                ; Calcula SRAM_PONTO - LATITUDE  resultado em X2

                ; Carrega variaveis de LATITUDE

                mov  XL,LATITUDE_L
                mov  XML,LATITUDE_ML
                mov  XMH,LATITUDE_MH
                mov  XH,LATITUDE_H

                ; +---------------------------+
                ; | CARREGA LONGITUDE DA SRAM |
                ; +---------------------------+

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


SUBTRAI_CPL_X:
                ; Subtrai em 32 bits

                clr C

                mov a,SRAM_PONTO_L
                subb a,XL
                mov XL,a

                mov a,SRAM_PONTO_ML
                subb a,XML
                mov XML,a

                mov a,SRAM_PONTO_MH
                subb a,XMH
                mov XMH,a

                mov a,SRAM_PONTO_H
                subb a,XH
                mov XH,a

                ; inverte

                clr C

                mov a,XL
                cpl a
                add a,#1
                mov XL,a

                mov a,XML
                cpl a
                addc a,#0
                mov XML,a

                mov a,XMH
                cpl a
                addc a,#0
                mov XMH,a

                mov a,XH
                cpl a
                addc a,#0
                mov XH,a

AGORA_Y:
                ; Carrega variaveis de LONGITUDE

                mov  YL,LONGITUDE_L
                mov  YML,LONGITUDE_ML
                mov  YMH,LONGITUDE_MH
                mov  YH,LONGITUDE_H

                ; +---------------------------+
                ; | CARREGA LONGITUDE DA SRAM |
                ; +---------------------------+

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

                mov SRAM_PONTO_L,A

SUBTRAI_CPL_Y:
                ; Subtrai em 32 bits

                clr C

                mov a,SRAM_PONTO_L
                subb a,YL
                mov YL,a

                mov a,SRAM_PONTO_ML
                subb a,YML
                mov YML,a

                mov a,SRAM_PONTO_MH
                subb a,YMH
                mov YMH,a

                mov a,SRAM_PONTO_H
                subb a,YH
                mov YH,a

                ; Inverte

                clr C

                mov a,YL
                cpl a
                add a,#1
                mov YL,a

                mov a,YML
                cpl a
                addc a,#0
                mov YML,a

                mov a,YMH
                cpl a
                addc a,#0
                mov YMH,a

                mov a,YH
                cpl a
                addc a,#0
                mov YH,a

SUBTRAI_SAIDA:


                pop PSW
                pop DPH
                pop DPL

                RET












_COMPARA_ATAN:

         mov R6,DIR_COMP_L          ; Primeiro Guarda DIR_COMP
         mov R7,DIR_COMP_H


; INCREMENTA O DIRCOMP EM 30 ( 16 BITS)

         ; Incrementa 30 em 16 bits
         clr C

         mov a,DIR_COMP_L
         add a,#12
         mov DIR_COMP_L,a

         mov a,DIR_COMP_H
         addc a,#0
         mov DIR_COMP_H,a

        ; Testa se da mais que 360

         mov a,DIR_COMP_H
         mov b,#HIGH(360)

         lcall _TESTA

         jb FLAG_EH_MAIOR,ATN_TESTA_360_MAIOR
         jb FLAG_EH_IGUAL,ATN_TESTA_360_L
         jb FLAG_EH_MENOR,ATN_COMP1_DIR_H     ; CONTINUA NORMALMENTE

ATN_TESTA_360_L:

         mov a,DIR_COMP_L
         mov b,#LOW(360)

         lcall _TESTA

         jb FLAG_EH_MAIOR,ATN_TESTA_360_MAIOR
         jb FLAG_EH_IGUAL,ATN_COMP1_DIR_H     ; CONTINUA NORMALMENTE
         jb FLAG_EH_MENOR,ATN_COMP1_DIR_H     ; CONTINUA NORMALMENTE


ATN_TESTA_360_MAIOR:

         ; PEGA DIRCOMP(> 360) - 360
         clr C
         mov a,DIR_COMP_L
         subb a,#LOW(360)
         mov DIR_COMP_L,a

         mov a,DIR_COMP_H
         subb a,#HIGH(360)
         mov DIR_COMP_H,a


; Compara DIRECAO com DIR_COMP
ATN_COMP1_DIR_H:

         mov a,DIRECAO_H
         mov b,DIR_COMP_H

         lcall _TESTA

         jb FLAG_EH_MAIOR,ATN_TESTA_MAIOR_30H
         jb FLAG_EH_IGUAL,ATN_COMP1_DIR_L
         jb FLAG_EH_MENOR,ATN_COMP_CONTINUA

ATN_COMP1_DIR_L:

         mov a,DIRECAO_L
         mov b,DIR_COMP_L

         lcall _TESTA

         jb FLAG_EH_MAIOR,ATN_TESTA_MAIOR_30H
         jb FLAG_EH_IGUAL,ATN_COMP_CONTINUA
         jb FLAG_EH_MENOR,ATN_COMP_CONTINUA

;---------------------
ATN_TESTA_MAIOR_30H:

         mov a,R7           ; DIR_COMP_H
         mov b,#HIGH(12)

         lcall _TESTA

         jb FLAG_EH_MAIOR,ATN_TESTA_MAIOR_330H
         jb FLAG_EH_IGUAL,ATN_TESTA_MAIOR_30L
         jb FLAG_EH_MENOR,ATN_COMP_CONTINUA

ATN_TESTA_MAIOR_30L:

         mov a,R6           ; DIRECAO_L
         mov b,#LOW(12)

         lcall _TESTA

         jb FLAG_EH_MAIOR,ATN_TESTA_MAIOR_330H
         jb FLAG_EH_IGUAL,ATN_COMP_CONTINUA
         jb FLAG_EH_MENOR,ATN_COMP_CONTINUA

ATN_TESTA_MAIOR_330H:

         mov a,R7           ; DIR_COMP_H
         mov b,#HIGH(348)

         lcall _TESTA

         jb FLAG_EH_MAIOR,ATN_COMP_CONTINUA
         jb FLAG_EH_IGUAL,ATN_TESTA_MAIOR_330L
         jb FLAG_EH_MENOR,ATN_COMP_ERRO2

ATN_TESTA_MAIOR_330L:

         mov a,R6           ; DIRECAO_L
         mov b,#LOW(348)

         lcall _TESTA

         jb FLAG_EH_MENOR,ATN_COMP_ERRO2
        ; jb FLAG_EH_MAIOR,COMP_CONTINUA
        ; jb FLAG_EH_IGUAL,COMP_CONTINUA

;---------------------

ATN_COMP_CONTINUA:

; DECREMENTA O DIRCOMP EM 30 ( 16 BITS)

      ; DIR_COMP <- DIR_COMP - 60

         clr C

         mov a,DIR_COMP_L
         subb a,#24
         mov DIR_COMP_L,a

         mov a,DIR_COMP_H
         subb a,#0
         mov DIR_COMP_H,a

         ;*****************
         jnc ATN_COMP2_DIR_H
         ;*****************
         ;Se deu carry, a subtracao deu < 0

         setb FLAG_MENOR_ZERO

         clr C

         ; Complementa 1 em 16 bits

         mov a,#low(0ffffh)
         subb a,DIR_COMP_L
         mov DIR_COMP_L,a

         mov a,#high(0ffffh)
         subb a,DIR_COMP_H
         mov DIR_COMP_H,a

        ;-- Guarda Diferenca

         mov a,DIR_COMP_L
         mov ACIMA_360,A

         ;----
         clr C

         mov a,#LOW(360)
         subb a,DIR_COMP_L
         mov DIR_COMP_L,a

         mov a,#HIGH(360)
         subb a,DIR_COMP_H
         mov DIR_COMP_H,a

ATN_COMP2_DIR_H:

         mov a,DIRECAO_H
         mov b,DIR_COMP_H

         lcall _TESTA

         jb FLAG_EH_MAIOR,ATN_COMP_OK
         jb FLAG_EH_IGUAL,ATN_COMP2_DIR_L
         jb FLAG_EH_MENOR,ATN_COMP_ERRO

ATN_COMP2_DIR_L:

         mov a,DIRECAO_L
         mov b,DIR_COMP_L

         lcall _TESTA

         jb FLAG_EH_MAIOR,ATN_COMP_OK
         jb FLAG_EH_IGUAL,ATN_COMP_OK
        ;jb FLAG_EH_MENOR,ATN_COMP_ERRO

ATN_COMP_ERRO:

         jnb FLAG_MENOR_ZERO,ATN_COMP_ERRO2

         ; Agora testa a sobra que vem depois do 360

         clr C
         mov a,#24
         subb a,ACIMA_360
         mov ACIMA_360,a

         mov a,DIRECAO_L
         mov b,ACIMA_360

         lcall _TESTA

         jb FLAG_EH_IGUAL,ATN_COMP_OK
         jb FLAG_EH_MENOR,ATN_COMP_OK

         ;jb FLAG_EH_MAIOR,ATN_COMP_ERRO2

ATN_COMP_ERRO2:
         setb FLAG_ANG_ERRO

         sjmp ATN_COMP_DIR_SAIDA

ATN_COMP_OK:
         clr FLAG_ANG_ERRO

ATN_COMP_DIR_SAIDA:

         clr FLAG_MENOR_ZERO

         RET












