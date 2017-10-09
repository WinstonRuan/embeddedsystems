/****************************************/
/* POB-TECHNOLOGY						*/
/* file pad.c							*/	
/* 16-02-2006							*/
/* Demo for PobBoTDemo					*/
/* author Pierre Seguin					*/ 
/* e-mail :pseguin@pob-technology.com	*/
/****************************************/
#include "POB-EYE.h"
#include "pad.h"





/**************** externs variables defined in main.c***************/
extern UInt8 X_Cursor;
extern UInt8 Y_Cursor;
/*******************************************************************/


//function to get the position of the joystick and manage the X-Y coordinates of the cursor
//return 1 if the cursor has moved, else return 0
UInt8 GetPad(void)
{
	UInt8 exit=0;
	if (GetPortAnalog(RIGHT_LEFT_AXE)>200)		//test the right_left axe
	{		
		Y_Cursor-=2;	//move the cursor to the right
		exit=1;
	}

	else if (GetPortAnalog(RIGHT_LEFT_AXE)<50)  //test the right_left axe
	{		
		Y_Cursor+=2;	//move the cursor to the left
		exit=1;
	}

	if (GetPortAnalog(UP_DOWN_AXE)>200)			//test the up_down axe
	{		
		X_Cursor+=2;	//move the cursor to the top
		exit=1;
	}

	else if (GetPortAnalog(UP_DOWN_AXE)<50) //test the up down axe
	{		
		X_Cursor-=2;	//move the cursor to the bottom
		exit=1;
	}

	return exit;
}




//function to know if the button is pressed
// return 1 if it's true, else return 0
UInt8 GetButton (void)
{
	if (GetPortAnalog(BUTTON)<100)
	{		
		return 1;
	}
	else
	{		
		return 0;
	}
}
