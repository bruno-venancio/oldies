;
; ROTINA DE ATENDIMENTO A INT1
; 
_INT1_INTERRUPT_HANDLE:
       
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

		_M_DELAY_MILLISECONDS 5

		sbic _INT0_PORT_INPUT,_INT0_BIT
		rjmp INT0_EXIT

		call _BUZZER_ON
		_M_DELAY_MILLISECONDS 80
		call _BUZZER_OFF

		_M_DELAY_MILLISECONDS 1400

		sbic _INT0_PORT_INPUT,_INT0_BIT
		rjmp INT0_MARCA

		call _BUZZER_ON
		_M_DELAY_MILLISECONDS 150
		call _BUZZER_OFF
		
		_SET _FLAG_MENU 
        
		rjmp INT0_EXIT
		
		// Marca / Desmarca
INT0_MARCA:
        
		lds Acc,_FLAG_SAT_FOUND
	    cpi Acc,_ON
		brne INT0_EXIT 
 
        lds Acc,_FLAG_PARDAL_LOCALIZADO
		cpi Acc,_ON
		breq INT0_PULA

		_SET _FLAG_MARCA_PONTO

		rjmp INT0_EXIT
INT0_PULA:
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






