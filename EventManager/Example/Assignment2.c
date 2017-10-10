//	Author: Robert Dykes
//	Date:   10/09/17
//

//Include
#include <pob-eye.h>
#include "EventManager.h"
#include <stdio.h>


//Forward Declarations
void RotateLeft();
void RotateRight();
void MoveForward();
void StopMovingLeft();

int main(void)
{
	//Initialize POB-Eye 2
	InitPobeye2();
	InitUART0(115200);

	EM_Init();
	EM_StartPeriodicEvent(0,1000,RotateLeft);
	EM_StartPeriodicEvent(1,5000,RotateRight);
	EM_StartPeriodicEvent(2,10000,MoveForward);
	EM_StartTimerEvent(3,20000,StopMovingLeft);
	while(1)
	{
		EM_Run();
	}		
		
	return 0;
}

void RotateLeft()
{
	SendBufferToUART0((unsigned char *)"RotateLeft\n", 11);
}
	
void RotateRight()
{
	SendBufferToUART0((unsigned char *)"RotateRight\n", 12);
}
	
void MoveForward()
{
	SendBufferToUART0((unsigned char *)"MoveForward\n", 12);
}

void StopMovingLeft()
{
	SendBufferToUART0((unsigned char *)"StopMovingLeft\n", 15);
	EM_StopPeriodicEvent(0);
}