

_TIMER_INTERRUPT_HANDLE:
		//
		//save all registers and SREG
		//
		push		Acc							;save ACC					;<===analizar para ver sé e necessário salvar todos os registros
		in			Acc,SREG					;save SREG
		_M_PUSH_LOWER_REGS						;save all regs
		_M_PUSH_UPPER_REGS
		
        lds AccT,_FLAG_APITA
		cpi AccT,_ON
	 	brne TIMER_NAO_APITA

	 	lds AccH,_VAR_BEEP
 		subi AccH,1
	 	sts _VAR_BEEP,AccH 

	 	tst AccH
	 	brne TIMER_SAIDA
	
		lds Acc,_TEMPO
		ldi TempH,1
		mul Acc,TempH
		
		sts _VAR_BEEP,Acc

		/*
		lds AccT,_VAR_BEEP_ANT    // PEGA O VAR_BEEP ANTERIOR

		cp AccT,Acc
		breq TIMER_VAR_IGUAIS
		sts _VAR_BEEP_ANT,Acc    // AGORA VAR_BEEP ANTERIOR E IGUAL A NOVA

		call _BUZZER_OFF

		rjmp TIMER_SAIDA
		*/
		_LED1_OFF

		call _BUZZER_ALTERNATE

		rjmp TIMER_SAIDA
		 
TIMER_NAO_APITA:
        
		lds AccH,_TICKS
 		subi AccH,1
	 	sts _TICKS,AccH 
		tst AccH
		brne TIMER_SAIDA

		ldi AccH,12
		sts _TICKS,AccH

		call _LED_VD_SWAP

		
TIMER_SAIDA:	
		//
		//POP ALL REGISTERS AND SREG
		//
		_M_POP_UPPER_REGS						;restore all regs
		_M_POP_LOWER_REGS
		out			SREG,Acc					;restore SREG
		pop			Acc							;restore Acc
		ret