;  +------------------------------------------------------------------------------+
;  | Rotinas para ler e escrever na Serial Data Flash AT45D0xxA via interface SPI |
;-=| Autor: Bruno Marcio Diogo Venancio                             Data 11/09/16 |=-
;  | Microcontrolador : ATMEGA328P                                  v1.0          |
;  +------------------------------------------------------------------------------+

;------------------------------------------------------------------------------------

/*	    
	_AT45DB161B_INIT:
		push	Acc						;save useds
		in	AccT,SREG					;save SREG
		cli							;disable interrupts
		sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_RESET_BIT	;RESET=high
		sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	;CS=high
		cbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_WP_BIT	;WP=low(WRITE PROTECTED)
		sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_SCK_BIT	;SCK=high
		sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_SO_BIT	;SO=high input with pull up
		sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_SI_BIT	;SI=high
		sbi	_AT45DB161B_PORT_DIR,_AT45DB161B_RESET_BIT	;RESET=output
		sbi	_AT45DB161B_PORT_DIR,_AT45DB161B_CS_BIT		;CS=output
		sbi	_AT45DB161B_PORT_DIR,_AT45DB161B_WP_BIT		;WP=low(WRITE PROTECTED) output
		sbi	_AT45DB161B_PORT_DIR,_AT45DB161B_SCK_BIT	;SCK=output
		cbi	_AT45DB161B_PORT_DIR,_AT45DB161B_SO_BIT		;SO=input with pull up
		sbi	_AT45DB161B_PORT_DIR,_AT45DB161B_SI_BIT		;SI=output
		call	_AT45DB161B_RESET				;reset device
		
		ldi Acc,0
		ldi Acc,(1<<SPE|1<<MSTR|1<<CPOL|1<<CPHA)
		out SPCR,Acc
		
		in Acc,SPSR
		ldi Acc,(1<<SPI2X)     ; frequencia da SPI = Fxtal /2
		out SPSR,Acc

		out	SREG,AccT					;restore SREG
		pop	Acc
		ret

	;>--------------------------------------------------------------<
	;| OBJECT     : _AT45DB161B					|
	;>--------------------------------------------------------------<
	;| FUNCTION   : Reset device					|
	;>--------------------------------------------------------------<
	;| INPUT      : None						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	Interrupts are disable during initialization	|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|			
	_AT45DB161B_RESET:
		push	Acc						;save useds
		in	Acc,SREG					;save SREG
		cli							;disable interrupts
		sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	;CS=HIGH
		cbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_RESET_BIT	;RESET=LOW
		pushaw
		_M_DELAY_MICROSECONDS 	40				;pulse low per 40us
		popaw
		sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_RESET_BIT	;RESET=HIGH
		_M_DELAY_MILLISECONDS	30				;wait 30 milliseconds
		out	SREG,Acc					;restore SREG state
		pop	Acc
		ret


;+------------------------------------------------------------------------------+
;| Rotina: Envia/recebe dados pela porta SPI                                    |
;|        - Byte enviado assim como e' recebido                                 |
;|        - Envio/recebimento simultaneo                                        |
;|        - Espera pelo envio/recebimento seja completado antes de sair         |
;| Entrada/saida: dado no Acc para enviar, retorna dado recebido tambem em Acc  |
;+------------------------------------------------------------------------------+

_AT45_MASTER_IO:

           out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB 
           in   Acc,SPDR                   ; Ler dado de entrada

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

           cbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	;CS=LOW

           push Acc

           ldi Acc,0x57
           call _AT45_MASTER_IO

AT45_VERI_LOOP:

           call _AT45_MASTER_IO
           in Acc,SPDR

           sbrs Acc,7
		   rjmp AT45_VERI_LOOP
           pop ACC
           
		   sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	;CS=HIGH


           ret



;+-------------------------------------------------------------------------------+
;| Rotina: MAIN MEMORY PAGE READ                                                 |
;| Entradas: DPTR -> Numero da pagina (0 a 4095) , BUFFER_BYTE -> 0 a 255        |
;| Saida: Dado no ACC                                                            |
;| Funcao : Permite a leitura diretamente das 4096 paginas na memoria principal  |
;|          sem precisar usar os buffers e deixando os dados dos buffers intactos|
;+-------------------------------------------------------------------------------+

_AT45_MAIN_PAGE_READ:

            pushw X

            mov Acc,XH                      ; Filtra dptr para ir somente ate 4095
            andi Acc,0b00001111
            mov XH,Acc

            clc                             ; adequa o dptr da forma exigida pelo AT45cXX
            mov Acc,XL
            rol Acc
            mov XL,Acc

            mov Acc,XH
            rol Acc
            mov XH,Acc


          ;  clc                             ; adequa o dptr da forma exigida pelo AT45cXX
           ; mov Acc,XL
           ; rol Acc
           ; mov XL,Acc

           ; mov Acc,XH
           ; rol Acc
           ; mov XH,Acc

          ;-
            cbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	; habilita data-flash

          ;-- Master IO
            ldi Acc,0x52                  ; Comando MAIN MEMORY PAGE READ
            
			;call _AT45_MASTER_IO  
            out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB1:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB1 
           in   Acc,SPDR                   ; Ler dado de entrada			
						        

          ; Manda parte alta da pagina

           mov Acc,XH
	   
          ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB2:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB2 
           in   Acc,SPDR                   ; Ler dado de entrada       
          ;--

          ; Manda parte baixa da pagina
          
		  ;-- Master IO
		   mov Acc,XL 
   
           ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB3:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB3
           in   Acc,SPDR                   ; Ler dado de entrada        
          ;--

          ; Manda byte buffer

          ;-- Master IO
		   lds Acc,_BUFFER_BYTE
  
		   ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB4:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB4 
           in   Acc,SPDR                   ; Ler dado de entrada                    

          ; 4 Bytes irrelevantes,mas requeridos

          ;-- Master IO

           ;call _AT45_MASTER_IO  
            out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB5:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB5 
           in   Acc,SPDR                   ; Ler dado de entrada
          
		  ;-- Master IO

           ;call _AT45_MASTER_IO  
            out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB6:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB6 
           in   Acc,SPDR                   ; Ler dado de entrada

          ;-- Master IO

          ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB7:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB7 
           in   Acc,SPDR                   ; Ler dado de entrada

          ;-- Master IO

           ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB8:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB8 
           in   Acc,SPDR                   ; Ler dado de entrada
          ;--

           ; Dado no ACC

          ;-- Master IO
           ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB9:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB9 
           in   Acc,SPDR                   ; Ler dado de entrada


           swap Acc  ; desembaralha o dado

		   sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	;CS=HIGH

           popw X
		   
           ret



;+-------------------------------------------------------------------------------+
;| Rotina: MAIN MEMORY PAGE PROGRAM THROUGH BUFFER 1 e 2                         |
;| Entradas: DPTR -> Numero da pagina (0 a 4095) , BUFFER_BYTE -> 0 a 255        |
;|           Dado a ser gravado no ACC                                           |
;| Funcao : Permite a escrita diretamente das 4096 paginas na memoria principal  |
;|          passando pelos buffers 1 ou 2                                        |
;+-------------------------------------------------------------------------------+

_AT45_MAIN_PAGE_WRITE_B1:

     	   sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_WP_BIT	;set unprotected

           pushw X 
           lds AccT,_BUFFER_BYTE

           push Acc                        ; Guarda dado

           mov Acc,XH                      ; Filtra dptr para ir somente ate 4095
           andi Acc,0b00001111
           mov XH,Acc

           clc                             ; adequa o dptr da forma exigida pelo AT45cXX
           mov Acc,XL
           rol Acc
           mov XL,Acc

           mov Acc,XH
           rol Acc
           mov XH,Acc


          ; clc                             ; adequa o dptr da forma exigida pelo AT45cXX
          ; mov Acc,XL
          ; rol Acc
          ; mov XL,Acc

          ; mov Acc,XH
           ;rol Acc
          ; mov XH,Acc

           cbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	; habilita data-flash

          ;-- Master IO
           ldi Acc,0x82                    ; MAIN MEMORY PAGE PROGRAM THROUGH BUFFER 1
           out SPDR,Acc

		   call _AT45_MASTER_IO          
		   
           mov Acc,XH
		   out SPDR,Acc                    ; Carrega dado de saida
		   
           call _AT45_MASTER_IO          

          ; Manda parte baixa da pagina
          
		   mov Acc,XL 
           out SPDR,Acc                    ; Carrega dado de saida

           call _AT45_MASTER_IO          

           lds Acc,_BUFFER_BYTE             ; Manda byte buffer
           call _AT45_MASTER_IO

           pop Acc                         ; Recupera dado

           swap Acc

           call _AT45_MASTER_IO            ; grava na memoria

		   sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	;CS=HIGH

       	   cbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_WP_BIT	;set protected

           call _AT45_GET_STATUS                           ; Verifica se esta pronto

           sts _BUFFER_BYTE,AccT

           popw X
		   

           ret





;+-----------------------------------+
;| Rotina: INCREMENTA ENDERECO       |
;| Funcao : ENDERECO = ENDERECO +1   |
;+-----------------------------------+

_INCREMENTA_ENDERECO:
    
           lds Acc,_BUFFER_BYTE                        ; Incrementa endereco
		   ldi Temp,1
           add Acc,Temp
           sts _BUFFER_BYTE,Acc

           brcc INCREMENTA_FIM
		   adiw X,1
        
INCREMENTA_FIM:

           RET


_DECREMENTA_ENDERECO:

           clc 

           lds Acc,_BUFFER_BYTE
           subi Acc,1
           sts _BUFFER_BYTE,Acc

           mov Acc,XL
           sbci Acc,0
           mov XL,Acc

           mov Acc,XH
           sbci Acc,0
           mov XH,Acc

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

           pushw X
		  
           mov Acc,XH                      ; Filtra dptr para ir somente ate 4095
           andi Acc,0b00001111
           mov XH,Acc

           clc                             ; adequa o dptr da forma exigida pelo AT45cXX
           mov Acc,XL
           rol Acc
           mov XL,Acc

           mov Acc,XH
           rol Acc
           mov XH,Acc


         ;  clc                             ; adequa o dptr da forma exigida pelo AT45cXX
          ; mov Acc,XL
          ; rol Acc
          ; mov XL,Acc

         ;  mov Acc,XH
         ;  rol Acc
         ;  mov XH,Acc

           cbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	; habilita data-flash

           ldi Acc,0x53                   ; Comando MAIN MEMORY PAGE TO BUFFER 1 TRANFER
           call _AT45_MASTER_IO

           mov Acc,XH
           call _AT45_MASTER_IO

           mov Acc,XL
           call _AT45_MASTER_IO

           call _AT45_MASTER_IO           ; Byte irrelevante

   		   sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	;CS=HIGH

           call _AT45_GET_STATUS         ; Verifica se esta pronto

           popw X
		   
           ret

		   
;+--------------------------------------------------------+
;| Rotina: BUFFER 1 e 2 WRITE                             |
;| Entradas: BUFFER_BYTE -> 0 a 255                       |
;|           XL com o bit da parte alta dos 512           |
;|           Dado a ser gravado no ACC                    |
;| Funcao :  Escreve diretamente no Buffer 1 de 256 bytes |
;|                                                        |
;+--------------------------------------------------------+

_WRITE_BUFFER1:

           push ACC                       ; Salva dado

           sts _VARIAVEL,Acc

           cbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	; habilita data-flash

           ldi Acc,0x84                   ; Comando:
           call _AT45_MASTER_IO           ; BUFFER 1 WRITE

           ldi Acc,0
           call _AT45_MASTER_IO           ; Byte irrelevante


           ;mov   Acc,XL                   ; Bit da parte alta dos 512bytes 
		   ;andi  Acc,1
           ldi Acc,0
		   call _AT45_MASTER_IO             


           lds Acc,_BUFFER_BYTE            ; Manda byte buffer
           call _AT45_MASTER_IO

           pop Acc                        ; Recupera dado

           swap Acc  ;embaralha

           call _AT45_MASTER_IO

   		   sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	;CS=HIGH

           lds Acc,_VARIAVEL

           ret


		   
;+--------------------------------------------------------+
;| Rotina: BUFFER 1 e 2 WRITE                             |
;| Entradas: BUFFER_BYTE -> 0 a 255                       |
;|           XL com o bit da parte alta dos 512           |
;|           Dado a ser gravado no ACC                    |
;| Funcao :  Escreve diretamente no Buffer 1 de 256 bytes |
;|                                                        |
;+--------------------------------------------------------+

_WRITE_BUFFER2:

           push ACC                       ; Salva dado

           sts _VARIAVEL,Acc

           cbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	; habilita data-flash

           ldi Acc,0x87                   ; Comando:
           call _AT45_MASTER_IO           ; BUFFER 1 WRITE

           ldi Acc,0
           call _AT45_MASTER_IO           ; Byte irrelevante


           mov   Acc,XL                   ; Bit da parte alta dos 512bytes 
		   andi  Acc,1
           call _AT45_MASTER_IO             


           lds Acc,_BUFFER_BYTE            ; Manda byte buffer
           call _AT45_MASTER_IO

           pop Acc                        ; Recupera dado

           swap Acc  ;embaralha

           call _AT45_MASTER_IO

   		   sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	;CS=HIGH

           lds Acc,_VARIAVEL

           ret


;+---------------------------------------------------------------------------+
;| Rotina: BUFFER 1 TO MAIN MEMORY PAGE PROGRAM WITH BUILT-IN ERASE          |
;| Entradas: DPTR -> Numero da pagina (0 a 4095)                             |
;|                                                                           |
;| Funcao : Transfere dados do buffer 1 para as paginas da memoria principal |
;|          passando pelos buffers 1 ou 2                                    |
;+---------------------------------------------------------------------------+

_AT45_BUFFER1_TO_MAIN_MEMORY:

           pushw X
           push Acc

           mov Acc,XH                      ; Filtra dptr para ir somente ate 4095
           andi Acc,0b00001111
           mov XH,Acc

           clc                             ; adequa o dptr da forma exigida pelo AT45cXX
           mov Acc,XL
           rol Acc
           mov XL,Acc

           mov Acc,XH
           rol Acc
           mov XH,Acc


          ; clc                             ; adequa o dptr da forma exigida pelo AT45cXX
          ; mov Acc,XL
          ; rol Acc
         ;  mov XL,Acc

        ;   mov Acc,XH
        ;   rol Acc
        ;   mov XH,Acc
		   
         ;-

           cbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	   ; Habilita data-flash

           ldi Acc,0x83                   ; BUFFER 1 TO MAIN MEMORY PAGE PROGRAM WITH BUILT-IN ERASE
           call _AT45_MASTER_IO

           mov Acc,XH
           call _AT45_MASTER_IO

           mov Acc,XL
           call _AT45_MASTER_IO

           ldi Acc,0                       ; Manda byte irrelevante
           call _AT45_MASTER_IO

           sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	;CS=HIGH

           call _AT45_GET_STATUS         ; Verifica se esta pronto

           pop ACC
           popw X

           ret





;+-----------------------------------+
;| Rotina: CLEAR ALL                 |
;| Funcao : Apaga a memoria inteira  |
;+-----------------------------------+

_AT45_CLEAR_ALL:
          
		   push Acc
		   pushw X

		   ldi Acc,0
           sts _BUFFER_BYTE,Acc
		   ldiw X,0

_AT45_CLEAR_ALL_LOOP:

           ldi Acc,0xFF
           call  _WRITE_BUFFER1

           call _INCREMENTA_ENDERECO

           lds Acc,_BUFFER_BYTE
		   cpi Acc,0xFF
		   brne _AT45_CLEAR_ALL_LOOP

		   mov Acc,XL
		   cpi Acc,1
		   brne _AT45_CLEAR_ALL_LOOP
		   		   
           ldiw X,0

_AT45_CLEAR_ALL_LOOP2:

           call  _AT45_BUFFER1_TO_MAIN_MEMORY

           adiw X,1
           mov Acc,XH
		   cpi Acc,HIGH(2047)
		   brne _AT45_CLEAR_ALL_LOOP2
           
           mov Acc,XL
		   cpi Acc,LOW(2047)
		   brne _AT45_CLEAR_ALL_LOOP2

           ldiw X,2047
           ldi Acc,0
		   sts _BUFFER_BYTE,Acc

_AT45_CLEAR_ALL_LOOP3:

           ldi Acc,0xFF
           call _AT45_MAIN_PAGE_WRITE_B1

           lds Acc,_BUFFER_BYTE
		   inc Acc
           sts _BUFFER_BYTE,Acc

		   cpi Acc,0xFF
		   brne _AT45_CLEAR_ALL_LOOP3

           popw X
		   pop Acc
		   ret




;+-------------------------------------------------------------------------------+
;| Rotina: MAIN MEMORY CONTINUOUS                                                |
;| Entradas: DPTR -> Numero da pagina (0 a 4095) , BUFFER_BYTE -> 0 a 255        |
;| Saida: Dado no ACC                                                            |
;| Funcao : Permite a leitura diretamente das 4096 paginas na memoria principal  |
;|          sem precisar usar os buffers e deixando os dados dos buffers intactos|
;+-------------------------------------------------------------------------------+

_AT45_READ_CONTINUOUS:

           ;pushw X
		  
           mov Acc,XH                      ; Filtra dptr para ir somente ate 4095
           andi Acc,0b00001111
           mov XH,Acc

           clc                             ; adequa o dptr da forma exigida pelo AT45cXX
           mov Acc,XL
           rol Acc
           mov XL,Acc

           mov Acc,XH
           rol Acc
           mov XH,Acc
		   
         ;  clc                             ; adequa o dptr da forma exigida pelo AT45cXX
         ;  mov Acc,XL
          ; rol Acc
         ;  mov XL,Acc

          ; mov Acc,XH
         ;  rol Acc
         ;  mov XH,Acc

           cbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_CS_BIT	; habilita data-flash

  ;-- Master IO
           ldi Acc,0x68                   ; Comando READ CONTINUOUS
            
		   ;call _AT45_MASTER_IO  
           out SPDR,Acc                   ; Carrega dado de saida

CMASTER_B01:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B01 
           in   Acc,SPDR                   ; Ler dado de entrada			
			
          ; Manda parte alta da pagina

           mov Acc,XH
	   
          ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

CMASTER_B02:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B02 
           in   Acc,SPDR                   ; Ler dado de entrada       
          ;--

          ; Manda parte baixa da pagina
          
		  ;-- Master IO
		   mov Acc,XL 
   
           ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

CMASTER_B03:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B03
           in   Acc,SPDR                   ; Ler dado de entrada        
          ;--

          ; Manda byte buffer

          ;-- Master IO
		   lds Acc,_BUFFER_BYTE
  
		   ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

CMASTER_B04:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B04 
           in   Acc,SPDR                   ; Ler dado de entrada                    

          ; 4 Bytes irrelevantes,mas requeridos

          ;-- Master IO

           ;call _AT45_MASTER_IO  
           ldi Acc,0
		   out SPDR,Acc                    ; Carrega dado de saida

CMASTER_B05:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B05 
           in   Acc,SPDR                   ; Ler dado de entrada
          
		  ;-- Master IO

           ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

CMASTER_B06:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B06 
           in   Acc,SPDR                   ; Ler dado de entrada

          ;-- Master IO

          ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

CMASTER_B07:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B07 
           in   Acc,SPDR                   ; Ler dado de entrada

          ;-- Master IO

           ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

CMASTER_B08:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B08 
           in   Acc,SPDR                   ; Ler dado de entrada
          ;--
		  
        ;   popw X
		   
           ret


*/

;  +------------------------------------------------------------------------------+
;  | Rotinas para ler e escrever na Serial Data Flash AT45D0xxA via interface SPI |
;-=| Autor: Bruno Marcio Diogo Venancio                             Data 11/09/16 |=-
;  | Microcontrolador : ATMEGA328P                                  v1.0          |
;  +------------------------------------------------------------------------------+

;------------------------------------------------------------------------------------

	    
	_AT45DB161B_INIT:
		push	Acc						;save useds
		in	AccT,SREG					;save SREG
		cli							;disable interrupts
		sbi	_AT45DB161B_RESET_OUTPUT,_AT45DB161B_RESET_BIT	;RESET=high
		sbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	;CS=high
		cbi	_AT45DB161B_WP_OUTPUT,_AT45DB161B_WP_BIT	;WP=low(WRITE PROTECTED)
		sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_SCK_BIT	;SCK=high
		sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_SO_BIT	;SO=high input with pull up
		sbi	_AT45DB161B_PORT_OUTPUT,_AT45DB161B_SI_BIT	;SI=high
		
		sbi	_AT45DB161B_RESET_DIR,_AT45DB161B_RESET_BIT	;RESET=output
		sbi	_AT45DB161B_PORT_DIRC,_AT45DB161B_CS_BIT		;CS=output
		sbi	_AT45DB161B_WP_DIR,_AT45DB161B_WP_BIT		;WP=low(WRITE PROTECTED) output
		sbi	_AT45DB161B_PORT_DIR,_AT45DB161B_SCK_BIT	;SCK=output
		cbi	_AT45DB161B_PORT_DIR,_AT45DB161B_SO_BIT		;SO=input with pull up
		sbi	_AT45DB161B_PORT_DIR,_AT45DB161B_SI_BIT		;SI=output
	
	    call	_AT45DB161B_RESET				;reset device
		

		sbi	_AT45DB161B_PORT_OUTPUT,2
		sbi _AT45DB161B_PORT_DIR,2

		ldi Acc,0
		ldi Acc,(1<<SPE)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA)
		out SPCR,Acc
		
		in Acc,SPSR
		ldi Acc,(1<<SPI2X)     ; frequencia da SPI = Fxtal /2
		out SPSR,Acc

		out	SREG,AccT					;restore SREG
		pop	Acc
		ret

	;>--------------------------------------------------------------<
	;| OBJECT     : _AT45DB161B					|
	;>--------------------------------------------------------------<
	;| FUNCTION   : Reset device					|
	;>--------------------------------------------------------------<
	;| INPUT      : None						|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	Interrupts are disable during initialization	|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|			
	_AT45DB161B_RESET:
		push	Acc						;save useds
		in	Acc,SREG					;save SREG
		cli							;disable interrupts
		sbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	;CS=HIGH
		cbi	_AT45DB161B_RESET_OUTPUT,_AT45DB161B_RESET_BIT	;RESET=LOW
		pushaw
		_M_DELAY_MICROSECONDS 	40				;pulse low per 40us
		popaw
		sbi	_AT45DB161B_RESET_OUTPUT,_AT45DB161B_RESET_BIT	;RESET=HIGH
		_M_DELAY_MILLISECONDS	30				;wait 30 milliseconds
		out	SREG,Acc					;restore SREG state
		pop	Acc
		ret


;+------------------------------------------------------------------------------+
;| Rotina: Envia/recebe dados pela porta SPI                                    |
;|        - Byte enviado assim como e' recebido                                 |
;|        - Envio/recebimento simultaneo                                        |
;|        - Espera pelo envio/recebimento seja completado antes de sair         |
;| Entrada/saida: dado no Acc para enviar, retorna dado recebido tambem em Acc  |
;+------------------------------------------------------------------------------+

_AT45_MASTER_IO:

           out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB 
           in   Acc,SPDR                   ; Ler dado de entrada


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

           cbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	;CS=LOW

           push Acc

           ldi Acc,0x57
           call _AT45_MASTER_IO

AT45_VERI_LOOP:

           call _AT45_MASTER_IO
           in Acc,SPDR

           sbrs Acc,7
		   jmp AT45_VERI_LOOP
           pop ACC
           
		   sbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	;CS=HIGH


           ret



;+-------------------------------------------------------------------------------+
;| Rotina: MAIN MEMORY PAGE READ                                                 |
;| Entradas: DPTR -> Numero da pagina (0 a 4095) , BUFFER_BYTE -> 0 a 255        |
;| Saida: Dado no ACC                                                            |
;| Funcao : Permite a leitura diretamente das 4096 paginas na memoria principal  |
;|          sem precisar usar os buffers e deixando os dados dos buffers intactos|
;+-------------------------------------------------------------------------------+

_AT45_MAIN_PAGE_READ:

            pushw X

            mov Acc,XH                      ; Filtra dptr para ir somente ate 4095
            andi Acc,0b00001111
            mov XH,Acc

            clc                             ; adequa o dptr da forma exigida pelo AT45cXX
            mov Acc,XL
            rol Acc
            mov XL,Acc

            mov Acc,XH
            rol Acc
            mov XH,Acc


          ;  clc                             ; adequa o dptr da forma exigida pelo AT45cXX
          ;  mov Acc,XL
          ;  rol Acc
          ;  mov XL,Acc

          ;  mov Acc,XH
          ;  rol Acc
          ;  mov XH,Acc

          ;-
            cbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	; habilita data-flash

          ;-- Master IO
            ldi Acc,0x52                  ; Comando MAIN MEMORY PAGE READ
            
			;call _AT45_MASTER_IO  
            out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB1:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB1 
		   in   Acc,SPDR                   ; Ler dado de entrada			
						        

          ; Manda parte alta da pagina

           mov Acc,XH
	   
          ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB2:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB2 
		   in   Acc,SPDR                   ; Ler dado de entrada       
          ;--

          ; Manda parte baixa da pagina
          
		  ;-- Master IO
		   mov Acc,XL 
   
           ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB3:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB3
		   in   Acc,SPDR                   ; Ler dado de entrada        
          ;--

          ; Manda byte buffer

          ;-- Master IO
		   lds Acc,_BUFFER_BYTE
  
		   ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB4:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB4 
		   in   Acc,SPDR                   ; Ler dado de entrada                    

          ; 4 Bytes irrelevantes,mas requeridos

          ;-- Master IO

           ;call _AT45_MASTER_IO  
            out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB5:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB5 
		   in   Acc,SPDR                   ; Ler dado de entrada
          
		  ;-- Master IO

           ;call _AT45_MASTER_IO  
            out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB6:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB6 
		   in   Acc,SPDR                   ; Ler dado de entrada

          ;-- Master IO

          ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB7:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB7 
		   in   Acc,SPDR                   ; Ler dado de entrada

          ;-- Master IO

           ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB8:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB8 
		   in   Acc,SPDR                   ; Ler dado de entrada
          ;--

           ; Dado no ACC

          ;-- Master IO
           ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

MASTER_BBB9:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp MASTER_BBB9 
		   in   Acc,SPDR                   ; Ler dado de entrada


           swap Acc  ; desembaralha o dado

		   sbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	;CS=HIGH

           popw X
		   
           ret



;+-------------------------------------------------------------------------------+
;| Rotina: MAIN MEMORY PAGE PROGRAM THROUGH BUFFER 1 e 2                         |
;| Entradas: DPTR -> Numero da pagina (0 a 4095) , BUFFER_BYTE -> 0 a 255        |
;|           Dado a ser gravado no ACC                                           |
;| Funcao : Permite a escrita diretamente das 4096 paginas na memoria principal  |
;|          passando pelos buffers 1 ou 2                                        |
;+-------------------------------------------------------------------------------+

_AT45_MAIN_PAGE_WRITE_B1:

     	   sbi	_AT45DB161B_WP_OUTPUT,_AT45DB161B_WP_BIT	;set unprotected

           pushw X 
           lds AccT,_BUFFER_BYTE

           push Acc                        ; Guarda dado

           mov Acc,XH                      ; Filtra dptr para ir somente ate 4095
           andi Acc,0b00001111
           mov XH,Acc

           clc                             ; adequa o dptr da forma exigida pelo AT45cXX
           mov Acc,XL
           rol Acc
           mov XL,Acc

           mov Acc,XH
           rol Acc
           mov XH,Acc


         ;  clc                             ; adequa o dptr da forma exigida pelo AT45cXX
         ;  mov Acc,XL
         ;  rol Acc
         ;  mov XL,Acc

          ; mov Acc,XH
          ; rol Acc
          ; mov XH,Acc

           cbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	; habilita data-flash

          ;-- Master IO
           ldi Acc,0x82                    ; MAIN MEMORY PAGE PROGRAM THROUGH BUFFER 1
           out SPDR,Acc

		   call _AT45_MASTER_IO          
		   
           mov Acc,XH
		   out SPDR,Acc                    ; Carrega dado de saida
		   
           call _AT45_MASTER_IO          

          ; Manda parte baixa da pagina
          
		   mov Acc,XL 
           out SPDR,Acc                    ; Carrega dado de saida

           call _AT45_MASTER_IO          

           lds Acc,_BUFFER_BYTE             ; Manda byte buffer
           call _AT45_MASTER_IO

           pop Acc                         ; Recupera dado

           swap Acc

           call _AT45_MASTER_IO            ; grava na memoria

		   sbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	;CS=HIGH

       	   cbi	_AT45DB161B_WP_OUTPUT,_AT45DB161B_WP_BIT	;set protected

           call _AT45_GET_STATUS                           ; Verifica se esta pronto

           sts _BUFFER_BYTE,AccT

           popw X
		   

           ret





;+-----------------------------------+
;| Rotina: INCREMENTA ENDERECO       |
;| Funcao : ENDERECO = ENDERECO +1   |
;+-----------------------------------+

_INCREMENTA_ENDERECO:
    
           lds Acc,_BUFFER_BYTE                        ; Incrementa endereco
		   ldi Temp,1
           add Acc,Temp
           sts _BUFFER_BYTE,Acc

           brcc INCREMENTA_FIM
		   adiw X,1
        
INCREMENTA_FIM:

           RET


_DECREMENTA_ENDERECO:

           clc 

           lds Acc,_BUFFER_BYTE
           subi Acc,1
           sts _BUFFER_BYTE,Acc

           mov Acc,XL
           sbci Acc,0
           mov XL,Acc

           mov Acc,XH
           sbci Acc,0
           mov XH,Acc

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

           pushw X
		  
           mov Acc,XH                      ; Filtra dptr para ir somente ate 4095
           andi Acc,0b00001111
           mov XH,Acc

           clc                             ; adequa o dptr da forma exigida pelo AT45cXX
           mov Acc,XL
           rol Acc
           mov XL,Acc

           mov Acc,XH
           rol Acc
           mov XH,Acc


         ;  clc                             ; adequa o dptr da forma exigida pelo AT45cXX
         ;  mov Acc,XL
         ;  rol Acc
         ;  mov XL,Acc

          ; mov Acc,XH
          ; rol Acc
          ; mov XH,Acc

           cbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	; habilita data-flash

           ldi Acc,0x53                   ; Comando MAIN MEMORY PAGE TO BUFFER 1 TRANFER
           call _AT45_MASTER_IO

           mov Acc,XH
           call _AT45_MASTER_IO

           mov Acc,XL
           call _AT45_MASTER_IO

           call _AT45_MASTER_IO           ; Byte irrelevante

   		   sbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	;CS=HIGH

           call _AT45_GET_STATUS         ; Verifica se esta pronto

           popw X
		   
           ret

		   
;+--------------------------------------------------------+
;| Rotina: BUFFER 1 e 2 WRITE                             |
;| Entradas: BUFFER_BYTE -> 0 a 255                       |
;|           XL com o bit da parte alta dos 512           |
;|           Dado a ser gravado no ACC                    |
;| Funcao :  Escreve diretamente no Buffer 1 de 256 bytes |
;|                                                        |
;+--------------------------------------------------------+

_WRITE_BUFFER1:

           push ACC                       ; Salva dado

           sts _VARIAVEL,Acc

           cbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	; habilita data-flash

           ldi Acc,0x84                   ; Comando:
           call _AT45_MASTER_IO           ; BUFFER 1 WRITE

           ldi Acc,0
           call _AT45_MASTER_IO           ; Byte irrelevante


           ;mov   Acc,XL                   ; Bit da parte alta dos 512bytes 
		   ;andi  Acc,1
           ldi Acc,0
		   call _AT45_MASTER_IO             


           lds Acc,_BUFFER_BYTE            ; Manda byte buffer
           call _AT45_MASTER_IO

           pop Acc                        ; Recupera dado

           swap Acc  ;embaralha

           call _AT45_MASTER_IO

   		   sbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	;CS=HIGH

           lds Acc,_VARIAVEL

           ret


		   
;+--------------------------------------------------------+
;| Rotina: BUFFER 1 e 2 WRITE                             |
;| Entradas: BUFFER_BYTE -> 0 a 255                       |
;|           XL com o bit da parte alta dos 512           |
;|           Dado a ser gravado no ACC                    |
;| Funcao :  Escreve diretamente no Buffer 1 de 256 bytes |
;|                                                        |
;+--------------------------------------------------------+

_WRITE_BUFFER2:

           push ACC                       ; Salva dado

           sts _VARIAVEL,Acc

           cbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	; habilita data-flash

           ldi Acc,0x87                   ; Comando:
           call _AT45_MASTER_IO           ; BUFFER 1 WRITE

           ldi Acc,0
           call _AT45_MASTER_IO           ; Byte irrelevante


           mov   Acc,XL                   ; Bit da parte alta dos 512bytes 
		   andi  Acc,1
           call _AT45_MASTER_IO             


           lds Acc,_BUFFER_BYTE            ; Manda byte buffer
           call _AT45_MASTER_IO

           pop Acc                        ; Recupera dado

           swap Acc  ;embaralha

           call _AT45_MASTER_IO

   		   sbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	;CS=HIGH

           lds Acc,_VARIAVEL

           ret


;+---------------------------------------------------------------------------+
;| Rotina: BUFFER 1 TO MAIN MEMORY PAGE PROGRAM WITH BUILT-IN ERASE          |
;| Entradas: DPTR -> Numero da pagina (0 a 4095)                             |
;|                                                                           |
;| Funcao : Transfere dados do buffer 1 para as paginas da memoria principal |
;|          passando pelos buffers 1 ou 2                                    |
;+---------------------------------------------------------------------------+

_AT45_BUFFER1_TO_MAIN_MEMORY:

           pushw X
           push Acc

           mov Acc,XH                      ; Filtra dptr para ir somente ate 4095
           andi Acc,0b00001111
           mov XH,Acc

           clc                             ; adequa o dptr da forma exigida pelo AT45cXX
           mov Acc,XL
           rol Acc
           mov XL,Acc

           mov Acc,XH
           rol Acc
           mov XH,Acc


         ;  clc                             ; adequa o dptr da forma exigida pelo AT45cXX
         ;  mov Acc,XL
         ;  rol Acc
         ;  mov XL,Acc

         ;  mov Acc,XH
         ;  rol Acc
         ;  mov XH,Acc
		   
         ;-

           cbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	   ; Habilita data-flash

           ldi Acc,0x83                   ; BUFFER 1 TO MAIN MEMORY PAGE PROGRAM WITH BUILT-IN ERASE
           call _AT45_MASTER_IO

           mov Acc,XH
           call _AT45_MASTER_IO

           mov Acc,XL
           call _AT45_MASTER_IO

           ldi Acc,0                       ; Manda byte irrelevante
           call _AT45_MASTER_IO

           sbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	;CS=HIGH

           call _AT45_GET_STATUS         ; Verifica se esta pronto

           pop ACC
           popw X

           ret





;+-----------------------------------+
;| Rotina: CLEAR ALL                 |
;| Funcao : Apaga a memoria inteira  |
;+-----------------------------------+

_AT45_CLEAR_ALL:
          
		   push Acc
		   pushw X

		   ldi Acc,0
           sts _BUFFER_BYTE,Acc
		   ldiw X,0

_AT45_CLEAR_ALL_LOOP:

           ldi Acc,0xFF
           call  _WRITE_BUFFER1

           call _INCREMENTA_ENDERECO

           lds Acc,_BUFFER_BYTE
		   cpi Acc,0xFF
		   brne _AT45_CLEAR_ALL_LOOP

		   mov Acc,XL
		   cpi Acc,1
		   brne _AT45_CLEAR_ALL_LOOP
		   		   
           ldiw X,0

_AT45_CLEAR_ALL_LOOP2:

           call  _AT45_BUFFER1_TO_MAIN_MEMORY

           adiw X,1
           mov Acc,XH
		   cpi Acc,HIGH(2047)
		   brne _AT45_CLEAR_ALL_LOOP2
           
           mov Acc,XL
		   cpi Acc,LOW(2047)
		   brne _AT45_CLEAR_ALL_LOOP2

           ldiw X,2047
           ldi Acc,0
		   sts _BUFFER_BYTE,Acc

_AT45_CLEAR_ALL_LOOP3:

           ldi Acc,0xFF
           call _AT45_MAIN_PAGE_WRITE_B1

           lds Acc,_BUFFER_BYTE
		   inc Acc
           sts _BUFFER_BYTE,Acc

		   cpi Acc,0xFF
		   brne _AT45_CLEAR_ALL_LOOP3

           popw X
		   pop Acc
		   ret




;+-------------------------------------------------------------------------------+
;| Rotina: MAIN MEMORY CONTINUOUS                                                |
;| Entradas: DPTR -> Numero da pagina (0 a 4095) , BUFFER_BYTE -> 0 a 255        |
;| Saida: Dado no ACC                                                            |
;| Funcao : Permite a leitura diretamente das 4096 paginas na memoria principal  |
;|          sem precisar usar os buffers e deixando os dados dos buffers intactos|
;+-------------------------------------------------------------------------------+

_AT45_READ_CONTINUOUS:

           ;pushw X
		  
           mov Acc,XH                      ; Filtra dptr para ir somente ate 4095
           andi Acc,0b00001111
           mov XH,Acc

           clc                             ; adequa o dptr da forma exigida pelo AT45cXX
           mov Acc,XL
           rol Acc
           mov XL,Acc

           mov Acc,XH
           rol Acc
           mov XH,Acc
		   
         ;  clc                             ; adequa o dptr da forma exigida pelo AT45cXX
         ;  mov Acc,XL
         ;  rol Acc
         ;  mov XL,Acc

          ; mov Acc,XH
          ; rol Acc
          ; mov XH,Acc

           cbi	_AT45DB161B_PORT_OUTPUTC,_AT45DB161B_CS_BIT	; habilita data-flash

  ;-- Master IO
           ldi Acc,0x68                   ; Comando READ CONTINUOUS
            
		   ;call _AT45_MASTER_IO  
           out SPDR,Acc                   ; Carrega dado de saida

CMASTER_B01:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B01 
           in   Acc,SPDR                   ; Ler dado de entrada			
			
          ; Manda parte alta da pagina

           mov Acc,XH
	   
          ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

CMASTER_B02:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B02 
           in   Acc,SPDR                   ; Ler dado de entrada       
          ;--

          ; Manda parte baixa da pagina
          
		  ;-- Master IO
		   mov Acc,XL 
   
           ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

CMASTER_B03:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B03
           in   Acc,SPDR                   ; Ler dado de entrada        
          ;--

          ; Manda byte buffer

          ;-- Master IO
		   lds Acc,_BUFFER_BYTE
  
		   ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

CMASTER_B04:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B04 
           in   Acc,SPDR                   ; Ler dado de entrada                    

          ; 4 Bytes irrelevantes,mas requeridos

          ;-- Master IO

           ;call _AT45_MASTER_IO  
           ldi Acc,0
		   out SPDR,Acc                    ; Carrega dado de saida

CMASTER_B05:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B05 
           in   Acc,SPDR                   ; Ler dado de entrada
          
		  ;-- Master IO

           ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

CMASTER_B06:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B06 
           in   Acc,SPDR                   ; Ler dado de entrada

          ;-- Master IO

          ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

CMASTER_B07:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B07 
           in   Acc,SPDR                   ; Ler dado de entrada

          ;-- Master IO

           ;call _AT45_MASTER_IO  
           out SPDR,Acc                    ; Carrega dado de saida

CMASTER_B08:
           in   Acc,SPSR                   ; pega o status
		   sbrs Acc,SPIF                   ; verifica se SPIF flag is busy
		   rjmp CMASTER_B08 
           in   Acc,SPDR                   ; Ler dado de entrada
          ;--
		  
        ;   popw X
		   
           ret


































