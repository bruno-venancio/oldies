DEF_LOOP1_TRAMP:
       ljmp DEF_LOOP1

;
; Rotina de desfragmentacao da memoria flash
;

_DEFRAG:

       ; Carrega contador de pontos da memoria

       mov  dptr,#EE_CONT_PONTOS_L ; Carrega o LSB do contador
       lcall _EE_READ

       mov CONT_PONTOS_L,a

       mov  dptr,#EE_CONT_PONTOS_H ; Carrega o MSB do contador

       lcall _EE_READ

       mov CONT_PONTOS_H,a

       ;---

       mov a,AUXR                ; Seta processador para acessar RAM interna
       clr acc.1
       mov AUXR,a

       mov CONTADOR16_L,#0
       mov CONTADOR16_H,#0

       mov R7,#9

     ; PROCURA FFFFFFF

PROCURA_FF:

       mov BUFFER_BYTE,#0
       mov dptr,#0
       lcall _RODA_ADDRESS          ; Faz DPTR +(CONTADOR16 * 10)

       mov R7,#8

PROC_MINI_LOOP:

       lcall _AT45_MAIN_PAGE_READ

       cjne a,#0ffh,DEF_LOOP1_TRAMP

       lcall _INCREMENTA_ENDERECO

       djnz R7,PROC_MINI_LOOP

       mov WORK,CONTADOR16_L          ; ENDEREÇO DE UM GAP
       mov WORKH,CONTADOR16_H

       ; PEGA O ENDEREÇO DO ULTIMO DADO

       mov CONTADOR16_L,CONT_PONTOS_L
       mov CONTADOR16_H,CONT_PONTOS_H

       ; CONTADOR16 <- CONTADOR16 -1

       clr C
       mov a,CONTADOR16_L
       subb a,#1
       mov CONTADOR16_L,a

       mov a,CONTADOR16_H
       subb a,#0
       mov CONTADOR16_H,a

       ;--

       mov BUFFER_BYTE,#0
       mov dptr,#0
       lcall _RODA_ADDRESS       ; Faz DPTR +(CONTADOR16 * 10)

       inc AUXR1                 ; Troca para DPTR2

       mov dptr,#SRAM_PONTOS     ; Aponta para buffer de pontos na SRAM na dptr2

       inc AUXR1                 ; Troca para DPTR1

       mov R7,#11

DEF_PEGA_ULT:

       lcall _AT45_MAIN_PAGE_READ
       
       push acc
       lcall _INCREMENTA_ENDERECO
       pop acc

       inc AUXR1          ; Troca para DPTR2

       movx @dptr,a       ; manda dado para buffer
       inc  dptr          ; inc dptr2

       inc AUXR1          ; Troca para DPTR1

       djnz R7,DEF_PEGA_ULT

       ; VOLTA ENDERECO DO GAP

       mov CONTADOR16_L,WORK          ; ENDEREÇO DE UM GAP
       mov CONTADOR16_H,WORKH


       ;Carrega endereco do GAP

       mov BUFFER_BYTE,#0
       mov dptr,#0
       lcall _RODA_ADDRESS       ; Faz DPTR +(CONTADOR16 * 10)

       lcall _AT45_MAIN_PAGE_TO_BUFFER1_TRANSF

       inc AUXR1          ; Troca para DPTR2

       mov dptr,#SRAM_PONTOS

       mov R7,#11

GRAVA_GAP:

       movx a,@dptr

       push acc    ; Guarda dado

       inc dptr    ; inc dptr2

       inc AUXR1          ; Troca para DPTR1

       pop acc     ; Recupera dado

       lcall _AT45_MAIN_PAGE_WRITE_B1

       lcall _INCREMENTA_ENDERECO

       inc AUXR1          ; Troca para DPTR2

       djnz R7,GRAVA_GAP

       inc AUXR1          ; Troca para DPTR1
       

         ; Decrementa CONT_PONTOS

         clr C
         mov a,CONT_PONTOS_L
         subb a,#1
         mov CONT_PONTOS_L,a

         mov a,CONT_PONTOS_H
         subb a,#0
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

         cpl LED

         ljmp _DEFRAG



PROCURA_FF_TRAMP:
       ljmp PROCURA_FF



DEF_LOOP1:

 ; Incrementa o contador de 16 bits

         clr C
         mov a,CONTADOR16_L
         add a,#1
         mov CONTADOR16_L,a

         mov a,CONTADOR16_H
         addc a,#0
         mov CONTADOR16_H,a

         mov a,CONTADOR16_L
         cjne a,CONT_PONTOS_L,PROCURA_FF_TRAMP

         mov a,CONTADOR16_H
         cjne a,CONT_PONTOS_H,PROCURA_FF_TRAMP

         mov CONTADOR16_L,#0
         mov CONTADOR16_H,#0

         mov R7,#2
DEF_L2:
         lcall _BUZZER_SAT_FOUND
         djnz R7,DEF_L2

         sjmp $

