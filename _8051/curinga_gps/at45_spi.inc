;  +------------------------------------------------------------------------------+
;  | Rotinas para ler e escrever na Serial Data Flash AT45D0xxA via interface SPI |
;-=| Autor: Bruno Marcio Diogo Venancio                             Data 20/09/03 |=-
;  | Microcontrolador : AT89S8252 em 22.1184MHz                     v1.0          |
;  +------------------------------------------------------------------------------+


;----{ Comandos e definicoes de bits do AT45D081A }--------------------

;-| Read commands |--

STSR       EQU   57H                ; Status register
;WRSR       EQU
MMPR       EQU   52H                ; Main memory page read
B1RD       EQU   54H                ; Buffer 1 read
B2RD       EQU   56H                ; Buffer 1 read
CARD       EQU   68H                ; Continuous array read

;-| Write commands |--

B1WR       EQU   84H                ; Buffer 1 write
B2WR       EQU   87H                ; Buffer 2 write
PAER       EQU   81H                ; Page Erase
BLER       EQU   50H                ; Block erase
MP2B1      EQU   82H                ; Main memory page program through buffer 1
MP2B2      EQU   85H                ; Main memory page program through buffer 2

B1MPE      EQU   83H                ; Buffer1 to memory page program with built-in erase
B2MPE      EQU   86H                ; Buffer2 to memory page program with built-in erase

B1MP       EQU   88H                ; Buffer1 to memory page program without built-in erase
B2MP       EQU   89H                ; Buffer2 to memory page program without built-in erase

;-| Additional Commands |--

MPB1T      EQU   53H                ; Main Memory Page to Buffer 1 Transfer
MPB2T      EQU   55H                ; Main Memory Page to Buffer 2 Transfer

MPB1C      EQU   60H                ; Main Memory Page to Buffer 1 comapare
MPB2C      EQU   61H                ; Main Memory Page to Buffer 2 comapare

APRB1      EQU   58H                ; Auto Page Rewrite through buffer 1
APRB2      EQU   59H                ; Auto Page Rewrite through buffer 2

;------------------------------------------------------------------------------------

;+------------------------------------------------------------------------------+
;| Rotina: Envia/recebe dados pela porta SPI                                    |
;|        - Byte enviado assim como e' recebido                                 |
;|        - Envio/recebimento simultaneo                                        |
;|        - Espera pelo envio/recebimento seja completado antes de sair         |
;| Entrada/saida: dado no Acc para enviar, retorna dado recebido tambem em Acc  |
;+------------------------------------------------------------------------------+

_AT45_MASTER_IO:

           mov SPDAT,a                    ; Carrega dado de saida

MASTER_BBB:
           mov a,SPSTA                    ; pega o status
           jnb acc.7,MASTER_BBB

           mov   a,SPDAT                  ; Ler dado de entrada

           ret


;+---------------------------------------------------------------------------------+
;| Rotina: Recebe status da Flash data                                             |
;| Saida : A                                                                       |
;|                                                                                 |
;| Formato do byte de status:                                                      |
;|  +---------+------+------+------+------+------+------+------+                   |
;|  |Bit7     | Bit6 | Bit5 | Bit4 | Bit3 | Bit2 | Bit1 | Bit0 |                   |
;|  |RDY/~BUSY| COMP |   1  |  0   |  0   |  0   |  X   |  X   |                   |
;|  +---------+------+------+------+------+------+------+------+                   |
;|                                                                                 |
;| Descricao dos bits:                                                             |
;| - RDY/~BUSY -> Quando em 0,dispositivo ocupado , quando em 1 esta livre         |
;| - Operacoes que envolvem bit 6 e 7 : MAIN MEMORY PAGE TO BUFFER TRANSFER,       |
;|   MAIN MEMORY PAGE TO BUFFER COMPARE, BUFFER TO MAIN MEMORY PAGE PROGRAM WITH   |
;|   BUILT-IN ERASE, PAGE ERASE, BLOCK ERASE, MAIN MEMROY PAGE PROGRAM , AUTO PAGE |
;|   REWRITE e BUFFER TO MAIN MEMORY PAGE PROGRAM WITHOUT BUILT-IN ERASE.          |
;| - Bit 3,4 e 5 : Densidade da Flash                                              |
;|   Tabela dos modelos:                                                           |
;|                                                                                 |
;|   +------------------------+                                                    |
;|   |Bit  5 4 3 2  Densidade |                                                    |
;|   |                        |                                                    |
;|   |     0 0 1 1     1M     |                                                    |
;|   |     0 1 0 1     2M     |                                                    |
;|   |     0 1 1 1     4M     |                                                    |
;|   |     1 0 0 1     8M     |                                                    |
;|   |     1 0 1 1    16M     |                                                    |
;|   |     1 1 0 1    32M     |                                                    |
;|   |     1 1 1 1    64M     |                                                    |
;|   |     0 1 0 0    128M    |                                                    |
;|   |     0 1 1 0    256M    |                                                    |
;|   |     1 0 0 0    512M    |                                                    |
;|   +------------------------+                                                    |
;|                                                                                 |
;| - Bits X,X : Reservado para uso futuro                                          |
;+---------------------------------------------------------------------------------+



_AT45_GET_STATUS:

           clr CS_

           push ACC

           mov a,#57H
           acall  _AT45_MASTER_IO

AT45_VERI_LOOP:

           acall  _AT45_MASTER_IO
           mov a,SPDAT

           jnb ACC.7,AT45_VERI_LOOP

           pop ACC
           setb CS_


           ret



;+-------------------------------------------------------------------------------+
;| Rotina: MAIN MEMORY PAGE READ                                                 |
;| Entradas: DPTR -> Numero da pagina (0 a 4095) , BUFFER_BYTE -> 0 a 255        |
;| Saida: Dado no ACC                                                            |
;| Funcao : Permite a leitura diretamente das 4096 paginas na memoria principal  |
;|          sem precisar usar os buffers e deixando os dados dos buffers intactos|
;+-------------------------------------------------------------------------------+

_AT45_MAIN_PAGE_READ:

           push DPH                       ; salva DPTR
           push DPL

            mov a,DPH                      ; Filtra dptr para ir somente ate 4095
            anl a,#00001111B
            mov DPH,a

            clr C                          ; adequa o dptr da forma exigida pelo AT45cXX
            mov a,DPL
            rlc a
            mov DPL,a

            mov a,DPH
            rlc a
            mov DPH,a

         ;  clr C                          ; adequa o dptr da forma exigida pelo AT45cXX
         ;  mov a,DPL
         ;  rlc a
         ;  mov DPL,a

         ;  mov a,DPH
         ;  rlc a
         ;  mov DPH,a

          ;-
           clr CS_                        ; Habilita a Data-Flash

          ;-- Master IO
           mov SPDAT,#52H                 ; Comando MAIN MEMORY PAGE READ
MASTER_B01:
           mov a,SPSTA                    ; pega o status
           jnb acc.7,MASTER_B01
          ;--

          ; Manda parte alta da pagina

          ;-- Master IO
           mov SPDAT,DPH                    ; Carrega dado de saida
MASTER_B02:
           mov a,SPSTA                    ; pega o status
           jnb acc.7,MASTER_B02
          ;--

          ; Manda parte baixa da pagina
          ;-- Master IO
           mov SPDAT,DPL                    ; Carrega dado de saida
MASTER_B03:
           mov a,SPSTA                    ; pega o status
           jnb acc.7,MASTER_B03
          ;--

          ; Manda byte buffer

          ;-- Master IO
           mov SPDAT,BUFFER_BYTE                    ; Carrega dado de saida
MASTER_B04:
           mov a,SPSTA                    ; pega o status
           jnb acc.7,MASTER_B04
          ;--

          ; 4 Bytes irrelevantes,mas requeridos

          ;-- Master IO
           mov SPDAT,a                    ; Carrega dado de saida
MASTER_B05:
           mov a,SPSTA                    ; pega o status
           jnb acc.7,MASTER_B05
          ;--
          ;-- Master IO
           mov SPDAT,a                    ; Carrega dado de saida
MASTER_B06:
           mov a,SPSTA                    ; pega o status
           jnb acc.7,MASTER_B06
          ;--
          ;-- Master IO
           mov SPDAT,a                    ; Carrega dado de saida
MASTER_B07:
           mov a,SPSTA                    ; pega o status
           jnb acc.7,MASTER_B07
          ;--
          ;-- Master IO
           mov SPDAT,a                    ; Carrega dado de saida
MASTER_B08:
           mov a,SPSTA                    ; pega o status
           jnb acc.7,MASTER_B08
           mov   a,SPDAT                  ; Ler dado de entrada
          ;--

           ; Dado no ACC

          ;-- Master IO
           mov SPDAT,a                    ; Carrega dado de saida
MASTER_B09:
           mov a,SPSTA                    ; pega o status
           jnb acc.7,MASTER_B09
           mov   a,SPDAT                  ; Ler dado de entrada
          ;--

           swap a   ; desembaralha o dado

           setb CS_

           pop DPL                        ; Recupera DPTR
           pop DPH


           ret



;+-------------------------------------------------------------------------------+
;| Rotina: MAIN MEMORY PAGE PROGRAM THROUGH BUFFER 1 e 2                         |
;| Entradas: DPTR -> Numero da pagina (0 a 4095) , BUFFER_BYTE -> 0 a 255        |
;|           Dado a ser gravado no ACC                                           |
;| Funcao : Permite a escrita diretamente das 4096 paginas na memoria principal  |
;|          passando pelos buffers 1 ou 2                                        |
;+-------------------------------------------------------------------------------+

_AT45_MAIN_PAGE_WRITE_B1:

           setb MEM_WP              ; Desproteje a memoria contra gravaçao

           push DPL
           push DPH
           push buffer_byte

           push acc                 ; Guarda dado

           mov a,DPH                ; Filtra dptr para ir somente ate 4095
           anl a,#00001111B
           mov DPH,a

          clr C                    ; adequa o dptr da forma exigida pelo AT45cXX
          mov a,DPL
          rlc a
          mov DPL,a

          mov a,DPH
          rlc a
          mov DPH,a
         ;-

        ;  clr C                    ; adequa o dptr da forma exigida pelo AT45cXX
        ;  mov a,DPL
        ;  rlc a
        ;  mov DPL,a

        ;  mov a,DPH
        ;  rlc a
        ;  mov DPH,a


           clr CS_

           mov a,#82H               ; Comando:
           acall  _AT45_MASTER_IO     ; MAIN MEMORY PAGE PROGRAM THROUGH BUFFER 1

           mov a,DPH
           acall  _AT45_MASTER_IO     ; Manda parte alta da pagina

           mov a,DPL
           acall  _AT45_MASTER_IO     ; Manda parte baixa da pagina

           mov a,BUFFER_BYTE        ; Manda byte buffer
           acall  _AT45_MASTER_IO

           pop acc                  ; Recupera dado

           swap a

           acall  _AT45_MASTER_IO     ; grava na memoria

           setb CS_                  ; Desabilita a Data-Flash

          ; clr MEM_WP

           acall  _AT45_GET_STATUS    ; Verifica se esta pronto

           pop buffer_byte
           pop DPH
           pop DPL


           ret





;+-----------------------------------+
;| Rotina: INCREMENTA ENDERECO       |
;| Funcao : ENDERECO = ENDERECO +1   |
;+-----------------------------------+

_INCREMENTA_ENDERECO:

           clr C
           mov a,BUFFER_BYTE                        ; Incrementa endereco
           add a,#1
           mov BUFFER_BYTE,a
           jnc INCREMENTA_FIM

           inc DPTR
          ; clr C

INCREMENTA_FIM:

           RET


_DECREMENTA_ENDERECO:

           clr C

           mov a,BUFFER_BYTE
           subb a,#1
           mov BUFFER_BYTE,a

           mov a,dpl
           subb a,#0
           mov dpl,a

           mov a,dph
           subb a,#0
           mov dph,a

           ret


;-----------------------------------------------------------------------


;+-------------------------------------------------------------------------------+
;| Rotina: MAIN MEMORY PAGE TO BUFFER1 TRANSFER                                  |
;| Entradas: DPTR -> Numero da pagina (0 a 4095)                                 |
;| Saida: Dado no ACC                                                            |
;| Funcao : Permite a leitura diretamente das 4096 paginas na memoria principal  |
;|          sem precisar usar os buffers e deixando os dados dos buffers intactos|
;+-------------------------------------------------------------------------------+

_AT45_MAIN_PAGE_TO_BUFFER1_TRANSF:

           push DPL
           push DPH


          mov a,DPH                      ; Filtra dptr para ir somente ate 4095
          anl a,#00001111B
          mov DPH,a

          clr C                          ; adequa o dptr da forma exigida pelo AT45cXX
          mov a,DPL
          rlc a
          mov DPL,a

          mov a,DPH
          rlc a
          mov DPH,a


        ;  clr C                          ; adequa o dptr da forma exigida pelo AT45cXX
        ;  mov a,DPL
        ;  rlc a
        ;  mov DPL,a

        ;  mov a,DPH
        ;  rlc a
        ;  mov DPH,a


           clr CS_                        ; Habilita a Data-Flash

           mov a,#53H                     ; Comando MAIN MEMORY PAGE TO BUFFER 1 TRANFER
           acall  _AT45_MASTER_IO

           mov a,DPH
           acall  _AT45_MASTER_IO

           mov a,DPL
           acall  _AT45_MASTER_IO

           acall  _AT45_MASTER_IO          ; Byte irrelevante

           setb CS_                       ; Desabilita a Data-Flash

           acall  _AT45_GET_STATUS         ; Verifica se esta pronto

           pop DPH
           pop DPL


           ret





;+-----------------------------------+
;| Rotina: CLEAR ALL                 |
;| Funcao : Apaga a memoria inteira  |
;+-----------------------------------+

_AT45_CLEAR_ALL:

           push DPL
           push DPH
           push ACC

           mov BUFFER_BYTE,#0

_AT45_CLEAR_ALL_LOOP:

           mov A,#0FFH
           acall  _WRITE_BUFFER1

           inc BUFFER_BYTE

           mov A,BUFFER_BYTE

           cjne A,#0FFH,_AT45_CLEAR_ALL_LOOP

           cpl LED5

           mov DPTR,#0

_AT45_CLEAR_ALL_LOOP2:

           acall  _AT45_BUFFER1_TO_MAIN_MEMORY

           inc DPTR
           mov A,DPH
           mov B,#HIGH(2047)
           cjne A,B,_AT45_CLEAR_ALL_LOOP2

           mov A,DPL
           mov B,#LOW(2047)
           cjne A,B,_AT45_CLEAR_ALL_LOOP2

           mov dptr,#2047
           mov BUFFER_BYTE,#0

_AT45_CLEAR_ALL_LOOP3:

           mov A,#0ffh
           acall _AT45_MAIN_PAGE_WRITE_B1

           inc BUFFER_BYTE
           mov A,BUFFER_BYTE

           cjne A,#0ffh,_AT45_CLEAR_ALL_LOOP3

           pop ACC
           pop DPH
           pop DPL


           RET



;+--------------------------------------------------------+
;| Rotina: BUFFER 1 READ                                  |
;| Entradas: BUFFER_BYTE -> 0 a 255                       |
;| Saida: Dado no Acc                                     |
;| Funcao : Le diretamente no Buffer 1 de 256 bytes       |
;+--------------------------------------------------------+

_READ_BUFFER1:

           clr CS_                        ; Habilita a Data-Flash

           mov a,#54H                     ; Comando:
           acall  _AT45_MASTER_IO          ; BUFFER 1 READ

           mov a,#0
           acall  _AT45_MASTER_IO          ; Byte irrelevante

           mov a,#0
           acall  _AT45_MASTER_IO          ; Byte irrelevante

           mov a,BUFFER_BYTE              ; Endereco do byte
           acall  _AT45_MASTER_IO

           acall  _AT45_MASTER_IO          ; Byte irrelevante

           acall  _AT45_MASTER_IO          ; Pega o byte lido e coloca em A

           setb CS_                       ; Desabilita a Data-Flash

           ret



;+--------------------------------------------------------+
;| Rotina: BUFFER 1 e 2 WRITE                             |
;| Entradas: BUFFER_BYTE -> 0 a 255                       |
;|           Dado a ser gravado no ACC                    |
;| Funcao :  Escreve diretamente no Buffer 1 de 256 bytes |
;|                                                        |
;+--------------------------------------------------------+

_WRITE_BUFFER1:

           push ACC                       ; Salva dado

           mov VARIAVEL,a

           clr CS_                        ; Habilita a Data-Flash

           mov a,#84H                     ; Comando:
           acall  _AT45_MASTER_IO          ; BUFFER 1 WRITE

           mov a,#0
           acall  _AT45_MASTER_IO          ; Byte irrelevante


           mov a,#0
           acall  _AT45_MASTER_IO          ; Byte irrelevante


           mov a,BUFFER_BYTE              ; Manda byte buffer
           acall  _AT45_MASTER_IO

           pop acc                        ; Recupera dado

           swap a  ;embaralha

           acall  _AT45_MASTER_IO

           setb CS_                       ; Habilita Data-Flash

           mov a,VARIAVEL

           ret




;+---------------------------------------------------------------------------+
;| Rotina: BUFFER 1 TO MAIN MEMORY PAGE PROGRAM WITH BUILT-IN ERASE          |
;| Entradas: DPTR -> Numero da pagina (0 a 4095)                             |
;|                                                                           |
;| Funcao : Transfere dados do buffer 1 para as paginas da memoria principal |
;|          passando pelos buffers 1 ou 2                                    |
;+---------------------------------------------------------------------------+

_AT45_BUFFER1_TO_MAIN_MEMORY:

           push DPL
           push DPH
           push acc

           mov a,DPH                      ; Filtra dptr para ir somente ate 4095
           anl a,#00001111B
           mov DPH,a

           clr C                          ; Adequa o dptr da forma exigida pelo AT45cXX
           mov a,DPL
           rlc a
           mov DPL,a

           mov a,DPH
           rlc a
           mov DPH,a


        ; at45db161

         ;  clr C                         ; Adequa o dptr da forma exigida pelo AT45cXX
         ;  mov a,DPL
         ;  rlc a
         ;  mov DPL,a

         ;  mov a,DPH
         ;  rlc a
         ;  mov DPH,a


         ;-

           clr CS_                        ; Habilita a Data-Flash

           mov a,#83H                     ; Comando:
           acall _AT45_MASTER_IO          ; BUFFER 1 TO MAIN MEMORY PAGE PROGRAM WITH BUILT-IN ERASE

           mov a,DPH
           acall _AT45_MASTER_IO          ; Manda parte alta da pagina


           mov a,DPL
           acall _AT45_MASTER_IO          ; Manda parte baixa da pagina


           mov a,#0                       ; Manda byte irrelevante
           acall _AT45_MASTER_IO

           setb CS_                       ; Desabilita Flash


           acall _AT45_GET_STATUS         ; Verifica se esta pronto

           pop ACC
           pop DPH
           pop DPL


           ret





