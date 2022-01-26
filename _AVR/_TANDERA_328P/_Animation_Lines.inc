/*
	#-----------------------------------------------------------------------------------# 
	|   _     _                    ___        _                 _   _					|
	|  | |   (_)                  / _ \      (_)               | | (_)					|
	|  | |    _ _ __   ___  ___  / /_\ \_ __  _ _ __ ___   __ _| |_ _  ___  _ __		|
	|  | |   | | '_ \ / _ \/ __| |  _  | '_ \| | '_ ` _ \ / _` | __| |/ _ \| '_ \		|
	|  | |___| | | | |  __/\__ \ | | | | | | | | | | | | | (_| | |_| | (_) | | | |		|
	|  \_____/_|_| |_|\___||___/ \_| |_/_| |_|_|_| |_| |_|\__,_|\__|_|\___/|_| |_|		|
	|																					|
	#-----------------------------------------------------------------------------------# 
*/

_DEMO_ANIMATION_LINES_MSG1:
	.DB	"Lines",0

_DEMO_ANIMATION_LINES:
	ldi		Acc,_WHITE							;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,2								;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			;Set Screen Normal 
	call	_SSD1306_Cls
	//
	//draw start message
	//
	ldiw	Z,_DEMO_ANIMATION_LINES_MSG1*2
	call	fn_ShowMessageCenter
	call	fn_ShowScreenBorder						;show border
	_m_delay_milliseconds	2000
	call	_SSD1306_Cls
	]
	ldi		Acc,_INVERT							;Set invert mode
	call	_SSD1306_Set_Operator	
_DEMO_ANIMATION_LINES_START:			
	ldiaw	0
	ldiawt	0
	ldiw	x,_SSD1306_LCD_WIDTH-1
	ldiw	y,_SSD1306_LCD_HEIGHT-1
	//
	//MAIN LOOP
	//
_DEMO_ANIMATION_LINES_LOOP:	
	call	_SSD1306_Draw_Line					;draw line
	push	Acc
	ldi		Acc,_WHITE							;white operator
	call	fn_ShowPressKey						;show Presskey
	pop		Acc
	call	_SSD1306_Refresh
	call	_KEY_READ							;read key
	brcs	_DEMO_ANIMATION_LINES_EXIT			;exit if key pressed
	adiw	AccH:Acc,4
	sbiw	XH:XL,4
	cpiaw	_SSD1306_LCD_WIDTH
	brlo	_DEMO_ANIMATION_LINES_LOOP
	rjmp	_DEMO_ANIMATION_LINES_START
	//
	//Exit
	//
_DEMO_ANIMATION_LINES_EXIT:
	call	_KEY_RELEASE
	ret