/** \anchor proto3
 *
 * @brief POB-Proto example 3. 
 *
 * @file

An example to move the POB-BOT with the joystick.

 * @author Foucault Alisson
 */  

#include <pob-eye.h>
#include "pad.h"

// external functions declarations (for PrintTextOnPobLCD)
extern void InitAsciiBuffer();
extern void PrintTextOnPobLCD(int row, int col, char *string, UInt8 *Screen_Buffer);
extern void HLightText(int row, int col, int size, UInt8 *Screen_Buffer);

int	main(void)
{	
	//variable declaration (128 is the center of the LCD screen)
	int			top_bottom_axe = 128;
	int			right_left_axe = 128;
	char		top_bottom[10]; 
	char		right_left[10];

	//This buffers will stock the pixels to display, 
	GraphicBuffer		LCD_Buffer_Video;
	UInt8					LCD_Buffer[LCD_WIDTH*LCD_HEIGHT*BITS];
	UInt8					 ASCII_Buffer[LCD_WIDTH*LCD_HEIGHT*BITS];  // Buffer to stock the ascii table in bitmap format

	//Initialize POB-EYE (lib), POB-LCD (lib), POB-PROTO(file Functions.c) and Servomotors
	InitPobeye2();
	InitLCD();			
	InitPobProto();
	SwitchOnAllServo();

	// Initialize the Graphic buffer with 128 per 64, one pixel per bit and LCD_Buffer
	InitGraphicBuffer( &LCD_Buffer_Video, LCD_WIDTH,LCD_HEIGHT,ONE_BIT,LCD_Buffer);

	//Clear and draw the buffer to make clean the screen
	ClearGraphicBuffer(&LCD_Buffer_Video);
	DrawLCD(&LCD_Buffer_Video);
	
	// Init Ascii buffer, use to write in the LCD screen with PrintTextOnPobLCD function
	InitAsciiBuffer(); 

	while (1)
	{		
		//get the values of pad
		top_bottom_axe = GetPortAnalog(UP_DOWN_AXE);
		right_left_axe = GetPortAnalog(RIGHT_LEFT_AXE);
		
		if (top_bottom_axe > 200)
			MoveBot(RUN);
		else if (top_bottom_axe < 100)
			MoveBot(BACK);
		else if (right_left_axe >200)
			MoveBot(RIGHT);
		else if (right_left_axe < 100)
			MoveBot(LEFT); 
		else
			MoveBot(STOP);

	    //convert it from int to string
		PrintToABuffer(top_bottom, "%d", top_bottom_axe); 
		PrintToABuffer(right_left, "%d", right_left_axe);
		
		//display it
		PrintTextOnPobLCD(1, 2, "Top", LCD_Buffer);
		PrintTextOnPobLCD(2, 1, "Bottom", LCD_Buffer);
		PrintTextOnPobLCD(5, 3, top_bottom, LCD_Buffer);
		PrintTextOnPobLCD(1, 8, "Right", LCD_Buffer);
		PrintTextOnPobLCD(2, 8, "Left", LCD_Buffer);
		PrintTextOnPobLCD(5, 8, right_left, LCD_Buffer);
		DrawLCD(&LCD_Buffer_Video);
	}
return 0;	
}
