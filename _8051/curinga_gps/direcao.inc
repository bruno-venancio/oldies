
_VER_ABERTURA:

         push DPL
         push DPH

         ; +------------------------------------+
         ; |PROCURA PONTOS COM A MESMA ABERTURA |
         ; +------------------------------------+

         mov dptr,#SRAM_PONTOS+9     ; Aponta para buffer de pontos na SRAM
         lcall _RODA_DPTR_SRAM2      ; Faz DPTR +(CONTADOR8 * 11)
         lcall _SRAM_INTERNA
         lcall _SRAM_READ
         
         clr acc.7                   ; Limpa MSB para o caso de pontos marcados por usuario

         mov DIR_COMP_H,a
         inc dptr

         lcall _SRAM_READ

         mov DIR_COMP_L,a

         acall COMPARA_ABERTURA

         pop DPH
         pop DPL
         ret



;
;
;
;

COMPARA_ABERTURA:

         mov R6,DIR_COMP_L          ; Primeiro Guarda DIR_COMP
         mov R7,DIR_COMP_H


; INCREMENTA O DIRCOMP EM 30 ( 16 BITS)

         ; Incrementa 30 em 16 bits
         clr C

         mov a,DIR_COMP_L
         add a,#45
         mov DIR_COMP_L,a

         mov a,DIR_COMP_H
         addc a,#0
         mov DIR_COMP_H,a

        ; Testa se da mais que 360

         mov a,DIR_COMP_H
         mov b,#HIGH(360)

         lcall _TESTA

         jb FLAG_EH_MAIOR,TESTA_360_MAIOR
         jb FLAG_EH_IGUAL,TESTA_360_L
         jb FLAG_EH_MENOR,COMP1_DIR_H     ; CONTINUA NORMALMENTE

TESTA_360_L:

         mov a,DIR_COMP_L
         mov b,#LOW(360)

         lcall _TESTA

         jb FLAG_EH_MAIOR,TESTA_360_MAIOR
         jb FLAG_EH_IGUAL,COMP1_DIR_H     ; CONTINUA NORMALMENTE
         jb FLAG_EH_MENOR,COMP1_DIR_H     ; CONTINUA NORMALMENTE


TESTA_360_MAIOR:

         ; PEGA DIRCOMP(> 360) - 360
         clr C
         mov a,DIR_COMP_L
         subb a,#LOW(360)
         mov DIR_COMP_L,a

         mov a,DIR_COMP_H
         subb a,#HIGH(360)
         mov DIR_COMP_H,a


; Compara DIRECAO com DIR_COMP
COMP1_DIR_H:

         mov a,DIRECAO_H
         mov b,DIR_COMP_H

         lcall _TESTA

         jb FLAG_EH_MAIOR,TESTA_MAIOR_30H
         jb FLAG_EH_IGUAL,COMP1_DIR_L
         jb FLAG_EH_MENOR,COMP_CONTINUA

COMP1_DIR_L:

         mov a,DIRECAO_L
         mov b,DIR_COMP_L

         lcall _TESTA

         jb FLAG_EH_MAIOR,TESTA_MAIOR_30H
         jb FLAG_EH_IGUAL,COMP_CONTINUA
         jb FLAG_EH_MENOR,COMP_CONTINUA

;---------------------
TESTA_MAIOR_30H:

         mov a,R7           ; DIR_COMP_H
         mov b,#HIGH(45)

         lcall _TESTA

         jb FLAG_EH_MAIOR,TESTA_MAIOR_330H
         jb FLAG_EH_IGUAL,TESTA_MAIOR_30L
         jb FLAG_EH_MENOR,COMP_CONTINUA

TESTA_MAIOR_30L:

         mov a,R6           ; DIRECAO_L
         mov b,#LOW(45)

         lcall _TESTA

         jb FLAG_EH_MAIOR,TESTA_MAIOR_330H
         jb FLAG_EH_IGUAL,COMP_CONTINUA
         jb FLAG_EH_MENOR,COMP_CONTINUA

TESTA_MAIOR_330H:

         mov a,R7           ; DIR_COMP_H
         mov b,#HIGH(315)

         lcall _TESTA

         jb FLAG_EH_MAIOR,COMP_CONTINUA
         jb FLAG_EH_IGUAL,TESTA_MAIOR_330L
         jb FLAG_EH_MENOR,COMP_ERRO2

TESTA_MAIOR_330L:

         mov a,R6           ; DIRECAO_L
         mov b,#LOW(315)

         lcall _TESTA

         jb FLAG_EH_MENOR,COMP_ERRO2
        ; jb FLAG_EH_MAIOR,COMP_CONTINUA
        ; jb FLAG_EH_IGUAL,COMP_CONTINUA

;---------------------

COMP_CONTINUA:

; DECREMENTA O DIRCOMP EM 30 ( 16 BITS)

      ; DIR_COMP <- DIR_COMP - 60

         clr C

         mov a,DIR_COMP_L
         subb a,#90
         mov DIR_COMP_L,a

         mov a,DIR_COMP_H
         subb a,#0
         mov DIR_COMP_H,a

         ;*****************
         jnc COMP2_DIR_H
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

COMP2_DIR_H:

         mov a,DIRECAO_H
         mov b,DIR_COMP_H

         lcall _TESTA

         jb FLAG_EH_MAIOR,COMP_OK
         jb FLAG_EH_IGUAL,COMP2_DIR_L
         jb FLAG_EH_MENOR,COMP_ERRO

COMP2_DIR_L:

         mov a,DIRECAO_L
         mov b,DIR_COMP_L

         lcall _TESTA

         jb FLAG_EH_MAIOR,COMP_OK
         jb FLAG_EH_IGUAL,COMP_OK
        ;jb FLAG_EH_MENOR,COMP_ERRO

COMP_ERRO:

         jnb FLAG_MENOR_ZERO,COMP_ERRO2

         ; Agora testa a sobra que vem depois do 360

         clr C
         mov a,#90
         subb a,ACIMA_360
         mov ACIMA_360,a

         mov a,DIRECAO_L
         mov b,ACIMA_360

         lcall _TESTA

         jb FLAG_EH_IGUAL,COMP_OK
         jb FLAG_EH_MENOR,COMP_OK

         ;jb FLAG_EH_MAIOR,COMP_ERRO2

COMP_ERRO2:
         setb FLAG_DIRECAO_ERRO

         sjmp COMP_DIR_SAIDA

COMP_OK:
         clr FLAG_DIRECAO_ERRO

COMP_DIR_SAIDA:

         clr FLAG_MENOR_ZERO

         RET










