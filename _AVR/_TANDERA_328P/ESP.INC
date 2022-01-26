;
; Modulo comunicacao WIFI ESP8266
;
_PROCURA_WIFI:
      cli
      call _BUZZER_ON
	  _M_DELAY_MILLISECONDS 100

	  call _BUZZER_OFF

      ;LISTA REDES E GRAVA OS DADOS NA SRAM INTERNA
      
      ldi Acc,0
      sts _CONTADOR_REG,Acc
      sts _WORK2L,Acc
      sts _WORK2H,Acc
	        
      ldiw Z,AT_CWLAP*2
      call _SEND_STRING_ESP

	  ldiw X,_SRAM_PONTOS 
 	  
PROC_WI_L1:

      call _ESP_DATA_RX                 ; Recebe dado da ESP8266
      brcs PROC_WI_L1

	  ; Procura +
	  cpi Acc,'+'
      brne PROC_E

	  rjmp PROC_GRAVA_REGISTRO

PROC_E:
      cpi Acc,'E'
	  lbreq PROC_WI_ERR

	  rjmp PROC_WI_L1

PROC_GRAVA_REGISTRO:

      st X+,Acc              ; Poe + na memoria
      
	  rcall INC_COUNT        ; Incrementa contador de bytes 

PROC_ABRE_ASPAS:
      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs PROC_ABRE_ASPAS

	  cpi Acc,'"'
	  brne PROC_ABRE_ASPAS

      st X+,Acc              ; grava aspas na memoria

	  rcall INC_COUNT        ; Incrementa contador de bytes 

PROC_ABRE_ASPAS_LOOP2:
      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs PROC_ABRE_ASPAS_LOOP2

      st X+,Acc              ; grava caractere na memoria
	  rcall INC_COUNT        ; Incrementa contador de bytes 
	  
	  cpi Acc,'"'
	  brne PROC_ABRE_ASPAS_LOOP2

PROCURA_VIRGULA_L1:
      call _ESP_DATA_RX      
      brcs PROCURA_VIRGULA_L1  ; Pega virgula

      st X+,Acc              ; grava caractere na memoria
	  rcall INC_COUNT        ; Incrementa contador de bytes 

PROCURA_VIRGULA_L2:
      call _ESP_DATA_RX      
      brcs PROCURA_VIRGULA_L2  ; Pega dado

	  cpi Acc,','
	  breq PROCURA_FIM
	  
      st X+,Acc              ; grava caractere na memoria
      rcall INC_COUNT        ; Incrementa contador de bytes 
	  
	  rjmp PROCURA_VIRGULA_L2



PROCURA_FIM:
	  
	  lds AccH,_CONTADOR_REG
      inc AccH           ; conta numero de redes disponiveis
	  sts _CONTADOR_REG,AccH

	  ;Poe um LF no final do registro
	  ldi Acc,_LF
	  st X+,Acc

	  ; Testa rapidamente se ja chegou no final do buffer
	  rjmp PROC_WI_TST
	  ;********************************
	  
	  ; Procura um "OK ou "+"		  
PROC_WI_L2:
      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs PROC_WI_L2
  
      cpi Acc,'O'
	  brne PROC_WI_L2_MAIS
	  
; procura "K"
PROC_WI_L2_K:
      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs PROC_WI_L2_K

	  cpi Acc,'K'
	  breq PROC_WI_L2_CR
	  	  
	  ldi Acc,'3'
	  sts _COD_ERRO,Acc
	  
      _SET _FLAG_OK_ERRO 
	  ret
;procura CR
PROC_WI_L2_CR:
      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs PROC_WI_L2_CR

	  cpi Acc,_CR
	  breq PROC_WI_L2_LF

	  ldi Acc,'3'
	  sts _COD_ERRO,Acc
	  
      _SET _FLAG_OK_ERRO 
	  ret

PROC_WI_L2_LF:
      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs PROC_WI_L2_LF

	  cpi Acc,_LF
	  breq TESTA_SEM_REDES

      ldi Acc,'3'
	  sts _COD_ERRO,Acc
	  
      _SET _FLAG_OK_ERRO 
	  ret


; Procura +
  
PROC_WI_L2_MAIS:
      cpi Acc,'+'
	  brne PROC_WI_L2 
	  
	  jmp PROC_GRAVA_REGISTRO 
	




TESTA_SEM_REDES:

      ; Testa se nao ha redes disponiveis

      lds Acc,_CONTADOR_REG
	  cpi Acc,0
      brne _PROCURA_WIFI_SAIDA        ; Pula se for zero
      ldi Acc,'2'
	  sts _COD_ERRO,Acc
	  
      _SET _FLAG_OK_ERRO

      ret




PROC_WI_TST:
   /*  
      ldi Acc,HIGH(311)
      lds AccH,_Work2H
      cp Acc,AccH
	  brne PROC_WI_L1
	  	  
	  ldi Acc,LOW(311)
      lds AccH,_Work2L
      cp Acc,AccH
	  lbrne PROC_WI_L1
	 */
	 
	  ldi AccT,low(380)
	  ldi AccTH,high(380)

	  lds Acc,_Work2L
	  lds AccH,_Work2H

	  cp Acc,AccT
	  cpc AccH,AccTH
	  
	  lbrlo PROC_WI_L2    ; volta para ver a proxima linha
	  
	  rjmp _PROCURA_WIFI_SAIDA
	  	 
PROC_WI_ERR:

	  st X+,Acc
      rcall INC_COUNT
	  
PROC_WI_ERR_1:
      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs PROC_WI_ERR_1

      st X+,Acc
	  call INC_COUNT         ; incrementa contador de bytes  (WORK)

      cpi Acc,'R'
      brne PROC_WI_TST

PROC_WI_ERR_2:
      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs PROC_WI_ERR_2

      st X+,Acc
	  call INC_COUNT         ; incrementa contador de bytes  (WORK)

      cpi Acc,'R'
      brne PROC_WI_TST
	  
PROC_WI_ERR_3:
      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs PROC_WI_ERR_3

      st X+,Acc
	 
	  call INC_COUNT         ; incrementa contador de bytes  (WORK)

      cpi Acc,'O'
      brne PROC_WI_TST

PROC_ERR_SAIDA:
	   
	  ldi Acc,'3'
	  sts _COD_ERRO,Acc

	  _SET _FLAG_OK_ERRO

	  RET
 

_PROCURA_WIFI_SAIDA:

      _M_DELAY_MILLISECONDS 200

      _CLEAR _FLAG_OK_ERRO
	  
	  RET 


	  	
 /*   

// ENCHEU O BUFFER E AGORA AGUARDA O FIM DA LISTAGEM
	 
PROC_ENCHEU_AGUARDA:
      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs PROC_ENCHEU_AGUARDA
 
      cpi Acc,'O'
      brne PROC_ENCHEU_AGUARDA
	
PROC_ENCHEU_WI_L3:

      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs PROC_ENCHEU_WI_L3
	     
      cpi Acc,'K'
      brne PROC_ENCHEU_AGUARDA
	 
PROC_ENCHEU_WI_L4:

      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs PROC_ENCHEU_WI_L4
	        
      cpi Acc,_CR
      brne PROC_ENCHEU_AGUARDA
	  	  
      _CLEAR _FLAG_OK_ERRO

      ret

PROC_ENCHEU_WI_ERR:
      ; procura ERROR (ROR)
      cpi Acc,'R'
      brne PROC_ENCHEU_AGUARDA
	  
PROC_ENCHEU_WI_ERR_1:
      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs PROC_ENCHEU_WI_ERR_1
	  
      cpi Acc,'O'
      brne PROC_ENCHEU_AGUARDA

PROC_ENCHEU_WI_ERR_2:
      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs PROC_ENCHEU_WI_ERR_2

      cpi Acc,'R'
      brne PROC_ENCHEU_AGUARDA

PROC_ENCHEU_WI_ERR_3:
      call _ESP_DATA_RX      ; Recebe dado da ESP8266
      brcs PROC_ENCHEU_WI_ERR_3

	  ldi Acc,'3'
	  sts _COD_ERRO,Acc      ; Erro, excesso de redes
	  
      _SET _FLAG_OK_ERRO

	  ret

	 */ 

;
; PROCURA NA SRAM A REDE COM SINAL MAIS FORTE
;

      ;+--------------+
      ;| CARREGA REDE1|
      ;+--------------+

_PROC_ACHA_REDE:
      cli
	       
	  ldiw X,_SRAM_PONTOS
	  
PROC_ACHA_R_L1:

      ld Acc,X+ 
      cpi Acc,'+'
      brne PROC_ACHA_R_L1

PROC_ACHA_R_L2:

      ld Acc,X+ 
      cpi Acc,'"'
      brne PROC_ACHA_R_L2

   ;   adiw X,1    ; pula "
	  
      ldiw Y,_BUFFER_REDE1

PROC_ACHA_R_CPY1:

      ld Acc,X+
      st Y+,Acc
      
      cpi Acc,'"'  
	  
      brne PROC_ACHA_R_CPY1     

      ld Acc,X+         ; pula ,
      st Y+,Acc

      ld Acc,X+         ; pula -
      st Y+,Acc

      ld Acc,X+         ; GUARDA VALOR INTESIDADE DO SINAL
      sts _WorkH,Acc
      st Y+,Acc

      ld Acc,X          ; GUARDA VALOR INTESIDADE DO SINAL
      sts _Work,Acc
      st Y,Acc

PROC_ACHA_REDE2:

      ld Acc,X+
      cpi Acc,_LF
      brne PROC_ACHA_REDE2

      lds Acc,_CONTADOR_REG                ; Se houver apenas uma rede, grava direto no buffer resultado

      cpi Acc,1
      brne PROC_ACHA2_R_L1

      rjmp PROC_ACHA_RESULTADO_L1

      ;+--------------+
      ;| CARREGA REDE2|
      ;+--------------+

PROC_ACHA2_R_L1:
     
      ld   Acc,X+
      cpi  Acc,'+'
      brne PROC_ACHA2_R_L1

PROC_ACHA2_R_L2:

      ld   Acc,X+
      cpi  Acc,'"'
      brne PROC_ACHA2_R_L2
      
    ;  adiw XH:XL,1       ; pula "

      ldiw Y,_BUFFER_REDE2

PROC_ACHA2_R_CPY1:

      ld Acc,X+
      st Y+,Acc

      cpi Acc,'"'

	  brne PROC_ACHA2_R_CPY1
     
      ld Acc,X+           ; pula ,
      st Y+,Acc

      ld Acc,X+           ; pula -
      st Y+,Acc

      ld Acc,X+           ; carrega valor da intensidade do sinal
      mov TempH,Acc
      st Y+,Acc

      ld Acc,X
      mov Temp,Acc
      st Y,Acc
	     
      lds AccH,_WORKH     ; Compara com sinal anterior
	       
	  cp AccH,TempH
	  
      brlo PROC_ACHA_RESULTADO_L1
	  breq PROC_ACHA_TESTA_LSB
	  
PROC_ACHA_TESTA:

      ldiw Z,_BUFFER_RESULTADO

      ldiw Y,_BUFFER_REDE2

      rjmp  PROC_ACHA_RESULTADO_CPY

PROC_ACHA_TESTA_LSB:
      
	   lds Acc,_WORK     ; Compara com sinal anterior
	   cp Acc,Temp
	   brlo PROC_ACHA_RESULTADO_L1
       rjmp PROC_ACHA_TESTA   
	      
	   // copia para o buffer resultado	      

PROC_ACHA_RESULTADO_L1:

      ldiw Z,_BUFFER_RESULTADO

      ldiw Y,_BUFFER_REDE1
	  
PROC_ACHA_RESULTADO_CPY:

		ld Acc,Y+
		st Z+,Acc
      
		cpi Acc,'"'
		brne PROC_ACHA_RESULTADO_CPY
	
		ld Acc,Y+       ; copia o ,
		st Z+,Acc
    
		ld Acc,Y+       ; copia o -
		st Z+,Acc
    
		ld Acc,Y+       ; copia o H do nivel sinal
		st Z+,Acc
		sts _WORKH,Acc  ; copia para futura comparacao
    
		ld Acc,Y        ; copia o L do nivel sinal
		st Z,Acc
		sts _WORK,Acc
    
	    
		; Verifica se tem mais redes
	  
		lds Acc,_CONTADOR_REG
		cpi Acc,2

		breq PROCURA_REDE_SAIDA
      
		cpi Acc,2

		brlo  PROCURA_REDE_SAIDA
	  
		lds AccH,_CONTADOR_REG
		dec AccH
		sts _CONTADOR_REG,AccH
	  
		; final da segunda rede

PROC_ACHA_REDE3:

		ld Acc,X+                      
      
		cpi Acc,_LF
		brne PROC_ACHA_REDE3
	  
		ldiw Z,_BUFFER_RESULTADO       ; r1

		ldiw Y,_BUFFER_REDE1           ; r0
	  
PROC_COPIA_RES_REDE1:

		ld Acc,Z+
		st Y+,Acc
	        
		cpi Acc,'"'
		brne PROC_COPIA_RES_REDE1
	  
		ld Acc,Z+    ; copia o ,
		st Y+,Acc
	  
		ld Acc,Z+    ; copia o -
		st Y+,Acc
	  
		ld Acc,Z+    ; copia o H do nivel sinal
		st Y+,Acc
	  
		ld Acc,Z     ; copia o L do nivel sinal
		st Y,Acc
	 
		rjmp PROC_ACHA2_R_L1
	  
PROCURA_REDE_SAIDA:

		_CLEAR _FLAG_OK_ERRO
      
		RET
	    

       ;+--------------+
       ;| Conecta WIFI |
       ;+--------------+

_CONECTA_WIFI:
        cli
		_M_DELAY_MILLISECONDS 1000

		ldiw Z,AT_CWJAP*2
		call _SEND_STRING_ESP
				
		ldiw X,_BUFFER_RESULTADO
			  	   
_CONECTA_WIFI_L1:

		ld   Acc,X+

		push Acc
		call _ESP_TX_PUTC
		pop Acc

		cpi  Acc,'"'
		brne _CONECTA_WIFI_L1
	   
		ldi Acc,','
		call _ESP_TX_PUTC

		ldiw Z,AT_CWJAP_PW3*2
		call _SEND_STRING_ESP

		call _GET_RESP_OK2

		lds Acc,_FLAG_OK_ERRO
		cpi acc,_ON
       
		brne _CONECTA_WIFI_L2

		dec TempH
		brne _CONECTA_WIFI
     
		_SET _FLAG_OK_ERRO
		ret

_CONECTA_WIFI_L2:
       
		_CLEAR _FLAG_OK_ERRO

		ret


/*
 +------------------------+
 | BAIXA PACOTE PRINCIPAL |
 +------------------------+
*/
_DATA_DOWNLOAD:
       cli
       ldi Acc,0
       sts _Work2L,Acc
       sts _Work2H,Acc
	   ldi Temp,3

DOWNLOAD_LOOP:

       ldiw Z,AT_CIPSTART*2
       call _SEND_STRING_ESP

       call _GET_RESP_OK4
	   	   	       
       lds Acc,_FLAG_OK_ERRO
       cpi acc,_ON
       brne DATA_DOWNLOAD_CONT

       ldi Acc,'1'
	   sts _COD_ERRO,Acc

	   _SET _FLAG_OK_ERRO
	       
       ret


DATA_DOWNLOAD_CONT:
              
       _M_DELAY_MILLISECONDS 500

      // clr LED3

       ldiw Z,AT_CIPSEND*2
       call _SEND_STRING_ESP

DATA_D_REC:

       call _ESP_DATA_RX             ; Recebe dado da ESP8266
     
	   brcs DATA_D_REC

       cpi Acc,'>'
       brne DATA_REC_ERR
	   
       ldiw Z,MSG_HTTP1*2            ;GET /packs/packs.cab HTTP/1.1 
       call _SEND_STRING_ESP         ;Host: www.curinga.ind.br (INTERCELL)
       
       call _GET_RESP_OK4

       lds Acc,_FLAG_OK_ERRO
       cpi Acc,_ON
       brne DATA_D_REC_PULA
	   	   
D_ENTRY:
       ldi Acc,'3'
	   sts _COD_ERRO,Acc

	   _SET _FLAG_OK_ERRO
       
	   ret

DATA_REC_ERR:
       cpi Acc,'l'
       brne DATA_D_REC

       rjmp D_ENTRY

DATA_D_REC_PULA:
       
       //  clr LED4

       // RECEBE E TRATA OS DADOS

       ldi Acc,0
       sts _Work2L,Acc
       sts _Work2H,Acc
	   
_DATA_D_REC_INI:
       call _ESP_DATA_RX      ; Recebe dado da ESP8266
      
       brcs _DATA_D_REC_INI
	  
       cpi Acc,'!'
       brne _DATA_D_REC_INI

	   ldiw X,_SRAM_NRO_PACOTE              ; Pega numero do pacote
       ldi Temp,5

PEGA_NRO_PACOTE:
     
       call _ESP_DATA_RX      ; Recebe dado da ESP8266
      
       brcs PEGA_NRO_PACOTE

       st X+,Acc
       
       dec Temp
       brne PEGA_NRO_PACOTE
	   
DATA_GET_I:
            
       call _ESP_DATA_RX      ; Recebe dado da ESP8266
      
       brcs DATA_GET_I
      	   
       cpi Acc,'I'
       brne DATA_GET_I

DATA_GET_I2:
       call _ESP_DATA_RX

	   brcs DATA_GET_I2

       ldiw Y,_BUFFER_MINI    ; quantidade de inclusoes de pontos

       st Y+,Acc

DATA_GET_I3:
       call _ESP_DATA_RX

	   brcs DATA_GET_I3 

       st Y+,Acc

       ldi Acc,0
       st Y,Acc
      /*
       ldiw Y,_BUFFER_MINI
      ; call _DECBIN2
	  
       sts _CONT_I,R4*/
	    
DATA_GET_I4:
       call _ESP_DATA_RX     ; PULA X
       brcs DATA_GET_I4
	   
DATA_GET_I5:
	   call _ESP_DATA_RX
       brcs DATA_GET_I5
	   
       call _ASC_TO_HEX
       andi Acc,0b00001111
       mov Temp,Acc

DATA_GET_I6:
       call _ESP_DATA_RX
       brcs DATA_GET_I6

       call _ASC_TO_HEX
       andi Acc,0b00001111
       mov TempH,Acc

       mov Acc,Temp
       swap Acc
       mov Temp,Acc

       or Acc,TempH

       sts _CONT_X,Acc


DATA_GET_I7:
       call _ESP_DATA_RX        ; Pula o "p"
       brcs DATA_GET_I6

DATA_GET_I75:
       call _ESP_DATA_RX        
       brcs DATA_GET_I75

	   ldiw Y,_BUFFER_MINI+4    ; aponta para Quantidade de pacotes

       st Y+,Acc                ; 1o digito

DATA_GET_I8:
       call _ESP_DATA_RX       
       brcs DATA_GET_I8
	   st Y+,Acc                ; 2o digito

DATA_GET_I9:
       call _ESP_DATA_RX       
       brcs DATA_GET_I9

	   st Y+,Acc                ; 3o digito

	   ldi Acc,0
	   st Y,Acc
	    
	   	   
DATA_D_REC_00:
       call _ESP_DATA_RX        ; Recebe dado da ESP8266
      
       brcs DATA_D_REC_00
            
       cpi Acc,'>'
       brne DATA_D_REC_00

DATA_HALT00:		
		call _ESP_DATA_RX        ; Recebe dado da ESP8266
      
		brcs DATA_HALT00
		
		cpi Acc,'m'
		brne DATA_HALT01

		_CLEAR _FLAG_GRAVA_HALT
		rjmp DATA_HALT_EX

DATA_HALT01:
        _SET _FLAG_GRAVA_HALT
		
DATA_HALT_EX:
	   
       ; Inicia endereco da memoria flash
	   
	   ldiw X,_SSD1306_DISPLAY_BUFFER    ; Buffer do lcd sera usado (1024 bytes)

DATA_COPIA_SRAM_00:
       call _ESP_DATA_RX
       brcs DATA_COPIA_SRAM_00

       cpi Acc,_CR
       brne DATA_COPIA_SRAM_01

       rjmp DATA_COPIA_SRAM_00

DATA_COPIA_SRAM_01:
 
       cpi Acc,_LF
       brne DATA_COPIA_SRAM_02

       rjmp DATA_COPIA_SRAM_00

DATA_COPIA_SRAM_02:

       cpi Acc,'O'
       brne DATA_COPIA_SRAM_05

	   
DATA_PULA_IPD:
       call _ESP_DATA_RX      ; Recebe dado da ESP8266
       brcs DATA_PULA_IPD
       
	   cpi Acc,':'
       brne  DATA_PULA_IPD

       rjmp DATA_COPIA_SRAM_00

DATA_COPIA_SRAM_05:
       cpi Acc,'P'
       brne DATA_COPIA_SRAM_06      // encontrou 'P'
       
	   rjmp VERIFICA_VERSAO_PKT  

DATA_COPIA_SRAM_06:

       st X+,Acc
	
       call INC_COUNT      ; Incrementa contador de bytes
	  
       lds Acc,_Work2H
       cpi acc,high(1024)
       brne DATA_COPIA_SRAM_00

       lds Acc,_Work2L
       cpi Acc,low(1024)
       brne DATA_COPIA_SRAM_00
       
       ldi Acc,'2'
	   sts _COD_ERRO,Acc

	   _SET _FLAG_OK_ERRO

	   RET


VERIFICA_VERSAO_PKT:
     
      ; Verifica versão do pacote
	         			     
       call VERIFICA_VERSAO
	          
       lds Acc,_FLAG_OK_ERRO
       cpi Acc,_ON
       breq DATA_INCLUI_CONTINUA

       ldi Acc,'0'
	   	    
	   sts _COD_ERRO,Acc

	   _SET _FLAG_OK_ERRO
	   
       RET
	     

DATA_INCLUI_CONTINUA:
  
        ldiw Y,_BUFFER_MINI     ; pega a quantidade pontos 
        call _DECBIN2

        sts _CONT_I,R4
	   
	    ldiw Y,_BUFFER_MINI+4   ; pega a quantidade de pacotes
        call _DECBIN2

	    sts _PACKETS_L,R4
	    sts _PACKETS_H,R5
	   
       ; +--------------------+
       ; | Processa protocolo |
       ; +--------------------+

        ldiw Y,_SSD1306_DISPLAY_BUFFER

       ; 
       ;  Adiciona ponto 
       ;
DATA_INCLUI:

       ;  inc AUXR1                 ; *** Troca para DPTR2 ***

         lds Acc,_CONT_PONTOS_H     ; Faz ENDERECO <- ( CRD_COUNTER * 11)
         mov R3,Acc

         lds Acc,_CONT_PONTOS_L
         mov r2,Acc

         ldi Acc,11
		 mov R0,Acc

		 ldi Acc,0
		 mov R1,Acc
         
         call _MUL_U16U16U32

         ; agora adiciona RESULTADO com dptr
         mov Acc,R4
         sts _BUFFER_BYTE,Acc

         mov Acc,R5
         mov XL,Acc

         mov Acc,R6
         mov XH,Acc
         
         ldi Acc,11
         sts _CONT2,Acc

         call _AT45_MAIN_PAGE_TO_BUFFER1_TRANSF

DATA_INCLUI_LOOP:

         ;inc AUXR1               ; *** Troca para DPTR1 ***
DATA_L1:

         call BYTE_TEXT2HEX       ; Pega dado na RAM (Y) Converte dois bytes em formato texto na memoria para um hex

         ;inc AUXR1               ; *** Troca para DPTR2 ***
		 call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         lds Acc,_CONT2
		 dec Acc
		 sts _CONT2,Acc

		 tst Acc
         brne DATA_INCLUI_LOOP

       ; inc AUXR1                 ; *** Troca para DPTR1 ***

         ; Incrementa CONT_PONTOS

         lds Acc,_CONT_PONTOS_L
		 ldi AccH,1
		 add Acc,AccH
         sts _CONT_PONTOS_L,Acc

         lds Acc,_CONT_PONTOS_H
         ldi AccH,0
		 adc Acc,AccH
         sts _CONT_PONTOS_H,Acc

        ; Salva CONT_PONTOS na EEPROM
        
         ldi AccH,low(_EE_CONT_PONTOS_L)
         ldi AccT,high(_EE_CONT_PONTOS_L)
    	 lds Acc,_CONT_PONTOS_L
    	 call _EEPROM_WRITE

         ldi AccH,low(_EE_CONT_PONTOS_H)
         ldi AccT,high(_EE_CONT_PONTOS_H)
   	     lds Acc,_CONT_PONTOS_H
	     call _EEPROM_WRITE

		 ; Grava HALT
		 // grava byte do first boot
        ldi AccH,low(_EE_FLAG_HALT)
		ldi AccT,high(_EE_FLAG_HALT)
		lds Acc,_FLAG_GRAVA_HALT
		call _EEPROM_WRITE
		
		 lds Acc,_CONT_I
		 dec Acc
		 sts _CONT_I,Acc

		 tst Acc
         lbrne DATA_INCLUI
         
         call GRAVA_VERSAO       ;  grava versao do pacote
		          
         lds Acc,_CONT_X         ; Se nao houver dados para apagar, sai
		 cpi Acc,0

         breq DATA_SAIDA         ; testa se for zero
        ;+--------------+
        ;| APAGA PONTOS |
        ;+--------------+
APAGA_PONTO_FLASH:

         call DATA_APAGA_FLASH
         
         lds Acc,_CONT_X
		 dec Acc
		 sts _CONT_X,Acc
	
	     brne APAGA_PONTO_FLASH

DATA_SAIDA:
   		 _CLEAR _FLAG_OK_ERRO
		 
         RET


; +---------------------+
; | APAGA DADO DA FLASH |
; +---------------------+

DATA_APAGA_FLASH:

         ;Carrega LATITUDE DA SRAM

         call BYTE_TEXT2HEX          ; dptr 1
         sts _TEMP_VAR_LAT_H,Acc

         call BYTE_TEXT2HEX
         sts _TEMP_VAR_LAT_MH,Acc

         call BYTE_TEXT2HEX
         sts _TEMP_VAR_LAT_ML,Acc

         call BYTE_TEXT2HEX
         sts _TEMP_VAR_LAT_L,Acc

         ;Carrega LONGITUDE DA SRAM

         call BYTE_TEXT2HEX
         sts _TEMP_VAR_LON_H,Acc

         call BYTE_TEXT2HEX
         sts _TEMP_VAR_LON_MH,Acc

         call BYTE_TEXT2HEX
         sts _TEMP_VAR_LON_ML,Acc

         call BYTE_TEXT2HEX
         sts _TEMP_VAR_LON_L,Acc

         ldi Acc,0
         sts _CONTADOR16_L,Acc
         sts _CONTADOR16_H,Acc

         ; inc AUXR1               ; *** Troca para DPTR2 ***

         rjmp DATA_APAGA_PARDAL_CONTINUA


DATA_APAGA_INCREMENTA:

         ; INCREMENTA CONTADOR16 E COMPARA COM O TOTAL DA MEMORIA


         lds Acc,_CONTADOR16_L
		 ldi AccH,1
		 add Acc,AccH
         sts _CONTADOR16_L,Acc

         lds Acc,_CONTADOR16_H
		 ldi AccH,0
         adc Acc,AccH
         sts _CONTADOR16_H,Acc

         lds Acc,_CONTADOR16_L
         lds AccH,_CONT_PONTOS_L
         cp Acc,AccH

         brne DATA_APAGA_PARDAL_CONTINUA

         lds Acc,_CONTADOR16_H
         lds AccH,_CONT_PONTOS_H
         cp Acc,AccH

         brne DATA_APAGA_PARDAL_CONTINUA

         rjmp DATA_APAGA_PARDAL_SAIDA
		 
DATA_APAGA_PARDAL_CONTINUA:
         
		 ldi Acc,0
         sts _BUFFER_BYTE,Acc
         ldiw X,0                ; DPTR 2

         call _RODA_ADDRESS

DATA_APAGA_PARDAL_CONTINUA_LOOP:

         ; Carrega LATITUDE (32BITS)

         call _AT45_MAIN_PAGE_READ
         sts _LATITUDE_H,Acc
         
         lds AccH,_TEMP_VAR_LAT_H 
         cp Acc,AccH

         brne DATA_APAGA_INCREMENTA

       ;-
         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
         sts _LATITUDE_MH,Acc
         
         lds AccH,_TEMP_VAR_LAT_MH
         cp Acc,AccH 

         brne DATA_APAGA_INCREMENTA

        ;-
         
         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
         sts _LATITUDE_ML,Acc

         lds AccH,_TEMP_VAR_LAT_ML
         cp Acc,AccH

         brne DATA_APAGA_INCREMENTA
		;-
         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ

         sts _LATITUDE_L,Acc
         lds AccH,_TEMP_VAR_LAT_L

         cp Acc,AccH

         lbrne  DATA_APAGA_INCREMENTA

         call _INCREMENTA_ENDERECO

       ; Carrega LONGITUDE (32BITS)

         call _AT45_MAIN_PAGE_READ

         sts _LONGITUDE_H,Acc
         lds AccH,_TEMP_VAR_LON_H

         cp Acc,AccH
         
         lbrne DATA_APAGA_INCREMENTA

         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
         sts _LONGITUDE_MH,Acc
         
         lds AccH,_TEMP_VAR_LON_MH
         cp Acc,AccH
         lbrne DATA_APAGA_INCREMENTA

       ;-
         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
         sts _LONGITUDE_ML,Acc
         
         lds AccH,_TEMP_VAR_LON_ML
         
         cp Acc,AccH
         
         lbrne DATA_APAGA_INCREMENTA
		;-
         call _INCREMENTA_ENDERECO

         call _AT45_MAIN_PAGE_READ
         sts _LONGITUDE_L,Acc
         
         lds AccH,_TEMP_VAR_LON_L
         cp Acc,AccH
         
         lbrne DATA_APAGA_INCREMENTA

         ; Se o ponto foi encontrado, apaga

         ldi Acc,0
         sts _BUFFER_BYTE,Acc
         ldiw X,0                ; DPTR 2

         call _RODA_ADDRESS

         call _AT45_MAIN_PAGE_TO_BUFFER1_TRANSF

         ;GRAVA LATITUDE
         ldi Acc,0xff
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         ldi Acc,0xff
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         ldi Acc,0xff
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         ldi Acc,0xff
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         ;GRAVA LONGITUDE
         ldi Acc,0xff
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         ldi Acc,0xff
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         ldi Acc,0xff
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO
        ;-
         ldi Acc,0xff
         call _AT45_MAIN_PAGE_WRITE_B1
         call _INCREMENTA_ENDERECO

         call _INCREMENTA_ENDERECO   ; aponta para o direcao_h
         ldi Acc,0

         call _AT45_MAIN_PAGE_WRITE_B1

        ;-

DATA_APAGA_PARDAL_SAIDA:

         ; inc AUXR1               ; *** Troca para DPTR1 ***
		 _CLEAR _FLAG_OK_ERRO
		 		
         ret
		 
;
;
;
VERIFICA_VERSAO:

         pushw X
         pushw Y

         ldiw X,_SRAM_NRO_PACOTE
		 
         ldiw Y,_EE_NRO_PACOTE

         ldi Temp,5

VERSAO_L1:

         ld Acc,X+
         mov TempH,Acc
        
		 mov AccT,YH
		 mov AccH,YL
		 
		 call _EEPROM_READ
         adiw Y,1
		          
         cp Acc,TempH
         brne VERSAO_SAIDA
         
         dec Temp
         brne VERSAO_L1

         _CLEAR _FLAG_OK_ERRO

         popw Y
         popw X

         RET


VERSAO_SAIDA:

         _SET _FLAG_OK_ERRO

         popw Y
         popw X
		 
         RET








;
;
;
GRAVA_VERSAO:

        pushw X
        pushw Y
		       
        ldiw X,_SRAM_NRO_PACOTE

        ldiw Y,_EE_NRO_PACOTE

        ldi Temp,5

GRAVA_VERSAO_L1:

        ld Acc,X+
        mov TempH,Acc

		        
        mov AccT,YH
		mov AccH,YL  
		call _EEPROM_WRITE
         
        adiw Y,1

        dec Temp
        brne GRAVA_VERSAO_L1

        popw Y
        popw X
		
		RET





;
;
;
BYTE_TEXT2HEX:

         push Temp
         push TempH

         ld Acc,Y+

         call _ASC_TO_HEX
         andi Acc,0b00001111
         mov Temp,Acc

         ld Acc,Y+


         call _ASC_TO_HEX
         andi Acc,0b00001111
         mov TempH,Acc

         mov Acc,Temp
         swap Acc
         mov Temp,Acc

         or Acc,TempH

         swap Acc
         
         pop TempH
         pop Temp
         
         RET






;
; Inicializa modulo ESP8266
;

_WIFI_INI:

		ldiw Z,AT_AT*2
		call _SEND_STRING_ESP

		_M_DELAY_MILLISECONDS 500
     
		ldiw Z,AT_ATE0*2
		call _SEND_STRING_ESP

		_M_DELAY_MILLISECONDS 500
    
		ldi AccT,3
WIFI_INI_T2:
		ldiw Z,AT_CWMODE1*2
		call _SEND_STRING_ESP

		_M_DELAY_MILLISECONDS 500
        
		ldiw Z,AT_CIFSR*2
		call _SEND_STRING_ESP

WIFI_CIFSR_L1:
        call _ESP_DATA_RX      
        brcs WIFI_CIFSR_L1 

		cpi Acc,'"'
        brne WIFI_CIFSR_L1

WIFI_CIFSR_L2:
		call _ESP_DATA_RX      
        brcs WIFI_CIFSR_L2 

		cpi Acc,'0'
		brne WIFI_CIF_NET_OK

		_SET _FLAG_OK_ERRO
		RET


WIFI_CIF_NET_OK:
                _CLEAR _FLAG_OK_ERRO
	    

	 	_M_DELAY_MILLISECONDS 200

		dec AccT
		brne WIFI_INI_T2

		_SET _FLAG_OK_ERRO

		RET
		
AT_CWQAP_ENTRY:
		ldi Acc,3
	   
AT_CWQAP_ENTRY_L1:
		ldiw Z,AT_CWQAP*2
		call _SEND_STRING_ESP
     
		call _GET_RESP_OK3

		lds Acc,_FLAG_OK_ERRO
		cpi Acc,_ON

		brne AT_CWQAP_ENTRY_SAIDA
	  
		_M_DELAY_MILLISECONDS 200

		dec AccT
		brne AT_CWQAP_ENTRY_L1
	  
		_SET _FLAG_OK_ERRO	
		  
AT_CWQAP_ENTRY_SAIDA:
 
		RET
		 

; Incrementa contador de bytes
INC_COUNT:

      push Acc
	  
      lds Acc,_Work2L
	  ldi AccH,1
      add Acc,AccH
      sts _Work2L,Acc

      lds Acc,_Work2H
	  ldi AccH,0
      adc Acc,AccH
      sts _Work2H,Acc

	  pop Acc

      RET
      



;
;
;
_SEND_STRING_ESP:

		lpm Acc,Z
		cpi Acc,0
		breq SEND_ESP_SAIDA
		cpi Acc,_NC
		breq SEND_STRING_PULA

		call _ESP_TX_PUTC

SEND_STRING_PULA:
		adiw Z,1
		rjmp _SEND_STRING_ESP

SEND_ESP_SAIDA:

		RET

      
  
;
;
_GET_RESP_OK:
		push Acc 
GET_L1:
		call _ESP_DATA_RX      ; pula CR,LF
		brcs GET_L1
        
		cpi Acc,'O'
		brne GET_L2

		_CLEAR _FLAG_OK_ERRO
		rjmp GET_SAIDA_PL
GET_L2:
		cpi Acc,'E'
		brne GET_L1     

		_SET _FLAG_OK_ERRO

GET_SAIDA_PL:
		pop Acc

		RET



;
;
_GET_RESP_OK2:

        call _ESP_DATA_RX      ; Recebe dado da ESP8266
      
        brcs _GET_RESP_OK2

	  
        cpi Acc,'P'
        brne  _GET_RESP_OK23

        _CLEAR _FLAG_OK_ERRO
        RET

_GET_RESP_OK23:
        cpi Acc,'L'
        brne  _GET_RESP_OK24

        _SET _FLAG_OK_ERRO

		RET

_GET_RESP_OK24:
        cpi Acc,'R'
        brne  _GET_RESP_OK2

		_SET _FLAG_OK_ERRO

        RET


;
;
_GET_RESP_OK3:
        
		push Acc
		push AccT

        call _ESP_DATA_RX      ; Recebe dado da ESP8266
     
        brcs _GET_RESP_OK3
		
        cpi Acc,'K'
        brne _GET_RESP_OK33

        _CLEAR _FLAG_OK_ERRO

        rjmp _GET_RESP_OK33_SAIDA

_GET_RESP_OK33:
        cpi Acc,'R'
        brne _GET_RESP_OK3

        _SET _FLAG_OK_ERRO

 _GET_RESP_OK33_SAIDA:
        pop AccT
		pop ACc       
		
		RET


;
;
_GET_RESP_OK4:
        
        call _ESP_DATA_RX      ; Recebe dado da ESP8266

        brcs _GET_RESP_OK4
		
        cpi  Acc,'K'
        brne GETRESP_L1

        _CLEAR _FLAG_OK_ERRO

        ret

GETRESP_L1:

        cpi Acc,'T'
        brne GETRESP_L3

		_M_DELAY_MILLISECONDS 1500

        _CLEAR _FLAG_OK_ERRO

        ret
		        

GETRESP_L3:

        cpi Acc,'R'
		brne _GET_RESP_OK4

		call _ESP_DATA_RX   
		cpi Acc,'R'
		brne _GET_RESP_OK4

		 _SET _FLAG_OK_ERRO
		 ret

; COMANDOS AT

AT_AT:
      .DB "AT",_CR,_LF,_NC,0
AT_ATE0:
      .DB "ATE0",_NC,_CR,_LF,0

AT_RST:
      .DB "AT+RST",_CR,0

AT_CWQAP:
      .DB "AT+CWQAP",_NC,_CR,_LF,0

AT_CWLAP:
      .DB "AT+CWLAP",_NC,_CR,_LF,0

AT_CWJAP:
      .DB "AT+CWJAP=",0x22,_NC,0   

;AT_CWJAP:
 ;      .DB "AT+CWJAP=",0x22,"Curinga",0x22,0,_NC  


AT_CIFSR:
      .DB "AT+CIFSR",_CR,_LF,0,_NC
AT_CWMODE1:
      .DB "AT+CWMODE=1",_CR,_LF,0

AT_CWJAP_PW2:
      .DB "12345678",0x22,_CR,_NC,0

AT_CWJAP_PW3:
      .DB 0X22,"12345678",0x22,_CR,_LF,0,_NC

AT_CIPSTART:
       .DB "AT+CIPSTART=",0X22,"TCP",0X22,",",0X22,"maisgps.com.br",0x22,",80",_CR,_LF,0
	
AT_CIPSEND:
      ;.DB "AT+CIPSEND=72",_CR,_LF,0
	  .DB "AT+CIPSEND=59",_CR,_LF,0
MSG_HTTP1:
      .DB "GET /packz/pack2.cab HTTP/1.1",_CR,_LF,_NC
      .DB "Host: www.maisgps.com.br",_CR,_LF,_CR,_LF,_NC,0
AT_CIPCLOSE:
      .DB "AT+CIPCLOSE",_CR,_LF,0

MSG_HTTP2A:
      .DB "GET /packz/packs.",0



