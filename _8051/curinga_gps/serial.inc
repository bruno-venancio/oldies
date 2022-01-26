;-----------------------------=| INTERRUPCAO SERIAL |=-----------------------------------

SERIAL_INT:


        push ACC                       ; coloca na pilha o ACC e o PSW
        push PSW
        push 00h

        mov R0,GUARDA_R0
        jb  TI,SERIAL_SAIDA

    ;---
        jnb RI,$
        mov A, SBUF                    ; Pega o dado da serial e bota no acumulador
        clr RI
    ;---

        cjne a,#'$',VER_FLAG  ; Eh igual ao START byte? ($)
        setb FLAG_START       ; SIM ,seta flag de start (Comeco de um frame


        mov  R0,#BUFFER_GPS   ; Aponta para BUFFER
        mov CONTADOR_BUF,#94  ; Numero maximo de bytes no buffer


        mov @R0,a             ; Ja grava o $ na memoria
        inc R0

        sjmp SERIAL_SAIDA     ; do protocolo

VER_FLAG:

        jnb FLAG_START,SERIAL_SAIDA    ; Testa a flag de start

        cjne R0,#BUFFER_GPS+3,VER_FLAG_CONT1


        cjne a,#'R',VER_FLAG_ERRO


        sjmp VER_FLAG_CONT

VER_FLAG_CONT1:

        cjne R0,#BUFFER_GPS+4,VER_FLAG_CONT

        cjne a,#'M',VER_FLAG_ERRO
        
        jb FLAG_APITA,PULA_LED_SERIAL_1   ; Se pardal foi localizado, pisca somente
                                          ; o led na interrupcao timer0
    
        jb FLAG_PARDAL_LOCALIZADO,PULA_LED_SERIAL_1

        clr LED5

PULA_LED_SERIAL_1:

        sjmp VER_FLAG_CONT



; Se o frame nao tiver 'R', então ignora
VER_FLAG_ERRO:

         clr FLAG_START

         sjmp SERIAL_SAIDA


VER_FLAG_CONT:

        mov  @R0,a      ; se sim, pega byte e coloca no buffer interno

        inc   R0                          ; aponta para a proxima posicao de memoria

        djnz CONTADOR_BUF,SERIAL_CONTINUA ; Rotina para proteger o buffer contra
                                          ; overflow
        mov CONTADOR_BUF,#94
        clr FLAG_START
        clr FLAG_POSSUI_DADOS_GPS
        sjmp SERIAL_SAIDA


SERIAL_CONTINUA:

        cjne  a,#'*',SERIAL_SAIDA

        jb FLAG_APITA,PULA_LED_SERIAL_2   ; Se pardal foi localizado, pisca somente
                                          ; o led na interrupcao timer0

        jb FLAG_PARDAL_LOCALIZADO,PULA_LED_SERIAL_2

        setb LED5

PULA_LED_SERIAL_2:

        setb FLAG_POSSUI_DADOS_GPS        ; Indica atraves desta flag, que ha dados
                                          ; no Buffer (internal RAM)
        mov CONTADOR_BUF,#94

        clr  FLAG_START                   ; Desliga a Flag que indica inicio de um
                                          ; frame do Protocolo
                                          
        
        ; incrementa FLAG_TESTA_KM a cada 120 ticks

        jb FLAG_CARRO_PARADO,SERIAL_SAIDA

        inc GPS_TICKS_COUNT            ; Conta o numero de ticks do modulo GPS

        mov a,GPS_TICKS_COUNT
        cjne a,#100,SERIAL_SAIDA

        mov GPS_TICKS_COUNT,#0

;------------------------------------------------------------------
SERIAL_SAIDA:

        mov GUARDA_R0,R0

        pop 00h
        pop PSW
        pop ACC



        RETI                              ; Sai da rotina de interrupcao serial



