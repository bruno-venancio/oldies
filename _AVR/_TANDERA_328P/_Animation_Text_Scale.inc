/*
	#-----------------------------------------------------------------------------------# 
	|																					|
	|                   _____         _     _____           _							|
	|                  |_   _|       | |   /  ___|         | |							|
	|                    | | _____  _| |_  \ `--.  ___ __ _| | ___						|
	|                    | |/ _ \ \/ / __|  `--. \/ __/ _` | |/ _ \						|
	|                    | |  __/>  <| |_  /\__/ / (_| (_| | |  __/						|
	|                    \_/\___/_/\_\\__| \____/ \___\__,_|_|\___|						|
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

_DEMO_ANIMATION_TEXT_SCALE_MSG1:
	.DB	"Text Scale",_nc,0
_DEMO_ANIMATION_TEXT_SCALE_MSG2:
	.DB	"Ab",_nc,0
_DEMO_ANIMATION_TEXT_SCALE_MSG3:
	.DB	"SCALE",0

_DEMO_ANIMATION_TEXT_SCALE:
	ldi		Acc,_WHITE							;Set White Logical Mode
	call	_SSD1306_Set_Operator			
	ldi		Acc,2								;Set Scale to 2
	call	_SSD1306_Set_Text_Scale			
	call	_SSD1306_Set_Display_Normal			;Set Screen Normal 
	call	_SSD1306_Cls
	//
	//draw start message
	//
	ldiw	Z,_DEMO_ANIMATION_TEXT_SCALE_MSG1*2
	call	fn_ShowMessageCenter
	call	fn_ShowScreenBorder						;show border
	_m_delay_milliseconds	2000
	call	_SSD1306_Cls
_DEMO_ANIMATION_TEXT_SCALE_START:
	ldi		Acc,1									;Scale  start =1 
	//
	//MAIN LOOP
	//
_DEMO_ANIMATION_TEXT_SCALE_LOOP:	
	call	_SSD1306_Set_Text_Scale				;set new scale
	call	_SSD1306_Cls						;Clear screen
	//Draw scale text
	ldiw	x,0									;Put Char 
	ldiw	y,0
	call	_SSD1306_Locate						;locate at start screen
	ldiw	Z,_DEMO_ANIMATION_TEXT_SCALE_MSG2*2	;Ab message
	call	_SSD1306_PutSF						;draw message
	//Draw Scale
	push	Acc
	ldi		Acc,1
	call	_SSD1306_Set_Text_Scale				;set scale to 1
	ldiw	Z,_DEMO_ANIMATION_TEXT_SCALE_MSG3*2	;Ab message
	call	_SSD1306_Get_Text_Width				;get this text width
	ldiw	X,_SSD1306_LCD_WIDTH				;width - text pos
	sub		XL,Acc
	sbc		XH,AccH
	ldiw	y,0
	call	_SSD1306_Locate						;locate at start screen
	call	_SSD1306_PutSF						;draw message
	//Draw Scale Number
	ldi		Acc,3
	call	_SSD1306_Set_Text_Scale				;set scale to 3
	ldiw	x,106
	ldiw	y,16
	call	_SSD1306_Locate						;locate at start screen
	pop		Acc									;restore scale
	push	Acc									;save again
	ldi		AccH,0
	call	_itos								;convert i to string
	call	_SSD1306_PutS						;draw Scale Value
	ldi		Acc,2
	call	_SSD1306_Set_Text_Scale				;set scale to 2
	call	fn_ShowPressKey						;show press key
	pop		Acc
	//Refresh screen
	call	_SSD1306_Refresh
	//CHECK IF KEY PRESSED
	pushaw
	ldiaw	500
_DEMO_ANIMATION_TEXT_SCALE_KEY_CHECK:
	call	_KEY_READ							;key pressed
	brcs	_DEMO_ANIMATION_TEXT_SCALE_EXIT		;yes, exit
	_m_delay_milliseconds	1					;wait a 1 millisecond
	sbiw	AccH:Acc,1							;a sec pass?
	brne	_DEMO_ANIMATION_TEXT_SCALE_KEY_CHECK	;no, wait
	popaw
	//PUMP SCALE
	inc		Acc									;new scale
	cpi		Acc,8								;max 8 in this demo
	lbrne	_DEMO_ANIMATION_TEXT_SCALE_LOOP
	rjmp	_DEMO_ANIMATION_TEXT_SCALE_START
	//
	//Exit
	//
_DEMO_ANIMATION_TEXT_SCALE_EXIT:
	popaw
	call	_KEY_RELEASE
	ret