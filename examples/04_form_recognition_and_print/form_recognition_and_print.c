/** \anchor form0
 *
 * @brief Form recognition. 
 *
 * @file

 Example of form recognition and print the result on UART and the LCD Screen.

 * @author Foucault Alisson
 */

#include <pob-eye.h>

// Include bitmap list and dictionnary of forms
#include "Bitmap.h" 
#include "pattern.h"

int main (void)
{	
	UInt8 i=0,Nb_Identify=0;
	
	// List of form
	Form ListOfForm[MAX_OF_FORM];

	// Struct of three pointers on the RGB components
	UInt8 * FrameFromCam;

	//The pixels will be stocked in the LCD buffer
	UInt8 LCD_Buffer [LCD_WIDTH*LCD_HEIGHT*BITS] ;
	GraphicBuffer ScreenBuffer ;

	// System and LCD screen initialization
	InitPobeye2();
	InitI2C(I2C_100_KHZ);
	InitCameraPobeye2();
	InitLCD();	

	// Get the pointer of the red,green and blue video buffer
	FrameFromCam = GetRGBFrame();
		
	// Init the Graphic buffer with 128 per 64, one pixel per bit and LCD_Buffer
	InitGraphicBuffer( &ScreenBuffer, LCD_WIDTH,LCD_HEIGHT,ONE_BIT,LCD_Buffer);

	// clear the graphic buffer
	ClearGraphicBuffer(&ScreenBuffer);

	while(1)
	{		
		// grab the RGB components
		GrabRGBFrame();				
		
		// Binary the three RGB Buffer
		BinaryRGBFrame(FrameFromCam); 
		
		// Try to identify the forms and make a list of it
		Nb_Identify=IdentifyForm(FrameFromCam,ListOfForm,pattern);	

		// Parse the list of the form and print result on the Pob-Terminal and the LCD Screen
		for (i=0;i<Nb_Identify;i++)
		{
			switch (ListOfForm[i].id)
			{
			case IDP_0_CROSS:
				// Draw bitmap on the buffer and the LCD screen
				DrawBitmap(0,0,IDB_CROSS,bitmap,&ScreenBuffer);
				DrawLCD(&ScreenBuffer);
			break;
				
			case IDP_1_BIGA:
				DrawBitmap(0,0,IDB_BIGA,bitmap,&ScreenBuffer);
				DrawLCD(&ScreenBuffer);
			break;

			case IDP_2_KING:
				DrawBitmap(0,0,IDB_KING,bitmap,&ScreenBuffer);
				DrawLCD(&ScreenBuffer);
			break;

			case IDP_3_TOWER:
				DrawBitmap(0,0,IDB_TOWER,bitmap,&ScreenBuffer);
				DrawLCD(&ScreenBuffer);
			break;
			
			case IDP_4_TREFLE:
				DrawBitmap(0,0,IDB_BIGA,bitmap,&ScreenBuffer);
				DrawLCD(&ScreenBuffer);
			break;
			
			case IDP_5_TRIANGLE:
				DrawBitmap(0,0,IDB_TRIANGLE,bitmap,&ScreenBuffer);
				DrawLCD(&ScreenBuffer);
			break;
			default:
			break;
			}				
		}		
	if (Nb_Identify == 0)
		{
		DrawBitmap(0,0,IDB_NOFORMS,bitmap,&ScreenBuffer);
		DrawLCD(&ScreenBuffer);
		}
	}
	return 0;
}

