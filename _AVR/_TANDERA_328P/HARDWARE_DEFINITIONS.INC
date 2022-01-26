
/*
	#-----------------------------------------------------------------------#
	|		       _   _               _									|
    |             | | | |             | |									|
    |             | |_| | __ _ _ __ __| |_      ____ _ _ __ ___				|
    |             |  _  |/ _` | '__/ _` \ \ /\ / / _` | '__/ _ \			|
    |             | | | | (_| | | | (_| |\ V  V / (_| | | |  __/			|
    |             \_| |_/\__,_|_|  \__,_| \_/\_/ \__,_|_|  \___|			|
    |																		|
    |																		|
    |            ______      __ _       _ _   _								|
    |            |  _  \    / _(_)     (_) | (_)							|
    |            | | | |___| |_ _ _ __  _| |_ _  ___  _ __  ___				|
    |            | | | / _ \  _| | '_ \| | __| |/ _ \| '_ \/ __|			|
    |            | |/ /  __/ | | | | | | | |_| | (_) | | | \__ \			|
    |            |___/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/			|
    |																		|
	#-----------------------------------------------------------------------#                                                           
	| Todos os Drivers, Objectos, Rotinas que dependem de hardware deverao  |
	| ser definidas aqui.													|
	#-----------------------------------------------------------------------#
	| Este documento usa TAB 4 para formatação								|
	#-----------------------------------------------------------------------#
	| Date: Brasilia ,23 de Setembro de 2016								|
	#-----------------------------------------------------------------------#
 */
 /*
	.INCLUDE    "OBJECT_BUZZER.INC"
    .INCLUDE    "AT45DB161.INC"
    .INCLUDE    "EEPROM.INC"  
	.INCLUDE    "OBJECT_LEDS_F.INC" 
	                                                          
;--------------------------------------------------------------------------------------------
;|                      AT45DB161 CONFIGURATION 
												
 .EQU	_AT45DB161B_PORT_OUTPUT	= PORTB			;data ports				
 .EQU	_AT45DB161B_PORT_DIR	= DDRB								
 .EQU	_AT45DB161B_PORT_INPUT	= PINB								
												
 .EQU	_AT45DB161B_RESET_OUTPUT= PORTB			;reset ports				
 .EQU	_AT45DB161B_RESET_DIR	= DDRB								
 .EQU	_AT45DB161B_RESET_INPUT	= PINB								
												
 .EQU	_AT45DB161B_WP_OUTPUT	= PORTB			;wp ports				
 .EQU	_AT45DB161B_WP_DIR	= DDRB								
 .EQU	_AT45DB161B_WP_INPUT	= PINB								
												
 .EQU	_AT45DB161B_SO_BIT	= 4								
 .EQU	_AT45DB161B_SI_BIT	= 3								
 .EQU	_AT45DB161B_SCK_BIT	= 5								
 .EQU	_AT45DB161B_CS_BIT	= 1 								
 .EQU	_AT45DB161B_WP_BIT	= 0								
 .EQU	_AT45DB161B_RESET_BIT = 2	


 ; INT0

 .EQU	_INT0_PORT_OUTPUT		= PORTD								 
 .EQU	_INT0_PORT_DIR			= DDRD								 
 .EQU	_INT0_PORT_INPUT		= PIND	
 .EQU   _INT0_BIT               = 2


 	#-------------------------------------------------------------------------------#
	| SSD1306 DRIVE																	|
	#-------------------------------------------------------------------------------#
	| DEFINIÇÃO DAS PORTAS E PINOS USADOS PELO DRIVE								|
	#-------------------------------------------------------------------------------#
*/
		
		//SLC AND SDA PORTS AND BIT
/*
		.EQU	_SSD1306_PORT_OUTPUT		= PORTC
		.EQU	_SSD1306_PORT_DIR			= DDRC
		.EQU	_SSD1306_PORT_INPUT			= PINC 

		.EQU	_SSD1306_SLC_BIT			= 4
		.EQU	_SSD1306_SDA_BIT			= 3
		
		.EQU	_SSD1306_I2C_ADDRESS		= 0x3C	// 011110+SA0+RW - 0x3C or 0x3D

		.INCLUDE	"_SSD1306_Drive.inc"






	.EXIT


*/


/*
	#-----------------------------------------------------------------------#
	|		       _   _               _									|
    |             | | | |             | |									|
    |             | |_| | __ _ _ __ __| |_      ____ _ _ __ ___				|
    |             |  _  |/ _` | '__/ _` \ \ /\ / / _` | '__/ _ \			|
    |             | | | | (_| | | | (_| |\ V  V / (_| | | |  __/			|
    |             \_| |_/\__,_|_|  \__,_| \_/\_/ \__,_|_|  \___|			|
    |																		|
    |																		|
    |            ______      __ _       _ _   _								|
    |            |  _  \    / _(_)     (_) | (_)							|
    |            | | | |___| |_ _ _ __  _| |_ _  ___  _ __  ___				|
    |            | | | / _ \  _| | '_ \| | __| |/ _ \| '_ \/ __|			|
    |            | |/ /  __/ | | | | | | | |_| | (_) | | | \__ \			|
    |            |___/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/			|
    |																		|
	#-----------------------------------------------------------------------#                                                           
	| Todos os Drivers, Objectos, Rotinas que dependem de hardware deverao  |
	| ser definidas aqui.													|
	#-----------------------------------------------------------------------#
	| Este documento usa TAB 4 para formatação								|
	#-----------------------------------------------------------------------#
	| Date: Brasilia ,23 de Setembro de 2016								|
	#-----------------------------------------------------------------------#
 */
 
	.INCLUDE    "OBJECT_BUZZER.INC"
    .INCLUDE    "AT45DB161.INC"
    .INCLUDE    "EEPROM.INC"  
	.INCLUDE    "OBJECT_LEDS_F.INC" 
	                                                          
;--------------------------------------------------------------------------------------------
;|                      AT45DB161 CONFIGURATION 

  
/*												
 .EQU	_AT45DB161B_PORT_OUTPUT	= PORTB		     	;data ports				
 .EQU	_AT45DB161B_PORT_DIR	= DDRB								
 .EQU	_AT45DB161B_PORT_INPUT	= PINB								
												
 .EQU	_AT45DB161B_PORT_OUTPUTC = PORTC			;data ports				
 .EQU	_AT45DB161B_PORT_DIRC	= DDRC								
 .EQU	_AT45DB161B_PORT_INPUTC	= PINC							

 
 .EQU	_AT45DB161B_RESET_OUTPUT= PORTB		        ;reset ports				
 .EQU	_AT45DB161B_RESET_DIR	= DDRB								
 .EQU	_AT45DB161B_RESET_INPUT	= PINB								
		 		 
 
 .EQU	_AT45DB161B_WP_OUTPUT	= PORTB			    ;wp ports				
 .EQU	_AT45DB161B_WP_DIR	= DDRB								
 .EQU	_AT45DB161B_WP_INPUT	= PINB
 								
												
 .EQU	_AT45DB161B_SO_BIT	= 4								
 .EQU	_AT45DB161B_SI_BIT	= 3								
 .EQU	_AT45DB161B_SCK_BIT	= 5								
 .EQU	_AT45DB161B_CS_BIT	= 1
 								
 .EQU	_AT45DB161B_WP_BIT	= 2							
 .EQU	_AT45DB161B_RESET_BIT = 0	


 ; INT0

 .EQU	_INT0_PORT_OUTPUT		= PORTD								 
 .EQU	_INT0_PORT_DIR			= DDRD								 
 .EQU	_INT0_PORT_INPUT		= PIND	
 .EQU   _INT0_BIT               = 2


 
	#-------------------------------------------------------------------------------#
	| SSD1306 DRIVE																	|
	#-------------------------------------------------------------------------------#
	| DEFINIÇÃO DAS PORTAS E PINOS USADOS PELO DRIVE								|
	#-------------------------------------------------------------------------------#
*/
	/*	
		//SLC AND SDA PORTS AND BIT

		.EQU	_SSD1306_PORT_OUTPUT		= PORTC
		.EQU	_SSD1306_PORT_DIR			= DDRC
		.EQU	_SSD1306_PORT_INPUT			= PINC 

		.EQU	_SSD1306_SLC_BIT			= 5
		.EQU	_SSD1306_SDA_BIT			= 4
		
		.EQU	_SSD1306_I2C_ADDRESS		= 0x3C	// 011110+SA0+RW - 0x3C or 0x3D

		.INCLUDE	"_SSD1306_Drive.inc"







	.EXIT

	*/

	           //    BIPCAR
/*
	#-----------------------------------------------------------------------#
	|		       _   _               _									|
    |             | | | |             | |									|
    |             | |_| | __ _ _ __ __| |_      ____ _ _ __ ___				|
    |             |  _  |/ _` | '__/ _` \ \ /\ / / _` | '__/ _ \			|
    |             | | | | (_| | | | (_| |\ V  V / (_| | | |  __/			|
    |             \_| |_/\__,_|_|  \__,_| \_/\_/ \__,_|_|  \___|			|
    |																		|
    |																		|
    |            ______      __ _       _ _   _								|
    |            |  _  \    / _(_)     (_) | (_)							|
    |            | | | |___| |_ _ _ __  _| |_ _  ___  _ __  ___				|
    |            | | | / _ \  _| | '_ \| | __| |/ _ \| '_ \/ __|			|
    |            | |/ /  __/ | | | | | | | |_| | (_) | | | \__ \			|
    |            |___/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/			|
    |																		|
	#-----------------------------------------------------------------------#                                                           
	| Todos os Drivers, Objectos, Rotinas que dependem de hardware deverao  |
	| ser definidas aqui.													|
	#-----------------------------------------------------------------------#
	| Este documento usa TAB 4 para formatação								|
	#-----------------------------------------------------------------------#
	| Date: Brasilia ,23 de Setembro de 2016								|
	#-----------------------------------------------------------------------#
 */
 
	//.INCLUDE    "OBJECT_BUZZER.INC"
    // .INCLUDE    "AT45DB161.INC"
   // .INCLUDE    "EEPROM.INC"  
	//.INCLUDE    "OBJECT_LEDS_F.INC" 
	                                                          
;--------------------------------------------------------------------------------------------
;|                      AT45DB161 CONFIGURATION 
												
 .EQU	_AT45DB161B_PORT_OUTPUT	= PORTB		     	;data ports				
 .EQU	_AT45DB161B_PORT_DIR	= DDRB								
 .EQU	_AT45DB161B_PORT_INPUT	= PINB								
												
 .EQU	_AT45DB161B_PORT_OUTPUTC = PORTC			;data ports				
 .EQU	_AT45DB161B_PORT_DIRC	= DDRC								
 .EQU	_AT45DB161B_PORT_INPUTC	= PINC							

 
 .EQU	_AT45DB161B_RESET_OUTPUT= PORTC		        ;reset ports				
 .EQU	_AT45DB161B_RESET_DIR	= DDRC								
 .EQU	_AT45DB161B_RESET_INPUT	= PINC								
		 		 
 
 .EQU	_AT45DB161B_WP_OUTPUT	= PORTD			    ;wp ports				
 .EQU	_AT45DB161B_WP_DIR	= DDRD								
 .EQU	_AT45DB161B_WP_INPUT	= PIND
 								
												
 .EQU	_AT45DB161B_SO_BIT	= 4								
 .EQU	_AT45DB161B_SI_BIT	= 3								
 .EQU	_AT45DB161B_SCK_BIT	= 5								
 .EQU	_AT45DB161B_CS_BIT	= 0 
 								
 .EQU	_AT45DB161B_WP_BIT	= 5								
 .EQU	_AT45DB161B_RESET_BIT = 1	


 ; INT0

 .EQU	_INT0_PORT_OUTPUT		= PORTD								 
 .EQU	_INT0_PORT_DIR			= DDRD								 
 .EQU	_INT0_PORT_INPUT		= PIND	
 .EQU   _INT0_BIT               = 2


/*
	#-------------------------------------------------------------------------------#
	| SSD1306 DRIVE																	|
	#-------------------------------------------------------------------------------#
	| DEFINIÇÃO DAS PORTAS E PINOS USADOS PELO DRIVE								|
	#-------------------------------------------------------------------------------#
*/
		
		//SLC AND SDA PORTS AND BIT

		.EQU	_SSD1306_PORT_OUTPUT		= PORTB
		.EQU	_SSD1306_PORT_DIR			= DDRB
		.EQU	_SSD1306_PORT_INPUT			= PINB 

		.EQU	_SSD1306_SLC_BIT			= 1
		.EQU	_SSD1306_SDA_BIT			= 0
		
		.EQU	_SSD1306_I2C_ADDRESS		= 0x3C	// 011110+SA0+RW - 0x3C or 0x3D

		.INCLUDE	"_SSD1306_Drive.inc"






	.EXIT



	

	
	