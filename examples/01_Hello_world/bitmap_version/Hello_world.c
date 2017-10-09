/** \anchor hello1
 *
 * @brief Hello world (on LCD screen). 
 *
 * @file

 An example to draw Hello World in the POB-LCD128 screen

 * @author Foucault Alisson
 */

#include <pob-eye.h>
#include "Bitmap.h"  // include bitmap list 

int main (void)
{	
	//Initialize Buffer
	UInt8 LCD_Buffer [LCD_WIDTH*LCD_HEIGHT*BITS] ; 
	GraphicBuffer ScreenBuffer ;

	//Initialize System
	InitPobeye2();
	InitLCD();

	// Init the Graphic buffer with 128 per 64, one pixel per bit and LCD_Buffer 
	InitGraphicBuffer( &ScreenBuffer, LCD_WIDTH,LCD_HEIGHT,ONE_BIT,LCD_Buffer);

	// Clear the graphic buffer 
	ClearGraphicBuffer(&ScreenBuffer);

	//Draw the picture on the buffer
	DrawBitmap(0,0,IDB_HELLO_WORLD,bitmap,&ScreenBuffer);
	
	//Draw buffer in the LCD screen.
	DrawLCD(&ScreenBuffer);
	
	return 0;
}
