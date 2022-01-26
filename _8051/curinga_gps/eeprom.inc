
;
; Rotinas para acesso a EEPROM
;


; ENTRADA: DPTR= ENDEREÇO
;          A= DADO
;

_EE_WRITE:

     ; mov   WDTRST,#01EH            ; reseta Watchdog
     ; mov   WDTRST,#0E1H

      push acc
      push psw

      mov EECON,#02H     ; Habilita EEE

      mov A,DADO
      movx @dptr,A

_EE_WRITE_LOOP:

      mov a,EECON
      jb acc.0,_EE_WRITE_LOOP

      mov EECON,#00H     ; Desabilita EEE

      pop psw
      pop ACC

      ret


;
; Rotina para ler da EEPROM
; ENTRADA: ENDEREÇO=DPTR
; SAIDA  : A


_EE_READ:

     ; mov   WDTRST,#01EH            ; reseta Watchdog
     ; mov   WDTRST,#0E1H

      mov EECON,#02H     ; Habilita EEE
      movx a,@dptr
      mov EECON,#0       ; Desabilita EEE

      ret









