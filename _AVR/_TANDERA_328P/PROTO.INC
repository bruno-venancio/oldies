/*
;
;
;
PROTOCOLO:
	
	;	sbis _INT0_PORT_INPUT,_INT0_BIT
      ;  rjmp PROTOCOLO

        ldi Acc,_OFF
		call _USART_RX_INT_ENABLE
			
		call _BEEP
				 		        
RECEPCAO:
         ldi Acc,_CR
         call _USART_TX

         ldi Acc,_LF
         call _USART_TX

		 ldi Acc,'>'
         call _USART_TX
		 		          
         ldi TEMPH,3

		 call _USART_RX

PROTO_EXECUTA_COMANDO_2:

         cpi Acc,'I'
         brne PROTO_EXECUTA_COMANDO_3

         rjmp UPLOAD

PROTO_EXECUTA_COMANDO_3:
         cpi Acc,'S'
		 brne PROTO_EXECUTA_COMANDO_ERRO

         ldi Acc,'O'
         call _USART_TX

         ldi Acc,'K'
         call _USART_TX

PROTO_EXECUTA_COMANDO_ERRO:
		 rjmp RECEPCAO
		 



;
; Faz  dos dados da flash para o PC
;


UPLOAD:
        ldi Acc,0
        sts _CONTADOR16_L,Acc
        sts _CONTADOR16_H,Acc

        ldi Acc,_CR
        call _USART_TX 

        ldi Acc,_LF
        call _USART_TX

        ldi TempH,11

UPLOAD_L1:
        ldi Acc,0
        sts _BUFFER_BYTE,Acc
        ldiw X,0

        call _RODA_ADDRESS

UPLOAD_CONT_LOOP:

        call _AT45_MAIN_PAGE_READ

        swap Acc

        mov AccT,Acc

        andi Acc,0b11110000

        swap Acc

        call _HEX_TO_ASCII

        call _USART_TX

        mov Acc,AccT

        andi Acc,0b00001111

        call _HEX_TO_ASCII

        call _USART_TX 

        call _INCREMENTA_ENDERECO

		dec TempH
		brne UPLOAD_CONT_LOOP
        
        ldi TempH,11
		
      ;  ldi Acc,_CR
       ; call _USART_TX

       ; ldi Acc,_LF
       ; call _USART_TX

        _M_DELAY_MICROSECONDS 50 
              

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
		brne UPLOAD_L1
        
        lds Acc,_CONTADOR16_H
		lds AccH,_CONT_PONTOS_H
		cp Acc,AccH
		brne UPLOAD_L1

UPLOAD_EXIT:

        ldi Acc,_CR
        call _USART_TX 

        ldi Acc,_LF
        call _USART_TX 
		
		ldi Acc,'P'
        call _USART_TX 


        jmp RECEPCAO
        

*/

