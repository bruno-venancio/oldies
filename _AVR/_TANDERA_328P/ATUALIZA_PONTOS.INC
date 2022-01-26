
/*
 +---------------------+
 |  ROTINA ATUALIZAÇÃO |
 +---------------------+
*/
  
_ATUALIZA_PONTOS:

      //
	  //SAVE SREG
	  //
	  push	Acc
      in		Acc,SREG
  	  //
	  //SAVE REGISTERS
	  //

	  _M_PUSH_LOWER_REGS						        ;save r0..r15
      _M_PUSH_UPPER_REGS						        ;save r16..r31

      cli
     ; call _ESP_TX_INIT   ; Inicia transmissao xxx
	 ; call _ESP_INIT      ; Inicia recepcao xxx

	  ldi Acc,_OFF
      call _USART_RX_INT_ENABLE 

     ; call fn_carrega_MAC     xxx                      ; Carrega endereco MAC memoria

	  sbi	_AT45DB161B_WP_OUTPUT,_AT45DB161B_WP_BIT	;set unprotected

      ; LCD <- Procurando WIFI...

      ldi		Acc,_WHITE							;Set White Logical Mode
      call	_SSD1306_Set_Operator			
	  ldi		Acc,1								;Set Scale to 2
      call	_SSD1306_Set_Text_Scale			
	  call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
	  call	_SSD1306_Cls

	  call	fn_ShowScreenBorder						;show border
	
	  ; Procurando WIFI...

	  ldiw	Z,MSG_WIFI1*2	                                  
      ldiw  X,0
	  ldiw	Y,5
	  call	_SSD1306_Locate							;locate msg
	  call	_SSD1306_PutSF
	
	  call _SSD1306_Refresh
	         _m_delay_milliseconds 4000 ; xxx
	  	                                    
      // INICIALIZA WIFI
	 	   
	 ; call _WIFI_INI xxx
	   
	  lds Acc,_FLAG_OK_ERRO
	  cpi Acc,_ON
	  ;breq WIFI_INI_OK xxx
	  // Se deu erro
	   
	  
	  ldiw	Z,MSG_WIFI_ON*2	                        ;LCD <- WIFI CONECTADA! (Esp ja havia conectado antes)
      ldiw  X,0
	  ldiw	Y,15
	  call	_SSD1306_Locate						    	;locate msg
	  call	_SSD1306_PutSF
	
	  call _SSD1306_Refresh

	  _m_delay_milliseconds 5000 ;xxx

	      jmp xuxui
  	    
	  rjmp WIFI_CONECTADO_ENTRY
	  
WIFI_INI_OK:
      ldi ACC,3
	  sts _REPEAT,Acc

WIFI_INI_L1:

	  call _PROCURA_WIFI

	  lds Acc,_FLAG_OK_ERRO
	  cpi Acc,_ON

	  brne WIFI_CON_TST_OK

	  lds Acc,_COD_ERRO
	  cpi Acc,'R'
	  brne WIFI_OTHER_ERR

	  lds Acc,_REPEAT
	  dec Acc
	  sts _REPEAT,Acc

	  tst Acc
	  brne WIFI_INI_L1

WIFI_OTHER_ERR:	  
	  // Se der erro:
	  lds Acc,_COD_ERRO
	  cpi Acc,'1'
	  brne WIFI_ERR_CD2
	  
	  ldiw	Z,MSG_WIFI_ERRO1*2	                        ;LCD <- ERRO NA COM. WIFI
	  rjmp WIFI_ERR_CD4
WIFI_ERR_CD2:
	  cpi Acc,'2'
	  brne WIFI_ERR_CD3
	  ldiw	Z,MSG_WIFI_ERRO2*2	                        ;LCD <- SEM REDES DISPONIVEIS!
	  rjmp WIFI_ERR_CD4


WIFI_ERR_CD3:
	  
	  ldiw	Z,MSG_WIFI_ERRO4*2	                        ;LCD <- ERRO NA WIFI

WIFI_ERR_CD4:

	  ldiw  X,0
	  ldiw	Y,15
	  call	_SSD1306_Locate						    	;locate msg
	  call	_SSD1306_PutSF
	
	  call _SSD1306_Refresh

	  rjmp ATUALIZA_SAIDA

      // ENCONTROU AS REDES

WIFI_CON_TST_OK:
       
      ldiw	Z,MSG_WIFI_ENC*2	                        ;LCD <- Redes Encontradas
	  ldiw  X,0
	  ldiw	Y,15
	  call	_SSD1306_Locate						    	;locate msg
	  call	_SSD1306_PutSF
	
	  call _SSD1306_Refresh

	  _M_DELAY_MILLISECONDS 2000

	  call	_SSD1306_Cls

	  call	fn_ShowScreenBorder						;show border
	
	  ldiw	Z,MSG_WIFI_CONECTANDO*2	                    ;LCD <- Conectando a rede:
	  ldiw  X,0
	  ldiw	Y,5
	  call	_SSD1306_Locate						        ;locate msg
	  call	_SSD1306_PutSF  
	  
	  call _SSD1306_Refresh
	  
	  call _PROC_ACHA_REDE                         ;PROCURA NA SRAM A REDE COM SINAL MAIS FORTE

	  lds Acc,_FLAG_OK_ERRO
	  cpi Acc,_ON
	  brne WIFI_CON_TST_OK_CONT
	  // Erro nome da rede!

	  call	_SSD1306_Cls
	  call	fn_ShowScreenBorder						;show border

      ldiw	Z,MSG_ERRO_NOME*2	                   ;LCD <- ERRO NOME DE REDE
	  ldiw  X,0
	  ldiw	Y,15
	  call	_SSD1306_Locate						   ;locate msg
	  call	_SSD1306_PutSF  
	  
	  call _SSD1306_Refresh
	  _M_DELAY_MILLISECONDS 3000

	  jmp ATUALIZA_SAIDA

WIFI_CON_TST_OK_CONT:
	  
	  ldiw X,_BUFFER_RESULTADO

	  ldi AccT,20

WIFI_NOME_REDE:
       
	  ld Acc,X
	  cpi Acc,'"'
	  breq WIFI_NOME_TERM
	  adiw XH:XL,1

	  dec AccT
	  brne WIFI_NOME_REDE    ; testa se nao estorou o buffer em nao achar as "

	  ldi Acc,'1'
	  sts _COD_ERRO,Acc
	  rjmp WIFI_OTHER_ERR

WIFI_NOME_TERM:
      pushw X
	  ldi Acc,0        ; caractere de fim de string
	  st X,Acc

	  ldiw  X,8	 
	  ldiw	Y,15
	  call	_SSD1306_Locate						    	; imprime prompt >
	  ldi Acc,'>'
      call	_SSD1306_PutC
	  	  
	  ldiw Z,_BUFFER_RESULTADO
	  ldiw  X,15	 
	  ldiw	Y,15
	  call	_SSD1306_Locate						    	; nome da rede
      call	_SSD1306_PutS
	
	  call _SSD1306_Refresh
	  
	  popw X
	  ldi Acc,'"'      ; Devolve o aspas
	  st X,Acc
      
	  _M_DELAY_MILLISECONDS 500
	  
	  // Conecta a rede
	  ldi TEMPH,3

	  call _CONECTA_WIFI      //  <-- CONECTA REDE
  
      lds Acc,_FLAG_OK_ERRO
	  cpi Acc,_ON
	  lbreq WIFI_CON_ERRO

	  // Conectado com sucesso!

WIFI_CONECTADO_ENTRY:
xuxui:
      _CLEAR _FLAG_OK_ERRO

	  call	_SSD1306_Cls

	  call	fn_ShowScreenBorder					       ;show border
	
      ldiw	Z,MSG_WIFI_CONECTADO*2	                   ;LCD <- Conectado com sucesso!
	  ldiw  X,0
	  ldiw	Y,5
	  call	_SSD1306_Locate						       ;locate msg
	  call	_SSD1306_PutSF  
	  
	  call _SSD1306_Refresh

	  _M_DELAY_MILLISECONDS 2000
	  	  
		                             
	  /*
	  +--------------------+
	  | DOWNLOAD DE PONTOS |
	  +--------------------+
	  */
	   
      ldi		Acc,_WHITE							;Set White Logical Mode
      call	_SSD1306_Set_Operator			
	  ldi		Acc,1								;Set Scale to 2
      call	_SSD1306_Set_Text_Scale			
	  call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
	  call	_SSD1306_Cls
      call	fn_ShowScreenBorder						;show border
	
	  // LCD <- Baixando dados...

	  ldiw	Z,MSG_WIFI_DOWNLOAD*2	                
      ldiw  X,0
	  ldiw	Y,5
	  call	_SSD1306_Locate							;locate msg
	  call	_SSD1306_PutSF
	
	  call _SSD1306_Refresh
	      jmp xixiu  ;xxx
	      
	  ldi TempH,3	 ; Nro. de tentativas		
	  	                                           
	  call _DATA_DOWNLOAD         //   <---  GRAVA OS DADOS
	  
	//  call _BEEP
	  
	  lds Acc,_FLAG_OK_ERRO
	  cpi Acc,_ON
	  
	  lbreq DOWN_ERRO
	  	 
	  // Testa se ha mais pacotes
	/*  lds Acc,_PACKETS_L  
	  tst Acc
	  brne PCK_L1

	  lds Acc,_PACKETS_H                            
	  tst Acc
	  brne PCK_L1
	  */
	 ; rjmp LCD_DOWN_ATUALIZADO  



;LCD_DOWN_ATUALIZADO:
 
      ;Reconfigura display

	  ldi		Acc,_WHITE							;Set White Logical Mode
      call	_SSD1306_Set_Operator			
	  ldi		Acc,1								;Set Scale to 2
      call	_SSD1306_Set_Text_Scale			
	  call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
	  call	_SSD1306_Cls

	  call	fn_ShowScreenBorder						;show border
	  	    	  		  
      // LCD <- Pacote Nro.xxxx

	  ldiw	Z,MSG_WIFI_PCK_NRO*2	                      
	  ldiw  X,0
	  ldiw	Y,5
	  call	_SSD1306_Locate						      
	  call	_SSD1306_PutSF  

	  ldiw Z,_SRAM_NRO_PACOTE
	  ldiw X,74
	  
	  ldi AccT,5

LCD_NRO_PACK:
      ldiw Y,5
	  ld Acc,Z+
	  call	_SSD1306_Locate						    
	  call	_SSD1306_PutC
	  adiw X,8
	  dec AccT
	  brne LCD_NRO_PACK

	  _M_DELAY_MILLISECONDS 2000
  	 	
	  // LCD <- Gravado com sucesso!
	  ldiw	Z,MSG_WIFI_GRAVADO*2	                      
	  ldiw  X,0
	  ldiw	Y,15
	  call	_SSD1306_Locate						      
	  call	_SSD1306_PutSF  
	  
	  call _SSD1306_Refresh
	    
	  _M_DELAY_MILLISECONDS 6000   ; xxx
	  
	  rjmp ATUALIZA_SAIDA
	  

WIFI_CON_ERRO:

	  call _BEEP

      call	_SSD1306_Cls
	  call	fn_ShowScreenBorder						   ;show border

      ldiw	Z,MSG_WIFI_ERRO5*2	                       ;LCD <- REDE NAO CONECTADA!
	  ldiw  X,0
	  ldiw	Y,5
	  call	_SSD1306_Locate						       ;locate msg
	  call	_SSD1306_PutSF  
	  
	  call _SSD1306_Refresh

	  rjmp ATUALIZA_SAIDA

xixiu:
       ldi acc,'0'      ; XXX
	   sts _COD_ERRO,ACC
	  	  
// Verifica e seleciona qual mensagem de erro sera apresentada

DOWN_ERRO:
      lds Acc,_COD_ERRO

	  cpi Acc,'0'              ; Curinga ja atualizado
	  breq DOWN_ATUALIZADO
	  cpi Acc,'1'              ; Erro de conexao
	  breq DOWN_ERRO1
	  cpi Acc,'2'
	  breq DOWN_ERRO2          ; Erro buffer overflow 
	  cpi Acc,'3'              ; Erro de conexao
	  breq DOWN_ERRO1
	  

DOWN_ATUALIZADO:

      ldi		Acc,_WHITE							;Set White Logical Mode
      call	_SSD1306_Set_Operator			
	  ldi		Acc,1								;Set Scale to 2
      call	_SSD1306_Set_Text_Scale			
	  call	_SSD1306_Set_Display_Normal			    ;Set Screen Normal 
	
	  call	_SSD1306_Cls
	  
      ldiw	Z,MSG_DOWN_ERRO2*2	                       ;LCD <- CURINGA JA ATUALIZADO
	  rjmp DOWN_ERRO_CONT

DOWN_ERRO1:
    ;  ldiw	Z,MSG_DOWN_ERRO1*2	                       ;LCD <- FALHA CONEXAO SERVIDOR
      call	_SSD1306_Cls
	  ldiw	Z,MSG_DOWN_ERRO2A*2	                       ;LCD <- CURINGA JA ATUALIZADO *
	 
	  rjmp DOWN_ERRO_CONT
DOWN_ERRO2:
      ldiw Z,MSG_GRAV_ERRO*2                           ;LCD <- Erro de overflow! 
	  
DOWN_ERRO_CONT:
	  
	  ldiw  X,0
	 ; ldiw	Y,15
	  ldiw Y,0
	  call	_SSD1306_Locate						       ;locate msg
	  call	_SSD1306_PutSF  
	  
	  call _SSD1306_Refresh
	  
ATUALIZA_SAIDA:

      cbi	_AT45DB161B_WP_OUTPUT,_AT45DB161B_WP_BIT   ; write protect

      ldiw Z,AT_CWQAP*2
      call _SEND_STRING_ESP 
        
	  _M_DELAY_MILLISECONDS 3000
		 
      //
	  //RESTORE REGISTERS
	  //
	  _M_POP_UPPER_REGS						;restore r16..r31
	  _M_POP_LOWER_REGS
	  
	  //
	  //RESTORE SREG
	  //
	  out		SREG,Acc
	  pop		Acc
	
	  RET
	
	
// Mensagens LCD
	
MSG_WIFI1:
	   .DB " Procurando WIFI... ",_NC,0

MSG_WIFI_ERRO:
       .DB " Erro na Wifi!",_NC,0

MSG_WIFI_ON:
       .DB " WIFI conectada!",0,_NC

MSG_WIFI_ENC:
       .DB " Redes Encontradas!",0

MSG_WIFI_NO_ENC:
       .DB " WIFI NAO Encontrada!",0

MSG_WIFI_ERRO1:
       .DB " ERRO NA COM. WIFI",_NC,0

MSG_WIFI_ERRO2:
       .DB " SEM REDES DISPONIVEIS!",0

MSG_WIFI_ERRO3:
       .DB " EXCESSO DE REDES!",0,_NC

MSG_WIFI_ERRO4:
       .DB " ERRO NA WIFI!",_NC,0

MSG_WIFI_ERRO5:
       .DB " REDE NAO CONECTADA!",_NC,0

MSG_WIFI_CONECTANDO:
       .DB " Conectando a rede:",0
	   
MSG_WIFI_CONECTADO:
       .DB " Rede conectada!",_NC,0

MSG_WIFI_PREPARA:
       .DB " Preparando pacotes...",_NC,0

MSG_WIFI_DOWNLOAD:
       .DB " Baixando dados...",_NC,0

MSG_WIFI_PCK_NRO:
       .DB " Pacote Nro:",0,_NC

MSG_WIFI_GRAVADO:
       .DB " Gravado com sucesso!",0

MSG_ERRO_NOME:
       .DB " Erro nome da rede!",0



MSG_DOWN_ERRO1:
       .DB " Falha con.servidor!",_NC,0

MSG_DOWN_ERRO2:
       .DB " Essa atualizacao ja ",_CR,_LF,_NC
	   .DB " existe na memoria",_NC,0  

;MSG_DOWN_ERRO2A:
      ; .DB " Essa atualizacao ja ",_CR,_LF,_NC
	  ; .DB " existe na memoria*",0  

MSG_DOWN_ERRO2A:
       .DB " Aparelho atualizado!",_CR,_LF,_NC
	   .DB "                     ",0  

MSG_GRAV_ERRO:
       .DB " Erro de overflow!",_NC,0
	    
 