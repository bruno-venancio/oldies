;*
; "Bit-bang" serial I/O functions for the 8051.
;*
;* These routines transmit and receive serial data using two general
;* I/O pins, in 8 bit, No parity, 1 stop bit format. They are useful
;* for performing serial I/O on 8051 derivatives not having an
;* internal UART, or for implementing a second serial channel.
;*
;* Dave Dunfield - May 17, 1994
;*
;* NOTE that R0 and R1 are used by the functions. You may wish to
;* add PUSH/POP instructions to save/restore these registers.


;* The serial baud rate is determined by the processor crystal, and
;* this constant which is calculated as: (((crystal/baud)/12) -5) / 2
;BITTIM	EQU	45			(((11059200/9600)/12) -5) / 2
;BITTIM	EQU	93		;	(((22118400/9600)/12) -5) / 2
;BITTIM	EQU	205		;	(((24000000/4800)/12) -5) / 2
BITTIM	EQU	127		;	(((30000000/19200)/6) -5) / 2
;BITTIM	EQU	63		;	(((30000000/38400)/6) -5) / 2
BITTIM2 EQU	255		;	(((30000000/9600)/6) -5) / 2
;BITTIM	EQU	93		;	(((22118400)/19200/6) -5) / 2




;*
;* Transmit character in A via TXD line
;*

_TXD_2:
        push    00h
        push    01h
        push    acc

        clr	TXD2		;Drop line for start bit
	mov	R0,#BITTIM	;Wait full bit-time
	djnz	R0,$		;For START bit
	nop
	nop

	mov	R1,#8		;Send 8 bits
putc1:
	rrc	A		;Move next bit into carry
	mov	TXD2,C		;Write next bit
	mov	R0,#BITTIM	;Wait full bit-time
	djnz	R0,$		;For DATA bit
	djnz	R1,putc1	;write 8 bits
	setb	TXD2		;Set line high
	rrc	A		;Restore ACC contents
	mov	R0,#BITTIM	;Wait full bit-time
	djnz	R0,$		;For STOP bit

        pop     acc
	pop     01h
	pop     00h

        ret
;*
;* Receive a character from the RXD line and return in A
;*
_RXD_2:
   	jb	RXD2,$		;Wait for start bit
	mov	R2,#BITTIM/2	;Wait 1/2 bit-time
	djnz	R2,$		;To sample in middle

	jb	RXD2,_RXD_2	;Insure valid
	mov	R1,#8		;Read 8 bits
getc1:
	mov	R2,#BITTIM	;Wait full bit-time
	djnz	R2,$		;For DATA bit
	mov	C,RXD2		;Read bit
	rrc	A		;Shift it into ACC
	djnz	R1,getc1	;read 8 bits

       ; setb CTS

        ret			;go home



;*
;* Receive a character from the RXD line and return in A
;*
_RXD_3:
        jnb BOTAO_GRAVA,RXD_JMP
   	jb	RXD2,_RXD_3	;Wait for start bit
	mov	R2,#BITTIM/2	;Wait 1/2 bit-time
	djnz	R2,$		;To sample in middle

	jb	RXD2,_RXD_3	;Insure valid
	mov	R1,#8		;Read 8 bits
getc1b:
	mov	R2,#BITTIM	;Wait full bit-time
	djnz	R2,$		;For DATA bit
	mov	C,RXD2		;Read bit
	rrc	A		;Shift it into ACC
	djnz	R1,getc1b	;read 8 bits


        ret			;go home

RXD_JMP:

        mov a,#20
        lcall _delay_ms

        jb BOTAO_GRAVA,$

        mov a,#20
        lcall _delay_ms

        jnb BOTAO_GRAVA,$

        djnz R8,RXD_JMP

       ljmp BUZZER_TEST





;*
;* Transmit character in A via TXD line
;*

_TXD_ESP:
        push    acc

        clr	ESP_TXD		;Drop line for start bit
	mov	R2,#BITTIM2	;Wait full bit-time
	djnz	R2,$		;For START bit
	nop
	nop

	mov	R3,#8		;Send 8 bits
putc1_es:
	rrc	A		;Move next bit into carry
	mov	ESP_TXD,C	;Write next bit
	mov	R2,#BITTIM2	;Wait full bit-time
	djnz	R2,$		;For DATA bit
	djnz	R3,putc1_es	;write 8 bits
	setb	ESP_TXD		;Set line high
	rrc	A		;Restore ACC contents
	mov	R2,#BITTIM2	;Wait full bit-time
	djnz	R2,$		;For STOP bit

        pop     acc

        ret


;*
;* Receive a character from the RXD line and return in A
;*
_RXD_ESP:
   	jb	ESP_RXD,$		;Wait for start bit
	mov	R2,#BITTIM2/2	;Wait 1/2 bit-time
	djnz	R2,$		;To sample in middle

	jb	ESP_RXD,_RXD_ESP	;Insure valid
	mov	R1,#8		;Read 8 bits
getc1_es:
	mov	R2,#BITTIM2	;Wait full bit-time
	djnz	R2,$		;For DATA bit
	mov	C,ESP_RXD	;Read bit
	rrc	A		;Shift it into ACC
	djnz	R1,getc1_es	;read 8 bits

        jnb ESP_RXD,$

        ret			;go home



