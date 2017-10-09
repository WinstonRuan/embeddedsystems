// POB-Technology 2009
//
// File : camera_to_lcd.c
// Author : Foucault Alisson 
//
// This file is the .h needed to print the value of the joystick on the LCD screen
//

#ifndef _PAD
#define PAD

#define RIGHT_LEFT_AXE	1		//the right-left axe is on the RA1 analogic input
#define UP_DOWN_AXE	0		//the up-down axe is on the RA0 analogic input
#define BUTTON	4						//the button is on the RA4 analogic input 

void InitPobProto();
void InitAsciiBuffer();
void PrintTextOnPobLCD(int row, int col, char *string, UInt8 *Screen_Buffer);

#endif
