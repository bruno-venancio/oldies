
	;+----------------------------------------------------------------------+
        ;| CALCULA ANGULO ENTRE DOIS PONTOS. UM CENTRAL COM COORDENADAS         |
	;| E P(CX,CY) E O OUTRO P(PX,PY)					|
	;|						 			|
	;| ENTRADA      : WorkH:Work   Center CX point				|
	;|		  Work2H:Work2L Center CY point				|
	;|		  X	     PX point					|
	;|		  Y	     PY point					|
	;| SAIDA        : WorkH:Work   Angle in degrees				|
	;| DESTROI      : r0..r11						|
	;+----------------------------------------------------------------------+

_INT_ATN2PTS:
		;/---------------------------------------------
		;| Calcula o deltra para saber e qual quadrante esta
		;\---------------------------------------------

                lcall LOAD_DELTA_XY


                mov Work,#0     ; Center X
                mov WorkH,#0

                mov Work2L,#0   ; Center Y
                mov Work2H,#0

                mov     a,XML
                jb     acc.7,_INT_ATN2PTS20            ;X>=0? if no, branch to 2o or 3o quadrant

                mov     a,YML
                jb     acc.7,_INT_ATN2PTS10            ;y>=0? if no, branch to 4o quadrant

		;/---------------------------------------------
		;| 1o quadrante retorna  atn(y/x)
		;\---------------------------------------------
		lcall	_INT_ATN_XY			;Calcula atn(y/x)
		ret
		;/---------------------------------------------
		;| 4o quadrante retorn 360-atn(y/x)
		;\---------------------------------------------
_INT_ATN2PTS10:
		lcall	_INT_ATN_XY			;Calcula 360+atn(y/x)

                clr C
                mov     a,Work
                subb    a,#low(-360)
                mov     Work,a

                mov     a,WorkH
                subb    a,#high(-360)
                mov     WorkH,a

		ret

_INT_ATN2PTS20:
		;/---------------------------------------------
		;| 2o and 3o  quadrante retorna 180 + atn(y/x)
		;\---------------------------------------------
		lcall	_INT_ATN_XY			;Calcula 180+atn(y/x)

                clr C
                mov     a,Work
                subb    a,#low(-180)
                mov     Work,a

                mov     a,WorkH
                subb    a,#high(-180)
                mov     WorkH,a

		ret




	;+----------------------------------------------------------------------+
        ;| Calcula Arco tangente entre valores Y/X                              | 
	;|									|
	;| ENTRADA      : X	     value					|
	;|		  Y	     value					|
	;| SAIDA        : WorkH:Work   atn(Y/X)	angle -90..0..90		|
	;| DESTROI      : r0..r11						|
	;+----------------------------------------------------------------------+
_INT_ATN_XY:

		mov	Work,XML			;get X signal

                mov     a,Work
                xrl     a,YML
                mov     Work,a  			;multiply signals

                anl     a,#80h          		;remain only signal multiply
                mov     Work,a

		push	Work				;save signal

		mov	r0,#0
                mov     a,#0
		
                cjne a,XL,_INT_ATN_XYM10               ;X=0? , If no, Branch
                cjne a,XML,_INT_ATN_XYM10

		mov	Work,#90			;else 90 degree
		ljmp	_INT_ATN_XY35
_INT_ATN_XYM10:

                mov a,#0

                cjne a,YL,_INT_ATN_XYM20               ;Y=0? , If no, Branch
                cjne a,YML,_INT_ATN_XYM20

		mov     Work,#0				;else 0 degree
		ljmp	_INT_ATN_XY35
_INT_ATN_XYM20:

                mov     a,XML                            ;X positive?
                jnb     acc.7,_INT_ATN_XY00             ;yes, branch

                mov     a,XL                            ;else X=-X
                cpl     a
                mov     XL,a

                mov     a,XML
                cpl     a
                mov     XML,a

                clr     c
                mov     a,XL
                subb    a,#-1
                mov     XL,a

                mov     a,XML
                subb    a,#-1
                mov     XML,a

_INT_ATN_XY00:
                mov     a,YML                            ;Y positive?
                jnb     acc.7,_INT_ATN_XY10             ;yes, branch

                mov     a,YL                            ;else Y=-Y
                cpl     a
                mov     YL,a

                mov     a,YML
                cpl     a
                mov     YML,a

                clr     c
                mov     a,YL
                subb    a,#-1
                mov     YL,a

                mov     a,YML
                subb    a,#-1
                mov     YML,a

_INT_ATN_XY10:
		mov	r0,XL				;mov X and Y to appropriate registers assume divident X
		mov	r1,XML
		mov	r2,YL
		mov	r3,YML
		mov	WorkH,#_ON			;assume data inverted


                mov a,XML                                ;X>Y ?
                mov b,XL

                mov R5,YL
                mov R6,YML

                lcall _testa16

               	jb  FLAG_EH_MAIOR,_INT_ATN_XY15       	;yes, branch
               	jb  FLAG_EH_IGUAL,_INT_ATN_XY15

                mov     WorkH,#_OFF                     ;else data not inverted
		mov	r0,YL				;mov X and Y to appropriate registers
		mov	r1,YML
		mov	r2,XL
		mov	r3,XML
_INT_ATN_XY15:
		lcall	_DIV_U16U16U16F16		;divide fractionary Y/X
		mov     a,R0
		mov     R4,a

		mov     a,R1
		mov     R5,a

		mov     a,R2            		;move integer to r7:r6
		mov     R6,a

		mov     a,R3
		mov     R7,a

		mov     dptr,#_INTF_ATN_TABLE
		mov	Work,#45			;start angle

_INT_ATN_XY20:
	        clr a
        	movc a,@a+dptr 	;get data from atn table and put into R11:R10:R9:R8
		mov	R8,a

		inc  dptr

                clr a
                movc a,@a+dptr
		mov	R9,a
                inc dptr

                clr a
                movc a,@a+dptr
		mov	R10,a
                inc dptr

                clr a
                movc a,@a+dptr
		mov	R11,a

                inc dptr

                clr C                   	;compare computed=table(i)
                mov    a,R4
                subb   a,R8

                mov    a,R5
                subb   a,R9

                mov    a,R6
                subb   a,R10

                mov    a,R7
                subb   a,R11

                jc  _INT_ATN_XY30       		;yes, end

		inc	Work				;next value

		mov a,Work
		cjne a,#90,_INT_ATN_XY20        	;90o degree ? no, until reatch

_INT_ATN_XY30:
                mov a,WorkH
                cjne a,#_ON,_INT_ATN_XY35               ;data inverted ? no, branch

		mov	WorkH,#90			;else invert get value
                clr C
                mov a,WorkH
                subb a,Work
                mov Work,a        			;then put into correctly register
_INT_ATN_XY35:
                pop     00H                             ;restore multiply sinal
		mov	WorkH,#0			;assume high part 0

		mov a,r0				;plus?
		jnb acc.7,_INT_ATN_XY40 		;yes, brach

		mov a,Work  				;else turn result negative
		cpl a
		add a,#1
		mov Work,a

                mov a,Work
                jnb acc.7,_INT_ATN_XY40

		mov	WorkH,#-1			;expand negative signal
_INT_ATN_XY40:

		ret

	;+----------------------------------------------+
	;| Tabela de arco tangente  46o ate 89o  	|
	;| esta formatada em 16bits(parte inteira) e	|
	;| 16bits (parte fracionaria)			|
	;+----------------------------------------------+

_INTF_ATN_TABLE:
		;  FFL  FFH  IIL  IIH
		DB 019H,009H,001H,000H ; 46o=10919
		DB 087H,012H,001H,000H ; 47o=11287
		DB 051H,01CH,001H,000H ; 48o=11C51
		DB 07FH,026H,001H,000H ; 49o=1267F
		DB 017H,031H,001H,000H ; 50o=13117
		DB 022H,03CH,001H,000H ; 51o=13C22
		DB 0AAH,047H,001H,000H ; 52o=147AA
		DB 0B9H,053H,001H,000H ; 53o=153B9
		DB 05BH,060H,001H,000H ; 54o=1605B
		DB 09BH,06DH,001H,000H ; 55o=16D9B
		DB 089H,07BH,001H,000H ; 56o=17B89
		DB 035H,08AH,001H,000H ; 57o=18A35
		DB 0B0H,099H,001H,000H ; 58o=199B0
		DB 00EH,0AAH,001H,000H ; 59o=1AA0E
		DB 068H,0BBH,001H,000H ; 60o=1BB68
		DB 0D6H,0CDH,001H,000H ; 61o=1CDD6
		DB 077H,0E1H,001H,000H ; 62o=1E177
		DB 06EH,0F6H,001H,000H ; 63o=1F66E
		DB 0E1H,00CH,002H,000H ; 64o=20CE1
		DB 0FEH,024H,002H,000H ; 65o=224FE
		DB 0FCH,03EH,002H,000H ; 66o=23EFC
		DB 019H,05BH,002H,000H ; 67o=25B19
		DB 09FH,079H,002H,000H ; 68o=2799F
		DB 0E7H,09AH,002H,000H ; 69o=29AE7
		DB 05BH,0BFH,002H,000H ; 70o=2BF5B
		DB 07AH,0E7H,002H,000H ; 71o=2E77A
		DB 0E3H,013H,003H,000H ; 72o=313E3
		DB 057H,045H,003H,000H ; 73o=34557
		DB 0C7H,07CH,003H,000H ; 74o=37CC7
		DB 068H,0BBH,003H,000H ; 75o=3BB68
		DB 0C3H,002H,004H,000H ; 76o=402C3
		DB 0DCH,054H,004H,000H ; 77o=454DC
		DB 063H,0B4H,004H,000H ; 78o=4B463
		DB 001H,025H,005H,000H ; 79o=52501
		DB 0D9H,0ABH,005H,000H ; 80o=5ABD9
		DB 052H,050H,006H,000H ; 81o=65052
		DB 089H,01DH,007H,000H ; 82o=71D89
		DB 0F4H,024H,008H,000H ; 83o=824F4
		DB 0ADH,083H,009H,000H ; 84o=983AD
		DB 018H,06EH,00BH,000H ; 85o=B6E18
		DB 0F8H,04CH,00EH,000H ; 86o=E4CF8
		DB 0C5H,014H,013H,000H ; 87o=1314C5
		DB 0E1H,0A2H,01CH,000H ; 88o=1CA2E1
		DB 03BH,04AH,039H,000H ; 89o=394A3B
		
		
		



