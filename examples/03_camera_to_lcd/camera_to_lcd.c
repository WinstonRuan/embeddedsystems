/** \anchor camera
 *
 * @brief Grab frame from camera. 
 *
 * @file

An example to grab a frame from the pobeye camera and draw this frame on the LCD screen.

 * @author Foucault Alisson
 */
																		
//POB-Eye include
#include <pob-eye.h> 


int main (void)
{

	//LCD buffer, 128*64 pixels, 1 byte per pixel
	UInt8 LCD_Buffer [64*64*BYTES];
	
	// Graphic buffer
	GraphicBuffer LCD_Screen; 
	
	// Frame of camera
	UInt8 * FrameFromCam;

	//Initialize POB-Eye and LCD screen
	InitPobeye2();
	InitI2C(I2C_100_KHZ);
	InitCameraPobeye2();
	InitLCD();


	// Init the graphic buffer : 64 per 64, 1 bytes per pixel and LCD_Buffer
	InitGraphicBuffer(&LCD_Screen,64,64,EIGHT_BITS,LCD_Buffer);

	// Clear the graphic buffer
	ClearGraphicBuffer(&LCD_Screen);
	
	// Get the pointer of the red,green and blue video buffer
	FrameFromCam = GetRGBFrame();

	while (1)
	{
		// Grab the RGB components
		GrabRGBFrame();
		
		// Binary the RGB buffer
		BinaryRGBFrame(FrameFromCam);

		// Put the pixel from the red component witch is binary, to the lcd buffer
		DrawComponentInABufferVideo(FrameFromCam,&LCD_Screen);

		// Draw Left LCD with the camera frame
		DrawLeftLCD(&LCD_Screen);
	}

	return 0;
}
