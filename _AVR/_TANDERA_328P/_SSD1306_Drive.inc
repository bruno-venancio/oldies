
/*
	#-------------------------------------------------------------------------------#
	|																				|
	|      _____ ___________  __   _____  _____  ____  ______      _				|
	|     /  ___/  ___|  _  \/  | |____ ||  _  |/ ___| |  _  \    (_)				|
	|     \ `--.\ `--.| | | |`| |     / /| |/' / /___  | | | |_ __ ___   _____		|
	|      `--. \`--. \ | | | | |     \ \|  /| | ___ \ | | | | '__| \ \ / / _ \		|
	|     /\__/ /\__/ / |/ / _| |_.___/ /\ |_/ / \_/ | | |/ /| |  | |\ V /  __/		|
	|     \____/\____/|___/  \___/\____/  \___/\_____/ |___/ |_|  |_| \_/ \___|		|
	|																				|
	#-------------------------------------------------------------------------------#                                                           
	| Este Drive Implementa todas as rotinas necessárias para acesso e controle do	|
	| chip SSD1306 que é um Display Grafico para OLED 128x64 pixels de resolução	|
	#-------------------------------------------------------------------------------#
	| Este documento usa TAB 4 para formatação										|
	#-------------------------------------------------------------------------------#
	| Date: Brasilia ,23 de Setembro de 2016										|
	#-------------------------------------------------------------------------------#
	| Para usar este Drive devera ser definido pelo usuario as contantes que segue	|
	| Exemplo abaixo com PORTAS e BITS definidos aleatoriamente como exemplo		|
	|																				|
	| DEFINIÇOES DA PORTA DE COMUNICAÇÃO E BITS										|
	|																				|
	|	//SLC AND SDA PORTS AND BITS												|
	|																				|
	|	.EQU	_SSD1306_PORT_OUTPUT		= PORTD									|
	|	.EQU	_SSD1306_PORT_DIR			= DDRD									|
	|	.EQU	_SSD1306_PORT_INPUT			= PIND									|
	|																				|
	|	.EQU	_SSD1306_SLC_BIT			= 4										|
	|	.EQU	_SSD1306_SDA_BIT			= 5										|
	|	.EQU	_SSD1306_I2C_ADDRESS		= 0x3C									|
	|																				|
	#-------------------------------------------------------------------------------#
	|	FUNÇÕES IMPLEMENTADAS														|
	#-------------------------------------------------------------------------------#
	|	INICIALIZA SSD1306															|
	|																				|
	|	rcall	_SSD1306_INIT														|
	#-------------------------------------------------------------------------------#

 */

 //UNCOMMENT BELOW LINE TO USE DRAW LINE

#define	__USE_GRAPH_LINE__

//UNCOMMENT BELOW LINE TO USE DRAW CIRCLE

#define	__USE_GRAPH_CIRCLE__

 //carrega funcoes e drivers necessarios

#ifndef	__I2C__
	//.EQU  PORTAS PARA O DRIVE I2C	
	.EQU	_I2C_PORT_OUT	= _SSD1306_PORT_OUTPUT
	.EQU	_I2C_PORT_IN	= _SSD1306_PORT_INPUT
	.EQU	_I2C_PORT_DIR	= _SSD1306_PORT_DIR

	.EQU	_I2C_BIT_SCL	= _SSD1306_SLC_BIT
	.EQU	_I2C_BIT_SDA	= _SSD1306_SDA_BIT
	//INCLUI O DRIVE
	.INCLUDE "COMM\I2C\I2C.INC"
#endif
//MULTIPLY 16X16=32
#ifndef __INTEGER_MUL_S16S16S32__
	.INCLUDE "MATH\INTEGER\MUL_S16S16S32\MUL_S16S16S32.INC"
#endif
//DIVIDE 32/32=32
#ifndef __INTEGER_DIV_S32S32S32__
	.INCLUDE "MATH\INTEGER\DIV_S32S32S32\DIV_S32S32S32.INC"
#endif

//LOAD GRAPHICS LIBRARYS IF NEED
#ifdef	__USE_GRAPH_LINE__
	#ifndef __GRAPH_LINE__
		.INCLUDE "GRAPH\LINE\LINE.INC"
	#endif
#endif
#ifdef	__USE_GRAPH_CIRCLE__
	#ifndef	__GRAPH_CIRCLE__
	 	.INCLUDE "GRAPH\CIRCLE\CIRCLE.INC"
	#endif
#endif
 
//.EQU  __SSD1306_ DRIVE

#ifndef __SD1306__DRIVE__
	#define  __SSD1306__DRIVE__
	
	.message "+--------------------+"
	.message "|   _SSD1306_ DRIVE  |" 
	.message "+--------------------+"


	/*
		#-----------------------#
		| CONSTANT DEFINICTIONS	|
		#-----------------------#
	*/
		
	.EQU  _SSD1306_LCD_WIDTH					= 128
	.EQU  _SSD1306_LCD_HEIGHT					= 32

	.EQU  _SSD1306_CMD_SET_CONTRAST				= 0x81
	.EQU  _SSD1306_CMD_DISPLAY_RAM_ON			= 0xA4
	.EQU  _SSD1306_CMD_DISPLAY_RAM_OFF			= 0xA5
	.EQU  _SSD1306_CMD_DISPLAY_NORMAL			= 0xA6
	.EQU  _SSD1306_CMD_DISPLAY_INVERTED			= 0xA7
	.EQU  _SSD1306_CMD_DISPLAY_OFF				= 0xAE
	.EQU  _SSD1306_CMD_DISPLAY_ON				= 0xAF

	.EQU  _SSD1306_CMD_SET_DISPLAY_OFFSET		= 0xD3
	.EQU  _SSD1306_CMD_SET_COM_PINS				= 0xDA

	.EQU  _SSD1306_CMD_SET_VCOM_DETECT			= 0xDB

	.EQU  _SSD1306_CMD_SET_DISPLAY_CLOCK_DIV	= 0xD5
	.EQU  _SSD1306_CMD_SET_PRE_CHARGE_PERIOD	= 0xD9

	.EQU  _SSD1306_CMD_SET_MULTIPLEX			= 0xA8

	.EQU  _SSD1306_CMD_SET_LOW_COLUMN			= 0x00
	.EQU  _SSD1306_CMD_SET_HIGH_COLUMN			= 0x10

	.EQU  _SSD1306_CMD_SET_START_LINE			= 0x40

	.EQU  _SSD1306_CMD_MEMORY_ADDRESSING_MODE	= 0x20
	.EQU  _SSD1306_CMD_COLUMN_ADDRESS			= 0x21
	.EQU  _SSD1306_CMD_PAGE_ADDRESS				= 0x22

	.EQU  _SSD1306_CMD_SET_COM_SCAN_INC			= 0xC0
	.EQU  _SSD1306_CMD_SET_COM_SCAN_DEC			= 0xC8

	.EQU  _SSD1306_CMD_SET_SEG_REMAP			= 0xA0

	.EQU  _SSD1306_CMD_CHARGE_PUMP				= 0x8D

	// Scrolling .EQU s
	.EQU  _SSD1306_CMD_ACTIVATE_SCROLL			= 0x2F
	.EQU  _SSD1306_CMD_DEACTIVATE_SCROLL		= 0x2E
	.EQU  _SSD1306_CMD_SET_VERTICAL_SCROLL_AREA	= 0xA3
	.EQU  _SSD1306_CMD_RIGHT_HORIZONTAL_SCROLL	= 0x26
	.EQU  _SSD1306_CMD_LEFT_HORIZONTAL_SCROLL	= 0x27
	.EQU  _SSD1306_CMD_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL	= 0x29
	.EQU  _SSD1306_CMD_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL	= 0x2A
		
	.EQU  _SSD1306_BIT_CO	= 7
	.EQU  _SSD1306_BIT_DC	= 6


	.EQU	_SSD1306_TEXT_MODE_NORMAL	= 0
	.EQU	_SSD1306_TEXT_MODE_INVERTED	= 1

	.EQU	_SSD1306_OPERATOR_AND		= 0
	.EQU	_SSD1306_OPERATOR_BLACK		= 0
	.EQU	_BLACK						= 0

	.EQU	_SSD1306_OPERATOR_OR		= 1
	.EQU	_SSD1306_OPERATOR_WHITE		= 1	
	.EQU	_WHITE						= 1

	.EQU	_SSD1306_OPERATOR_XOR		= 3
	.EQU	_SSD1306_OPERATOR_INVERT	= 3
	.EQU	_INVERT						= 3
	
	.EQU	_SSD1306_OPERATOR_SET		= 4
	
	/*
		#-----------------------#
		| VARIABLES DEFINICTIONS|
		#-----------------------#
	*/
	.DSEG
		_SSD1306_X:					.BYTE	_WORD
		_SSD1306_Y:					.BYTE	_WORD
		_SSD1306_TEXT_SCALE:		.BYTE	_BYTE
		_SSD1306_TEXT_MODE:			.BYTE	_BYTE
		_SSD1306_OPERATOR:			.BYTE	_BYTE
	.CSEG


	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|  COMPUTE SCALE					|
	;|  								|
	;|\									|
	;| | Input  : R3:R2 Vector			|
	;| |		  R4	Scale			|
	;|/  Output : R7:R6 Value Scaled	|
	;|   Destroy: Flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________	     
	_SSD1306_Compute_Scale:
		push	r0
		push	r1
		mul		r4,r2
		movw	r7:r6,r1:r0
		mul		r4,r3
		add		r7,r0
		pop		r1
		pop		r0
		ret
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   INICIALIZA __SSD1306_			|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : cy=0 if Init ok		|
	;|			  cy=1 if Init error	|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;				/
	;				| Obs:  Durante a Inicialização deste Drive as interrupções serão
	;				|		desabilitadas e habilitadas no final da rotina
	;				\_______________________________________________________________________
	_SSD1306_INIT:
		push	Acc								;save Acc
		in		Acc,SREG						;save SREG
		pushaw
		pushawT
		pushw	X
		pushw	Y
		cli										;disable interrupts
		//.EQU  AD0 (LOW) THAT MEANS I2C ADDRESS = 0B1101000 IN HEX = 0X68
		clc										;set AD0=0
		//INITIALIZE I2C LINE
		call	_I2C_INIT
		//INITIALIZE DISPLAY
		#ifndef __TEST_MODE__
			call	_SSD1306_Display_Initialize
			brcs	_SSD1306_INIT_ERROR				;branch if error
		#endif
		
		#ifdef __USE_GRAPH_LINE__
			//set pixel routine for line
			ldiaw	_SSD1306_Draw_Pixel				;address of set pixel routine
			call	_GRAPH_LINE_SET_PIXEL_ADDR
		#endif
		#ifdef __USE_GRAPH_CIRCLE__
			//set pixel routine for circle
			ldiaw	_SSD1306_Draw_Pixel				;address of set pixel routine
			call	_GRAPH_CIRCLE_SET_PIXEL_ADDR
		#endif
		//Set Scale
		ldi		Acc,1
		sts		(_SSD1306_Text_Scale),Acc
		//set Operotor
		ldi		Acc,_SSD1306_OPERATOR_WHITE
		sts		(_SSD1306_OPERATOR),Acc
		//Set Progress Bar Default Values
		ldiaw	0
		call	_SSD1306_Set_Progress_Bar_Value			;value=0
		ldiw	x,0
		ldiw	y,0
		call	_SSD1306_Set_Progress_Bar_Coords		;x,y=0
		ldiw	x,63
		ldiw	y,31
		call	_SSD1306_Set_Progress_Bar_Sizes			;width=64;height=32
		ldiw	X,0
		ldiw	Y,100
		call	_SSD1306_Set_Progress_Bar_Range			;MinValue=0;MaxValue=100
		//Restore SREG & Acc
		popw	Y
		popw	X
		popawt
		popaw									;Restore SREG
		out		SREG,Acc
		pop		Acc								;restore Acc
		clc
		ret
	_SSD1306_INIT_ERROR:
		popw	Y
		popw	X
		popawt
		popaw									;Restore SREG
		out		SREG,Acc
		pop		Acc								;restore Acc
		sec
		ret

	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Set_Text_Scale			|
	;|  								|
	;|\									|
	;| | Input  : Acc	Scale			|
	;|/  Output : None					|
	;|   Destroy:						|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Set_Text_Scale:
		sts	(_SSD1306_Text_Scale),Acc
		ret
	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Set_Display_Normal		|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy: cy=1 if error			|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Set_Display_Normal:
		push	Acc
		ldi		Acc,_SSD1306_CMD_DISPLAY_NORMAL
		call	_SSD1306_Command_Write
		pop		Acc
		ret
	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Set_Display_Inverted	|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy: cy=1 if error			|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Set_Display_Inverted:
		push	Acc
		ldi		Acc,_SSD1306_CMD_DISPLAY_INVERTED
		call	_SSD1306_Command_Write
		pop		Acc
		ret

	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Set_Logical_Operator	|
	;|  								|
	;|\									|
	;| | Input  : Acc	Operator		|
	;|/  Output : None					|
	;|   Destroy:						|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Set_Operator:
		sts	(_SSD1306_OPERATOR),Acc
		ret		
	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Get_Logical_Operator	|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : Acc Operator			|
	;|   Destroy:						|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Get_Operator:
		lds		Acc,(_SSD1306_OPERATOR)
		ret		
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_SEND_DATA_OR_COMMAND	|
	;|  								|
	;|\									|
	;| | Input  : Acc	data or command	|
	;| |			    to be write		|
	;|/  Output : cy=0 if write ok		|
	;|			  cy=1 if write error	|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;		/
	;		| Device address format
	;		|
	;		|	+---+---+---+---+---+---+---+---+
	;		|	| 1 | 1 | 0 | 1 | 0 | 0 |S/L|r/w|
	;		|	+---+---+---+---+---+---+---+---+
	;		|
	;		|
	;		| a6..a0 = Device address
	;		| r/w	 = 0 (write) 1 (read)
	;		|			         write
	;		|	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	;		|	|s|6|5|4|3|2|1|0|0|A|7|6|5|4|3|2|1|0|A|7|6|5|4|3|2|1|0|s| 
	;		|	|t|a|a|a|a|a|a|S|w|C|c|D|r|r|r|r|r|r|C|d|d|d|d|d|d|d|d|t| 
	;		|	|t| | | | | | |L| |K|o|C| | | | | | |K| | | | | | | | |p| 
	;		|	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	;		|	  \- Device Addr-/ 0 \-control reg -/0 \---  data ---/  
	;		|
	;		|stt = start,r=mpu register 0..255,d=data 0..255, stp=stop
	;		|write time at 4Mhz ~490uS
	;		\_______________________________________________________________________________________

	.DSEG
		_SSD1306_Send_Command:			.byte	_BYTE
	.CSEG

	_SSD1306_Send_Data_or_Command:
		pushaw
		push	Acc								;save data
		//
		//SEND START CONDITION TO SLAVE
		//
		rcall	_I2C_START						;issue start condition
		//
		//SEND DEVICE ADDRESS TO SLAVE AND WRITE
		//
		ldi		Acc,(_SSD1306_I2C_ADDRESS<<1)	;send address
		rcall	_I2C_BYTE_OUT					;Send device address
		rcall	_I2C_ACK_IN						;get ack state
		brcs	_SSD1306_Data_Write_10			;exit, if error
		//
		//SEND CONTROL 
		//
		lds		Acc,(_SSD1306_Send_Command)		;get send command
		//
		// ISSUE COMMAND OR DATA
		//
		rcall	_I2C_BYTE_OUT					;send Register
		rcall	_I2C_ACK_IN						;get ack state
		brcs	_SSD1306_Data_Write_10			;branch if error
		pop		Acc								;restore Data
		push	Acc								;save data
		rcall	_I2C_BYTE_OUT					;send data
		rcall	_I2C_ACK_IN						;read Ack
		brcs	_SSD1306_Data_Write_10			;branch if no ack
		//
		//SEND STOP CONDITION TO SLAVE
		//
		rcall	_I2C_STOP						;issue stop condition
		pop		Acc								;restore data
		popaw
		clc										;set ok
		ret
	_SSD1306_Data_Write_10:
		rcall	_I2C_STOP						;isse stop condition
		pop		Acc								;restore data
		popaw
		sec										;set error
		ret
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_DATA_WRITE				|
	;|  								|
	;|\									|
	;| | Input  : Acc	data to be write|
	;|/  Output : cy=0 if write ok		|
	;|			  cy=1 if write error	|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Data_Write:
		push	Acc
		ldi		Acc,(0<<_SSD1306_BIT_CO)|(1<<_SSD1306_BIT_DC)			;Set command to be write co=0 D/C=1
		sts		(_SSD1306_Send_Command),Acc
		pop		Acc
		call	_SSD1306_Send_Data_Or_Command
		ret
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_COMMAND_WRITE			|
	;|  								|
	;|\									|
	;| | Input  : Acc command to be write|
	;|/  Output : cy=0 if write ok		|
	;|			  cy=1 if write error	|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Command_Write:
		push	Acc
		ldi		Acc,(0<<_SSD1306_BIT_CO)|(0<<_SSD1306_BIT_DC)		;Set data to be write co=0 D/C=0
		sts		(_SSD1306_Send_Command),Acc
		pop		Acc
		call	_SSD1306_Send_Data_Or_Command
		ret
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_DISPLAY_INITIALIZE		|
	;|  								|
	;|\									|
	;| | Input  : none					|
	;|/  Output : cy=0 if Init ok		|
	;|			  cy=1 if Init error	|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Display_Initialize:
		push	Acc
		ldi		Acc,_SSD1306_CMD_DISPLAY_OFF
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,_SSD1306_CMD_SET_DISPLAY_CLOCK_DIV
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,0x80								//ratio 80
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,_SSD1306_CMD_SET_MULTIPLEX
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,0x1f
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,_SSD1306_CMD_SET_DISPLAY_OFFSET
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,0
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,_SSD1306_CMD_SET_START_LINE | 0X00
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,_SSD1306_CMD_CHARGE_PUMP
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,0x14
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,_SSD1306_CMD_MEMORY_ADDRESSING_MODE | 0X00
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,0
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,_SSD1306_CMD_SET_SEG_REMAP | 0X01
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,_SSD1306_CMD_SET_COM_SCAN_DEC
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,_SSD1306_CMD_SET_COM_PINS
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,0x02
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,_SSD1306_CMD_SET_CONTRAST
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,0x8F				//0x9f or 0xcc
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,_SSD1306_CMD_SET_PRE_CHARGE_PERIOD
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,0xf1		//0x22 or 0xF1
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,_SSD1306_CMD_SET_VCOM_DETECT
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,0x40
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,_SSD1306_CMD_DISPLAY_RAM_ON
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,_SSD1306_CMD_DISPLAY_NORMAL
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		ldi		Acc,_SSD1306_CMD_DISPLAY_ON
		call	_SSD1306_Command_Write
		lbrcs	_SSD1306_Display_Initialize_Error		//exit if error
		clc								//set Initialization Ok
		pop		Acc
		ret
	_SSD1306_Display_Initialize_Error:
		sec
		pop		Acc
		ret
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_REFRESH				|
	;|  								|
	;|\									|
	;| | Input  :						|
	;|/  Output : cy=0 if write ok		|
	;|			  cy=1 if write error	|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	| All data in SS1306_DISPLAY_BUFFER IS SENT TO DISPLAY
	;	|
	;	| Esta função deve ser chamada sempre que se desejar atualizar ou visualizar
	;	| as informações gráficas que estão no display_buffer
	;	\__________________________________________________________________________________

	;.DSEG
	;	_SSD1306_DISPLAY_BUFFER:	.BYTE	_SSD1306_LCD_WIDTH*_SSD1306_LCD_HEIGHT/8
	.CSEG

	_SSD1306_Refresh:
		pushaw
		pushw	X
		pushw	Z
		ldi		Acc,_SSD1306_CMD_COLUMN_ADDRESS
		call	_SSD1306_Command_Write
		brcs	_SSD1306_Refresh_Error				;exit if error
		ldi		Acc,0
		call	_SSD1306_Command_Write
		brcs	_SSD1306_Refresh_Error				;exit if error
		ldi		Acc,127
		call	_SSD1306_Command_Write
		brcs	_SSD1306_Refresh_Error				;exit if error
		ldi		Acc,_SSD1306_CMD_PAGE_ADDRESS
		call	_SSD1306_Command_Write
		brcs	_SSD1306_Refresh_Error				;exit if error
		ldi		Acc,0
		call	_SSD1306_Command_Write
		brcs	_SSD1306_Refresh_Error				;exit if error
		ldi		Acc,7
		call	_SSD1306_Command_Write
		brcs	_SSD1306_Refresh_Error				;exit if error
		//
		//SEND START CONDITION TO SLAVE
		//
		rcall	_I2C_START						;issue start condition
		//
		//SEND DEVICE ADDRESS TO SLAVE AND WRITE
		//
		ldi		Acc,(_SSD1306_I2C_ADDRESS<<1)	;send address
		rcall	_I2C_BYTE_OUT					;Send device address
		rcall	_I2C_ACK_IN						;get ack state
		brcs	_SSD1306_Refresh_Error			;exit, if error
		//
		//SEND CONTROL 
		//
		ldi		Acc,(0<<_SSD1306_BIT_CO)|(1<<_SSD1306_BIT_DC)		;Set data to be write co=0 D/C=0 (WRONG SETTING BUT WORKING, WHATS WHAT'S HAPPENING)
		//
		// ISSUE CONTROL
		//
		rcall	_I2C_BYTE_OUT					;send Register
		rcall	_I2C_ACK_IN						;get ack state
		brcs	_SSD1306_Refresh_Error			;branch if error
		//
		//SEND DATA BUFFER
		//
		ldiw	Z,_SSD1306_DISPLAY_BUFFER		;z-->display buffer
		ldiw	X,128*8
	_SSD1306_Refresh_Loop:		
		ld		Acc,Z+							;get data from buffer
		rcall	_I2C_BYTE_OUT					;send data
		rcall	_I2C_ACK_IN						;read Ack
		brcs	_SSD1306_Refresh_Error			;branch if no ack
		sbiw	XH:XL,1
		brne	_SSD1306_Refresh_Loop	
		//
		//SEND STOP CONDITION TO SLAVE
		//
		rcall	_I2C_STOP						;issue stop condition
		popw	Z
		popw	X
		popaw
		clc
		ret
	_SSD1306_Refresh_Error:
		popw	Z
		popw	X
		popaw
		sec
		ret
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_SET_CONTRAST			|
	;|  								|
	;|\									|
	;| | Input  : Acc Constrast 0..255	|
	;|/  Output : cy=0 if write ok		|
	;|			  cy=1 if write error	|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Set_Contrast:
		push	Acc
		ldi		Acc,_SSD1306_CMD_SET_CONTRAST
		call	_SSD1306_Command_Write
		brcs	_SSD1306_Set_Contrast_Error		//exit if error
		pop		Acc
		push	Acc
		call	_SSD1306_Command_Write
		pop		Acc
		clc
		ret
	_SSD1306_Set_Contrast_Error:
		pop		Acc
		sec
		ret  
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Set_Dimmer				|
	;|  								|
	;|\									|
	;| | Input  : Acc bit0 =0 off =1 on	|
	;|/  Output : cy=0 if write ok		|
	;|			  cy=1 if write error	|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
/*	_SSD1306_Set_Dimmer:
		push	Acc
		ldi		Acc,_SSD1306_CMD_CHARGE_PUMP
		call	_SSD1306_Command_Write
		brcs	_SSD1306_Set_Dimmer_Error		//exit if error
		pop		Acc
		push	Acc
		push	AccH
		ldi		AccH,0x10
		sbrc	Acc,0
		ldi		AccH,0x14
		mov		Acc,AccH
		call	_SSD1306_Command_Write
		pop		AccH
		brcs	_SSD1306_Set_Dimmer_Error		//exit if error
		pop		Acc
		clc
		ret
	_SSD1306_Set_Dimmer_Error:
		pop		Acc
		sec
		ret  */		
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306 Fill Pattern			|
	;|  								|
	;|\									|
	;| | Input  : Acc byte to			|
	;|/  Output : cy=0 if write ok		|
	;|			  cy=1 if write error	|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	| Fill  Display Entire Screen with byte Pattern 
	;	\__________________________________________________________________________________
	_SSD1306_Fill_Pattern:
		push	Acc
		pushw	X
		pushw	Z
		ldiw	Z,_SSD1306_DISPLAY_BUFFER		;Z-->display buffer
		ldiw	X,128*8
	_SSD1306_Fill_Loop:
		st		Z+,Acc							;set pattern
		sbiw	XH:XL,1							;next byte
		brne	_SSD1306_Fill_Loop
		popw	Z
		popw	X
		pop		Acc
		ret
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Cls					|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : cy=0 if write ok		|
	;|			  cy=1 if write error	|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	| Fill  Display Entire Screen with byte 0x00
	;	\__________________________________________________________________________________
	_SSD1306_Cls:
		push	Acc
		ldi		Acc,0
		call	_SSD1306_Fill_Pattern
		pop		Acc
		ret
	
	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Get_Font_Pixel			|
	;|  								|
	;|\									|
	;| | Input  : Acc Ascii Chracter code|
	;| |		  XL  pixel Colum 0..7	|
	;| |		  YL  pixel Row   0..7	|
	;|/  Output : cy=0 pixel 0			|
	;|			  cy=1 pixel 1			|
	;|   Destroy: None					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	|	Include Font
	#include "_SSD1306_Font_5x8.Inc"
	;	|
	;	\__________________________________________________________________________________
	_SSD1306_Get_Font_Pixel:
		cpi		Acc,' '
		brlo	_SSD1306_Get_Font_Pixel_Exit	;invalid ascii, exit
		cpi		Acc,'~'+1
		brsh	_SSD1306_Get_Font_Pixel_Exit	;invalid ascii exit
		push	Acc
		push	XL
		push	YL
		pushw	Z
		ldiw	Z,_SSD1306_FONT_5X8*2			;Z-->font
		subi	Acc,' '
		ldi		AccH,0
		lsl		Acc								;ASCII code x 8 to get font pattern
		rol		AccH
		lsl		Acc
		rol		AccH
		lsl		Acc
		rol		AccH
		add		ZL,Acc							;AccH:Acc-->font(ASCII)
		adc		ZH,AccH
		andi	XL,7							;only 0..7
		andi	YL,7							;only 0..7
		add		ZL,YL							;Z->Font(y)
		ldi		AccH,0
		adc		ZH,AccH
		lpm		Acc,Z							;Get Bits
		inc		XL								;Compensate bit 0
	_SSD1306_Get_Font_Pixel_Loop:
		rol		Acc								;Send pixel to cy
		dec		XL								;reach bit?
		brne	_SSD1306_Get_Font_Pixel_Loop	;until reach
		popw	Z
		pop		YL
		pop		XL
		pop		Acc
	_SSD1306_Get_Font_Pixel_Exit:
		ret
	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Draw_Pixel				|
	;|  								|
	;|\									|
	;| | Input  : X  pixel Colum		|
	;| |		  Y  pixel Row			|
	;|/  Output : None					|
	;|   Destroy: Flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	|											   1 1 1 1
	;	|							 1				   2 2 2 2 
	;	|		 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4         4 5 6 7
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		\
	;	|	   0| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	   1| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	   2| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	   3| | | | | | | | | | | | | | | |......-| | | | |		\
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+     / Page 0
	;	|	   4| | | | | | | | | | | | | | | |......-| | | | |     |
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+	    |
	;	|	   5| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	   6| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	   7| | | | | | | | | | | | | | | |......-| | | | |		/
	;	|	----+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+-----\-------------------------------------
	;	|	   8| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	   9| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	  10| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	  11| | | | | | | | | | | | | | | |......-| | | | |		\
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+     / Page 1
	;	|	  12| | | | | | | | | | | | | | | |......-| | | | |     |
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+	    |
	;	|	  13| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	  14| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	  15| | | | | | | | | | | | | | | |......-| | | | |		/
	;	|	----+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+-----\-------------------------------------
	;	|
	;	|	Set Logical Operator before call this function
	;	|
	_SSD1306_Draw_Pixel:
		push	Temp
		cpiw	X,0										;x<0?
		lbrlt	_SSD1306_Draw_Pixel_Exit				;yes, exit
		cpiw	X,_SSD1306_LCD_WIDTH					;x>lcd width
		lbrge	_SSD1306_Draw_Pixel_Exit				;yes, exit
		cpiw	Y,0										;y<0?
		brlt	_SSD1306_Draw_Pixel_Exit				;yes, exit
		cpiw	Y,_SSD1306_LCD_HEIGHT					;Y>lcd height
		brge	_SSD1306_Draw_Pixel_Exit
		push	r0
		push	r1
		pushw	Z
		push	YL
		push	AccT
		pushaw
		ldiw	Z,_SSD1306_DISPLAY_BUFFER				;Z-->DISPLAY BUFFER
		add		ZL,XL									;Z-->DISPLAY COLUNM Z(X)
		ldi		AccH,0
		adc		ZH,AccH

		/*
		mov		AccH,YL									;Compute Page int(y)/8*128
		lsr		AccH
		lsr		AccH
		lsr		AccH
		ldi		AccT,128
		mul		AccH,AccT
		*/

		mov		AccH,YL									;Compute Page int(y)*16 2 clock better
		andi	AccH,0xf8
		ldi		AccT,16
		mul		AccH,AccT


		add		ZL,r0									;Z-->DISPLAY Z(X,PAGE)
		ldi		AccT,0
		adc		ZH,r1
		pushw	Z
		ldiw	Z,_SSD1306_BIT_MASK_TABLE*2				;Get bit mas
		andi	YL,7
		add		ZL,YL
		ldi		YL,0
		adc		ZH,YL
		lpm		AccH,Z									;AccH=BIT MASK
		popw	Z
	_SSD1306_Draw_Pixel_Mask_10:
		lds		Temp,(_SSD1306_OPERATOR)				;get operator
		cpi		Temp,_SSD1306_OPERATOR_OR				;or operator?
		brne	_SSD1306_Draw_Pixel_Mask_20				;no, branch
		//OR OPERATOR
		ld		AccT,Z									;get pixels
		or		AccT,AccH								;or mask
		st		Z,AccT
		rjmp	_SSD1306_Draw_Pixel_Mask_Exit
	_SSD1306_Draw_Pixel_Mask_20:
		cpi		Temp,_SSD1306_OPERATOR_AND				;and operator?
		brne	_SSD1306_Draw_Pixel_Mask_30				;no, branch
		//AND OPERATOR
		ld		AccT,Z									;get pixels
		com		AccH
		and		AccT,AccH								;and mask
		st		Z,AccT		
		rjmp	_SSD1306_Draw_Pixel_Mask_Exit
	_SSD1306_Draw_Pixel_Mask_30:
		cpi		Temp,_SSD1306_OPERATOR_XOR				;xor operator?
		brne	_SSD1306_Draw_Pixel_Mask_Exit			;no, branch
		//XOR OPERATOR
		ld		AccT,Z									;get pixels
		eor		AccT,AccH								;and mask
		st		Z,AccT		
	_SSD1306_Draw_Pixel_Mask_Exit:
		popaw
		pop		AccT
		pop		YL
		popw	Z
		pop		r1
		pop		r0
	_SSD1306_Draw_Pixel_Exit:
		pop		Temp
		ret
	/*
		#---------------------------------#
		| Pixel Set Mask table			  |
		#---------------------------------#
	*/
	_SSD1306_BIT_MASK_TABLE:
		.DB	0X01,0X02,0X04,0X08,0X10,0X20,0X40,0X80

	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Draw_Line				|
	;|  								|
	;|\									|
	;| | Input  : AccH:Acc   x start	|
    ;| |	      AccTH:AccT y start	|
	;| |		  X end					|
	;| |		  Y end					|
	;|/  Output : None					|
	;|   Destroy: r0,,,r12,Temp,flags	|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	|	Set Logical Operator Before call this function
	;	\__________________________________________________________________________________

#ifdef	__USE_GRAPH_LINE__
/*
	_SSD1306_Draw_Line:
		pushaw
		pushawt
		pushw	x
		pushw	y
		push	Temp
		call	_GRAPH_MOVE_TO						;set start coordinate
		call	_GRAPH_LINE_TO						;draw a line
		pop		Temp
		popw	y
		popw	x
		popawt
		popaw
		ret
		*/
#endif
	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Draw_Circle			|
	;|  								|
	;|\									|
	;| | Input  : AccTH:AccT Radius		|
	;| |		  X Center				|
	;| |		  Y Center				|
	;|/  Output : None					|
	;|   Destroy: Temp,flags			|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	|	Set Logical Operator Before call this function
	;	\__________________________________________________________________________________
#ifdef __USE_GRAPH_CIRCLE__
/*	_SSD1306_Draw_Circle:
		pushaw
		pushawt
		pushw	x
		pushw	y
		push	Temp
		//mov		Acc,Temp
		call	_GRAPH_CIRCLE_SET_COLOR				;set Color
		call	_GRAPH_CIRCLE						;draw a circle
		pop		Temp
		popw	y
		popw	x
		popawt
		popaw
		ret
		*/	
#endif
	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Locate					|
	;|  								|
	;|\									|
	;| | Input  : X end					|
	;| |		  Y end					|
	;|/  Output : None					|
	;|   Destroy:						|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	| Locate graph cursor at coordinates x,y
	;	\__________________________________________________________________________________
	_SSD1306_Locate:
		stsw	(_SSD1306_X),X			
		stsw	(_SSD1306_Y),Y
		ret
	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_PutC					|
	;|  								|
	;|\									|
	;| | Input  : Acc ASCII Code		|
	;|/  Output : None					|
	;|   Destroy: r0..r7				|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	| Put Character with ascii code at coordinates defined byte locate function
	;	\__________________________________________________________________________________
	_SSD1306_PutC:
		cpi		Acc,_LF								;Line Feed?
		brne	_SSD1306_PutC_10					;no, branch
		pushaw
		ldiaw	_SSD1306_Font_Height				;fontheight*scale
		movw	r3:r2,AccH:Acc
		lds		r4,(_SSD1306_Text_Scale)
		call	_SSD1306_Compute_Scale
		ldsaw	(_SSD1306_Y)
		add		Acc,r6								;y+=fontheight*scale
		adc		AccH,r7
		stsaw	(_SSD1306_Y)
		popaw
		ret
	_SSD1306_PutC_10:
		cpi		Acc,_CR								;Line Feed
		brne	_SSD1306_PutC_20
		pushaw
		ldiaw	0
		stsaw	(_SSD1306_X)
		popaw
		ret
	_SSD1306_PutC_20:
		pushw	X
		pushw	Y
		pushaw
		pushawt
		push	Temp
		ldiw	x,0
		ldiw	y,0
	_SSD1306_PutC_Loop:
		push	Acc									;save ascii code
		call	_SSD1306_Get_Font_Pixel				;get pixel of font
		ldi		Acc,0								;Assume pixel 0
		brcc	_SSD1306_PutC_Loop_10				;branch if pixel=0
		ldi		Acc,1								;Else Pixel 1
	_SSD1306_PutC_Loop_10:
		//PLOT char Pixel
		pushw	X
		pushw	Y
		//SCALE
		movw	r3:r2,XH:XL							;x*scale+offset
		lds		r4,(_SSD1306_Text_Scale)
		call	_SSD1306_Compute_Scale
		movw	XH:XL,r7:r6
		ldsawt	(_SSD1306_X)
		add		XL,AccT
		adc		XH,AccTH

		movw	r3:r2,YH:YL							;y*scale+offset
		lds		r4,(_SSD1306_Text_Scale)
		call	_SSD1306_Compute_Scale
		movw	YH:YL,r7:r6
		ldsawt	(_SSD1306_Y)
		add		YL,AccT
		adc		YH,AccTH
		cpi		Acc,1								;set Pixel?
		brne	_SSD1306_PutC_Loop_20
		lds		Temp,(_SSD1306_OPERATOR)			;get operator to draw text
		pushaw
		pushawt
		lds		Acc,(_SSD1306_Text_Scale)
		ldi		AccH,0
		mov		AccT,Acc
		ldi		AccTH,0
		call	_SSD1306_Draw_Box_Full				;Set Pixel on screen buffer
		popawt
		popaw
	_SSD1306_PutC_Loop_20:
		//END SCALE
		popw	Y
		popw	X
		pop		Acc									;restore ascii code
		adiw	XH:XL,1								;next x
		cpiw	X,_SSD1306_FONT_WIDTH
		lbrlo	_SSD1306_PutC_Loop
		ldiw	X,0
		adiw	YH:YL,1								;next y
		cpiw	Y,_SSD1306_FONT_HEIGHT
		lbrlo	_SSD1306_PutC_Loop
		//Update x coords with char width *scale
		ldiaw	_SSD1306_Font_Width					;fontwidth*scale
		movw	r3:r2,AccH:Acc
		lds		r4,(_SSD1306_Text_Scale)
		call	_SSD1306_Compute_Scale
		ldsaw	(_SSD1306_X)
		add		Acc,r6								;x+=fontheight*scale
		adc		AccH,r7
		stsaw	(_SSD1306_X)
				
		pop		Temp
		popawt
		popaw
		popw	Y
		popw	X
		ret
	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_PutSF					|
	;|  								|
	;|\									|
	;| | Input  : Z-->String Flash		|
	;|/  Output : None					|
	;|   Destroy:						|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	| Put Character STring into Flash at coordinates defined byte locate function
	;	\__________________________________________________________________________________
	_SSD1306_PutSF:
		push	Acc
	_SSD1306_PutSF_Loop:
		lpm		Acc,Z+							;get character from flash
		cpi		Acc,0							;zero terminator?
		breq	_SSD1306_PutSF_Exit				;yes, exit
		call	_SSD1306_PutC
		rjmp	_SSD1306_PutSF_Loop
	_SSD1306_PutSF_Exit:
		pop		Acc
		ret
	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_PutS					|
	;|  								|
	;|\									|
	;| | Input  : Z-->String SRAM		|
	;|/  Output : None					|
	;|   Destroy:						|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	| Put Character String into SRAM at coordinates defined byte locate function
	;	\__________________________________________________________________________________
	_SSD1306_PutS:
		push	Acc
	_SSD1306_PutS_Loop:
		ld		Acc,Z+							;get character from flash
		cpi		Acc,0							;zero terminator?
		breq	_SSD1306_PutS_Exit				;yes, exit
		call	_SSD1306_PutC
		rjmp	_SSD1306_PutS_Loop
	_SSD1306_PutS_Exit:
		pop		Acc
		ret
	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Set_Screen_UpsideDown	|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy:						|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	| Set Screen in upsidedown in y axis
	;	\__________________________________________________________________________________
	_SSD1306_SET_SCREEN_UPSIDEDOWN:
		push	Acc
		ldi		Acc,_SSD1306_CMD_SET_SEG_REMAP | 0X0
		call	_SSD1306_Command_Write
		brcs	_SSD1306_SET_SCREEN_UPSIDEDOWN_Error	//exit if error
		ldi		Acc,_SSD1306_CMD_SET_COM_SCAN_INC
		call	_SSD1306_Command_Write
		brcs	_SSD1306_SET_SCREEN_UPSIDEDOWN_Error	//exit if error
		clc
		pop		Acc
		Ret
	_SSD1306_SET_SCREEN_UPSIDEDOWN_Error:
		pop		Acc
		sec
		ret
		 
	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Set_Screen_Normal		|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : None					|
	;|   Destroy:						|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	| Set Screen in upsidedown in y axis
	;	\__________________________________________________________________________________
	_SSD1306_SET_SCREEN_NORMAL:
		push	Acc
		ldi		Acc,_SSD1306_CMD_SET_SEG_REMAP | 0X0 
		call	_SSD1306_Command_Write
		brcs	_SSD1306_SET_SCREEN_NORMAL_Error	//exit if error
		ldi		Acc,_SSD1306_CMD_SET_COM_SCAN_INC
		call	_SSD1306_Command_Write
		brcs	_SSD1306_SET_SCREEN_NORMAL_Error	//exit if error
		clc
		pop		Acc
		Ret
	_SSD1306_SET_SCREEN_NORMAL_Error:
		pop		Acc
		sec
		ret
	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Horizontal_Line		|
	;|  								|
	;|\									|
	;| | Input  : AccH:Acc width		|
	;| |		  X  pixel Colum		|
	;| |		  Y  pixel Row			|
	;|/  Output : None					|
	;|   Destroy: Flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	|											   1 1 1 1
	;	|							 1				   2 2 2 2 
	;	|		 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4         4 5 6 7
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		\
	;	|	   0| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	   1| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	   2| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	   3| | | | | | | | | | | | | | | |......-| | | | |		\
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+     / Page 0
	;	|	   4| | | | | | | | | | | | | | | |......-| | | | |     |
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+	    |
	;	|	   5| | | | |*|*|*|*|*| | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	   6| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	   7| | | | | | | | | | | | | | | |......-| | | | |		/
	;	|	----+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+-----\-------------------------------------
	;	|	   8|*|*|*|*|*| | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	   9| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	  10| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	  11| | | | | | | | | | | | | | | |......-| | | | |		\
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+     / Page 1
	;	|	  12| | | | | | | | | | | | | | | |......-| | | | |     |
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+	    |
	;	|	  13| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	  14| | | | | | | | | | | | | | | |......-| | | | |		|
	;	|		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+		|
	;	|	  15| | | | | | | | | | | | | | | |......-| | | | |		/
	;	|	----+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+......-+-+-+-+-+-----\-------------------------------------
	;	|
	;	| LOGICAL OPERATION MAY BE _SSD1306_OPERATOR_OR,_SSD1306_OPERATOR_AND,_SSD1306_OPERATOR_XOR
	;	|
	_SSD1306_Draw_Horizontal_Line:
		push	r0
		push	r1
		push	Temp
		pushaw
		pushawt
		pushw	X
		pushw	Y
		//check if y<0
		cpiw	Y,0
		brlt	_SSD1306_Horizontal_Line_FINISH_1			;exit if yes
		//check if y>lcd height
		cpiw	Y,_SSD1306_LCD_HEIGHT
		brge	_SSD1306_Horizontal_Line_FINISH_1			;exit if yes
		//check if x>lcd width
		cpiw	X,_SSD1306_LCD_WIDTH
		brge	_SSD1306_Horizontal_Line_FINISH_1			;exit if yes
		//check if start coord x + width < 0
		movw	AccTH:AccT,AccH:Acc
		add		AccT,XL
		adc		AccTH,XH
		cpi		AccT,0
		ldi		Temp,0
		cpc		AccTH,temp
		brge	_SSD1306_Horizontal_Line_10					;branch if (x+width)>=0
		rjmp	_SSD1306_Horizontal_Line_FINISH_1
	_SSD1306_Horizontal_Line_FINISH_2:
		popw	Z
	_SSD1306_Horizontal_Line_FINISH_1:
		popw	Y
		popw	X
		popawt
		popaw
		pop		Temp
		pop		r1
		pop		r0
		ret													;else return
	_SSD1306_Horizontal_Line_10:
		//check if x<0 if so, width=width-abs(x) then x=0
		cpiw	X,0
		brge	_SSD1306_Horizontal_Line_20					;if x>=0 branch
		com		XL											;width-abs(x)
		com		XH
		subi	XL,-1
		sbci	XH,-1
		sub		Acc,XL
		sbc		AccH,XH
		ldiw	X,0											;x=0
	_SSD1306_Horizontal_Line_20:
		//check if (x+width-1)
		movw	AccTH:AccT,XH:XL							;x+width>lcd width
		add		AccT,Acc
		adc		AccTH,AccH
		cpi		AccT,low(_SSD1306_lcd_Width)
		ldi		Temp,high(_SSD1306_lcd_Width)
		cpc		AccTH,temp
		brlt	_SSD1306_Horizontal_Line_30					;branch if (x+width)<lcd_width
		ldiawt	_SSD1306_LCD_WIDTH
		sub		AccT,XL										;width=lcd_width-x
		sbc		AccTH,XH
		movw	AccH:Acc,AccTH:AccT
	_SSD1306_Horizontal_Line_30:
		//compute start position
		pushw	Z
		ldiw	Z,_SSD1306_DISPLAY_BUFFER					;Z-->DISPLAY BUFFER
		mov		Temp,YL
		andi	Temp,0x0f8									;int(y)*16
		ldi		AccT,16
		mul		AccT,Temp
		add		ZL,r0					
		adc		ZH,r1
		add		ZL,XL										;int(y)*16+x
		adc		ZH,XH
		ldi		AccT,0x01									;compute mask
		mov		Temp,YL
		andi	Temp,7										;mask end?
	_SSD1306_Horizontal_Line_40:
		breq	_SSD1306_Horizontal_Line_50					;yes, branch
		rol		AccT										;next mask
		dec		Temp
		rjmp	_SSD1306_Horizontal_Line_40					;until done
	_SSD1306_Horizontal_Line_50:
		//PERFORME LOGICAL OPERATION
		lds		Temp,(_SSD1306_OPERATOR)					;get operation
		cpi		Temp,(_SSD1306_OPERATOR_OR)					;or?
		breq	_SSD1306_Horizontal_Line_OR					;yes, branch
		cpi		Temp,(_SSD1306_OPERATOR_AND)				;and?
		breq	_SSD1306_Horizontal_Line_AND
		//perform xor operation
	_SSD1306_Horizontal_Line_XOR:
		cpi		Acc,0
		ldi		Temp,0
		cpc		AccH,Temp
		lbreq	_SSD1306_Horizontal_Line_FINISH_2
		ld		Temp,Z
		eor		Temp,AccT									;mask operation xor
		st		Z+,Temp
		sbiw	AccH:Acc,1									;end byte?
		rjmp	_SSD1306_Horizontal_Line_XOR				;no, continue
		//perform or operation
	_SSD1306_Horizontal_Line_OR:
		cpi		Acc,0
		ldi		Temp,0
		cpc		AccH,Temp
		lbreq	_SSD1306_Horizontal_Line_FINISH_2
		ld		Temp,Z
		or		Temp,AccT
		st		Z+,Temp
		sbiw	AccH:Acc,1									;end byte?
		rjmp	_SSD1306_Horizontal_Line_OR
		//perform and operation
	_SSD1306_Horizontal_Line_AND:
		cpi		Acc,0
		ldi		Temp,0
		cpc		AccH,Temp
		lbreq	_SSD1306_Horizontal_Line_FINISH_2
		ld		Temp,Z
		com		AccT										;com mask
		and		Temp,Acct
		com		AccT										;com mask again
		st		Z+,temp
		sbiw	AccH:Acc,1									;end byte?
		rjmp	_SSD1306_Horizontal_Line_AND				;no, continue

	;   |  |  |  |  |  |  |  |  |  |  |	
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Draw_Box				|
	;|  								|
	;|\									|
	;| | Input  : X  x					|
	;| |		  Y  y					|
	;| |		  AccH:Acc width		|
	;| |		  AccTH:AccT height		|
	;|/  Output : None					|
	;|   Destroy: Flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	|	Set Logical Operator Before call this function
	;	\__________________________________________________________________________________
	_SSD1306_Draw_Box:
		push	Temp
		ldi		Temp,0
		cpi		Acc,0
		cpc		AccH,Temp
		breq	_SSD1306_DRAW_BOX_EXIT
		cpi		AccT,0
		cpc		AccTH,Temp
		breq	_SSD1306_DRAW_BOX_EXIT
		pushw	Y
		call	_SSD1306_Draw_Horizontal_Line
		add		YL,Acct
		adc		YH,AccTH
		sbiw	YH:YL,1
		call	_SSD1306_Draw_Horizontal_Line
		popw	Y
		pushw	Y
		pushawt
	_SSD1306_DRAW_BOX_10:
		call	_SSD1306_Draw_Pixel
		pushw	X
		add		XL,Acc
		adc		XH,AccH
		sbiw	XH:XL,1
		call	_SSD1306_Draw_Pixel
		popw	X
		adiw	YH:YL,1
		subiawt	1
		brne	_SSD1306_DRAW_BOX_10
		popawt
		popw	Y
_SSD1306_DRAW_BOX_EXIT:
		pop		Temp
		ret
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Draw_Box_Full			|
	;|  								|
	;|\									|
	;| | Input  : X  x					|
	;| |		  Y  y					|
	;| |		  AccH:Acc width		|
	;| |		  AccTH:AccT height		|
	;|/  Output : None					|
	;|   Destroy: Flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	|	Set Logical Operator Before call this function
	;	\___________________________________________________________________________________
	_SSD1306_Draw_Box_Full:
		push	Temp
		ldi		Temp,0
		cpi		Acc,0
		cpc		AccH,Temp
		breq	_SSD1306_DRAW_BOX_FULL_EXIT
		cpi		AccT,0
		cpc		AccTH,Temp
		breq	_SSD1306_DRAW_BOX_FULL_EXIT
		pushawt
		pushw	Y
	_SSD1306_Draw_Box_Full_Loop:
		call	_SSD1306_Draw_Horizontal_Line
		adiw	YH:YL,1
		subi	AccT,1
		sbci	AccTH,0
		brne	_SSD1306_Draw_Box_Full_Loop
		popw	Y
		popawt
	_SSD1306_DRAW_BOX_FULL_EXIT:
		pop		Temp
		ret
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Get_Char_Width			|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : AccH:Acc char width	|
	;|   Destroy: Flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Get_Char_Width:
		push	r2
		push	r3
		push	r4
		push	r5
		push	r6
		push	r7
		ldiaw	_SSD1306_FONT_WIDTH
		//Compute 
	_SSD1306_Get_Char_Width_Compute:
		movw	r3:r2,AccH:Acc
		lds		r4,(_SSD1306_Text_Scale)
		call	_SSD1306_Compute_Scale
		movw	AccH:Acc,r7:r6
	_SSD1306_Get_Char_Width_Restore:
		pop		r7
		pop		r6
		pop		r5
		pop		r4
		pop		r3
		pop		r2
		ret
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Get_Char_Height		|
	;|  								|
	;|\									|
	;| | Input  : None					|
	;|/  Output : AccH:Acc char Height	|
	;|   Destroy: Flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Get_Char_Height:
		push	r2
		push	r3
		push	r4
		push	r5
		push	r6
		push	r7
		ldiaw	_SSD1306_FONT_HEIGHT
		rjmp	_SSD1306_Get_Char_Width_Compute
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Get_Text_Width			|
	;|  								|
	;|\									|
	;| | Input  : Z-->Text in FLASH		|
	;|/  Output : AccH:Acc Text width	|
	;|   Destroy: Flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Get_Text_Width:
		push	r2
		push	r3
		push	r4
		push	r5
		push	r6
		push	r7		
		pushawt
		pushw	Z
		ldiawt	0									;char count
	_SSD1306_Get_Text_Width_loop:
		lpm		Temp,Z+								;get character
		cpi		Temp,0								;end terminator?
		breq	_SSD1306_Get_Text_Width_loop_10		;yes, branch
		cpi		Temp,_NC							;null char (no count0
		breq	_SSD1306_Get_Text_Width_loop		;get next char
		addiawt	1									;count char
		rjmp	_SSD1306_Get_Text_Width_loop		;get next char
	_SSD1306_Get_Text_Width_loop_10:
		movw	r3:r2,AccTH:AccT					;compute text width
		call	_SSD1306_Get_Char_Width
		mov		r4,Acc
		call	_SSD1306_Compute_Scale				;multiply by scale
		movw	AccH:Acc,r7:r6						;get text width
		popw	Z
		popawt
		rjmp	_SSD1306_Get_Char_Width_Restore		;Restore registers 0..7
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Get_Text_Center_X		|
	;|  								|
	;|\									|
	;| | Input  : Z-->Text in FLASH		|
	;|/  Output : AccH:Acc Center		|
	;|   Destroy: Flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Get_Text_Center_X:
		pushawt
		call	_SSD1306_Get_Text_Width				;get text width
		ldiawt	_SSD1306_LCD_WIDTH					;get graph width
		sub		AccT,Acc							;compute (graph_width-text_width)/2
		sbc		AccTH,AccH
		lsr		AccTH
		ror		AccT
		movw	AccH:Acc,AccTH:AccT					;move to appropriate register
		popawt
		ret
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Get_Text_Center_Y		|
	;|  								|
	;|\									|
	;| | Input  : Z-->Text in FLASH		|
	;|/  Output : AccH:Acc Center		|
	;|   Destroy: Flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Get_Text_Center_Y:
		pushawt
		call	_SSD1306_Get_Char_Height			;get text height
		ldiawt	_SSD1306_LCD_HEIGHT					;get graph height
		sub		AccT,Acc							;compute (graph_height-text_height)/2
		sbc		AccTH,AccH
		lsr		AccTH
		ror		AccT
		movw	AccH:Acc,AccTH:AccT					;move to appropriate register
		popawt
		ret
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Get_BitMap_Pixel		|
	;|  								|
	;|\									|
	;| | Input  : x,y coordinates		|
	;| |		  Z--> bmp in flash		|
	;|/  Output : temp Bit 0 or 1		|
	;|   Destroy: r2..r7,Flags			|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	| Bmp Header is formated in Flash as bellow
	;	|	;+------------+
	;	|	;| BMP Header |
	;	|	;+------------+
	;	|	.DW      1 ;Bits Color Bit	offset 0
	;	|	.DW     66 ;Width Pixels	offset 2
	;	|	.DW     17 ;Height Pixels	offset 4
	;	|	.DW      9 ;Width Bytes		offset 6
	;	|	.DW    153 ;Size Bytes		offset 8
	;	|	;+------------+
	;	|	;|  BMP Data  |
	;	|	;+------------+
	;	|	.DB 0x00,0x00,0x00,0x00,0x00......
	;	|
	;	\___________________________________________________________________________________
	.EQU	_BMP_OFFSET_BIT_SIZE	= 0
	.EQU	_BMP_OFFSET_WIDTH		= 2
	.EQU	_BMP_OFFSET_HEIGHT		= 4
	.EQU	_BMP_OFFSET_WIDTH_BYTE	= 6
	.EQU	_BMP_OFFSET_SIZE		= 8
	.EQU	_BMP_OFFSET_PIXELS		= 10


	_SSD1306_Get_BitMap_Pixel:
	_SSD1306_Get_Bmp_Pixel:
		cpiw	X,0									;x<0 ?
		brge	_SSD1306_Get_BitMap_Pixel_10		;no, branch
		ldi		Temp,0								;else return bit 0
		ret
	_SSD1306_Get_BitMap_Pixel_10:
		cpiw	Y,0									;y<0?
		brge	_SSD1306_Get_BitMap_Pixel_20		;no, branch
		ldi		Temp,0								;else return bit 0
		ret
	_SSD1306_Get_BitMap_Pixel_20:
		pushaw
		call	_SSD1306_Get_BitMap_Width
		cp		XL,Acc
		cpc		XH,AccH
		brlt	_SSD1306_Get_BitMap_Pixel_30		;no, branch
		popaw
		ldi		Temp,0								;else return bit 0
		ret
	_SSD1306_Get_BitMap_Pixel_30:
		call	_SSD1306_Get_BitMap_Height
		cpc		YL,Acc
		cpc		YH,AccH
		brlt	_SSD1306_Get_BitMap_Pixel_40		;no, branch
		popaw
		ldi		Temp,0								;else return bit 0
		ret
	_SSD1306_Get_BitMap_Pixel_40:
		//
		//Compute pixel position pos=y*width_byte+(x\8)
		//
		pushw	Z
		movw	r3:r2,YH:YL							;move to apropriate registers
		pushw	Z
		adiw	ZH:ZL,_BMP_OFFSET_WIDTH_BYTE		;x>bmp height?
		lpm		r4,Z								;in this drive is use only byte low means max width = 2040 possible never used
		popw	Z
		call	_SSD1306_Compute_Scale
		movw	AccH:Acc,XH:XL						;x/8
		lsr		AccH
		ror		Acc
		lsr		AccH
		ror		Acc
		lsr		AccH
		ror		Acc
		add		Acc,r6								;y*bmp_width+(x/8)
		adc		AccH,r7
		add		ZL,Acc
		adc		ZH,AccH
		adiw	ZH:ZL,_BMP_OFFSET_PIXELS			;Z-->bits(y*bmp_width+(x/8)
		//Compute bit mask
		ldi		AccH,0x80
		mov		Acc,XL
		andi	Acc,7
	_SSD1306_Get_BitMap_Pixel_50:
		breq	_SSD1306_Get_BitMap_Pixel_60		;mask compute branch
		lsr		AccH
		dec		Acc
		rjmp	_SSD1306_Get_BitMap_Pixel_50
	_SSD1306_Get_BitMap_Pixel_60:
		lpm		Acc,Z								;get pixel
		and		Acc,AccH							;mask pixel
		ldi		Temp,0								;assume pixel = 0
		breq	_SSD1306_Get_BitMap_Pixel_70		;yes, branch
		ldi		Temp,1								;else pixel = 1
	_SSD1306_Get_BitMap_Pixel_70:
		popw	Z
		popaw
		ret
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Get_BitMap_Width		|
	;|  								|
	;|\									|
	;| | Input  : Z--> bmp in flash		|
	;|/  Output : AccH:Acc width in pixels
	;|   Destroy: flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Get_BitMap_Width:
	_SSD1306_Get_Bmp_Width:
		pushw	Z
		adiw	ZH:ZL,_BMP_OFFSET_WIDTH
		lpm		Acc,Z+
		lpm		AccH,Z
		popw	Z
		ret		
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Get_BitMap_Height		|
	;|  								|
	;|\									|
	;| | Input  : Z--> bmp in flash		|
	;|/  Output : AccH:Acc width in pixels
	;|   Destroy: flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Get_BitMap_Height:
	_SSD1306_Get_Bmp_Height:
		pushw	Z
		adiw	ZH:ZL,_BMP_OFFSET_HEIGHT
		lpm		Acc,Z+
		lpm		AccH,Z
		popw	Z
		ret		
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Draw_BitMap			|
	;|  								|
	;|\									|
	;| | Input  : Z--> bmp in flash		|
	;| |		  X,Y  coordinates		|
	;|/  Output : none					|
	;|   Destroy: flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	_SSD1306_Draw_BitMap:
	_SSD1306_Draw_Bmp:
		push	Temp
		lds		Temp,(_SSD1306_OPERATOR)		;save operation mode
		push	temp
		pushaw
		pushw	X
		pushw	Y
		pushw	Z
		stsw	(_SSD1306_X),X					;store coordinates
		stsw	(_SSD1306_Y),Y
		ldiw	x,0								;start bmp pixel scan
		ldiw	y,0
	_SSD1306_Draw_BitMap_10:
		call	_SSD1306_Get_BitMap_Pixel		;get pixel
		lds		Acc,(_SSD1306_OPERATOR)			;logical operator is set?
		cpi		Acc,_SSD1306_OPERATOR_SET		;
		breq	_SSD1306_Draw_BitMap_20			;yes, branch
		cpi		Temp,1							;active pixel?
		brne	_SSD1306_Draw_BitMap_20			;no, branch
		call	_SSD1306_Draw_BitMap_Set
		rjmp	_SSD1306_Draw_BitMap_30			;pump next pixel
		//
		//Set Pixels
		//
	_SSD1306_Draw_BitMap_Set:
		pushw	x								;else draw it on screen
		pushw	y
		ldsaw	(_SSD1306_X)					;add screen offset X
		add		XL,Acc				
		adc		XH,AccH
		ldsaw	(_SSD1306_Y)					;add screen offset Y
		add		YL,Acc				
		adc		YH,AccH
		call	_SSD1306_Draw_Pixel
		popw	y
		popw	x
		ret
	_SSD1306_Draw_BitMap_20:
		ldi		Acc,_SSD1306_OPERATOR_AND		;assume AND operator
		cpi		Temp,0
		breq	_SSD1306_Draw_BitMap_20A		;yes, branch
		ldi		Acc,_SSD1306_OPERATOR_OR		;else operator OR
	_SSD1306_Draw_BitMap_20A:
		sts		(_SSD1306_OPERATOR),Acc			;set operator
		rcall	_SSD1306_Draw_BitMap_Set		;set pixel
		ldi		Acc,_SSD1306_OPERATOR_SET		;restore operator
		sts		(_SSD1306_OPERATOR),Acc			;set operator
		//pump coordinates
	_SSD1306_Draw_BitMap_30:
		adiw	XH:XL,1							;next column 
		call	_SSD1306_Get_BitMap_Width
		cp		XL,Acc
		cpc		XH,AccH
		brlt	_SSD1306_Draw_BitMap_10			;until x>width
		ldiw	X,0
		adiw	YH:YL,1							;next row
		call	_SSD1306_Get_BitMap_Height
		cp		YL,Acc
		cpc		YH,AccH
		brlt	_SSD1306_Draw_BitMap_10			;until y>height
		popw	Z
		popw	Y
		popw	X
		popaw
		pop		Temp
		sts		(_SSD1306_OPERATOR),Temp		;restore operation mode
		pop		Temp
		ret
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Set_Progress_Bar_Value	|
	;|  								|
	;|\									|
	;| | Input  : AccH:Acc Value		|
	;|/  Output : none					|
	;|   Destroy: flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	| Seta Valor que será mostrado no progress bar
	;	\__________________________________________________________________________________
	.DSEG
		_SSD1306_Progress_Bar_Value:	.byte	_WORD
	.CSEG

	_SSD1306_Set_Progress_Bar_Value:
		stsaw	(_SSD1306_Progress_Bar_Value)
		ret
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Set_Progress_Bar_Coods	|
	;|  								|
	;|\									|
	;| | Input  : x,y	Coordinates		|
	;|/  Output : none					|
	;|   Destroy: flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	| Seta Coordenas onde o Progress Bar Sera mostrado
	;	\__________________________________________________________________________________
	.DSEG
		_SSD1306_Progress_Bar_Coord_X:	.byte	_WORD
		_SSD1306_Progress_Bar_Coord_Y:	.byte	_WORD
	.CSEG

	_SSD1306_Set_Progress_Bar_Coords:
		stsw	(_SSD1306_Progress_Bar_Coord_X),X
		stsw	(_SSD1306_Progress_Bar_Coord_Y),Y
		ret
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Set_Progress_Bar_Sizes	|
	;|  								|
	;|\									|
	;| | Input  : X		Width			|
	;| |		  Y		Height			|
	;|/  Output : none					|
	;|   Destroy: flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	| Seta Largura e Altura do Progress Bar
	;	\__________________________________________________________________________________
	.DSEG
		_SSD1306_Progress_Bar_Size_Width:	.byte	_WORD
		_SSD1306_Progress_Bar_Size_Height:	.byte	_WORD
	.CSEG

	_SSD1306_Set_Progress_Bar_Sizes:
		stsw	(_SSD1306_Progress_Bar_Size_Width),X
		stsw	(_SSD1306_Progress_Bar_Size_Height),Y
		ret
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Set_Progress_Bar_Range	|
	;|  								|
	;|\									|
	;| | Input  : x		Min Value		|
	;| |		  y		Max Value		|
	;|/  Output : none					|
	;|   Destroy: flags					|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	| Seta Valor Minimo e Valor Maximo do Progress Bar
	;	| Se valor Max for menor que Min será invertido a entrada ou seja Min=Max Max=min
	;	| Se Valor Min or igual a Max, condição invalida, nada será feito
	;	\__________________________________________________________________________________
	.DSEG
		_SSD1306_Progress_Bar_Range_Min_Value:	.byte	_WORD
		_SSD1306_Progress_Bar_Range_Max_Value:	.byte	_WORD
	.CSEG

	_SSD1306_Set_Progress_Bar_Range:
		cpw		X,Y											;x<y?
		brlt	_SSD1306_Set_Progress_Bar_Range_10			;yes, branch
		breq	_SSD1306_Set_Progress_Bar_Range_Exit		;exit if iqual values means invalid
		stsw	(_SSD1306_Progress_Bar_Range_Min_Value),Y	;else invert
		stsw	(_SSD1306_Progress_Bar_Range_Max_Value),X
		ret
	_SSD1306_Set_Progress_Bar_Range_10:
		stsw	(_SSD1306_Progress_Bar_Range_Min_Value),X
		stsw	(_SSD1306_Progress_Bar_Range_Max_Value),Y
	_SSD1306_Set_Progress_Bar_Range_Exit:
		ret
	;+--+--+--+--+--+--+--+--+--+--+--+-+
	;|   SSD1306_Draw_Progress_Bar		|
	;|  								|
	;|\									|
	;| | Input  : none					|
	;|/  Output : none					|
	;|   Destroy: r0..r11,flags			|
	;|   			       				|
	;+--+--+--+--+--+--+--+--+--+--+--+-+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\___\___________________________________________________
	;	/
	;	| Desenha Progress Bar na Tela de Acordo com os dados das funcoes anteriores que sao:
	;	|
	;	|	_SSD1306_Set_Progress_Bar_Value:	Valor do Progress Bar
	;	|	_SSD1306_Set_Progress_Bar_Coords:	Coordenadas	
	;	|	_SSD1306_Set_Progress_Bar_Size:		Tamanho
	;	|	_SSD1306_Set_Progress_Bar_Range:	Valore Minimos e Maximo
	;	|
	;	| nenhum ou algums ou todos os valores acima podem ser chamados antes de desenhar
	;	| os valores defaults para o progress bar sao:
	;	|
	;	|  value=0	x,y=0	width=64;height=32	range=0;100
	;	|
	;	| A Forma do Progress Bar segue:
	;	|
	;	|	    :............. Width ...................:
	;	|  (x,y)+---------------------------------------+..
	;	|       |										| :
	;	|		| #####(Bar)#######-------------------- | Height
	;	|		| (min)							  (max)	| :
	;	|		+---------------------------------------+..
	;	\__________________________________________________________________________________
	_SSD1306_Draw_Progress_Bar:
		push	Temp
		lds		Temp,(_SSD1306_Operator)					;save operator
		push	Temp
		pushaw
		pushawt
		pushw	X
		pushw	Y
		//chech sizes
		ldsaw	(_SSD1306_Progress_Bar_Size_Width)
		cpiaw	5											;min size width 5
		lbrlt	_SSD1306_Draw_Progress_Bar_Exit				;exit is less than
		ldsaw	(_SSD1306_Progress_Bar_Size_Height)
		cpiaw	5											;min size height 5			
		lbrlt	_SSD1306_Draw_Progress_Bar_Exit				;exit is less than
		//
		//Draw Border
		//
		ldsw	X,(_SSD1306_Progress_Bar_Coord_X)			;get coords
		ldsw	Y,(_SSD1306_Progress_Bar_Coord_Y)
		ldsaw	(_SSD1306_Progress_Bar_Size_Width)			;get Sizes		
		ldsawt	(_SSD1306_Progress_Bar_Size_Height)
		push	Acc
		ldi		Acc,_WHITE									;operator=white
		call	_SSD1306_Set_Operator
		pop		Acc
		rcall	_SSD1306_Draw_Box							;Draw Border Line
		//
		//Draw Black Inside border
		//
		ldsw	X,(_SSD1306_Progress_Bar_Coord_X)			;get coords
		adiw	XH:XL,1										;front border
		ldsw	Y,(_SSD1306_Progress_Bar_Coord_Y)
		adiw	YH:YL,1										;fronte border
		ldsaw	(_SSD1306_Progress_Bar_Size_Width)			;get Sizes		
		subiaw	2											;compensate both sides
		ldsawt	(_SSD1306_Progress_Bar_Size_Height)
		subiawt	2											;compsesate both sides
		push	Acc
		ldi		Acc,_BLACK									;operator=Black(Inside)
		call	_SSD1306_Set_Operator
		pop		Acc
		rcall	_SSD1306_Draw_Box_Full						;Draw Inside Border
		//
		//Draw Progress Bar,Computed as below
		//
		//	ProgressBarWidth=(Value-MinValue)*(width-4))/(MaxValue-MinValue)
		//
		ldsw	X,(_SSD1306_Progress_Bar_Coord_X)			;get coords
		adiw	XH:XL,2										;front border
		ldsw	Y,(_SSD1306_Progress_Bar_Coord_Y)
		adiw	YH:YL,2										;fronte border
		ldsaw	(_SSD1306_Progress_Bar_Size_Width)			;get Sizes		
		subiaw	4											;compensate both sides
		movw	r1:r0,AccH:Acc								;move width to 1st operand
		ldsaw	(_SSD1306_Progress_Bar_Value)				;load value
		ldsawt	(_SSD1306_Progress_Bar_Range_Min_Value)		;value-minvalue
		sub		Acc,AccT
		sbc		AccH,AccTH
		movw	r3:r2,AccH:Acc								;move vlaue to 2nd operand
		call	_MUL_S16S16S32								;value*(width-4)
		movw	r1:r0,r4:r5									;move result to dividend
		movw	r3:r2,r7:r6
		ldsaw	(_SSD1306_Progress_Bar_Range_Max_Value)		;(MaxValue-MinValue)
		ldsawt	(_SSD1306_Progress_Bar_Range_Min_Value)
		sub		Acc,AccT
		sbc		AccH,AccTH
		movw	r5:r4,AccH:Acc								;move (max-min) to divisor
		clr		r6
		clr		r7
		call	_DIV_S32S32S32								;progress_bar_width=(value*(width-4))/(max_value-min_value)
		movw	AccH:Acc,R1:R0								;
		ldsawt	(_SSD1306_Progress_Bar_Size_Height)			;get Height
		subiawt	4											;compsesate both sides
		push	Acc
		ldi		Acc,_WHITE									;operator=White(Progress Bar)
		call	_SSD1306_Set_Operator
		pop		Acc
		rcall	_SSD1306_Draw_Box_Full						;Draw Inside Border
	_SSD1306_Draw_Progress_Bar_Exit:
		popw	Y
		popw	X
		popawt
		popaw
		pop		Temp
		sts		(_SSD1306_Operator),Temp					;restore operator
		pop		Temp
		ret
		ret
#endif

.exit


