l/*
	#-----------------------------------------------------------------------------------# 
	|	 _________  _________    ___        _                 _   _						|
	|    | ___ \  \/  || ___ \  / _ \      (_)               | | (_)					|
	|    | |_/ / .  . || |_/ / / /_\ \_ __  _ _ __ ___   __ _| |_ _  ___  _ __			|
	|    | ___ \ |\/| ||  __/  |  _  | '_ \| | '_ ` _ \ / _` | __| |/ _ \| '_ \			|
	|    | |_/ / |  | || |     | | | | | | | | | | | | | (_| | |_| | (_) | | | |		|
	|    \____/\_|  |_/\_|     \_| |_/_| |_|_|_| |_| |_|\__,_|\__|_|\___/|_| |_|		|
	|																					|
	#-----------------------------------------------------------------------------------# 
*/

/*
	#-------------------------------------------------------#
	| INCLUSÃO DEIMAGENS BMP EM FORMATO BINARIO				|
	| CONVERTIDO PELO APP BITMAP TO HEX						|
	#-------------------------------------------------------#
*/

	.INCLUDE	"RESOURCES\logo_nano.inc"
	.INCLUDE	"RESOURCES\BMP_Fly_Bird_2.inc"
	.INCLUDE	"RESOURCES\BMP_Fly_Bird_3.inc"
	.INCLUDE	"RESOURCES\BMP_Fly_Bird_4.inc"
	.INCLUDE	"RESOURCES\BMP_Fly_Bird_5.inc"
	.INCLUDE	"RESOURCES\BMP_Fly_Bird_6.inc"
	.INCLUDE	"RESOURCES\BMP_Fly_Bird_7.inc"
	.INCLUDE	"RESOURCES\BMP_Fly_Bird_8.inc"
	.INCLUDE	"RESOURCES\BMP_Fly_Bird_9.inc"
	.INCLUDE	"RESOURCES\BMP_Fly_Bird_10.inc"


/*
	#-------------------------------------------------------#
	|														|
	|				MAIN ANIMATION LOOP						|
	|														|
	#-------------------------------------------------------#
*/

_DEMO_ANIMATION_BMP_TABLE:
		.DW	BMP_logo
		.DW	BMP_logo
		.DW	BMP_logo
		.DW	BMP_logo
		.DW	BMP_logo
		.DW	BMP_logo
		.DW	BMP_logo
		.DW	BMP_logo
		.DW	BMP_logo
		.DW	BMP_logo
_DEMO_ANIMATION_BMP_MSG:
	.DB	"Animation",0

_DEMO_ANIMATION_BMP:
	//
	//START LOOP
	//
	ldi		Acc,_White							;Set Write Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,2
	call	_SSD1306_Set_Text_Scale				;set Text Scale
	call	_SSD1306_Set_Display_Normal			;Set Screen Normal 
	call	_SSD1306_Cls





	//
	//draw start message
	//
	;ldiw	Z,_DEMO_ANIMATION_BMP_MSG*2			;show animation text
	;call	fn_ShowMessageCenter
	;call	fn_ShowScreenBorder					;show border
	;_m_delay_milliseconds	2000
	call	_SSD1306_Cls
	call	_SSD1306_Refresh
_DEMO_ANIMATION_BMP_START:
	ldiw	x,-130								;start coordiantes to draw
	ldiw	y,0
	ldiw	Z,_DEMO_ANIMATION_BMP_TABLE*2		;z--first frame
	ldi		Acc,0								;frame count
	//
	//MAIN LOOP
	//
_DEMO_ANIMATION_BMP_LOOP:
	push	Acc
	ldi		Acc,0xff							;Draw and Refresh a sequence of bird flying images
	call	_SSD1306_Fill_Pattern
	pushw	Z
	lpm		Acc,Z+								;Get bmp 
	lpm		AccH,Z+
	movw	ZH:ZL,AccH:Acc						;move to z 
	lsl		ZL
	rol		ZH
	call	_SSD1306_Draw_Bmp					;Draw BMP
	;call	fn_ShowPressKey
	call	_SSD1306_Refresh					;Refresh Screen
	popw	Z
	pop		Acc
	adiw	ZH:ZL,2								;next bmp
	inc		Acc
	cpi		Acc,10
	brlo	_DEMO_ANIMATION_BMP_LOOP_10			;until done
	adiw	XH:XL,25
	ldiw	Z,_DEMO_ANIMATION_BMP_TABLE*2		;Reload Animation
	ldi		Acc,0								;Animation frames
_DEMO_ANIMATION_BMP_LOOP_10:
	;call	_KEY_READ
	lbrcs	_DEMO_ANIMATION_BMP_EXIT
	adiw	XH:XL,2
	cpiw	X,150
	brlt	_DEMO_ANIMATION_BMP_LOOP
	rjmp	_DEMO_ANIMATION_BMP_START
	//
	//Exit
	//
_DEMO_ANIMATION_BMP_EXIT:
;	call	_KEY_RELEASE
	ret