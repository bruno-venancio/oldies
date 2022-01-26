$Title(Detector de pardais e pontos de perigo via GPS VN80)
$Date(11-08-14)

;    +--------------------=-=-=-=-=-=-=-=-=-=-=-----------------+
;    | Projeto: Detector de pardais e pontos de perigo via GPS  |
;  -=| Microcontrolador: ATMEL AT89C51ED2 em 30MHz modo X2      |=-
;    | Programador: Bruno Marcio Diogo Venancio   v.8.40 PCB    |
;    | Data de inicio:11/12/2015             modelo 8           |
;    +--------------------=-=-=-=-=-=-=-=-=-=-=-----------------+
;        
;-------| TABELA DE EQUATES |-----------------------------------------------
;
;=============| E/S |======================================
;

BUZZER       BIT   P1.4

LED1         BIT   P0.4
LED2         BIT   P0.3
LED3         BIT   P0.2
LED4         BIT   P0.1
LED5         BIT   P0.0

BOTAO_GRAVA  BIT   P3.3

;Serial 2 (bit banged)

TXD2	BIT	P3.1		  
RXD2	BIT	P1.3	    	 ;Rx
RTS     BIT     P2.6
CTS     BIT     P2.5

ESP_TXD BIT     P2.3
ESP_RXD BIT     P3.2

LED_VR  BIT     P3.4
LED_VM  BIT     P3.5

;-| Pinos da interface SPI

CS_        BIT  P1.1         ; chip select
MOSI       BIT  P1.7
MISO       BIT  P1.5
SCK        BIT  P1.6

MEM_RESET  BIT  P1.2
MEM_WP     BIT  P1.0

;========================| Ponteiros de buffers |========================

BUFFER_MINI       DATA  080H

BUFFER_GPS        DATA  085H

BUFFER_MONTAGEM   DATA  0E3H


BUFFER_REDE1      DATA  080H
BUFFER_REDE2      DATA  0A0H

BUFFER_RESULTADO  DATA  0C0H

;=======| Codigos ASCII para serial |====================================

CR              EQU   0DH                ;Codigo do ENTER
LF              EQU   0AH                ;Codigo do Line feed
EOS             EQU   1AH                ;Caractere de final para cada string
DEL             EQU   7Fh                ;Codigo do Del
BEEP            EQU   07H                ;Codigo do Beep
SPACE           EQU   20H                ;Codigo do Espaco
ESC             EQU   1Bh
BS              EQU   08H                ;Codigo do Back Space
EOT             EQU   04H                ;End of transmission
NPFF            EQU   0CH                ;NP form feed , new page
XON             EQU   11H                ;Flow control start character
XOFF            EQU   13H                ;Flow control stop character


;========================| Variaveis do Sistema |========================

; Variaveis para rotina SQRTBI

COUNT_L         EQU   R2
COUNT_H         EQU   R3
ACC_L           EQU   R4
ACC_H           EQU   R5

SQRX_L          EQU   R6
SQRX_H          EQU   R7

;----------------------------------

VAR_GERAL       DATA  29H
BYTE_PROTOCOLO  DATA  29H ;** VARIAVEL HIBRIDA

LATITUDE_L      DATA  2AH
LATITUDE_ML     DATA  2BH
LATITUDE_MH     DATA  2CH
LATITUDE_H      DATA  30H

LONGITUDE_L     DATA  31H
LONGITUDE_ML    DATA  32H
LONGITUDE_MH    DATA  33H
LONGITUDE_H     DATA  34H

EE_PARDAL_POINT_L      DATA  35H
EE_PARDAL_POINT_ML     DATA  36H
EE_PARDAL_POINT_MH     DATA  37H
EE_PARDAL_POINT_H      DATA  38H

;----------------------------------

CONTADOR_BUF    DATA  39H

GUARDA_R0       DATA  3AH

CONT_PONTOS_H   DATA  3BH
CONT_PONTOS_L   DATA  3CH

CONTADOR16_H    DATA  3DH
CONTADOR16_L    DATA  3EH

VELOCIDADE      DATA  3FH
VARIAVEL        DATA  40H

DIRECAO_L       DATA  41H
DIRECAO_H       DATA  42H

DIR_COMP_H      DATA  43H
DIR_COMP_L      DATA  44H


ACIMA_360       DATA  45H
LSB             DATA  45H    ; VARIAVEL HIBRIDA

;ACUMULADOR_L    DATA  46H
;ACUMULADOR_ML   DATA  47H
;ACUMULADOR_MH   DATA  48H
;ACUMULADOR_H    DATA  49H

GUARDA_DPL1     DATA 46H
GUARDA_DPH1     DATA 47H

GUARDA_DPL2     DATA 48H
GUARDA_DPH2     DATA 49H
                       

BUFFER_BYTE     DATA  4AH
TEMPO           DATA  4BH

GPS_TICKS_COUNT DATA  4CH
RAM_PONTOS_CONT DATA  4DH

X2_L            DATA  4EH
X2_ML           DATA  4FH
X2_MH           DATA  50H
X2_H            DATA  51H

Y2_L            DATA  52H
Y2_ML           DATA  53H
Y2_MH           DATA  54H
Y2_H            DATA  55H


RESULTADO1_L    DATA  56H
RESULTADO1_ML   DATA  57H
RESULTADO1_MH   DATA  58H
RESULTADO1_H    DATA  59H

RESULTADO2_L    DATA  5AH
RESULTADO2_ML   DATA  5BH
RESULTADO2_MH   DATA  5CH
RESULTADO2_H    DATA  5DH

TEMP_VAR_LAT_L    DATA  5EH
TEMP_VAR_LAT_ML   DATA  5FH
TEMP_VAR_LAT_MH   DATA  60H
TEMP_VAR_LAT_H    DATA  61H

TEMP_VAR_LON_L    DATA  62H
TEMP_VAR_LON_ML   DATA  63H
TEMP_VAR_LON_MH   DATA  64H
TEMP_VAR_LON_H    DATA  65H

CONTADOR24_L      DATA  62H  ;  HIBRIDA  USADA NO PROTOCOLO
CONTADOR24_H      DATA  63H  ;  HIBRIDA     '        '
CONTADOR24_HH     DATA  64H  ;  HIBRIDA     '        '

DISTANCIA_L     DATA  66H
DISTANCIA_H     DATA  67H

TEMPO_CHEGADA   DATA  68H
DADO            DATA  69H

CONTADOR8       DATA  6AH

TICKS           DATA  6BH
VAR_BEEP        DATA  6CH

CONT2           DATA  6DH       ; VARIAVEL HIBRIDA
DEGREE          DATA  6DH

MINUTE          DATA  6EH

CONT_X          DATA  6FH       ; VARIAVEL HIBRIDA
DEC_MINUTE_L    DATA  6FH

DEC_MINUTE_H    DATA  70H

Work            DATA  71H
WorkH           DATA  72H

Work2L          DATA  73H
Work2H          DATA  74H

R8              DATA  75H
R9              DATA  76H
R10             DATA  77H
R11             DATA  78H

XL              DATA  79H
XML             DATA  7AH
XMH             DATA  7BH
XH              DATA  7CH

DATA_CONTADOR   DATA  7DH  ; VARIAVEL HIBRIDA

YL              DATA  7DH
YML             DATA  7EH
YMH             DATA  7FH
YH              DATA  28H

SRAM_PONTO_L    DATA  27H
SRAM_PONTO_ML   DATA  26H
SRAM_PONTO_MH   DATA  25H
SRAM_PONTO_H    DATA  24H
CONT_I          DATA  23h
BYTE_BARRA      DATA  22H


;-----| Recursos do AT89C51ED2 |--------------------------------------

CKCON0       EQU  08FH           ; Clock control register
CKCON1       EQU  0AFH           ; Clock control register
AUXR1        EQU  0A2H           ; Dual DPTR register
AUXR         EQU  08EH           ; Auxiliary register

POF          EQU 00010000b       ; Power Off Flag bit

;PCA SFRs

IEN0         EQU   0A8H          ;  Interrupt enable control 0


CCON         EQU   0D8H          ; PCA Timer/Counter Control
CMOD         EQU   0D9H          ; PCA Timer/Counter Mode
CL           EQU   0E9H          ; PCA Timer/Counter Low Byte
CH           EQU   0F9H          ; PCA Timer/Counter High Byte

CCAPM0       EQU   0DAH          ; PCA Timer/Counter Mode 0
CCAPM1       EQU   0DBH          ; PCA Timer/Counter Mode 1
CCAPM2       EQU   0DCH          ; PCA Timer/Counter Mode 2
CCAPM3       EQU   0DDH          ; PCA Timer/Counter Mode 3
CCAPM4       EQU   0DEH          ; PCA Timer/Counter Mode 4

CCAP0H       EQU   0FAH          ; PCA Compare Capture Module 0 H
CCAP1H       EQU   0FBH          ; PCA Compare Capture Module 1 H
CCAP2H       EQU   0FCH          ; PCA Compare Capture Module 2 H
CCAP3H       EQU   0FDH          ; PCA Compare Capture Module 3 H
CCAP4H       EQU   0FEH          ; PCA Compare Capture Module 4 H

CCAP0L       EQU   0EAH          ; PCA Compare Capture Module 0 L
CCAP1L       EQU   0EBH          ; PCA Compare Capture Module 1 L
CCAP2L       EQU   0ECH          ; PCA Compare Capture Module 2 L
CCAP3L       EQU   0EDH          ; PCA Compare Capture Module 3 L
CCAP4L       EQU   0EEH          ; PCA Compare Capture Module 4 L

;Timer 2 registers

ET2          BIT  IE.5           ; Bit de habilitacao da interrupcao T2
T2CON	     DATA 0C8H           ; Registros de controle para T2
T2MOD	     DATA 0C9H
RCAP2L	     DATA 0CAH
RCAP2H	     DATA 0CBH
TL2	     DATA 0CCH
TH2	     DATA 0CDH

PT2	     BIT  0BDH               ; Seta prioridade maxima para T2

CPRL2	     BIT  0C8H           ; Bits do registro T2CON
CT2	     BIT  0C9H
TR2	     BIT  0CAH
EXEN2	     BIT  0CBH
TCLK	     BIT  0CCH
RCLK	     BIT  0CDH
EXF2	     BIT  0CEH
TF2	     BIT  0CFH

; keyboard registers

KBF          EQU  09EH           ; Keyboard flag register
KBE          EQU  09DH           ; Keyboard input enable
KBLS         EQU  09CH           ; Keyboard level selector register

; Watchdog

WDTRST       DATA 0A6H           ; Watchdog reset register
WDTPRG       DATA 0A7H           ; Watchdog timer out register

; Baud rate generator

BDRCON       EQU  9BH            ; Baud rate control
BRL          EQU  9AH            ; Baud rate reload


;----{ Definicao de bits e e registros da SPI do AT89c51ED2 }---------

SPCON        EQU  0C3H           ; Serial peripheral control register
SPSTA        EQU  0C4H           ; Serial peripheral status & control reg.
SPDAT        EQU  0C5H           ; Serial peripheral Data register

SPIF         EQU  10000000B      ; Interrupt Flag

IPH0         EQU  0B7H           ; Interrupt priority high register
IPL0         EQU  0B8H

IEN1         EQU  0B1H           ; Interrupt enable register (SPI e KBD)

IPL1         EQU  0B2H           ;   "       priority bit
IPH1         EQU  0B3H           ;           "  "

EECON        EQU  0D2H           ; EEPROM control register


;=====| Definicao das FLAGS do sistema |==========

FLAG_POSSUI_DADOS_GPS BIT   2FH.0
FLAG_START            BIT   2FH.1
FLAG_EH_IGUAL         BIT   2FH.2
FLAG_EH_MENOR         BIT   2FH.3
FLAG_EH_MAIOR         BIT   2FH.4
FLAG_DIRECAO_ERRO     BIT   2FH.5
FLAG_GRAVA_EEPROM     BIT   2FH.6
FLAG_MENOR_ZERO       BIT   2FH.7

FLAG_SAT_FOUND        BIT   2EH.0
FLAG_ANG_ERRO         BIT   2EH.1
FLAG_APITA            BIT   2EH.3
FLAG_BARRA            BIT   2EH.4
FLAG_CARREGA_R7       BIT   2EH.5
hexflag               BIT   2EH.6
FLAG_OK_ERRO          BIT   2EH.7

FLAG_ATAN_OK          BIT   2DH.0
FLAG_PONTO_ENCONTRADO BIT   2DH.1
FLAG_WIFI             BIT   2DH.2
FLAG_UP_INTEIRO       BIT   2DH.3
FLAG_POI_DOWNLOAD_START BIT 2DH.4
FLAG_PARDAL_LOCALIZADO  BIT 2DH.5
FLAG_APAGA_EEPROM       BIT 2DH.6
FLAG_CARRO_PARADO       BIT 2DH.7


;==========|Constantes|=================================================

BAUD4800L             EQU   03DH    ; Constante para 4800 BPS em 30Mhz TIMER2

BAUD4800H             EQU   0FFH

BAUD19200L            EQU   0CFH    ; Constante para 19200 BPS em 30Mhz TIMER2

BAUD19200H            EQU   0FFH

BAUD9600L             EQU   09EH    ; Constante para 19200 BPS em 30Mhz TIMER2
BAUD9600H             EQU   0FFH


_ON                   EQU    0FFH
_OFF                  EQU    00H

;==========| DEFINICOES DE ESPACO PARA A EEPROM EXTERNA |=================

EE_CONT_PONTOS_H     XDATA     0003H
EE_CONT_PONTOS_L     XDATA     0004H


EE_TEMPO_CHEGADA     XDATA     0005H
EE_PONTOS_FIRST      XDATA     0007H


EE_ACUMULADOR_L      XDATA     0010H
EE_ACUMULADOR_ML     XDATA     0011H
EE_ACUMULADOR_MH     XDATA     0012H
EE_ACUMULADOR_H      XDATA     0013H

EE_NRO_PACOTE        XDATA     0015H



;==========| DEFINICOES DE MEM. PARA A SRAM INTERNA |==================

SRAM_BUFFER    XDATA  00000H

SRAM_PONTOS    XDATA  0100H  ; Endereço de inicio dos pontos temporarios na SRAM interna

SRAM_DATA      XDATA  05FFH  ; Endereço para guardar dados de data e hora
SRAM_NRO_PACOTE XDATA 060FH  ; Guarda numero de pacote de atualização



;                 /------\
;              -=| INICIO |=-
;                 \------/


        ORG 0000H                ; Reset

        sjmp INICIALIZA

      ;---
        ORG 0003H                ; Vetor da interrupcao Externa 0 (INT0)


        RETI
      ;---

        ORG 000BH                ; Vetor da interrupcao do TIMER 0

        ljmp INT_TIMER0

      ;---

        ORG 0013H                ; Vetor da interrupcao Externa 1 (INT1)
        ljmp _GRAVA_POSICAO

      ;---

        ORG 001BH                ; Vetor da interrupcao do TIMER 1

        RETI

      ;---

        ORG 0023H                ; Vetor da interrupcao serial

        ljmp SERIAL_INT

      ;---

        ORG 002BH

        reti

        ORG $

INICIALIZA:

         mov   SP,#7             ; Inicializa stack pointer

         lcall _HARD_INI         ; Chama rotina de inicializacao do hardware

;------------{ INICIALIZA O HARDWARE }------------------------

         setb BUZZER

         clr FLAG_POSSUI_DADOS_GPS
         clr FLAG_START
         clr FLAG_EH_IGUAL
         clr FLAG_EH_MENOR
         clr FLAG_EH_MAIOR
         clr FLAG_GRAVA_EEPROM
         clr FLAG_MENOR_ZERO
         clr FLAG_SAT_FOUND
         clr FLAG_APAGA_EEPROM
         clr FLAG_CARREGA_R7
         clr hexflag
         clr FLAG_APITA
         clr FLAG_PONTO_ENCONTRADO
         clr FLAG_ATAN_OK
         clr FLAG_ANG_ERRO
         clr FLAG_PARDAL_LOCALIZADO
         setb FLAG_CARRO_PARADO
         clr FLAG_BARRA
         clr FLAG_WIFI

         mov RAM_PONTOS_CONT,#0    ; Zera contador de pontos na SRAM interna
         mov CONTADOR8,#0
         mov GUARDA_R0,#BUFFER_GPS
         mov DATA_CONTADOR,#0

         mov CONTADOR16_L,#0
         mov CONTADOR16_H,#0

         mov CONTADOR_BUF,#94
         mov VAR_BEEP,#90

         mov TICKS,#12              ; Inicia TICKS com 1 para dar Overflow na primeira passagem
                                    ; no timer0
                                    
         mov GPS_TICKS_COUNT,#0

         mov VARIAVEL,#4         ; Numero maximo de apitos que o aparelho emitira quando estiver fora do ar
                 

        ; Carrega contador de pontos da memoria

         mov  dptr,#EE_CONT_PONTOS_L ; Carrega o LSB do contador
         lcall _EE_READ

         mov CONT_PONTOS_L,a

         mov  dptr,#EE_CONT_PONTOS_H ; Carrega o MSB do contador

         lcall _EE_READ

         mov CONT_PONTOS_H,a

       ;- Se o contador for FFFFH, zera-o

         mov a,CONT_PONTOS_L
         cjne a,#0FFH,CONTADOR_PONTOS_CONTINUA

         mov a,CONT_PONTOS_H
         cjne a,#0FFH,CONTADOR_PONTOS_CONTINUA

         mov dptr,#EE_CONT_PONTOS_H
         mov A,#HIGH(32936)
         mov DADO,A
         lcall _EE_WRITE

         mov dptr,#EE_CONT_PONTOS_L
         mov A,#LOW(32936)
         mov DADO,A
         lcall _EE_WRITE

         mov CONT_PONTOS_L,#LOW(32936)
         mov CONT_PONTOS_H,#HIGH(32936)

CONTADOR_PONTOS_CONTINUA:

         mov a,CONT_PONTOS_H
         mov b,CONT_PONTOS_L

         mov R6,#HIGH(32936)
         mov R5,#LOW(32936)

         lcall _TESTA16

         jb FLAG_EH_MAIOR,CONTADOR_PONTOS_CONTINUA2
         jb FLAG_EH_IGUAL,CONTADOR_PONTOS_CONTINUA2

         lcall _BUZZER_VOL_APITA

         clr LED2

         mov a,#2
         lcall _delay_Sec

         setb LED2
         
         ; conserta 
         
         mov dptr,#EE_CONT_PONTOS_H
         mov A,#HIGH(32936)
         mov DADO,A
         lcall _EE_WRITE

         mov dptr,#EE_CONT_PONTOS_L
         mov A,#LOW(32936)
         mov DADO,A
         lcall _EE_WRITE

         mov CONT_PONTOS_L,#LOW(32936)
         mov CONT_PONTOS_H,#HIGH(32936)



CONTADOR_PONTOS_CONTINUA2:

         mov a,CONT_PONTOS_H
         mov b,CONT_PONTOS_L

         mov R6,#HIGH(40936)
         mov R5,#LOW(40936)

         lcall _TESTA16
         
         jb FLAG_EH_MENOR,CONTADOR_PONTOS_CONTINUA3

         lcall _BUZZER_VOL_APITA

         clr LED3

         mov a,#2
         lcall _delay_Sec

         setb LED3

         ;Conserta

         mov dptr,#EE_CONT_PONTOS_H
         mov A,#HIGH(32936)
         mov DADO,A
         lcall _EE_WRITE

         mov dptr,#EE_CONT_PONTOS_L
         mov A,#LOW(32936)
         mov DADO,A
         lcall _EE_WRITE

         mov CONT_PONTOS_L,#LOW(32936)
         mov CONT_PONTOS_H,#HIGH(32936)

                
CONTADOR_PONTOS_CONTINUA3:

         mov  dptr,#EE_TEMPO_CHEGADA ; Carrega o tempo de chegada da eeprom
         lcall _EE_READ

         mov TEMPO_CHEGADA,a

         mov b,#15
         lcall _TESTA

         jb FLAG_EH_MENOR,CONTADOR_PONTOS_CONTINUA4
         jb FLAG_EH_IGUAL,CONTADOR_PONTOS_CONTINUA4

         ;if > 15 then TEMPO_CHEGADA=12

         mov TEMPO_CHEGADA,#12

CONTADOR_PONTOS_CONTINUA4:
        
         mov b,#6
         lcall _TESTA

         jb FLAG_EH_MAIOR,CONTADOR_PONTOS_CONTINUA5
         ;jb FLAG_EH_IGUAL,CONTADOR_PONTOS_CONTINUA5

         ;if <6 then TEMPO_CHEGADA=6
         mov TEMPO_CHEGADA,#12

CONTADOR_PONTOS_CONTINUA5:

         setb MEM_RESET                ; Solta reset da flash
         setb MEM_WP                   ; Desproteje a memoria contra gravaçao

         jb BOTAO_GRAVA,CONTINUA_NORMALMENTE

         mov a,#1
         lcall _delay_sec

         jb BOTAO_GRAVA,CONTINUA_NORMALMENTE

         lcall _SERIAL_INI_19200         ; Inicializa Serial 19200Bps 8N1

         ljmp PROTOCOLO

CONTINUA_NORMALMENTE:

         lcall _snd_beep1              ; Toca um beep ao ligar o aparelho

SEQ:
         MOV R7,#5
         MOV A,#11111110B
SEQ2:
         MOV P0,A

         PUSH ACC
         MOV A,#15
         LCALL _DELAY_MS
         POP ACC

         RL A
         DJNZ R7,SEQ2

         PUSH ACC
         lcall _snd_beep1
         POP ACC

         MOV R7,#6
SEQ3:
         MOV P0,A

         PUSH ACC
         MOV A,#15
         LCALL _DELAY_MS
         POP ACC

         RR A
         DJNZ R7,SEQ3

         MOV A,#15
         LCALL _DELAY_MS

         setb LED5

         MOV A,#15
         LCALL _DELAY_MS

         clr LED_VM

         MOV A,#15
         LCALL _DELAY_MS

         setb LED_VM

         ; Pisca o led do meio caso esteja vazio

         mov a,CONT_PONTOS_L
         cjne a,#1,PISCA_ZERO_CONT

         mov a,CONT_PONTOS_H
         cjne a,#0,PISCA_ZERO_CONT

         clr LED3

         lcall _BUZZER_VOL_APITA

         clr LED3
         mov a,#2
         lcall _delay_Sec

         setb LED3

PISCA_ZERO_CONT:

         ; Testa se memoria esta ok

         lcall _TESTA_MEMORIA

         jnb FLAG_OK_ERRO,TESTA_MEM_CONT

         lcall _BUZZER_VOL_APITA

         clr LED4

         mov a,#2
         lcall _delay_Sec

         setb LED4


TESTA_MEM_CONT:

         ; Desconecta de qualque rede

         mov dptr,#AT_CWQAP
         lcall _SEND_STRING_ESP

;
;------------{ INICIALIZA OS PERIFERICOS }------------------
;

         lcall _SERIAL_INI_4800        ; Inicializa Serial 9600Bps 8N1

         lcall TIMER0_INI              ; Inicializa Timer 0

         clr   EX0

         mov   IPL0,#00010001B         ; Prioridade na interrupcao para SERIAL, em
         mov   IPH0,#00010010B         ; seguida para TIMER0 depois INT0

         clr  TR0                      ; Desliga contador Timer0

        ;--

         ;~INT1
         setb  EX1                    ; Habilita interrupcao externa 1
         setb  IT1                    ; Ativa interrupcao na transicao de 0 para 1

         setb    ES
                   

         setb    EA                   ; Habilita todas interrupcoes

; Aguarda os dados chegarem ao buffer pela SERIAL
REINICIA:
         setb ES
         
REINICIA_LOOP:

         jnb FLAG_POSSUI_DADOS_GPS,REINICIA_LOOP
         clr FLAG_POSSUI_DADOS_GPS

         ;Desliga interrupcao serial
         clr ES



; Ex.do que vai haver a partir do endereco 89
;$GPRMC,144529.000,A,1550.8060,S,04758.5758,W,25.82,91.56,041016,,,A* 
;
; Pega os dados vindos do GPS,converte para binario e organiza na memoria
;

MONTAGEM:

     mov R0,#BUFFER_GPS
     mov R7,#2          ; Numero de virgulas a serem contadas para chegar

ACHA_STATUS:

     mov a,@R0 ; PEGA BYTE DO BUFFER_DADOS

     inc R0

     cjne a,#',',ACHA_STATUS

     djnz R7,ACHA_STATUS

     mov a,@R0

     cjne A,#'A',STATUS_INVALIDO

     mov VARIAVEL,#4

     ajmp MONTAGEM_CONT

STATUS_INVALIDO:

     clr FLAG_POSSUI_DADOS_GPS
     clr FLAG_SAT_FOUND
     
     clr TR0
     clr FLAG_APITA
     setb BUZZER
     setb LED5
     setb LED1
     setb LED2
     setb LED3
     setb LED4

     clr FLAG_PARDAL_LOCALIZADO

     mov a,PCON
     anl a, #POF                ; Testa Power Off Flag
     jnz PARTIDA_FRIA           ; POF=1 indica partida quente

     clr LED5

     mov a,VARIAVEL

     jz STATUS_INVALIDO_PULA

     lcall _BUZZER_SAT_NOTFOUND

     dec VARIAVEL

STATUS_INVALIDO_PULA:

     clr LED5

     mov R7,#5

TIME_FRIA:

     jb FLAG_WIFI,AJUSTA

     mov a,#1
     lcall _delay_sec

     djnz R7,TIME_FRIA


PARTIDA_FRIA:

     clr LED5

     setb FLAG_CARRO_PARADO

;------------
AJUSTA:

         jnb FLAG_WIFI,REINICIA
         clr FLAG_WIFI

         clr EA

         clr LED_VM
         setb LED5
         
         lcall _BUZZER_APITA_WI

         lcall _WIFI_INI
         
         jnb FLAG_OK_ERRO,WIFI_INI_OK
         
         lcall _ALL_LEDS_ON
         
         mov a,#250
         lcall _delay_sec
         
         lcall _ALL_LEDS_OFF
         
         ajmp WIFI_CON_ENTRY

WIFI_INI_OK:

         lcall _PROCURA_WIFI

         jnb FLAG_OK_ERRO,WIFI_CON_TST_OK
         
         sjmp WIFI_CON_SAIDA

WIFI_CON_TST_OK:

         clr LED2

         mov R8,#3

         lcall _DATA_DOWNLOAD

WIFI_CON_SAIDA:

         mov a,#250
         lcall _delay_ms

         lcall AT_CWQAP_ENTRY

         lcall _BUZZER_APITA_WI

WIFI_CON_ENTRY:

         setb LED_VM
         setb LED1
         setb LED2
         setb LED3
         setb LED4
         setb LED5

         setb EA

         ajmp REINICIA

MONTAGEM_CONT:

     jb FLAG_SAT_FOUND,MONTAGEM_CONT2

     setb LED5

     setb FLAG_SAT_FOUND

MONTAGEM_CONT2:

     mov a,PCON
     clr acc.4                    ; Seta o aparelho como partida "quente"
     mov PCON,a

     ;~INT1
     setb  EX1                    ; Habilita interrupcao externa 1
     setb  IT1                    ; Ativa interrupcao na transicao de 0 para 1

;-------------

; +---------------------------+
; | Envia coordenada Latitude |
; +---------------------------+

     mov R0,#BUFFER_GPS
     mov R7,#3          ; Numero de virgulas a serem contadas para chegar a latitude

ENVIA_COORDENADA_LATITUDE:

     mov a,@R0 ; PEGA BYTE DO BUFFER_DADOS

     inc R0

     cjne a,#',',ENVIA_COORDENADA_LATITUDE

     djnz R7,ENVIA_COORDENADA_LATITUDE

     mov R1,#BUFFER_MONTAGEM

TRANSF_BUFFER_LATITUDE:

     mov a,@R0

     mov @R1,a

     inc R1

     inc R0

     cjne a,#',',TRANSF_BUFFER_LATITUDE

     mov R1,#BUFFER_MONTAGEM

PRINT_LAT_LOOP:

     inc R1

     cjne R1,#BUFFER_MONTAGEM+10,PRINT_LAT_JMP ; Verifica se deu overflow no buffer

     sjmp PRINT_LAT_CONT

PRINT_LAT_JMP:

     mov a,@R1

     cjne a,#',',PRINT_LAT_LOOP

     ;---
PRINT_LAT_CONT:

     ; ********  Carrega as variaveis DEGREE, MINUTE E DEC_MINUTE ********

     ;### Carrega a DEGREE ###

     mov R0,#BUFFER_MONTAGEM    ; no buffer_montagem , a coordenada tem '.'
     mov R1,#BUFFER_MINI        ; e vai ser transferida para o buffer_mini sem o '.'

     mov R7,#2

TRANSF_LAT_LOAD_DEGREE:

     mov a,@R0
     mov @R1,a

     inc R0
     inc R1

     djnz R7,TRANSF_LAT_LOAD_DEGREE

     mov @R1,#0    ; Null caractere para a rotina DECBIN

  ; Transforma os ASCII da memoria em um binario

     mov R0,#BUFFER_MINI
     lcall _DECBIN

     mov a,R4   ; LSB do RESULTADO1 do DECBIN
     mov DEGREE,a

     sjmp LOAD_MINUTE_LAT


     ;### Carrega MINUTE ###

LOAD_MINUTE_LAT:

     mov R0,#BUFFER_MONTAGEM    ; no buffer_montagem , a coordenada tem '.'
     mov R1,#BUFFER_MINI        ; e vai ser transferida para o buffer_mini sem o '.'

LOAD_MINUTE_LAT_LOOP:

     mov a,@R0

     cjne a,#'.',ACHA_PONTO_LAT_MINUTE   ; Se achou o ´.´

     dec R0   ; Volta duas casa decimais do minutes
     dec R0

     mov a,@R0    ; Transfere para o Buffer_mini
     mov @R1,a

     inc R0
     inc R1

     mov a,@R0    ; Transfere para o Buffer_mini
     mov @R1,a

     inc R1

     mov @R1,#0   ; Grava null caractere para o decbin

; Transforma os ASCII da memoria em um binario

     mov R0,#BUFFER_MINI
     lcall _DECBIN

     mov a,R4   ; LSB do RESULTADO1 do DECBIN
     mov MINUTE,a

     sjmp LOAD_DEC_MINUTE_LAT

ACHA_PONTO_LAT_MINUTE:

     inc R0

     sjmp LOAD_MINUTE_LAT_LOOP

     ;### Carrega DEC_MINUTE ###

LOAD_DEC_MINUTE_LAT:

     mov R0,#BUFFER_MONTAGEM    ; no buffer_montagem , a coordenada tem '.'
     mov R1,#BUFFER_MINI        ; e vai ser transferida para o buffer_mini sem o '.'

LOAD_DEC_MINUTE_LAT_LOOP:

     mov a,@R0

     cjne a,#'.',ACHA_PONTO_LAT_DEC_MINUTE   ; Se achou o ´.´

     inc R0

     mov a,@R0        ; pega os 3 caracteres depois do '.' e carrega no DEC_MINUTE
     mov @R1,a

     inc R0
     inc R1

     mov a,@R0
     mov @R1,a

     inc R0
     inc R1

     mov a,@R0
     mov @R1,a

     inc R1   

     mov @R1,#0

; Transforma os ASCII da memoria em um binario

     mov R0,#BUFFER_MINI
     lcall _DECBIN

     ;multiplica o resultado por 10

     mov R0,#0
     mov R1,#10
     
     mov a,R5
     mov R2,a
     
     mov a,R4
     mov R3,a

     lcall _MUL16

     mov a,R4   ; LSB do RESULTADO1 do DECBIN
     mov DEC_MINUTE_L,a

     mov a,R5   ; MLSB do RESULTADO1 do DECBIN
     mov DEC_MINUTE_H,a

     sjmp DEGREE_TO_MINUTES_LAT

ACHA_PONTO_LAT_DEC_MINUTE:

     inc R0
     sjmp  LOAD_DEC_MINUTE_LAT_LOOP


; ******** Converte decimal para degree *****

DEGREE_TO_MINUTES_LAT:

      mov R1,DEGREE       ; Multiplica DEGREE X 60
      mov R0,#0

      mov R3,#60
      mov R2,#0
      lcall _MUL16

      mov a,MINUTE        ; Soma resultado com MINUTE

      clr C

      add a,R4            ; Multiplica resultado por 10000
      mov R4,a

      mov a,R5
      addc a,#0
      mov R5,a

      mov a,R4
      mov R1,a

      mov a,R5
      mov R0,a

      mov R3,#low(10000)
      mov R2,#high(10000)

      lcall _MUL16

      mov a,R4            ; Soma resultado com DEC_MINUTE e coloca em LATITUDE(32BITS)
      add a,DEC_MINUTE_L
      mov LATITUDE_L,a

      mov a,R5
      addc a,DEC_MINUTE_H
      mov LATITUDE_ML,a

      mov a,R6
      addc a,#0
      mov LATITUDE_MH,a

      mov a,R7
      addc a,#0
      mov LATITUDE_H,a

      ; Chegando aqui, a variavel LATITUDE (32BITS) esta carregada com os dados em
      ; MINUTES




; +----------------------------+
; | Envia coordenada longitude |
; +----------------------------+

     mov R0,#BUFFER_GPS
     mov R7,#5          ; numero de virgulas a serem contadas para chegar a longitude

ENVIA_COORDENADA_LONGITUDE:

     mov a,@R0 ; PEGA BYTE DO BUFFER_DADOS

     inc R0

     cjne a,#',',ENVIA_COORDENADA_LONGITUDE

     djnz R7,ENVIA_COORDENADA_LONGITUDE

     mov R1,#BUFFER_MONTAGEM

TRANSF_BUFFER_LONGITUDE:

     mov a,@R0

     mov @R1,a

     inc R1

     inc R0

     cjne a,#',',TRANSF_BUFFER_LONGITUDE

     mov R1,#BUFFER_MONTAGEM

PRINT_LON_LOOP:

     inc R1

     cjne R1,#BUFFER_MONTAGEM+10,PRINT_LON_JMP ; Verifica se deu overflow no buffer

     sjmp PRINT_LON_CONT

PRINT_LON_JMP:

     mov a,@R1

     cjne a,#',',PRINT_LON_LOOP

     ;---
PRINT_LON_CONT:


     ; ********  Carrega as variaveis DEGREE, MINUTE E DEC_MINUTE ********

     ;### Carrega a DEGREE ###

     mov R0,#BUFFER_MONTAGEM    ; no buffer_montagem , a coordenada tem '.'
     mov R1,#BUFFER_MINI        ; e vai ser transferida para o buffer_mini sem o '.'

     mov R7,#3

TRANSF_LON_LOAD_DEGREE:

     mov a,@R0
     mov @R1,a

     inc R0
     inc R1

     djnz R7,TRANSF_LON_LOAD_DEGREE

     mov @R1,#0    ; Null caractere para a rotina DECBIN

  ; Transforma os ASCII da memoria em um binario

     mov R0,#BUFFER_MINI
     lcall _DECBIN

     mov a,R4   ; LSB do RESULTADO1 do DECBIN
     mov DEGREE,a

     sjmp LOAD_MINUTE_LON


     ;### Carrega MINUTE ###

LOAD_MINUTE_LON:

     mov R0,#BUFFER_MONTAGEM    ; no buffer_montagem , a coordenada tem '.'
     mov R1,#BUFFER_MINI        ; e vai ser transferida para o buffer_mini sem o '.'

LOAD_MINUTE_LON_LOOP:

     mov a,@R0

     cjne a,#'.',ACHA_PONTO_LON_MINUTE   ; Se achou o ´.´

     dec R0   ; Volta duas casa decimais do minutes
     dec R0

     mov a,@R0    ; Transfere para o Buffer_mini
     mov @R1,a

     inc R0
     inc R1

     mov a,@R0    ; Transfere para o Buffer_mini
     mov @R1,a

     inc R1

     mov @R1,#0   ; Grava null caractere para o decbin

; Transforma os ASCII da memoria em um binario

     mov R0,#BUFFER_MINI
     lcall _DECBIN

     mov a,R4   ; LSB do RESULTADO1 do DECBIN
     mov MINUTE,a

     sjmp LOAD_DEC_MINUTE_LON


ACHA_PONTO_LON_MINUTE:

     inc R0

     sjmp LOAD_MINUTE_LON_LOOP

     ;### Carrega DEC_MINUTE ###

LOAD_DEC_MINUTE_LON:

     mov R0,#BUFFER_MONTAGEM    ; no buffer_montagem , a coordenada tem '.'
     mov R1,#BUFFER_MINI        ; e vai ser transferida para o buffer_mini sem o '.'

LOAD_DEC_MINUTE_LON_LOOP:

     mov a,@R0

     cjne a,#'.',ACHA_PONTO_LON_DEC_MINUTE   ; Se achou o ´.´

     inc R0

     mov a,@R0        ; pega os 3 caracteres depois do '.' e carrega no DEC_MINUTE
     mov @R1,a

     inc R0
     inc R1

     mov a,@R0
     mov @R1,a

     inc R0
     inc R1

     mov a,@R0
     mov @R1,a

     inc R1

     mov @R1,#0

; Transforma os ASCII da memoria em um binario

     mov R0,#BUFFER_MINI
     lcall _DECBIN

     ;multiplica o resultado por 10
     mov R0,#0
     mov R1,#10
     
     mov a,R5
     mov R2,a
     
     mov a,R4
     mov R3,a

     lcall _MUL16

     mov a,R4   ; LSB do RESULTADO1 do DECBIN
     mov DEC_MINUTE_L,a

     mov a,R5   ; MLSB do RESULTADO1 do DECBIN
     mov DEC_MINUTE_H,a

     sjmp DEGREE_TO_MINUTES_LON

ACHA_PONTO_LON_DEC_MINUTE:

     inc R0
     sjmp  LOAD_DEC_MINUTE_LON_LOOP


; ******** Converte decimal para degree *****

DEGREE_TO_MINUTES_LON:

      mov R1,DEGREE       ; Multiplica DEGREE X 60
      mov R0,#0

      mov R3,#60
      mov R2,#0
      lcall _MUL16

      mov a,MINUTE        ; Soma resultado com MINUTE

      clr C

      add a,R4            ; Multiplica resultado por 10000
      mov R4,a

      mov a,R5
      addc a,#0
      mov R5,a

      ;--
      mov a,R4
      mov R1,a

      mov a,R5
      mov R0,a

      mov R3,#low(10000)
      mov R2,#high(10000)

      lcall _MUL16

      mov a,R4            ; Soma resultado com DEC_MINUTE e coloca em LONGITUDE(32BITS)
      add a,DEC_MINUTE_L
      mov LONGITUDE_L,a

      mov a,R5
      addc a,DEC_MINUTE_H
      mov LONGITUDE_ML,a

      mov a,R6
      addc a,#0
      mov LONGITUDE_MH,a

      mov a,R7
      addc a,#0
      mov LONGITUDE_H,a

      ; Chegando aqui, a variavel LONGITUDE (32BITS) esta carregada com os dados em
      ; MINUTES


;+--------------------------------------------------------------------------------+
;|Chegando aqui, as coordenadas X e Y ja estao no buffer definitivo em binario 32b|
;+--------------------------------------------------------------------------------+

;+-----------------+
;| Pega Velocidade |
;+-----------------+

     mov R0,#BUFFER_GPS
     mov R7,#7        ; Numero de virgulas a serem contadas para chegar a Velocidade

ENVIA_VELOCIDADE:

     mov a,@R0 ; PEGA BYTE DO BUFFER_DADOS

     inc R0

     cjne a,#',',ENVIA_VELOCIDADE

     djnz R7,ENVIA_VELOCIDADE

     mov R1,#BUFFER_MONTAGEM

TRANSF_BUFFER_VELOCIDADE:

     mov a,@R0

     mov @R1,a

     inc R1

     inc R0

     cjne a,#',',TRANSF_BUFFER_VELOCIDADE

     ; Transfere Buffer_montagem para buffer_mini sem o '.'

     mov R0,#BUFFER_MONTAGEM    ; no buffer_montagem , a coordenada tem '.'
     mov R1,#BUFFER_MINI        ; e vai ser transferida para o buffer_mini sem o '.'

TRANSF_BUFFER_VELO_LOOP:

     mov a,@R0

     cjne a,#'.',TRANSF_BUFFER_VELO_LOOP_2

     mov @R1,#0    ; Grava caractere nulo usada na rotina Decbin

     sjmp TRANSF_BUFFER_VELO_SAIDA

TRANSF_BUFFER_VELO_LOOP_2:

     mov @R1,a

     inc R0
     inc R1

     sjmp TRANSF_BUFFER_VELO_LOOP

TRANSF_BUFFER_VELO_SAIDA:

     ; Transforma os ASCII da memoria em um binario de 32 bits

     mov R0,#BUFFER_MINI
     lcall _DECBIN

     mov a,R4         ; LSB do RESULTADO1 do DECBIN
     mov VELOCIDADE,a
     
     ; Testa se o carro esta parado ou muito devagar
     
     mov b,#6
     lcall _TESTA
     
     jb FLAG_EH_MENOR,TRANSF_VEL_PARADO
     jb FLAG_EH_IGUAL,TRANSF_VEL_PARADO
     jb FLAG_EH_MAIOR,TRANSF_VEL_MOVIMENTANDO

TRANSF_VEL_PARADO:

     setb FLAG_CARRO_PARADO
     
     sjmp TRANSF_VEL_CONT

TRANSF_VEL_MOVIMENTANDO:

     clr FLAG_CARRO_PARADO

TRANSF_VEL_CONT:

;+--------------+
;| Pega Direcao |
;+--------------+

     mov R0,#BUFFER_GPS
     mov R7,#8        ; Numero de virgulas a serem contadas para chegar a Direcao

ENVIA_DIRECAO:

     mov a,@R0        ; PEGA BYTE DO BUFFER_DADOS

     inc R0

     cjne a,#',',ENVIA_DIRECAO

     djnz R7,ENVIA_DIRECAO

     mov R1,#BUFFER_MONTAGEM

TRANSF_BUFFER_DIRECAO:

     mov a,@R0

     mov @R1,a

     inc R1

     inc R0

     cjne a,#',',TRANSF_BUFFER_DIRECAO

     ; Transfere Buffer_montagem para buffer_mini sem o '.'

     mov R0,#BUFFER_MONTAGEM    ; no buffer_montagem , a coordenada tem '.'
     mov R1,#BUFFER_MINI        ; e vai ser transferida para o buffer_mini sem o '.'

TRANSF_BUFFER_DIR_LOOP:

     mov a,@R0

     cjne a,#'.',TRANSF_BUFFER_DIR_LOOP_2

     mov @R1,#0    ; Grava caractere nulo usada na rotina Decbin

     sjmp TRANSF_BUFFER_DIR_SAIDA

TRANSF_BUFFER_DIR_LOOP_2:

     mov @R1,a

     inc R0
     inc R1

     sjmp TRANSF_BUFFER_DIR_LOOP

TRANSF_BUFFER_DIR_SAIDA:

         ; Agora transforma os ASCII da memoria em um binario de 32 bits

         mov R0,#BUFFER_MINI
         lcall _DECBIN

         mov DIRECAO_L,R4   ; LSB do RESULTADO1 do DECBIN

         mov DIRECAO_H,R5   ; MSB do RESULTADO1 do DECBIN


;********************************************************************************

         ; Se o botao de gravacao foi pressionado, grava na flash

         jnb FLAG_GRAVA_EEPROM,APAGA_EEPROM

         lcall _GRAVA_EEPROM

         clr FLAG_GRAVA_EEPROM

APAGA_EEPROM:

         ; Se o botao foi pressionado enquanto o ponto foi localizado,apaga(se for ponto manual)

         jnb FLAG_APAGA_EEPROM,AJUSTA_VOLUME

         clr TR0
         clr FLAG_APITA
         setb BUZZER
         setb LED5
         setb LED1
         setb LED2
         setb LED3
         setb LED4
         
         mov BYTE_BARRA,#11111111B

         clr FLAG_PARDAL_LOCALIZADO
         clr EX1

         lcall _APAGA_PARDAL_EEPROM

         ;Apaga da SRAM
         mov dptr,#SRAM_PONTOS       ; Aponta para buffer de pontos na SRAM
         lcall _RODA_DPTR_SRAM2      ; Faz DPTR +(CONTADOR8 * 11)
         lcall _SRAM_INTERNA

         mov a,#00h
         mov r7,#8

APAGA_SRAM_INTERNA:

         movx @dptr,a

         inc dptr

         djnz R7,APAGA_SRAM_INTERNA

         mov CONTADOR8,#0
         mov RAM_PONTOS_CONT,#0

         clr FLAG_APAGA_EEPROM

         setb EX1

         ajmp REINICIA
         
AJUSTA_VOLUME:
         
         jnb FLAG_WIFI,PROCURA_PARDAL
         clr FLAG_WIFI

         jnb FLAG_CARRO_PARADO,PROCURA_PARDAL
         
         clr EA

         clr LED_VM
         setb LED5

         lcall _BUZZER_APITA

         lcall _WIFI_INI

         jnb FLAG_OK_ERRO,WIFI_INI_OK2

         lcall _ALL_LEDS_ON

         mov a,#250
         lcall _delay_sec

         lcall _ALL_LEDS_OFF

         ajmp WIFI_CON_ENTRY2

WIFI_INI_OK2:

         lcall _PROCURA_WIFI
         
         jnb FLAG_OK_ERRO,WIFI_CON_TST_OK2

         ; Se deu erro de conexao, apita

         sjmp WIFI_CON_TST_SAIDA

WIFI_CON_TST_OK2:

         clr LED2

         mov R8,#3

         lcall _DATA_DOWNLOAD

WIFI_CON_TST_SAIDA:

         mov a,#250
         lcall _delay_ms
         
         lcall AT_CWQAP_ENTRY

WIFI_CON_ENTRY2:

         setb LED1
         setb LED2
         setb LED3
         setb LED4
         setb LED5

         setb LED_VM

         setb EA

;***********************************
;*                                 *
;* Rotina de localizacao de pardal *
;*                                 *
;***********************************

PROCURA_PARDAL:

         mov a,RAM_PONTOS_CONT       ; Se nao houverem pontos na Sram,
         jnz PROCURA_PARD01

         lcall _CARREGA_SRAM         ; faz primeira carga da SRAM

         mov a,RAM_PONTOS_CONT       ; Se ainda nao houverem pontos na Sram,
         jnz PROCURA_PARD01

        ; ljmp _TESTA_KM_OIL_TROCA               ; testa quilometragem do oleo depois Reinicia
         ljmp REINICIA
         
PROCURA_PARD01:

         lcall _PROCURA_PONTO_RAM

         jnb FLAG_PONTO_ENCONTRADO,PROCURA_PAR_NAO_ENC ; Se ponto NAO encontrado,pula

         lcall _VER_ABERTURA

         jb FLAG_DIRECAO_ERRO,PROCURA_PAR_NAO_ENC

         ;Calcula ATAN do ponto

         lcall _INT_ATN2PTS

         mov DIR_COMP_L,Work
         mov DIR_COMP_H,WorkH

         lcall _COMPARA_ATAN

         jb FLAG_ANG_ERRO,PROCURA_PAR_NAO_ENC
         
         jb FLAG_CARRO_PARADO,PROCURA_PAR_NAO_ENC

         sjmp  CALCULA_DISTANCIA

PROCURA_PAR_NAO_ENC:

         clr TR0
         clr FLAG_APITA
         setb BUZZER
         setb LED1
         setb LED2
         setb LED3
         setb LED4
         setb LED5

         mov BYTE_BARRA,#11111111B

         clr FLAG_PARDAL_LOCALIZADO

         inc CONTADOR8

         mov a,CONTADOR8

         cjne a,RAM_PONTOS_CONT,PROCURA_PARD01

         mov CONTADOR8,#0
         mov RAM_PONTOS_CONT,#0

         ljmp REINICIA

;+----------------------------------------------------+
;| Rotina para calcular a distancia entre dois pontos |
;| Realiza o calculo: D=SQR((X2-X1)^2+(Y2-Y1)^2)      |
;+----------------------------------------------------+

CALCULA_DISTANCIA:

         setb TR0
         setb FLAG_PARDAL_LOCALIZADO

        ;Carrega LATITUDE DA SRAM

         mov dptr,#SRAM_PONTOS       ; Aponta para buffer de pontos na SRAM
         lcall _RODA_DPTR_SRAM2      ; Faz DPTR +(CONTADOR8 * 11)
         lcall _SRAM_INTERNA
         lcall _SRAM_READ

         mov X2_H,a

         inc dptr

         lcall _SRAM_READ

         mov X2_MH,a

         inc dptr

         lcall _SRAM_READ

         mov X2_ML,a

         inc dptr

         lcall _SRAM_READ

         mov X2_L,a

        ;Carrega LONGITUDE DA SRAM

         inc dptr

         lcall _SRAM_READ

         mov Y2_H,a

         inc dptr

         lcall _SRAM_READ

         mov Y2_MH,a

         inc dptr

         lcall _SRAM_READ

         mov Y2_ML,a

         inc dptr

         lcall _SRAM_READ

         mov Y2_L,a

       ; Calcula X2-X1

         clr C
         mov a,LATITUDE_L
         subb a,X2_L
         mov RESULTADO1_L,a

         mov a,LATITUDE_ML
         subb a,X2_ML
         mov RESULTADO1_ML,a

         mov a,LATITUDE_MH
         subb a,X2_MH
         mov RESULTADO1_MH,a

         mov a,LATITUDE_H
         subb a,X2_H
         mov RESULTADO1_H,a

         jnc DEU_POSITIVO_X

         ;Se deu negativo, então realiza um complemento 2 em 32bits e deixa
         ;somente a magnitude

         clr     C
         mov     a,RESULTADO1_L
         cpl     a               ; complementa
         add     a,#1            ; e adiciona 1
         mov     RESULTADO1_L, a

         mov     a,RESULTADO1_ML ; Pega proximo byte
         cpl     a               ; complementa
         addc    a,#0            ; adiciona o carry
         mov     RESULTADO1_ML,a


         mov     a,RESULTADO1_MH
         cpl     a
         addc    a,#0
         mov     RESULTADO1_MH,a

         mov     a,RESULTADO1_H
         cpl     a
         addc    a,#0
         mov     RESULTADO1_H,a

DEU_POSITIVO_X:

         ;DIVIDE O RESULTADO POR 10 PARA FACILITAR O CALCULO

         mov R0,RESULTADO1_L
         mov R1,RESULTADO1_ML
         mov R2,RESULTADO1_MH
         mov R3,RESULTADO1_H

         mov R4,#10
         mov R5,#0
         mov R6,#0
         mov R7,#0

         lcall _DIV_U32U32U32

         mov RESULTADO1_L,R0
         mov RESULTADO1_ML,R1
         mov RESULTADO1_MH,R2
         mov RESULTADO1_H,R3


       ; Eleva o RESULTADO1 ao quadrado  (SOMENTE 16 BITS)

         mov R1,RESULTADO1_L
         mov R0,RESULTADO1_ML

         mov R3,RESULTADO1_L
         mov R2,RESULTADO1_ML

         lcall _MUL16

         mov RESULTADO1_L,R4
         mov RESULTADO1_ML,R5
         mov RESULTADO1_MH,R6
         mov RESULTADO1_H,R7

       ; Calcula Y2-Y1

         clr C
         mov a,LONGITUDE_L
         subb a,Y2_L
         mov RESULTADO2_L,a

         mov a,LONGITUDE_ML
         subb a,Y2_ML
         mov RESULTADO2_ML,a

         mov a,LONGITUDE_MH
         subb a,Y2_MH
         mov RESULTADO2_MH,a

         mov a,LONGITUDE_H
         subb a,Y2_H
         mov RESULTADO2_H,a

         jnc DEU_POSITIVO_Y

         ;Se deu negativo, então realiza um complemento 2 em 32bits e deixa
         ;somente a magnitude

         clr     C
         mov     a,RESULTADO2_L
         cpl     a               ; complementa
         add     a,#1            ; e adiciona 1
         mov     RESULTADO2_L, a

         mov     a,RESULTADO2_ML ; Pega proximo byte
         cpl     a               ; complementa
         addc    a,#0            ; adiciona o carry
         mov     RESULTADO2_ML,a

         mov     a,RESULTADO2_MH
         cpl     a
         addc    a,#0
         mov     RESULTADO2_MH,a

         mov     a,RESULTADO2_H
         cpl     a
         addc    a,#0
         mov     RESULTADO2_H,a

DEU_POSITIVO_Y:

         ;DIVIDE O RESULTADO POR 10 PARA FACILITAR O CALCULO

         mov R0,RESULTADO2_L
         mov R1,RESULTADO2_ML
         mov R2,RESULTADO2_MH
         mov R3,RESULTADO2_H

         mov R4,#10
         mov R5,#0
         mov R6,#0
         mov R7,#0

         lcall _DIV_U32U32U32

         mov RESULTADO2_L,R0
         mov RESULTADO2_ML,R1
         mov RESULTADO2_MH,R2
         mov RESULTADO2_H,R3


        ; Eleva o RESULTADO2 ao quadrado

         mov R1,RESULTADO2_L
         mov R0,RESULTADO2_ML

         mov R3,RESULTADO2_L
         mov R2,RESULTADO2_ML

         lcall _MUL16

         mov RESULTADO2_L,R4
         mov RESULTADO2_ML,R5
         mov RESULTADO2_MH,R6
         mov RESULTADO2_H,R7

        ; Soma os resultados e coloca resultado total em RESULTADO2

         clr c
         mov a,RESULTADO1_L
         add a,RESULTADO2_L
         mov RESULTADO2_L,a

         mov a,RESULTADO1_ML
         addc a,RESULTADO2_ML
         mov RESULTADO2_ML,a

         mov a,RESULTADO1_MH
         addc a,RESULTADO2_MH
         mov RESULTADO2_MH,a

         mov a,RESULTADO1_H
         addc a,RESULTADO2_H
         mov RESULTADO2_H,a

         ; Calcula a raiz quadrada do resultado (Somente em 16 bits)

         mov a,RESULTADO2_L
         mov b,RESULTADO2_ML

         lcall SQRTBI

         ; Transforma em metros multiplicando por 18 (1,8metros)

         mov R0,#0
         mov R1,a      ; R1 = SQRT

         mov R2,#0
         mov R3,#18

         lcall _MUL16  ; Multiplica por 18

         mov a,R4
         mov R3,a

         mov a,R5      ; Divide por 10 para tirar uma casa decimal
         mov R2,A

         mov R5,#10
         mov R4,#0

         lcall _DIV16

        ;-
         mov DISTANCIA_L,R3
         mov DISTANCIA_H,R2
         
         lcall _CALCULA_BARRA

         ; +----------------------------------+
         ; | Se VEL for > ou = entao continua |
         ; +----------------------------------+

         mov dptr,#SRAM_PONTOS+8     ; Aponta para buffer de pontos na SRAM
         lcall _RODA_DPTR_SRAM2      ; Faz DPTR +(CONTADOR8 * 11)
         lcall _SRAM_INTERNA
         lcall _SRAM_READ

         mov VARIAVEL,a

        ;-

         mov a,VELOCIDADE            ; Pega velocidade atual
         mov b,VARIAVEL              ; Pega velocidade gravada na eeprom

         lcall _TESTA                ; Testa quem eh maior

         jb FLAG_EH_MENOR,NAO_APITA


        ; Calcula o tempo de chegada ao ponto (T=D/V)


         mov R3,#low(514)
         mov R2,#high(514)

         mov R0,#0
         mov R1,VELOCIDADE

         lcall _MUL16      ; R5,R4  = VELOCIDADE EM M/S

         mov a,R4          ; Pega velocidade e divide por 1000 para ficar 0,514
         mov R3,a

         mov a,R5
         mov R2,a

         mov R5,#low(1000)
         mov R4,#high(1000)

         lcall _DIV16

         mov a,R3                   ; R3 contem a velocidade em M/S

         mov B,R3                   ; Salva velocidade

         mov R3,DISTANCIA_L
         mov R2,DISTANCIA_H

         mov R4,#0
         mov R5,B                   ; Restaura velocidade

         lcall _DIV16

         mov a,R3
         mov b,R2

         mov TEMPO,a                ; Guarda tempo atual

       ; Se TEMPO FOR > TEMPO_CHEGADA NAO APITA

         mov a,TEMPO
         mov b,TEMPO_CHEGADA

         lcall _TESTA

         jb FLAG_EH_MAIOR,NAO_APITA

         mov a,TEMPO                  ; Se TEMPO for =0 procura outros pontos
         jz PROCURA_PARDAL_SAIDA

         setb FLAG_APITA
         setb TR0
         setb FLAG_PARDAL_LOCALIZADO

         ajmp REINICIA                ; Atualiza posicao

NAO_APITA:

         clr FLAG_APITA
        ; clr TR0                        ; Desliga timer0
         setb BUZZER                    ; Desliga Buzzer
        ; setb LED                       ; Desliga LED


NAO_APITA_CONTINUA:

         ajmp REINICIA

PROCURA_PARDAL_SAIDA:

         clr FLAG_APITA
         clr TR0
         clr FLAG_PARDAL_LOCALIZADO
         setb BUZZER                    ; Desliga Buzzer
         setb LED1
         setb LED2
         setb LED3
         setb LED4
         setb LED5

         mov BYTE_BARRA,#11111111B

         inc CONTADOR8
         mov a,CONTADOR8

         cjne a,RAM_PONTOS_CONT,PROCURA_PARDAL_TRAMPOLIM

         lcall _CARREGA_SRAM            ; Carrega SRAM com novos pontos

         mov CONTADOR8,#0

         ljmp REINICIA

PROCURA_PARDAL_TRAMPOLIM:

         ajmp PROCURA_PARDAL


$INCLUDE(TESTA.INC)
$INCLUDE(SERIAL.INC)
$INCLUDE(GRAVA.INC)
$INCLUDE(DECBIN.INC)
$INCLUDE(AT45_SPI.INC)
$INCLUDE(BIOS.INC)
$INCLUDE(HARDWARE.INC)
$INCLUDE(DIRECAO.INC)
$INCLUDE(NSERIE.INC)
$INCLUDE(TIMER0.INC)
$INCLUDE(SERIAL2.INC)
$INCLUDE(EEPROM.INC)
$INCLUDE(LOAD_RAM.INC)
$INCLUDE(PROCPONT.INC)
$INCLUDE(MATH.INC)
$INCLUDE(ATN.INC)
$INCLUDE(COMP_ATN.INC)
$INCLUDE(PROTO.INC)
$INCLUDE(ESP.INC)



db 'Curinga GPS mod.VN100 WIFI'
db 'Software & hardware By Bruno Marcio Diogo Venancio 2006-16'
db 'Email: bruno.marcio@bol.com.br'
db 'www.curingagps.com.br'
db 'Pietro, meu filho, papai te ama!'


     END
