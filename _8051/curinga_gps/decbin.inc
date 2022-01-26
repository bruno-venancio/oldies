;29 Oct 00
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;DECBIN is called to convert an unsigned ASCII string in data memory
; to binary, returning the answer as r7,r6,r5,r5. The string must
; be stored MSD in low memory with a null character (binary 0) following
; the units position in high memory. The string can have 1 to 10 ASCII
; digits. DECBIN calls MUL2.
;
;
;call:
;	mov	r0,#string
;	call	DECBIN
;
;return:
;	binary equivalent = r7(MSBs),r6,r5,r4(LSBs)
;	r0=> null at high end of caller's buffer
;	acc destroyed, all others saved
;
;Notes: There is no testing for valid ASCII. The LS 4 bits of the
; assumed ASCII character are stripped off and used as the BCD
; equivalent.
;
;examples:
;              input string                  |        return
; +0  +1  +2  +3  +4  +5  +6  +7  +8  +9  +10   r7 r6 r5 r4    r0=>
;"0"  0   dc  dc  dc  dc  dc  dc  dc  dc  dc    0  0  0  0    string+1
;"1" "0"  0   dc  dc  dc  dc  dc  dc  dc  dc    0  0  0  0a   string+2
;"2" "5" "6"  0   dc  dc  dc  dc  dc  dc  dc    0  0  1  0    string+3
;"6" "5" "5" "3" "5"  0   dc  dc  dc  dc  dc    0  0  ff ff   string+5
;"4" "2" "9" "4" "9" "6" "7" "2" "9" "5"  dc    ff ff ff ff   string+10
; 0   dc  dc  dc  dc  dc  dc  dc  dc  dc  dc    0  0  0  0    string+0
;
;
;Original author: John Veazey, Ridgecrest, CA, 29 Oct 00
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
               ;29 Oct 00

_DECBIN:
;
;r7,r6,r5,r4 will be the accumulators, clear them out
;
	clr	a
	mov	r7,a
	mov	r6,a
	mov	r5,a
	mov	r4,a
;
;We are all done if r0 => 0
;
deb10:	mov	a,@r0
	jz	deb90
;
;Another digit found. Multiply the accumulated value by two
; and save on stack for later use
;
	acall	MUL2
	mov	a,r7
	push	acc
	mov	a,r6
	push	acc
	mov	a,r5
	push	acc
	mov	a,r4
	push	acc
;
;Multiply the accumulated value by two, two more times
; to form times eight
;
	acall	MUL2
	acall	MUL2
;
;Add the times two saved on the stack to the times eight in the
; accumulated value to make a total of times ten
;
	pop	acc
	add	a,r4
	mov	r4,a
	pop	acc
	addc	a,r5
	mov	r5,a
	pop	acc
	addc	a,r6
	mov	r6,a
	pop	acc
	addc	a,r7
	mov	r7,a
;
;Add the just-read digit to the accumulated value
;
	mov	a,@r0
	anl	a,#0fh
	add	a,r4
	mov	r4,a
	mov	a,r5
	addc	a,#0
	mov	r5,a
	mov	a,r6
	addc	a,#0
	mov	r6,a
	mov	a,r7
	addc	a,#0
	mov	r7,a
;
;Advance the caller's pointer to the next position
; and look for another digit

	inc	r0
	sjmp	deb10
;
;Return to caller with answer in r7,r6,r5,r4 and r0 set
;
deb90:	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;MUL2 multiplies r7,r6,r5,r4 by two.
;
;call:
;	call	MUL2
;
;return:
;	r7,r6,r5,r4 = 2(r7,r6,r5,r4)
;	acc destroyed
;	all other registers are saved
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
MUL2:
	mov	a,r4	;r4 = 2*r4
	add	a,r4
	mov	r4,a
	mov	a,r5	;r5 = c +2*r5
	addc	a,r5
	mov	r5,a
	mov	a,r6	;r6 = c +2*r6
	addc	a,r6
	mov	r6,a
	mov	a,r7	;r7 = c +2*r7
	addc	a,r7
	mov	r7,a
;
;Return to caller
;
	ret
;end DECBIN.ASM



