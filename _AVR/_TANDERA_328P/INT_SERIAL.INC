

INT_RX_HANDLE:
   		//
		//SAVE SREG
		//
		  push	Acc
	      in		Acc,SREG
		//
		//SAVE REGISTERS
		//

		  _M_PUSH_LOWER_REGS						;save r0..r15
		  _M_PUSH_UPPER_REGS						;save r16..r31

		  lds XL,_GUARDA_XL
		  lds XH,_GUARDA_XH 
		  
INT_USART_RX:
          lds  Acc,UCSR0A
		  sbrs Acc,RXC0
		  rjmp INT_USART_RX

		  lds  Acc,UDR0	          ; Get received data
		  sts _DADO_SERIAL,Acc
          
		  cpi Acc,'$'             ; Eh igual ao START byte? ($)
		  brne VER_FLAG
		   ;  _LED2_ON
          _SET _FLAG_START        ; seta flag de start (Comeco de um frame
          
		  LDIX _BUFFER_GPS        ; Aponta o ponteiro X para BUFFER GPS
          
		  ldi Acc,94
          sts _CONTADOR_BUF,Acc   ; Numero maximo de bytes no buffer
      
	      lds Acc,_DADO_SERIAL

		  st X+,Acc                ; Ja grava o $ na memoria
		  
          rjmp SERIAL_SAIDA

VER_FLAG:
          lds Acc,_FLAG_START
		  cpi Acc,_ON
		  lbrne SERIAL_SAIDA       ; Testa a flag de start

          ldi AccT,low(_BUFFER_GPS+3)
		  ldi AccTH,high(_BUFFER_GPS+3)

          cp XL,AccT
		  cpc XH,AccTH
          
		  brne VER_FLAG_CONT1

          lds Acc,_DADO_SERIAL
		  cpi Acc,'R'

		  brne VER_FLAG_ERRO
		  rjmp VER_FLAG_CONT

VER_FLAG_CONT1:

          ldi AccT,low(_BUFFER_GPS+4)
		  ldi AccTH,high(_BUFFER_GPS+4)

          cp XL,AccT
		  cpc XH,AccTH

    	  brne VER_FLAG_CONT

	      lds Acc,_DADO_SERIAL
		  cpi Acc,'M'

		  brne VER_FLAG_ERRO
		
		  rjmp VER_FLAG_CONT

VER_FLAG_ERRO:
          
		  _CLEAR _FLAG_START

          rjmp SERIAL_SAIDA

VER_FLAG_CONT:
          
          lds Acc,_DADO_SERIAL
		  st X+,Acc
		  
		  lds Acc,_CONTADOR_BUF ; Rotina para proteger o buffer contra overflow
		  subi Acc,1
		  sts _CONTADOR_BUF,Acc	
		  
		  tst Acc
		  
		  brne SERIAL_CONTINUA	  
		   
          lds Acc,94
          sts _CONTADOR_BUF,Acc
          
		  _CLEAR _FLAG_START
		  _CLEAR _FLAG_POSSUI_DADOS_GPS
		  rjmp SERIAL_SAIDA

SERIAL_CONTINUA:

          lds Acc,_DADO_SERIAL
		  
		  cpi Acc,'*'
		  brne SERIAL_SAIDA
		 ; _LED2_OFF		  
		  		  
		  _SET _FLAG_POSSUI_DADOS_GPS
		   
		  lds Acc,94
          sts _CONTADOR_BUF,Acc
          
		  _CLEAR _FLAG_START
	
          lds Acc, _GPS_TICKS_COUNT
		  inc Acc
		  sts _GPS_TICKS_COUNT,Acc

		  cpi Acc,24
		  brne SERIAL_VER1MINUTO

		  _SET _FLAG_CARREGA_SRAM

SERIAL_VER1MINUTO:
		  cpi Acc,34
		  brne SERIAL_SAIDA

		  _SET _FLAG_1_MINUTO

		  lds Acc,_FLAG_ACIMA_120KM
		  cpi Acc,_ON
		  brne SERIAL_VER1MINUTO_ABAIXO_CONT
		  ;Recarrega SRAM a cada 30 Seg +-
		  ldi Acc,20  
		  sts _GPS_TICKS_COUNT,Acc

		  rjmp SERIAL_SAIDA

SERIAL_VER1MINUTO_ABAIXO_CONT:
        
		  ldi Acc,15    
		  sts _GPS_TICKS_COUNT,Acc
		  
SERIAL_SAIDA:

          sts _GUARDA_XL,XL
		  sts _GUARDA_XH,XH
          
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

		  ret

      
