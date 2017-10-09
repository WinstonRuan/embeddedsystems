/****************************************/
/* POB-TECHNOLOGY						*/
/* fichier pad.h						*/	
/* 16-02-2006							*/
/* Demo for PobBoTDemo					*/
/* author Pierre Seguin					*/ 
/* e-mail :pseguin@pob-technology.com	*/
/****************************************/
#include "POB-EYE.h"





#ifndef _PAD
#define PAD


#define RIGHT_LEFT_AXE	0	//the right-left axe is on the RA0 analogic input
#define UP_DOWN_AXE	1		//the up-down axe is on the RA1 analogic input
#define BUTTON	4			//le button est sur l'entré analogique RA4

//Function to manage the cursor. Return 1 if it has moved, else return 0
UInt8 GetPad(void);

//Function to manage the button. Return 1 if the button is pressed, else return 0.
UInt8 GetButton (void);

#endif
