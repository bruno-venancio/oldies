;
;          mov a,LATITUDE_H
;          mov b,WAYPOINT_H
;

_TESTA:
         clr C

         cjne a,b,TESTA_RES
         ; se for igual
         setb FLAG_EH_IGUAL
         clr  FLAG_EH_MAIOR
         clr  FLAG_EH_MENOR
         sjmp TESTA_SAIDA

TESTA_RES:
         jc TESTA_MENOR   ; Se for menor, pula

         clr  FLAG_EH_IGUAL
         setb FLAG_EH_MAIOR
         clr  FLAG_EH_MENOR

         sjmp TESTA_SAIDA

TESTA_MENOR:
         clr  FLAG_EH_IGUAL
         clr  FLAG_EH_MAIOR
         setb FLAG_EH_MENOR

TESTA_SAIDA:

         ret
         
         
         
         
;
;          mov b,OPERANDO1 LOW
;          mov a,OPERANDO1 HIGH
;
;          mov R5,OPERANDO2 LOW
;          mov R6,OPERANDO2 HIGH
;

_TESTA16:
         clr C

         mov VAR_GERAL,R6
         cjne a,VAR_GERAL,TESTA16_RES

         mov a,b
         mov VAR_GERAL,R5
         cjne a,VAR_GERAL,TESTA16_RES

         ; se for igual

         setb FLAG_EH_IGUAL
         clr  FLAG_EH_MAIOR
         clr  FLAG_EH_MENOR
         sjmp TESTA16_SAIDA

TESTA16_RES:
         jc TESTA16_MENOR   ; Se for menor, pula

         clr  FLAG_EH_IGUAL
         setb FLAG_EH_MAIOR
         clr  FLAG_EH_MENOR

         sjmp TESTA16_SAIDA

TESTA16_MENOR:
         clr  FLAG_EH_IGUAL
         clr  FLAG_EH_MAIOR
         setb FLAG_EH_MENOR

TESTA16_SAIDA:

         ret



