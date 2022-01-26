/*
	#-----------------------------------------------------------------------------------# 
	|                    ______															|
	|                    | ___ \														|
	|                    | |_/ / __ ___   __ _ _ __ ___  ___ ___						|
	|                    |  __/ '__/ _ \ / _` | '__/ _ \/ __/ __|						|
	|                    | |  | | | (_) | (_| | | |  __/\__ \__ \						|
	|                    \_|  |_|  \___/ \__, |_|  \___||___/___/						|
	|                                     __/ |											|
	|                                    |___/											|
	|                 ___        _                 _   _								|
	|                / _ \      (_)               | | (_)								|
	|               / /_\ \_ __  _ _ __ ___   __ _| |_ _  ___  _ __						|
	|               |  _  | '_ \| | '_ ` _ \ / _` | __| |/ _ \| '_ \					|
	|               | | | | | | | | | | | | | (_| | |_| | (_) | | | |					|
	|               \_| |_/_| |_|_|_| |_| |_|\__,_|\__|_|\___/|_| |_|					|
	|																					|
	#-----------------------------------------------------------------------------------# 
*/

_DEMO_ANIMATION_PROGRESS_BAR_MSG1:
	.DB	"Progress",_nc,0
_DEMO_ANIMATION_PROGRESS_BAR_MSG2:
	.DB	"Bar",0

_DEMO_ANIMATION_PROGRESS_BAR:
	ldi		Acc,_WHITE							;Set White Logical Mode
	call	_SSD1306_Set_Operator	 		
	ldi		Acc,2								;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			;Set Screen Normal 
	call	_SSD1306_Cls
	//
	//draw start message
	//
	ldiw	Z,_DEMO_ANIMATION_PROGRESS_BAR_MSG1*2	;Progress
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,_SSD1306_LCD_HEIGHT/2-_SSD1306_FONT_HEIGHT*2
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF

	ldiw	Z,_DEMO_ANIMATION_PROGRESS_BAR_MSG2*2	;Bar
	call	_SSD1306_Get_Text_Center_X				;compute center x and y/2-height
	movw	XH:XL,AccH:Acc
	ldiw	Y,_SSD1306_LCD_HEIGHT/2
	call	_SSD1306_Locate							;locate msg
	call	_SSD1306_PutSF
	call	fn_ShowScreenBorder						;show border
	#ifndef	__TEST_MODE__
		_m_delay_milliseconds	2000
	#endif
	call	_SSD1306_Cls
_DEMO_ANIMATION_PROGRESS_BAR_START:
	ldiw	x,8
	ldiw	y,32
	call	_SSD1306_Set_Progress_Bar_Coords		;bar start coords
	ldiw	x,_SSD1306_LCD_WIDTH-8*2
	ldiw	y,32
	call	_SSD1306_Set_Progress_Bar_Sizes			;Bar Sizes
	ldiw	x,0										
	ldiw	y,100
	call	_SSD1306_Set_Progress_Bar_Range			;Bar Range
	ldiaw	0
	ldiawt	1
	//
	//MAIN LOOP
	//
_DEMO_ANIMATION_PROGRESS_BAR_LOOP:	
	call	_SSD1306_Cls							;Clear Screen
	call	_SSD1306_Set_Progress_Bar_Value			;set value
	call	_SSD1306_Draw_Progress_Bar				;draw bar
	ldiw	x,55
	ldiw	y,8
	call	_SSD1306_Locate							;set value location
	call	_ITOS									;convert to string
	call	_SSD1306_PutS							;put string
	push	Acc
	ldi		Acc,'%'
	call	_SSD1306_PutC							;show %
	pop		Acc
	call	_SSD1306_Refresh						;Refresh screen
	call	_Key_Read								;read key
	brcs	_DEMO_ANIMATION_PROGRESS_BAR_EXIT		;Exit if Pressed
	add		Acc,AccT								;value=k
	adc		AccH,AccTH
	cpiaw	100
	brlt	_DEMO_ANIMATION_PROGRESS_BAR_LOOP_10
	ldiaw	100
	negawt
	rjmp	_DEMO_ANIMATION_PROGRESS_BAR_LOOP
_DEMO_ANIMATION_PROGRESS_BAR_LOOP_10:
	cpiaw	0
	brge	_DEMO_ANIMATION_PROGRESS_BAR_LOOP
	ldiaw	0
	negawt
	rjmp	_DEMO_ANIMATION_PROGRESS_BAR_LOOP
	//
	//Exit
	//
_DEMO_ANIMATION_PROGRESS_BAR_EXIT:
	call	_KEY_RELEASE
	ret
