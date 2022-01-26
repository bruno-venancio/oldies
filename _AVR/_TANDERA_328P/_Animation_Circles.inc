/*
	#-----------------------------------------------------------------------------------# 
	|                          _____ _          _										|
	|                         /  __ (_)        | |										|
	|                         | /  \/_ _ __ ___| | ___  ___								|
	|                         | |   | | '__/ __| |/ _ \/ __|							|
	|                         | \__/\ | | | (__| |  __/\__ \							|
	|                          \____/_|_|  \___|_|\___||___/							|
	|																					|
	|																					|
	|                 ___        _                 _   _								|
	|                / _ \      (_)               | | (_)								|
	|               / /_\ \_ __  _ _ __ ___   __ _| |_ _  ___  _ __						|
	|               |  _  | '_ \| | '_ ` _ \ / _` | __| |/ _ \| '_ \					|
	|               | | | | | | | | | | | | | (_| | |_| | (_) | | | |					|
	|               \_| |_/_| |_|_|_| |_| |_|\__,_|\__|_|\___/|_| |_|					|
	|																					|
	#-----------------------------------------------------------------------------------# 
*/

_DEMO_ANIMATION_CIRCLES_MSG1:
	.DB	"Circles",0

_DEMO_ANIMATION_CIRCLES:
	ldi		Acc,_WHITE							;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,2								;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			;Set Screen Normal 
	call	_SSD1306_Cls
	//
	//draw start message
	//
	ldiw	Z,_DEMO_ANIMATION_CIRCLES_MSG1*2
	call	fn_ShowMessageCenter
	call	fn_ShowScreenBorder						;show border
	_m_delay_milliseconds	2000
	call	_SSD1306_Cls
	]
	ldi		Acc,_INVERT							;Set invert mode
	call	_SSD1306_Set_Operator	
_DEMO_ANIMATION_CIRCLES_START:			
	ldiw	x,_SSD1306_LCD_WIDTH/2				;circle center
	ldiw	y,_SSD1306_LCD_HEIGHT/2
	ldiawt	1									;radius=1
	//
	//MAIN LOOP
	//
_DEMO_ANIMATION_CIRCLES_LOOP:	
	call	_SSD1306_Draw_Circle				;draw Circle
	push	Acc
	ldi		Acc,_WHITE							;white operator
	call	fn_ShowPressKey						;show Presskey
	pop		Acc
	call	_SSD1306_Refresh
	call	_KEY_READ							;read key
	brcs	_DEMO_ANIMATION_CIRCLES_EXIT		;exit if key pressed
	addiawt	4
	cpiawt	_SSD1306_LCD_WIDTH					;until x>width
	brlo	_DEMO_ANIMATION_CIRCLES_LOOP
	rjmp	_DEMO_ANIMATION_CIRCLES_START
	//
	//Exit
	//
_DEMO_ANIMATION_CIRCLES_EXIT:
	call	_KEY_RELEASE
	ret