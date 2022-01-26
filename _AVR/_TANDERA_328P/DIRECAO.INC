
_VER_ABERTURA:

         _M_PUSH_UPPER_REGS

         ; +------------------------------------+
         ; |PROCURA PONTOS COM A MESMA ABERTURA |
         ; +------------------------------------+

         ldix _SRAM_PONTOS+9         ; Aponta para buffer de pontos na SRAM
		 call _RODA_DPTR_SRAM2       ; Faz DPTR +(CONTADOR8 * 11)
         
		 ld Acc,X+

         ;cbr Acc,7                   ; Limpa MSB para o caso de pontos marcados por usuario
		 andi Acc,0b00000001
         sts _DIR_COMP_H,Acc
         
		 ld Acc,X

         sts _DIR_COMP_L,Acc
		 
		 call COMPARA_ABERTURA

         _M_POP_UPPER_REGS

         ret



;
;
;
;

COMPARA_ABERTURA:

         lds Temp,_DIR_COMP_L          ; Primeiro Guarda DIR_COMP
         lds TempH,_DIR_COMP_H
		   		    
; INCREMENTA O DIRCOMP EM 30 ( 16 BITS)

         ; Incrementa 30 em 16 bits
         clc

         lds Acc,_DIR_COMP_L
         ldi AccT,45
		 add Acc,AccT
         sts _DIR_COMP_L,Acc

         lds Acc,_DIR_COMP_H
		 ldi AccT,0
         adc Acc,AccT
         sts _DIR_COMP_H,Acc

        ; Testa se da mais que 360

         lds Acc,_DIR_COMP_H
         ldi AccT,HIGH(360)

         cp Acc,AccT

         breq TESTA_360_L
         brsh TESTA_360_MAIOR
         brlo COMP1_DIR_H     ; CONTINUA NORMALMENTE

TESTA_360_L:

         lds Acc,_DIR_COMP_L
         ldi AccT,LOW(360)

         cp Acc,AccT

         breq COMP1_DIR_H     ; CONTINUA NORMALMENTE
         brsh TESTA_360_MAIOR
         brlo COMP1_DIR_H     ; CONTINUA NORMALMENTE


		 
TESTA_360_MAIOR:
           
         ; PEGA DIRCOMP(> 360) - 360
         clc
         /*
		 lds Acc,_DIR_COMP_L
         ldi AccT,low(360)
		 sub Acc,AccT
         sts _DIR_COMP_L,Acc

         lds Acc,_DIR_COMP_H
		 ldi AccT,high(360)
         sbc Acc,AccT
         sts _DIR_COMP_H,Acc
		 */
		 lds Acc,_DIR_COMP_L
		 lds AccH,_DIR_COMP_H

		 ldi AccT,low(360)
		 ldi AccTH,high(360)

		 sub Acc,AccT
		 sbc AccH,AccTH

		 sts _DIR_COMP_L,Acc
		 sts _DIR_COMP_H,AccH

; Compara DIRECAO com DIR_COMP
COMP1_DIR_H:

         lds Acc,_DIRECAO_H
         lds AccT,_DIR_COMP_H

         cp Acc,AccT

         breq COMP1_DIR_L
         brsh TESTA_MAIOR_30H
         brlo COMP_CONTINUA

COMP1_DIR_L:

         lds Acc,_DIRECAO_L
         lds AccT,_DIR_COMP_L

         cp Acc,AccT

         breq COMP_CONTINUA
         brsh TESTA_MAIOR_30H
         brlo COMP_CONTINUA

;---------------------
TESTA_MAIOR_30H:

         mov Acc,TempH           ; DIR_COMP_H
         ldi AccT,high(45)
		              
		 cp Acc,AccT

         breq TESTA_MAIOR_30L
         brsh TESTA_MAIOR_330H
         brlo COMP_CONTINUA

TESTA_MAIOR_30L:

         mov Acc,Temp           ; DIRECAO_L
         ldi AccT,low(45)

         cp Acc,AccT

         breq COMP_CONTINUA
         brsh TESTA_MAIOR_330H
         brlo COMP_CONTINUA

TESTA_MAIOR_330H:

         mov Acc,TempH           ; DIR_COMP_H
         ldi AccT,high(315)

         cp Acc,AccT

         breq TESTA_MAIOR_330L
         brsh COMP_CONTINUA
         lbrlo COMP_ERRO2

TESTA_MAIOR_330L:

         mov Acc,Temp           ; DIRECAO_L
         ldi AccT,low(315)

         cp Acc,AccT

         lbrlo COMP_ERRO2
        ; jb FLAG_EH_MAIOR,COMP_CONTINUA
        ; jb FLAG_EH_IGUAL,COMP_CONTINUA
		 
;---------------------

COMP_CONTINUA:


; DECREMENTA O DIRCOMP EM 30 ( 16 BITS)

      ; DIR_COMP <- DIR_COMP - 60

         clc

         lds Acc,_DIR_COMP_L
		 ldi AccT,90
         sub Acc,AccT
         sts _DIR_COMP_L,Acc

         lds Acc,_DIR_COMP_H
		 ldi AccT,0
		 sbc Acc,AccT
         sts _DIR_COMP_H,Acc

         ;*****************
         brcc COMP2_DIR_H
         ;*****************
         ;Se deu carry, a subtracao deu < 0

         _SET _FLAG_MENOR_ZERO

         clc

         ; Complementa 1 em 16 bits

        /* ldi Acc,low(0xffff)
         lds AccT,_DIR_COMP_L
		 sub Acc,AccT
         sts _DIR_COMP_L,Acc

         ldi Acc,high(0xffff)
         lds AccT,_DIR_COMP_H
		 sbc Acc,AccT
         sts _DIR_COMP_H,Acc
         */
		 ldi Acc,low(0xffff)
		 ldi AccH,high(0xffff)

		 lds AccT,_DIR_COMP_L
		 lds AccTH,_DIR_COMP_H

		 sub Acc,AccT
		 sbc AccH,AccTH

		 sts _DIR_COMP_L,Acc
		 sts _DIR_COMP_H,AccH
		 		 
		       
        ;-- Guarda Diferenca

         lds Acc,_DIR_COMP_L
         sts _ACIMA_360,Acc

        ;----
         clc

        /* ldi Acc,low(360)
         lds AccT,_DIR_COMP_L
		 sub Acc,AccT
         sts _DIR_COMP_L,Acc

         ldi Acc,high(360)
         lds AccT,_DIR_COMP_H
		 sbc Acc,AccT
         sts _DIR_COMP_H,Acc
		 */
		 
		 lds AccT,_DIR_COMP_L
		 lds AccTH,_DIR_COMP_H

		 ldi Acc,low(360)
		 ldi AccH,high(360)

		 sub Acc,AccT
		 sbc AccH,AccTH

		 sts _DIR_COMP_L,Acc
		 sts _DIR_COMP_H,AccH
		 
COMP2_DIR_H:

         lds Acc,_DIRECAO_H
         lds AccT,_DIR_COMP_H

         cp Acc,AccT

         breq COMP2_DIR_L
         brsh COMP_OK
         brlo COMP_ERRO

COMP2_DIR_L:

         lds Acc,_DIRECAO_L
         lds AccT,_DIR_COMP_L

         cp Acc,AccT

         breq COMP_OK
		 brsh COMP_OK
        ;jb FLAG_EH_MAIOR,COMP_OK
        ; brlo COMP_ERRO

COMP_ERRO:
         lds Acc,_FLAG_MENOR_ZERO
		 cpi Acc,_ON
         brne COMP_ERRO2

         ; Agora testa a sobra que vem depois do 360

         clc
         ldi Acc,90
         lds AccT,_ACIMA_360
		 sub Acc,AccT
         sts _ACIMA_360,Acc

         lds Acc,_DIRECAO_L
         lds AccT,_ACIMA_360

         cp Acc,AccT

         breq COMP_OK
         brlo COMP_OK

         ;jb FLAG_EH_MAIOR,COMP_ERRO2

COMP_ERRO2:
         _SET _FLAG_DIRECAO_ERRO

         rjmp COMP_DIR_SAIDA

COMP_OK:
         _CLEAR _FLAG_DIRECAO_ERRO

COMP_DIR_SAIDA:

         _CLEAR _FLAG_MENOR_ZERO

         RET

comp_erro23:

call _buzzer_on

	   jmp pc






