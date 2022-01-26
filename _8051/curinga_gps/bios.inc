;
;      |----------------------------------------------------|
;      |  ROTINAS PARA USO GERAL PARA O PROJETO P002        |
;      |                                                    |
;      | Programador: Bruno Marcio Diogo Venancio           |
;    -[| Email: bruno.marcio@bol.com.br                     |]-
;      |                                                    |
;      | BWControls automacao      www.bwcontrols.com.br    |
;      |                                        BMDV 2003   |
;      |----------------------------------------------------|
;
;                                             
;
;
;---------------------------{ ROTINAS DE TEMPORIZACAO }--------------------------
;
;   |############################################|
;--=| Rotina de delay de 1 mS vezes o valor de A |=-
;   |############################################|


_DELAY_MS:

        push ACC
        push B
DD:
        mov b,#155
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2
        djnz B,$       ; ~ 50  uS @ 30 Mhz x2

        djnz ACC,DD

        pop B
        pop ACC
        ret


;    |########################################|
; --=| Delay de 1 segundo vezes o valor de A  |=-
;    |########################################|

_DELAY_SEC:

        push ACC
        push B

        mov B,A
DDD:
        mov a,#200
        lcall _DELAY_MS   ; 250mS
        lcall _DELAY_MS   ; 500mS
        lcall _DELAY_MS   ; 750mS
        lcall _DELAY_MS   ; 1000mS

        djnz B,DDD

        pop B
        pop ACC
        ret


_DELAY_50US:

        push acc
        push b

        mov b,#250

        djnz B,$       ; ~ 50  uS @ 30 Mhz x2

        pop b
        pop acc
        ret


_DELAY_HALF_SEC:

        push acc

        mov a,#250
        lcall _delay_ms

        pop acc
        ret



;----------------| ROTINA DE INICIALIZACAO DA SERIAL |-----------------------

_SERIAL_INI_4800:

        push acc


        mov     SCON,#01010000B      ; TI setado indica transmissor pronto
                                     ; modo 1  ,REN

        mov RCAP2L,#BAUD4800L
        mov RCAP2H,#BAUD4800H
        mov T2CON,#00110100b

        clr ET2

        pop ACC

        ret

_SERIAL_INI_19200:

        push acc
        mov     SCON,#01010000B      ; TI setado indica transmissor pronto
                                     ; modo 1  ,REN
        mov RCAP2L,#BAUD19200L
        mov RCAP2H,#BAUD19200H
        mov T2CON,#00110100b

        clr ET2

        pop ACC

        ret


_SERIAL_INI_9600:

        push acc
        mov     SCON,#01010000B      ; TI setado indica transmissor pronto
                                     ; modo 1  ,REN
        mov RCAP2L,#BAUD9600L
        mov RCAP2H,#BAUD9600H
        mov T2CON,#00110100b

        clr ET2

        pop ACC

        ret


;
;
; Faz BUFFER_BYTE_DPTR <- BUFFER_BYTE_DPTR +(CONTADOR16 * 11)
;
_RODA_ADDRESS:

         push acc
         push psw
         push 00h
         push 01h
         push 02h
         push 03h
         push 04h
         push 05h
         push 06h
         push 07h

         mov a,CONTADOR16_L     ; Faz ENDERECO <- ( CRD_COUNTER * 11)
         mov r3,a

         mov a,CONTADOR16_H
         mov r2,a

         mov r0,#0
         mov r1,#11

         lcall _MUL16

         clr C                ; agora adiciona RESULTADO com dptr
         mov a,R4
         add a,BUFFER_BYTE
         mov BUFFER_BYTE,a

         mov a,R5
         addc a,dpl
         mov dpl,a

         mov a,R6
         addc a,dph
         mov dph,a


         pop 07h
         pop 06h
         pop 05h
         pop 04h
         pop 03h
         pop 02h
         pop 01h
         pop 00h
         pop psw
         pop acc

         ret

;
; Roda DPTR para TIMER0
; Faz DPTR <- DPTR +(RAM_PONTOS_CONT * 11)
;
_RODA_DPTR_SRAM:

         push acc
         push psw
         push b

         mov a,RAM_PONTOS_CONT
         mov b,#11

         mul ab

         clr C                ; agora adiciona RESULTADO com dptr
         add a,dpl
         mov dpl,a

         mov a,b
         addc a,dph
         mov dph,a

         pop b
         pop psw
         pop acc

         ret
;
; Roda DPTR para Rotina principal
; Faz DPTR <- DPTR +(CONTADOR8 * 11)
;

_RODA_DPTR_SRAM2:

         push acc
         push psw
         push b

         mov a,CONTADOR8
         mov b,#11

         mul ab

         clr C                ; agora adiciona RESULTADO com dptr
         add a,dpl
         mov dpl,a

         mov a,b
         addc a,dph
         mov dph,a

         pop b
         pop psw
         pop acc

         ret


_ALL_LEDS_ON:
         
         clr LED1
         clr LED2
         clr LED3
         clr LED4
         clr LED5
         RET

_ALL_LEDS_OFF:

         setb LED1
         setb LED2
         setb LED3
         setb LED4
         setb LED5
         RET






;    |-----------------------------------------|
;    | Rotina que gera um tom onde DPTR contem |
;    | a frequencia em Hz e A contem a duracao |
; --=| em Ms. S¢ funciona corretamente a 24Mhz |=--
;    | Porem basta ajustar a freq. fundamental |
;    | para outros valores de clock.           |
;    |-----------------------------------------|

snd_gen:
                push ACC
                push 00h
                push 01h
                push b

                mov b,#2
                mov R0,DPL
                mov R1,DPH
sl1:
                cpl buzzer

sl2:
                djnz R0,$
                djnz R1,sl2
                mov R0,DPL
                mov R1,DPH
                djnz ACC,sl1
                djnz b,sl1

                setb buzzer

                pop b
                pop 01h
                pop 00h
                pop ACC

                ret

;    |------------------------------------------------|
; --=| Esta rotina da um BIP de 2Khz (24mhz) por 30ms |=--
;    |------------------------------------------------|

_snd_beep1:
                push ACC
                push DPL
                push DPH

                mov A,#255
                mov DPTR,#0100h
                lcall snd_gen

                setb buzzer

                pop DPH
                pop DPL
                pop ACC

                RET


;    |------------------------------------------------|
; --=| Esta rotina da um BIP de 2Khz (24mhz) por 30ms |=--
;    |------------------------------------------------|

_snd_beep2:
                push ACC
                push DPL
                push DPH

                mov A,#0
                mov DPTR,#0200h
                lcall snd_gen

                setb buzzer

                pop DPH
                pop DPL
                pop ACC

                RET


;    |------------------------------------------------|
; --=| Esta rotina da um BIP de 2Khz (24mhz) por 30ms |=--
;    |------------------------------------------------|

_snd_beep3:
                push ACC
                push DPL
                push DPH

                mov A,#255
                mov DPTR,#090H
                lcall snd_gen

                setb buzzer

                pop DPH
                pop DPL
                pop ACC

                RET


_PISCA_LED_VM_3X:

                push acc
                
                setb LED1
                setb LED2
                setb LED3
                setb LED4
                setb LED5

                clr LED_VM
                mov a,#250
                lcall _delay_ms
                setb LED_VM
                mov a,#250
                lcall _delay_ms
                clr LED_VM
                mov a,#250
                lcall _delay_ms
                setb LED_VM
                mov a,#250
                lcall _delay_ms
                clr LED_VM
                mov a,#250
                lcall _delay_ms
                setb LED_VM

                pop acc
                ret





_PISCA_LED_1_3X:

                push acc

                setb LED2
                setb LED3
                setb LED4
                setb LED5

                clr LED1
                mov a,#250
                lcall _delay_ms
                setb LED1
                mov a,#250
                lcall _delay_ms
                clr LED1
                mov a,#250
                lcall _delay_ms
                setb LED1
                mov a,#250
                lcall _delay_ms
                clr LED1
                mov a,#250
                lcall _delay_ms
                setb LED1

                pop acc
                ret


_PISCA_LED_2_3X:

                push acc

                setb LED1
                setb LED3
                setb LED4
                setb LED5

                clr LED2
                mov a,#250
                lcall _delay_ms
                setb LED2
                mov a,#250
                lcall _delay_ms
                clr LED2
                mov a,#250
                lcall _delay_ms
                setb LED2
                mov a,#250
                lcall _delay_ms
                clr LED2
                mov a,#250
                lcall _delay_ms
                setb LED2

                pop acc
                ret


_PISCA_LED_3_3X:

                push acc

                setb LED1
                setb LED2
                setb LED4
                setb LED5

                clr LED3
                mov a,#250
                lcall _delay_ms
                setb LED3
                mov a,#250
                lcall _delay_ms
                clr LED3
                mov a,#250
                lcall _delay_ms
                setb LED3
                mov a,#250
                lcall _delay_ms
                clr LED3
                mov a,#250
                lcall _delay_ms
                setb LED3

                pop acc
                ret

_PISCA_LED_4_3X:

                push acc

                setb LED1
                setb LED2
                setb LED3
                setb LED5

                clr LED4
                mov a,#250
                lcall _delay_ms
                setb LED4
                mov a,#250
                lcall _delay_ms
                clr LED4
                mov a,#250
                lcall _delay_ms
                setb LED4
                mov a,#250
                lcall _delay_ms
                clr LED4
                mov a,#250
                lcall _delay_ms
                setb LED4

                pop acc
                ret

_PISCA_LED_5_3X:

                push acc

                setb LED1
                setb LED2
                setb LED3
                setb LED4

                clr LED5
                mov a,#250
                lcall _delay_ms
                setb LED5
                mov a,#250
                lcall _delay_ms
                clr LED5
                mov a,#250
                lcall _delay_ms
                setb LED5
                mov a,#250
                lcall _delay_ms
                clr LED5
                mov a,#250
                lcall _delay_ms
                setb LED5

                pop acc
                ret

_BUZZER_APITA:

         clr LED5
         lcall _snd_beep1
         setb LED5

         mov a,#50
         lcall _delay_ms

         clr LED5
         lcall _snd_beep1
         setb LED5

         mov a,#50
         lcall _delay_ms

         clr LED5
         lcall _snd_beep1
         setb LED5

         ret
         

_BUZZER_APITA_WI:

         clr LED_VM
         lcall _snd_beep1
         setb LED_VM

         mov a,#50
         lcall _delay_ms

         clr LED_VM
         lcall _snd_beep1
         setb LED_VM

         mov a,#50
         lcall _delay_ms

         clr LED_VM
         lcall _snd_beep1
       

         ret



_BUZZER_APITA2:

         clr LED1
         clr LED2
         clr LED3
         clr LED4
         clr LED5
         lcall _snd_beep1
         setb LED1
         setb LED2
         setb LED3
         setb LED4
         setb LED5

         mov a,#50
         lcall _delay_ms

         clr LED1
         clr LED2
         clr LED3
         clr LED4
         clr LED5
         lcall _snd_beep1
         setb LED1
         setb LED2
         setb LED3
         setb LED4
         setb LED5

         mov a,#50
         lcall _delay_ms

         clr LED1
         clr LED2
         clr LED3
         clr LED4
         clr LED5
         lcall _snd_beep1
         setb LED1
         setb LED2
         setb LED3
         setb LED4
         setb LED5

         ret

_BUZZER_VOL_APITA:

         mov b,a
         mov P0,A
         lcall _snd_beep1
         mov P0,#0FFH

         mov a,#50
         lcall _delay_ms

         mov a,b
         mov P0,a
         lcall _snd_beep1
         mov P0,#0FFH

         mov a,#50
         lcall _delay_ms

         mov a,b
         mov P0,A
         lcall _snd_beep1
         mov P0,#0FFH

         ret


_BUZZER_NEGADO:

        lcall _snd_beep2
        lcall _snd_beep1
        lcall _snd_beep2
        lcall _snd_beep1

        ret


_BUZZER_SAT_NOTFOUND:

        lcall _snd_beep2
        lcall _snd_beep1

        RET


_BUZZER_SAT_FOUND:

         clr LED5
         lcall _snd_beep1
         setb LED5

         ret







;    |-------------------------------------------------|
;    |         Rotina de DECREMENTO do DPTR            |
; --=|         entrada: DPTR  Saida = DPTR-1           |=--
;    |-------------------------------------------------|

_DEC_DPTR:
                push ACC

                clr C
                mov a,dpl
                subb a,#1
                mov dpl,a

                mov a,dph
                subb a,#0
                mov dph,a

                pop acc

                ret






_ARREDONDA_VEL:
               
         ;Testa se eh menor que 20Km/h

         mov a,VELOCIDADE
         mov b,#10
         lcall _TESTA

         jb FLAG_EH_MAIOR,ARREDONDA_40KM
         
         ; If < or = do:

         mov VARIAVEL,#10
         sjmp ARREDONDA_VEL_SAIDA

ARREDONDA_40KM:

         mov a,VELOCIDADE
         mov b,#21
         lcall _TESTA

         jb FLAG_EH_MAIOR,ARREDONDA_50KM

         ; If < ou = do:

         mov VARIAVEL,#21
         sjmp ARREDONDA_VEL_SAIDA

ARREDONDA_50KM:

         mov a,VELOCIDADE
         mov b,#27
         lcall _TESTA

         jb FLAG_EH_MAIOR,ARREDONDA_60KM

         ; If < ou = do:

         mov VARIAVEL,#27
         sjmp ARREDONDA_VEL_SAIDA

ARREDONDA_60KM:

         mov a,VELOCIDADE
         mov b,#32
         lcall _TESTA

         jb FLAG_EH_MAIOR,ARREDONDA_70KM

         ; If < ou = do:

         mov VARIAVEL,#32
         sjmp ARREDONDA_VEL_SAIDA

ARREDONDA_70KM:

         mov a,VELOCIDADE
         mov b,#38
         lcall _TESTA

         jb FLAG_EH_MAIOR,ARREDONDA_80KM

         ; If < ou = do:

         mov VARIAVEL,#38
         sjmp ARREDONDA_VEL_SAIDA


ARREDONDA_80KM:

         mov a,VELOCIDADE
         mov b,#43
         lcall _TESTA

         jb FLAG_EH_MAIOR,ARREDONDA_90KM

         ; If < ou = do:

         mov VARIAVEL,#43
         sjmp ARREDONDA_VEL_SAIDA


ARREDONDA_90KM:

         mov a,VELOCIDADE
         mov b,#48
         lcall _TESTA

         jb FLAG_EH_MAIOR,ARREDONDA_100KM

         ; If < ou = do:

         mov VARIAVEL,#48
         sjmp ARREDONDA_VEL_SAIDA

ARREDONDA_100KM:

         mov VARIAVEL,#54

ARREDONDA_VEL_SAIDA:

         RET


;  +-------------------------------------------------------------------------+
;  | Purpose: Routine to convert a 32bit binary number in ASCII              |
;  |                                                                         |
;  | INPUT : R4,R3,R2,R1     (BINARY)                                        |
;  |        (MSB)   (LSB)                                                    |
;  |                                                                         |
;  |                                                                         |
;  | OUPUT : 56h,57h,58h,59h,5Ah,5BH,5Ch,5Dh,5Eh,5Fh          (ASCII)        |
;  |   msb--^                                      ^------lsb                |
;  |                                                                         |
;  | Destroy all registers                                                   |
;  |                                                                         |
;  | PROGRAMMER: Bruno Marcio Diogo Venancio ( bruno.marcio@bol.com.br )     |
;  | BRAZIL 07/20/2002                                                       |
;  |          THIS CODE CAN BE FREELY DISTRIBUTED WITHOUT CHANGES            |
;  +-------------------------------------------------------------------------+
;


_BINTOASC32:

        MOV     R0,#56h
        MOV     DPTR,#TAB32           ; R=TAB(P)

COM132:
        MOV     A,#0                    
        MOVC    A,@A+DPTR             ; fetch data from table
        MOV     R7,A
        INC     DPTR

        MOV     A,#0
        MOVC    A,@A+DPTR
        MOV     R6,A
        INC     DPTR

        MOV     A,#0
        MOVC    A,@A+DPTR
        MOV     R5,A
        INC DPTR

        MOV     A,#0
        MOVC    A,@A+DPTR
        MOV     LSB,A


        MOV     B,#'0'               ; C <- 0 (em ASCII)


RETORNO2:

      CLR       C
      MOV       A,R1                  ; N <- N - R
      SUBB      A,LSB
      MOV       R1,A


      MOV       A,R2
      SUBB      A,R5
      MOV       R2,A

      MOV       A,R3
      SUBB      A,R6
      MOV       R3,A

      MOV       A,R4
      SUBB      A,R7
      MOV       R4,A

      JC        SAIDA32               ; N < 0? ,if yes , goto saida24

      INC       B                     ; else C <- C +1

      SJMP      RETORNO2

SAIDA32:

      MOV       A,B
      MOV       @R0,A                 ; TABSAIDA(PSAIDA) <- C

      MOV       A,R1
      ADD       A,LSB                  ; N <- N+ R
      MOV       R1,A


      MOV       A,R2
      ADDC      A,R5
      MOV       R2,A

      MOV       A,R3
      ADDC      A,R6
      MOV       R3,A

      MOV       A,R4
      ADDC      A,R7
      MOV       R4,A

      INC       R0                    ; PSAIDA=PSAIDA +1
                                
      CLR       A

      MOVC      A,@A+DPTR
      CJNE      A,#1,INCREMENTA32     ; TAB(P) =1?
      RET                             ; If yes, return

INCREMENTA32:

      INC       DPTR
      SJMP       COM132


TAB32:
     DB 3Bh,09AH,0CAH,00H    ;1000.000.000
     DB 05h,0F5H,0E1H,00H    ;100.000.000
     DB 00,98H,96H,80H          ; table with  1 A 10.000.000 in HEXA
     DB 0,0FH,42H,040H
     DB 0,01H,86H,0A0H
     DB 0,0,27H,10H
     DB 0,0,03H,0E8H
     DB 0,0,0,64H
     DB 0,0,0,0AH
     DB 0,0,0,1




;  _ASC_TO_HEX - See if char in ACC is ASCII-hex and if so convert to a hex nibble.
;   Returns nibble in A, HexFlag tells if char was really hex. The ACC is not
;   altered if the character is not ASCII hex. Upper and lower case letters
;   are recognized.

_ASC_TO_HEX:
            cjne     A,#'0',AH1        ; Test for ASCII numbers.
AH1:
            jc       AHBad             ; Is character is less than a '0'?
            cjne     A,#'9'+1,AH2      ; Test value range.
AH2:
            jc       AHVal09           ; Is character is between '0' and '9'?

            cjne     A,#'A',AH3        ; Test for upper case hex letters.
AH3:
            jc       AHBad             ; Is character is less than an 'A'?
            cjne     A,#'F'+1,AH4      ; Test value range.
AH4:
            jc       AHValAF           ; Is character is between 'A' and 'F'?

            cjne     A,#'a',AH5        ; Test for lower case hex letters.
AH5:
            jc       AHBad             ; Is character is less than an 'a'?
            cjne     A,#'f'+1,AH6      ; Test value range.
AH6:
            jnc      AHBad             ; Is character is between 'a' and 'f'?
            clr      C
            subb     A,#27h            ; Pre-adjust character to get a value.
            sjmp     AHVal09           ; Now treat as a number.

AHBad:
            clr      HexFlag           ; Flag char as non-hex, don't alter.
            sjmp     AHEX              ; Exit
AHValAF:
            clr      C
            subb     A,#7              ; Pre-adjust character to get a value.
AHVal09:
            clr      C
            subb     A,#'0'            ; Adjust character to get a value.
            setb     HexFlag           ; Flag character as 'good' hex.
AHEX:
            ret




;    |---------------------------------------------------|
;  -=| Rotina de coversao de HEX-> ASCII (somente NIBBLE)|=-
;    |---------------------------------------------------|

_HEX_TO_ASCII:

            anl       A,#0Fh           ; Filtra byte para passar somente 1 nibble
                                       ;
            cjne      A,#0Ah,HA1       ; Testa a faixa de valor
HA1:
            jc        HAVal09          ; Valor de 0 a 9.
            add       A,#7             ; valor e de A a F, entao da um ajuste extra
HAVal09:
            add       A,#'0'           ; ajusta valor para ASCII hex.
            ret


;
;  Rotina para setar processador para RAM INTERNA
;
_SRAM_INTERNA:

        push acc
        push psw

        mov a,AUXR                      ; Seta processador para acessar RAM interna
        clr acc.1
        mov AUXR,a

        pop psw
        pop acc

        ret


_SRAM_READ:

       push PSW

       mov a,AUXR                      ; Seta processador para acessar RAM interna
       clr acc.1
       mov AUXR,a

       mov     EECON,#0
       movx    a,@dptr

       pop PSW

       ret


_TXD:
          jb RTS,$

          clr TI
          mov SBUF,a
          jnb TI,$
	  clr TI

          RET






;
;
;
DEBUG_BYTE:

          push acc

          swap A

          anl A,#00001111B

          lcall _HEX_TO_ASCII

          clr TI
          mov SBUF,a
          jnb TI,$
	  clr TI

          pop acc

          anl a,#00001111b

          lcall _HEX_TO_ASCII

          clr TI
          mov SBUF,a
          jnb TI,$
	  clr TI


          RET


xyxy:
         clr TI
          mov SBUF,a
          jnb TI,$
	  clr TI

          push acc
          mov a,#50
          lcall _delay_ms
          pop acc
          
          sjmp xyxy

;
; Rotina para transformar KM EM KNOT
;


TRANSF_KM_NKOT:

         mov b,#20
         lcall _TESTA

         jb FLAG_EH_MAIOR,TRANSF_KM_NKOT_40
         jb FLAG_EH_MENOR,TRANSF_KM_NKOT_100   ; Caso seja "00" eh porque eh 100

         mov VARIAVEL,#10

         sjmp TRANSF_SAIDA


TRANSF_KM_NKOT_40:

         mov b,#40
         lcall _TESTA

         jb FLAG_EH_MAIOR,TRANSF_KM_NKOT_50

         mov VARIAVEL,#21

         sjmp TRANSF_SAIDA

TRANSF_KM_NKOT_50:

         mov b,#50
         lcall _TESTA

         jb FLAG_EH_MAIOR,TRANSF_KM_NKOT_60

         mov VARIAVEL,#27

         sjmp TRANSF_SAIDA

TRANSF_KM_NKOT_60:

         mov b,#60
         lcall _TESTA

         jb FLAG_EH_MAIOR,TRANSF_KM_NKOT_70

         mov VARIAVEL,#32

         sjmp TRANSF_SAIDA

TRANSF_KM_NKOT_70:

         mov b,#70
         lcall _TESTA

         jb FLAG_EH_MAIOR,TRANSF_KM_NKOT_80

         mov VARIAVEL,#38

         sjmp TRANSF_SAIDA

TRANSF_KM_NKOT_80:

         mov b,#80
         lcall _TESTA

         jb FLAG_EH_MAIOR,TRANSF_KM_NKOT_90

         mov VARIAVEL,#43

         sjmp TRANSF_SAIDA

TRANSF_KM_NKOT_90:

         mov b,#90
         lcall _TESTA

         jb FLAG_EH_MAIOR,TRANSF_KM_NKOT_100

         mov VARIAVEL,#48

         sjmp TRANSF_SAIDA

TRANSF_KM_NKOT_100:

         mov VARIAVEL,#54

TRANSF_SAIDA:

         RET



;
; CALCULA QUAL LED DA BARRA SERA ACESO
;

_CALCULA_BARRA:

         push acc
         push b
         push 05h
         push 06h

         mov b,DISTANCIA_L
         mov a,DISTANCIA_H

         mov R5,#low(250)
         mov R6,#high(250)

         lcall _TESTA16

         jb FLAG_EH_MENOR,CAL_BARRA1

         mov BYTE_BARRA,#11101111B

         sjmp CALCULA_BARRA_SAIDA


CAL_BARRA1:

         mov b,DISTANCIA_L
         mov a,DISTANCIA_H

         mov R5,#low(200)
         mov R6,#high(200)

         lcall _TESTA16

         jb FLAG_EH_MENOR,CAL_BARRA3

         mov BYTE_BARRA,#11100111B

         sjmp CALCULA_BARRA_SAIDA

CAL_BARRA3:

         mov b,DISTANCIA_L
         mov a,DISTANCIA_H

         mov R5,#low(150)
         mov R6,#high(150)

         lcall _TESTA16

         jb FLAG_EH_MENOR,CAL_BARRA4

         mov BYTE_BARRA,#11100011B

         sjmp CALCULA_BARRA_SAIDA

CAL_BARRA4:

         mov b,DISTANCIA_L
         mov a,DISTANCIA_H

         mov R5,#low(100)
         mov R6,#high(100)

         lcall _TESTA16

         jb FLAG_EH_MENOR,CAL_BARRA5

         mov BYTE_BARRA,#11100001B

         sjmp CALCULA_BARRA_SAIDA

CAL_BARRA5:

         mov BYTE_BARRA,#11100000B

CALCULA_BARRA_SAIDA:

         pop 06h
         pop 05h
         pop b
         pop acc
         ret


; ROTINA PARA O RD2

_ZERA_EEPROM:
         CLR LED4
         
         MOV DPTR,#0
         MOV R7,#0FFH
         MOV A,#0FFH
ZERA_L1:
         MOV DADO,#0FFH
         LCALL _EE_WRITE

         INC DPTR

         DJNZ R7,ZERA_L1
         
         SETB LED4
         
         JMP $





;
;
;
_BUZZER_AMOSTRA:

         jnb acc.4,BUZ1
         jnb acc.3,BUZ2
         jnb acc.2,BUZ3
         jnb acc.1,BUZ4
         jnb acc.0,BUZ5
         
         sjmp BUZ5

BUZ1:
         setb BUZZER

         sjmp BUZZER_AMOSTRA_SAIDA

BUZ2:
         setb BUZZER

         sjmp BUZZER_AMOSTRA_SAIDA

BUZ3:
         setb BUZZER

         sjmp BUZZER_AMOSTRA_SAIDA

BUZ4:
         setb BUZZER

         sjmp BUZZER_AMOSTRA_SAIDA

BUZ5:
         clr BUZZER

BUZZER_AMOSTRA_SAIDA:

         mov P0,a

         mov a,#2
         lcall _delay_sec

         mov P0,#0ffh

         setb BUZZER

         ret


;
;
;
_TESTA_MEMORIA:

        mov DPTR,#0
        mov BUFFER_BYTE,#0

        lcall _AT45_MAIN_PAGE_READ

        swap a

        mov b,a

        anl a,#11110000b

        rr a
        rr a
        rr a
        rr a

        lcall _HEX_TO_ASCII

        cjne A,#'0',TESTA_ERRO_SAI
 
        mov a,b

        anl a,#00001111b

        lcall _HEX_TO_ASCII

        cjne A,#'0',TESTA_ERRO_SAI

        lcall _INCREMENTA_ENDERECO

        lcall _AT45_MAIN_PAGE_READ

        swap a

        anl a,#11110000b

        rr a
        rr a
        rr a
        rr a

        lcall _HEX_TO_ASCII

        cjne A,#'1',TESTA_ERRO_SAI

        clr FLAG_OK_ERRO
        
        RET



TESTA_ERRO_SAI:

        setb FLAG_OK_ERRO
        
        RET
        

        




