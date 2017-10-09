// POB-Technology 2009
//
// File : Functions.c
// Author : Foucault Alisson
//
// This file is the .h needed to print the value of the joystick on the LCD screen and to 
// move the bot. 
//

#ifndef _PAD
#define PAD

#define RIGHT_LEFT_AXE	1		//the right-left axe is on the RA1 analogic input
#define UP_DOWN_AXE	0		//the up-down axe is on the RA0 analogic input
#define BUTTON	4						//the button is on the RA4 analogic input

#define RUN 0x05
#define STOP 0
#define LEFT 0x09
#define RIGHT 0x06
#define BACK 0x0A

void InitPobProto();
void MoveBot(UInt8 Way);

#endif
