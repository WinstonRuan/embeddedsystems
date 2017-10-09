/** \anchor form1
 *
 * @brief Form recognition (example 2). 
 *
 * @file

 Example more complex : The LCD screen is divided into 2 'virtual screen : 
  - The left screen display the real time video from the camera.           
  - The second screen display the form recognized by POBEYE                

 * @author Foucault Alisson
 */

#include <pob-eye.h>

//bitmap and form list
#include "bitmap.h" 
#include "pattern.h" 


#define BUFFER_WIDTH	64
#define BUFFER_HEIGHT	64

//function to draw picture on both screen, when a form is recognize
static void DrawWhatIsKnown(Form What);	
//function to draw rectangle arround the form known
static UInt8 DrawRect (Rect Rectangle,const GraphicBuffer *Target);	


UInt8 LCD_Left_Buffer [BUFFER_WIDTH*BUFFER_HEIGHT*BYTES];  // array to store the left lcd screen, one byte per pixel
UInt8 LCD_Right_Buffer [BUFFER_WIDTH*BUFFER_HEIGHT*BITS];  // array to store the right lcd screen one bit per pixel.

GraphicBuffer LCD_Left ;  // left graphic buffer 
GraphicBuffer LCD_Right ; // right graphic buffer 
UInt8 X_Pic=5,Y_Pic=5;	  // x,y to draw pictures of form known on the right LCD screen 	

int main (void)
{	
	Int16 i=0;
	
	// Number of form 
	Int16 Nb_Identify=0 ;   
	// List of forms
	Form  ListOfForm[MAX_OF_FORM];  
	// RGB Frame of camera
	UInt8 * FrameFromCam ;

	//Initialize POB-EYE and LCD Screen
	InitPobeye2();
	InitI2C(I2C_100_KHZ);
	InitCameraPobeye2();
	InitLCD();
			
	// Init Left and Right screens buffer
	InitGraphicBuffer( &LCD_Left,BUFFER_WIDTH,BUFFER_HEIGHT,EIGHT_BITS,LCD_Left_Buffer);
	InitGraphicBuffer( &LCD_Right,BUFFER_WIDTH,BUFFER_HEIGHT,ONE_BIT,LCD_Right_Buffer);

	// Clear the Left and Right screen
	ClearGraphicBuffer(&LCD_Left); 
	ClearGraphicBuffer(&LCD_Right);
	
	// Get the pointer of the red,green and blue video buffer 
	FrameFromCam = GetRGBFrame();

	while(1)
	{
		// Init the x,y of the first picture
		X_Pic=8;
		Y_Pic=8;
		
		// draw a frame in right buffer
		DrawBitmap(0,0,IDB_7_FRAME,bitmap,&LCD_Right);			

		// Grab the RGB components
		GrabRGBFrame();				
		
		// Binary the three RGB Buffer
		BinaryRGBFrame(FrameFromCam); 

		// draw red component in left buffer
		DrawComponentInABufferVideo(FrameFromCam, &LCD_Left );

		// try to identify the forms and make a list of it.
		Nb_Identify=IdentifyForm(FrameFromCam,ListOfForm,pattern);	
		
		// Parse the list of the form
		for(i=0;i<Nb_Identify;i++)
			DrawWhatIsKnown(ListOfForm[i]);
	
		// Draw the twis LCDs screens
		DrawLeftLCD(&LCD_Left);		//the left for the real time video
		DrawRightLCD(&LCD_Right);	//the right for the result
	}
	return 0;
}



void DrawWhatIsKnown (Form What)
{
	Rect Target;
	
	// draw the frame
	DrawBitmap(X_Pic,Y_Pic,IDB_8_LITTLE_FRAME,bitmap,&LCD_Right);   
	
	// the picture of what is known in the frame
	DrawBitmap(X_Pic+3,Y_Pic+3,What.id -1,bitmap,&LCD_Right); 
	 
	//manage the (x,y) To draw the next known form on the right lcd screen
	X_Pic+=28;
	if((X_Pic+28)>BUFFER_WIDTH)
	{
		Y_Pic+=26;
		X_Pic=8;
	}

	// draw a rectangle arround the known form on the left lcd screen
	
	Target.x=What.x - (What.width/2);		// manage the upper left corner of the form
	Target.y=What.y - (What.height/2);		
	Target.width=What.width;
	Target.height=What.height;
	
	DrawRect(Target,&LCD_Left);			// Draw the rectangle arround the form known.
}

UInt8 DrawRect (Rect Rectangle,const GraphicBuffer *Target)
{
	UInt8 i=0,X=0,Y=0,W=2,H=2;
	UInt16 tmp=0,SizeOfBuffer=0;
	
	SizeOfBuffer=Target->width*Target->height;

	/*******************************************************************/
	/* we have to manage a ratio between the video buffer of the frame        */
	/* from the cam (88*120 pixels), and the LCD screen (64*64 pixels)          */
	/*******************************************************************/

	X=(Rectangle.x*186)>>8;
	Y=(Rectangle.y*135)>>8;
	W+=((Rectangle.width)*186)>>8;
	H+=((Rectangle.height)*135)>>8;
	

	//draw the 2 horizontals lines
	for (i=0;i<W;i++)
	{
		tmp=X+Y*Target->width+i;  
		if (tmp>(SizeOfBuffer)) return 0;
		Target->buffer[tmp]=0xFF;

		tmp+=H*Target->width;	
		if (tmp>(SizeOfBuffer)) return 0;
		Target->buffer[tmp]=0xFF;
	}

	//draw the 2 verticals lines 
	for (i=0;i<H;i++)
	{
		tmp=X;
		tmp+=((i+Y)*Target->width);
		if (tmp>(SizeOfBuffer)) return 0;
		Target->buffer[tmp]=0xFF;

		tmp+=W;
		if (tmp>(SizeOfBuffer)) return 0;
		Target->buffer[tmp]=0xFF;
	}
	
	/****************************************************************/
	/* Drawing Horizontal line or vertical with a "for" instruction */
	/* it's more faster than using the DrawLine function            */
	/****************************************************************/

	return 1;
}
