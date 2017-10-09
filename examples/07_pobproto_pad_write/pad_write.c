/** \anchor proto1
 *
 * @brief POB-Proto example 1. 
 *
 * @file

 An example to print the value of the POB-PROTO joystick on the LCD screen 

 * @author Foucault Alisson
 */ 
#include <pob-eye.h>
#include "pad.h"

// external functions declarations (for PrintTextOnPobLCD)
extern void InitAsciiBuffer();
extern void PrintTextOnPobLCD(int row, int col, char *string, UInt8 *Screen_Buffer);

int	main(void)
{	
	//variable declaration (128 is the center of the LCD screen)
	UInt8 top_bottom_axe = 128;
	UInt8 right_left_axe = 128;
	int button = 0;
	char top_bottom[10]; 
	char right_left[10];

	//This buffers will store the pixels to display, 
	GraphicBuffer		LCD_Buffer_Video;
	UInt8					LCD_Buffer[LCD_WIDTH*LCD_HEIGHT*BITS];
	UInt8					ASCII_Buffer[LCD_WIDTH*LCD_HEIGHT*BITS];  // Buffer to stock the ascii table in bitmap format

	//Initialize POB-EYE (lib), POB-LCD (lib) and POB-PROTO(source code at end of this file)
	InitPobeye2();
	InitLCD();			
	InitPobProto();

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
		button = GetPortAnalog(BUTTON);

		if (button >=100)
		{
				ClearGraphicBuffer(&LCD_Buffer_Video);
				//convert it from int to char*
				PrintToABuffer(top_bottom, "%d", top_bottom_axe); 
				PrintToABuffer(right_left, "%d", right_left_axe);

				//Display values
				PrintTextOnPobLCD(1, 2, "Top", LCD_Buffer);
				PrintTextOnPobLCD(2, 1, "Bottom", LCD_Buffer);
				PrintTextOnPobLCD(5, 3, top_bottom, LCD_Buffer);
				PrintTextOnPobLCD(1, 8, "Right", LCD_Buffer);
				PrintTextOnPobLCD(2, 8, "Left", LCD_Buffer);
				PrintTextOnPobLCD(5, 8, right_left, LCD_Buffer);
				DrawLCD(&LCD_Buffer_Video);
		}
		// If button is pressed
		if (button < 100)
		{
				ClearGraphicBuffer(&LCD_Buffer_Video);
				DrawLCD(&LCD_Buffer_Video);
				PrintTextOnPobLCD(4, 1, "Button pressed", LCD_Buffer);
				DrawLCD(&LCD_Buffer_Video);
				WaitMs(100);
				ClearGraphicBuffer(&LCD_Buffer_Video);
				DrawLCD(&LCD_Buffer_Video);
		}
	}
return 0;	
}
