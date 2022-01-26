;
; ROTINA DE ATENDIMENTO A INT1
; 
_INT0_INTERRUPT_HANDLE:
       
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

		_M_DELAY_MILLISECONDS 10

		sbic _INT0_PORT_INPUT,_INT0_BIT
		rjmp INT0_EXIT

		;call _BUZZER_ON
		;_M_DELAY_MILLISECONDS 50
		;call _BUZZER_OFF

		lds Acc,_FLAG_CARRO_PARADO
		cpi Acc,_ON
		lbrne INT0_MARCA
		
		ldi Temp,0
		call _BUZZER_ON
				
INT0_LOOP1:
        _M_DELAY_MILLISECONDS 50
		inc Temp

		cpi Temp,20
		brge INT0_LOOP1_L1
		_LED1_ON
		rjmp INT0_LOOP1_L2

INT0_LOOP1_L1:
        _LED2_ON
		cpi Temp,41
		brlo INT0_LOOP1_L2
		
		_LED3_ON
		
INT0_LOOP1_L2:

		sbis _INT0_PORT_INPUT,_INT0_BIT
        rjmp INT0_LOOP1

		call _BUZZER_OFF
		_LED1_OFF
		_LED2_OFF
		_LED3_OFF
		        
		cpi Temp,20
		brge INT0_CONTINUA 

		lds Acc,_FLAG_SWAP_HORA
		com Acc
		sts _FLAG_SWAP_HORA,Acc	
		
		rjmp INT0_EXIT	

INT0_CONTINUA:
				 
		_SET _FLAG_WIFI   // Atualiza
        
		rjmp INT0_EXIT
		
		// Marca / Desmarca
INT0_MARCA:
        
		lds Acc,_FLAG_SAT_FOUND
	    cpi Acc,_ON
		brne INT0_EXIT 

        call _BUZZER_ON
		ldi Temp,0 

        lds Acc,_FLAG_PARDAL_LOCALIZADO
		cpi Acc,_ON
		breq INT0_PULA

INT0_LOOP2:

		; Verifica se eh para atualizar a vel da via ou se eh para marcar
		_M_DELAY_MILLISECONDS 50
		inc Temp

		sbis _INT0_PORT_INPUT,_INT0_BIT
        rjmp INT0_LOOP2

		call _BUZZER_OFF
		
		cpi Temp,19
		brge INT0_ATUALIZA_VEL		

		_SET _FLAG_MARCA_PONTO

		rjmp INT0_EXIT


INT0_ATUALIZA_VEL:
        _SET _FLAG_ATUAL_VEL_CORRENTE
		
		rjmp INT0_EXIT

INT0_PULA:
		_M_DELAY_MILLISECONDS 50
		inc Temp

		sbis _INT0_PORT_INPUT,_INT0_BIT
        rjmp INT0_PULA

		call _BUZZER_OFF

		cpi Temp,10
		brlo INT0_EXIT

        _SET _FLAG_APAGA_PONTO
		   
INT0_EXIT:	
        sbis _INT0_PORT_INPUT,_INT0_BIT
		rjmp INT0_EXIT
		
				 
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



