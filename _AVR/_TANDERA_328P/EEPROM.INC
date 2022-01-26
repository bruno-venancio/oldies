
#ifndef __EEPROM__

	#define __EEPROM__

	.message "[ (EEPROM) ]"

	
;---------------------------------------+
;|									
;| BY JOÃO DARTAGNAN ANTUNES OLIVEIRA					
;| DATA:13/04/2000						
;>----------------------------------------------------------------------<
;| E E P R O M 	R O U T I N E S						|
;| F O R  A V R  C H I P S  W I T H  5 1 2  B Y T E S  O R  M O R E 	|
;+----------------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM			|
;+----------------------------------------------------------------------+

	;>------------------------------------------------------<
	;| I M P L E M E N T E D  F U N C T I O N S 		|
	;+----------------------------------------------------------------------------------------------+
	;| _EEPROM_WRITE	WRITE DATA INTO EEPROM							|
	;|			SAMPLE USING EEPROM_WRITE TO WRITE DATA 0X33 INTO ADDRESS 0X14		|
	;|												|
	;|			LDI	ACC,0X33							|
	;|			LDI	ACCH,LOW(0X14)							|
	;|			LDI	ACCT,HIGH(0X14)							|
	;|			RCALL	EEPROM_WRITE							|
	;|----------------------------------------------------------------------------------------------|												|
	;| _EEPROM_READ		READ DATA FROM EEPROM							|
	;|			SAMPLE USING EEPROM_READ TO READ A DATA FROM A ADDRESS 0X14		|
	;|												|
	;|			LDI	ACCH,LOW(0X14)							|
	;|			LDI	ACCT,HIGH(0X14)							|
	;|			RCALL	EEPROM_READ	;AFTER THIS ACC CONTENTS HAVE A DATA READ	|
	;+----------------------------------------------------------------------------------------------+
	;+--------------------------+
	;| WRITE DATA INTO EEPROM   |
	;|			    |
	;| input: Acc  Data	    |
	;| 	  AccH Address low  |
	;|	  AccT Address High |
	;+--------------------------+
	_EEPROM_WRITE:
		sbic	EECR,EEPE			;wait last write operation is done
		rjmp	_EEPROM_WRITE			;wait
		out	EEARH,AccT			;set address High
		out	EEARL,AccH			;set address Low
		out	EEDR,Acc			;set data
		push	Acc				;Save Acc
		in	Acc,SREG			;get SREG state
		push	Acc				;Save Acc Again
		cli
		sbi	EECR,EEMPE  			;request eeprom write
		sbi	EECR,EEPE			;place write command
		pop	Acc				;restore SREG 
		out 	SREG,Acc
		pop	Acc				;restore Acc
		ret	
	;+--------------------------+
	;| READ DATA FROM EEPROM    |
	;|			    |
	;| input: AccH Address low  |
	;|	  AccT Address High | 	
	;| output: Acc data	    |
	;+--------------------------+
	_EEPROM_READ:
		sbic	EECR,EEPE			;wait last write operation is done
		rjmp	_EEPROM_READ			;wait
		out	EEARH,AccT			;set address high
		out	EEARL,AccH			;set address low
;		sbi	EECR,EERE			;place read command
		sbi	EECR,EERE			;place read command
		in	Acc,EEDR			;get data
		ret



;+----------------------------------------------------------------------+
;|									
;| BY JOÃO DARTAGNAN ANTUNES OLIVEIRA					
;| DATA:13/04/2000							
;>----------------------------------------------------------------------<
;| E E P R O M 	E X T E N S I O N  R O U T I N E S			
;| F O R  A V R  C H I P S  W I T H  5 1 2  B Y T E S  O R  M O R E 	|
;+----------------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM			
;+----------------------------------------------------------------------+

	;>------------------------------------------------------<
	;| I M P L E M E N T E D  F U N C T I O N S 		|
	;+------------------------------------------------------+---------------------------------------+
	;| _EEPROM_W_BYTE	WRITE BYTE INTO EEPROM							|
	;|												|
	;|			LDI	ACC,0X33							|
	;|			LDIW	Y,EEPROM_ADDRESS						|
	;|			call 	EEPROM_W_BYTE							|
	;+----------------------------------------------------------------------------------------------+
	;| _EEPROM_W_WORD	WRITE WORD INTO EEPROM							|
	;|												|
	;|			LDIAW	0X33								|
	;|			LDIW	Y,EEPROM_ADDRESS						|
	;|			call 	EEPROM_W_WORD							|
	;+----------------------------------------------------------------------------------------------+
	;| _EEPROM_W_LONG	WRITE LONG WORD INTO EEPROM						|
	;|												|
	;|			LDIAL	0X33								|
	;|			LDIW	Y,EEPROM_ADDRESS						|
	;|			call 	EEPROM_W_LONG							|
	;+----------------------------------------------------------------------------------------------+
	;| _EEPROM_W_STR_SRAM	WRITE STRING OF BYTES FROM SRAM INTO EEPROM				|
	;|												|
	;|			LDIW	X,SRAM_ADDRESS							|
	;|			LDIW	Y,EEPROM_ADDRESS						|
	;|			LDIAW	NUMBER_OF_BYTES							|
	;|			call 	EEPROM_W_STR_SRAM						|
	;+----------------------------------------------------------------------------------------------+
	;| _EEPROM_W_STR_FLASH	WRITE STRING OF BYTES FROM FLASH INTO EEPROM				|
	;|												|
	;|			LDIW	X,SRAM_ADDRESS							|
	;|			LDIW	Y,EEPROM_ADDRESS						|
	;|			LDIAW	NUMBER_OF_BYTES							|
	;|			call 	EEPROM_W_STR_FLASH						|
	;+----------------------------------------------------------------------------------------------+
	;| _EEPROM_R_BYTE	READ BYTE FROM EEPROM							|
	;|												|
	;|			LDIW	Y,EEPROM_ADDRESS						|
	;|			call 	EEPROM_R_BYTE		;ACC=READ BYTE				|
	;+----------------------------------------------------------------------------------------------+
	;| _EEPROM_R_WORD	READ WORD FROM EEPROM							|
	;|												|
	;|			LDIW	Y,EEPROM_ADDRESS						|
	;|			call 	EEPROM_W_WORD		;ACCW=READ WORD				|	
	;+----------------------------------------------------------------------------------------------+
	;| _EEPROM_R_LONG	READ LONG WORD FROM EEPROM						|
	;|												|
	;|			LDIW	Y,EEPROM_ADDRESS						|
	;|			call 	EEPROM_W_LONG		:ACCL=READ LONG WORD			|
	;+----------------------------------------------------------------------------------------------+
	;| _EEPROM_R_STR_SRAM	READ STRING OF BYTES FROM EEPROM INTO SRAM				|
	;|												|
	;|			LDIW	X,SRAM_ADDRESS							|
	;|			LDIW	Y,EEPROM_ADDRESS						|
	;|			LDIAW	NUMBER_OF_BYTES							|
	;|			call 	EEPROM_W_STR_SRAM						|
	;+----------------------------------------------------------------------------------------------+

	;>--------------------------------------------------------------<
	;| FUNCTION   : _EEPROM_W_BYTE					|
	;>--------------------------------------------------------------<
	;| PERFORM    : Write a byte into EEPROM			|
	;>--------------------------------------------------------------<
	;| INPUT      :	Acc	byte					|
	;|		Y	EEPROM ADDRESS				|
	;| OUTPUT     :	Y++						|
	;| DESTROYED  :	Flags						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_EEPROM_W_BYTE:
		push	AccH						;save useds
		push	AccT
		mov	AccH,YL						;move to appropriate registers
		mov	AccT,YH
		call 	_EEPROM_WRITE					;write byte into eeprom
	_EEPROM_W_BYTE_DONE:
		adiw	YL,1
		pop	AccT						;restore useds
		pop	AccH
		ret
	;>--------------------------------------------------------------<
	;| FUNCTION   : _EEPROM_W_WORD					|
	;>--------------------------------------------------------------<
	;| PERFORM    : Write a word into EEPROM			|
	;>--------------------------------------------------------------<
	;| INPUT      :	AccH:Acc Word					|
	;|		Y	 EEPROM ADDRESS				|
	;| OUTPUT     :	Y+=2						|
	;| DESTROYED  :	None						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_EEPROM_W_WORD:
		push	Acc						;save useds
		call 	_EEPROM_W_BYTE					;write low byte
		mov	Acc,AccH					;get high byte
		call 	_EEPROM_W_BYTE					;write high byte
		pop	Acc						;restore useds
		ret
	;>--------------------------------------------------------------<
	;| FUNCTION   : _EEPROM_W_LONG					|
	;>--------------------------------------------------------------<
	;| PERFORM    : Write a long word into EEPROM			|
	;>--------------------------------------------------------------<
	;| INPUT      :	AccTH:AccT:AccH:Acc Long Word			|
	;|		Y	 EEPROM ADDRESS				|
	;| OUTPUT     :	Y+=4						|
	;| DESTROYED  :	Flags						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_EEPROM_W_LONG:
		push	Acc						;save useds
		call 	_EEPROM_W_BYTE					;write low byte
		mov	Acc,AccH					;get high byte
		call 	_EEPROM_W_BYTE					;write high byte
		mov	Acc,AccT					;get 3nd byte
		call 	_EEPROM_W_BYTE					;write 3nd byte
		mov	Acc,AccTH					;get 4rt byte
		call 	_EEPROM_W_BYTE					;write 4rt byte
		pop	Acc						;restore useds
		ret
	;>--------------------------------------------------------------<
	;| FUNCTION   : _EEPROM_W_STR_SRAM				|
	;>--------------------------------------------------------------<
	;| PERFORM    : Write a string byte from SRAM into EEPROM	|
	;>--------------------------------------------------------------<
	;| INPUT      :	X	 SRAM ADDRESS				|
	;|		Y	 EEPROM ADDRESS				|
	;|		AccH:Acc NUMBER OF BYTES IN STRING		|
	;| OUTPUT     :	X+=NUMBER OF BYTES				|
	;|		Y+=NUMBER OF BYTES				|
	;| DESTROYED  :	Flags						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|		
	_EEPROM_W_STR_SRAM:
		pushaw							;save useds
		push	AccT
		ldi	AccT,0						;load zero
	_EEPROM_W_STR_SRAM_10:
		cp	Acc,AccT					;NUMBER OF BYTES == 0?
		cpc	AccH,AccT
		breq	_EEPROM_W_STR_SRAM_DONE				;yes, tranfer done
		push	Acc						;save low counter
		ld	Acc,X+						;get next data from SRAM
		call 	_EEPROM_W_BYTE					;write byte into EEPROM
		pop	Acc						;restore log counter
		subi	Acc,1						;NUMBER OF BYTES--
		sbci	AccH,0
		rjmp	_EEPROM_W_STR_SRAM_10				;get next data *<NOCHANGE>*
	_EEPROM_W_STR_SRAM_DONE:
		pop	AccT						;restore useds
		popaw
		ret
	;>--------------------------------------------------------------<
	;| FUNCTION   : _EEPROM_W_STR_FLASH				|
	;>--------------------------------------------------------------<
	;| PERFORM    : Write a string byte from FLASH into EEPROM	|
	;>--------------------------------------------------------------<
	;| INPUT      :	X	 FLASH ADDRESS				|
	;|		Y	 EEPROM ADDRESS				|
	;|		AccH:Acc NUMBER OF BYTES IN STRING		|
	;| OUTPUT     :	X+=NUMBER OF BYTES				|
	;|		Y+=NUMBER OF BYTES				|
	;| DESTROYED  :	Flags						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|		
	_EEPROM_W_STR_FLASH:
		pushaw							;save useds
		pushw	Z
		push	AccT
		ldi	AccT,0						;load zero
	_EEPROM_W_STR_FLASH_10:
		cp	Acc,AccT					;NUMBER OF BYTES == 0?
		cpc	AccH,AccT
		breq	_EEPROM_W_STR_FLASH_DONE			;yes, tranfer done
		push	Acc						;save low counter
		movw	ZL,XL
		lpm	Acc,Z						;get next data from SRAM
		call 	_EEPROM_W_BYTE					;write byte into EEPROM
		adiw	XL,1						;next EEPROM address
		pop	Acc						;restore log counter
		subi	Acc,1						;NUMBER OF BYTES--
		sbci	AccH,0
		rjmp	_EEPROM_W_STR_FLASH_10				;get next data *<NOCHANGE>*
	_EEPROM_W_STR_FLASH_DONE:
		pop	AccT						;restore useds
		popw	Z
		popaw
		ret
	;>--------------------------------------------------------------<
	;| FUNCTION   : _EEPROM_R_BYTE					|
	;>--------------------------------------------------------------<
	;| PERFORM    : read a byte from EEPROM				|
	;>--------------------------------------------------------------<
	;| INPUT      :	Y	EEPROM ADDRESS				|
	;| OUTPUT     :	Acc	Read byte				|
	;		Y++						|
	;| DESTROYED  :	Flags						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_EEPROM_R_BYTE:
		push	AccH						;save useds
		push	AccT
		mov	AccH,YL						;move to appropriate registers
		mov	AccT,YH
		call 	_EEPROM_READ					;read byte from eeprom
		rjmp	_EEPROM_W_BYTE_DONE				;*<NOCHANGE>*

	;>--------------------------------------------------------------<
	;| FUNCTION   : _EEPROM_R_WORD					|
	;>--------------------------------------------------------------<
	;| PERFORM    : read a word from EEPROM				|
	;>--------------------------------------------------------------<
	;| INPUT      :	Y	 EEPROM ADDRESS				|
	;| OUTPUT     :	AccH:Acc Read word				|
	;		Y+=2						|
	;| DESTROYED  :	Flags						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_EEPROM_R_WORD:
		call 	_EEPROM_R_BYTE					;read low byte
		push	Acc						;save read low byte
		call 	_EEPROM_R_BYTE					;read high byte
		mov	AccH,Acc					;move to appropriate register
		pop	Acc						;restore low byte
		ret
	;>--------------------------------------------------------------<
	;| FUNCTION   : _EEPROM_R_LONG					|
	;>--------------------------------------------------------------<
	;| PERFORM    : read a long from EEPROM				|
	;>--------------------------------------------------------------<
	;| INPUT      :	Y	 EEPROM ADDRESS				|
	;| OUTPUT     :	AccTH:AccT:AccH:Acc Read long word		|
	;		Y+=4						|
	;| DESTROYED  :	Flags						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_EEPROM_R_LONG:
		call 	_EEPROM_R_BYTE					;read 1st byte
		push	Acc						;save
		call 	_EEPROM_R_BYTE					;read 2nd byte
		push	Acc						;save
		call 	_EEPROM_R_BYTE					;read 3nd byte
		push	Acc						;save
		call 	_EEPROM_R_BYTE					;read 4nt byte
		mov	AccTH,Acc					;move to appropriate register
		pop	AccT
		pop	AccH
		pop	Acc
		ret
	;>--------------------------------------------------------------<
	;| FUNCTION   : _EEPROM_R_STR_SRAM				|
	;>--------------------------------------------------------------<
	;| PERFORM    : Read a string byte from EEPROM into SRAM	|
	;>--------------------------------------------------------------<
	;| INPUT      :	X	 SRAM ADDRESS				|
	;|		Y	 EEPROM ADDRESS				|
	;|		AccH:Acc NUMBER OF BYTES IN STRING		|
	;| OUTPUT     :	X+=NUMBER OF BYTES				|
	;|		Y+=NUMBER OF BYTES				|
	;| DESTROYED  :	Flags						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	----						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|		
	_EEPROM_R_STR_SRAM:
		pushaw							;save useds
		push	AccT
		ldi	AccT,0						;load zero
	_EEPROM_R_STR_SRAM_10:
		cp	Acc,AccT					;NUMBER OF BYTES == 0?
		cpc	AccH,AccT
		breq	_EEPROM_R_STR_SRAM_DONE				;yes, tranfer done
		push	Acc						;save low counter
		call 	_EEPROM_R_BYTE					;read byte from EEPROM
		st	X+,Acc						;store into SRAM
		pop	Acc						;restore log counter
		subi	Acc,1						;NUMBER OF BYTES--
		sbci	AccH,0
		rjmp	_EEPROM_R_STR_SRAM_10				;get next data *<NOCHANGE>*
	_EEPROM_R_STR_SRAM_DONE:
		pop	AccT						;restore useds
		popaw
		ret


#endif
.exit

