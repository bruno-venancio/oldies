

;            |------------------------------------------|
;-----------=|   ROTINA DE ATENDIMENTO A INTERRUPCAO    |=--------
;            |               TIMER 0                    |
;            |------------------------------------------|

INT_TIMER0:

        push PSW
        push acc
        push b
        push 07h

        clr TR0

        mov TH0,#HIGH(0)           ; Carrega o byte MSB no timer 1
        mov TL0,#LOW(0)            ; Carrega o byte LSB no timer 1

        jnb FLAG_APITA,TIMER0_NAO_APITA

        djnz VAR_BEEP,TIMER0_SAIDA
        mov a,TEMPO
        mov b,#6
        mul ab

        mov VAR_BEEP,a

        cpl BUZZER
        ;cpl LED

        cpl FLAG_BARRA
        jb FLAG_BARRA,BARRA_APAGADA

        mov P0,BYTE_BARRA
        sjmp BARRA_SAIDA

BARRA_APAGADA:
        mov P0,#11111111B

BARRA_SAIDA:

        sjmp TIMER0_SAIDA    ; Sai da interrupcao s

TIMER0_NAO_APITA:

        djnz TICKS,TIMER0_SAIDA
        mov TICKS,#12

       ; cpl LED

        cpl FLAG_BARRA
        jb FLAG_BARRA,BARRA_APAGADA2

        mov P0,BYTE_BARRA
        sjmp TIMER0_SAIDA

BARRA_APAGADA2:
        mov P0,#11111111B

TIMER0_SAIDA:

        setb TR0    ; Religa o contador do Timer 0

        pop 07h
        pop b
        pop acc
        pop PSW

        RETI                          ; Retorna da interrupcao



;------| ROTINA DE INICIALIZACAO DA INTERRUPCAO DO TIMER 0 |-------------

TIMER0_INI:

        clr TR0

        mov TH0,#HIGH(0)         ; Carrega o byte MSB no timer 1
        mov TL0,#LOW(0)          ; Carrega o byte LSB no timer 1

        ;setb TR0                    ; Liga Timer 0
        setb ET0                     ; Habilita interrupcao TIMER 0


        RET
