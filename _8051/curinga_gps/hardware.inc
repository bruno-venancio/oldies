
;
; Rotina de inicializacao do hardware
;

_HARD_INI:


     mov P0,#0FFH
     mov P1,#0FFH
     mov P2,#0FFH            
     mov P3,#0FFH

     mov CKCON0,#01111011b   ; Core em modo X2 e perifericos em normal


     mov CKCON1,#0           ; SPI em modo X2
     mov AUXR,#00010001B     ; XRAM interna ativa em 1792kb, ALE desat.

   ;-| Inicializa interface SPI

     setb CS_
     setb MISO
     setb MOSI
     setb SCK
     clr MEM_RESET
     clr MEM_WP

     mov SPCON,#01111100B     ; Inicializa o Master SPI, interrupcao
                              ; desabilitada,pino habilitado,MSB primeiro,
                              ; polaridade 1, fase 1 e clock rate /2
                              
     setb  LED_VM
     setb LED_VR

     mov a,#60
     lcall _DELAY_MS          ; Pequeno delay para as tensoes do circuito estabilizarem

     setb  ESP_RXD
     setb  ESP_TXD

   ;- Inicia variaveis


     ret

