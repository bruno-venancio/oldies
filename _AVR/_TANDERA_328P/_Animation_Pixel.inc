/*
	#-----------------------------------------------------------------------------------# 
	|     ______ _          _    ___        _                 _   _						|
	|     | ___ (_)        | |  / _ \      (_)               | | (_)					|
	|     | |_/ /___  _____| | / /_\ \_ __  _ _ __ ___   __ _| |_ _  ___  _ __			|
	|     |  __/| \ \/ / _ \ | |  _  | '_ \| | '_ ` _ \ / _` | __| |/ _ \| '_ \			|
	|     | |   | |>  <  __/ | | | | | | | | | | | | | | (_| | |_| | (_) | | | |		|
	|     \_|   |_/_/\_\___|_| \_| |_/_| |_|_|_| |_| |_|\__,_|\__|_|\___/|_| |_|		|
	|																					|
	#-----------------------------------------------------------------------------------# 
*/

_DEMO_ANIMATION_PIXEL_MSG1:
	.DB	"Pixels",_nc,0

_DEMO_ANIMATION_PIXEL:
	ldi		Acc,_WHITE							;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,2								;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			;Set Screen Normal 
	call	_SSD1306_Cls
	//
	//draw start message
	//
	ldiw	Z,_DEMO_ANIMATION_PIXEL_MSG1*2
	call	fn_ShowMessageCenter
	call	fn_ShowScreenBorder						;show border
	_m_delay_milliseconds	2000
	call	_SSD1306_Cls
	ldiw	x,-8								;start coords
	ldiw	y,-8
	ldiaw	0									;offset
	//
	//MAIN LOOP
	//
_DEMO_ANIMATION_PIXEL_LOOP:
	pushw	X
	pushw	Y
	call	_RND
	andi	Acc,7
	ldi		AccH,0

	add		XL,Acc
	adc		XH,AccH

	call	_RND
	andi	Acc,7
	ldi		AccH,0

	add		YL,Acc
	adc		YH,AccH
	call	_SSD1306_Draw_Pixel				;draw pixel
	popw	Y
	popw	X
	adiw	XH:XL,8							;next x
	cpiw	X,_SSD1306_LCD_WIDTH+8
	brlo	_DEMO_ANIMATION_PIXEL_LOOP		;until x>width
	call	_KEY_READ						;read key
	brcs	_DEMO_ANIMATION_PIXEL_EXIT		;exit if pressed
	ldiw	X,-8							;x=0
	adiw	YH:YL,8							;next y
	cpiw	Y,_SSD1306_LCD_HEIGHT+8			;until y>height
	brne	_DEMO_ANIMATION_PIXEL_LOOP
	push	Acc
	ldi		Acc,_WHITE						;set white operator
	call	fn_ShowPressKey					;show Press key
	pop		Acc
	call	_SSD1306_Refresh
_DEMO_ANIMATION_PIXEL_LOOP_10:
	call	_SSD1306_Cls
	rjmp	_DEMO_ANIMATION_PIXEL_LOOP
	//
	//Exit
	//
_DEMO_ANIMATION_PIXEL_EXIT:
	call	_KEY_RELEASE
	ret