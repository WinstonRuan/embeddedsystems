//
// POB-Technology 2008
// Project: Dualbot demo program
//
#include <pob-eye.h>

#include "pad.h"

/**< global cursor variable: */
extern UInt8 X_Cursor;	
extern UInt8 Y_Cursor;

/** @brief Get position of joystick and manage X/Y cursor coordinates
 *
 * @return 1 if cursor has moved, else return 0
 */
UInt8 GetPad(void)
{
	UInt8 exit=0;
	if (GetAnalog(RIGHT_LEFT_AXE)>200)		//test the right_left axe
	{		
		Y_Cursor-=2;	//move the cursor to the right
		exit=1;
	}

	else if (GetAnalog(RIGHT_LEFT_AXE)<50)  //test the right_left axe
	{		
		Y_Cursor+=2;	//move the cursor to the left
		exit=1;
	}

	if (GetAnalog(UP_DOWN_AXE)>200)			//test the up_down axe
	{		
		X_Cursor+=2;	//move the cursor to the top
		exit=1;
	}

	else if (GetAnalog(UP_DOWN_AXE)<50) //test the up down axe
	{		
		X_Cursor-=2;	//move the cursor to the bottom
		exit=1;
	}

	return exit;
}


/** @brief Get state of button 
 *
 * @return 1 if it's true, else return 0 
 */
UInt8 GetButton (void)
{
	UInt8 data = GetDualInput(INPUT_PORTB,INPUT_7);

	if (data==1)
	{		
		return 1;
	}
	else
	{		
		return 0;
	}
}
