/** 
Timing methods 

 */

#include <pob-eye.h>
#include "pattern.h"	//dictionary of forms
#include "pad.h"

//Function to initialize the POB-PROTO board
void InitPobProto (void)
{
	// struct to set the pob-proto
	PobProto	Proto; 

	//to get the position of the analogic joystick, you have to set the PORTA as analogic input
	Proto.porta=ALL_PORTA_AS_ANA;	

	//all pin of PORTC are configured to manage servomotors
	Proto.portc=RC7_AS_SERVO	| RC6_AS_SERVO |RC3_AS_SERVO |RC2_AS_SERVO|RC1_AS_SERVO |RC0_AS_SERVO;	

	//RD0 RD1 RD2 RD3 are configured as digitals output to gear DC motor, RD4 RD5 RD6 RD7 are configured as digitals input
	Proto.portd=RD7_AS_DI	| RD6_AS_DI	|RD5_AS_DI |RD4_AS_DI|RD3_AS_DO	|RD2_AS_DO	|RD1_AS_DO	|RD0_AS_DO;		
	
	//set the pob proto
	SetPobProto(&Proto);	
}

int main(void)
{
	//init POB-EYE
	InitPobeye2();
	InitI2C(I2C_100_KHZ);
	//pob-proto init
	InitPobProto();
	UInt8 *FrameFromCam;			//Struct to get the RGB components 
	Form  ListOfForm[MAX_OF_FORM];	//array used to save the ID of the forms which are recognized
	// Number of form 
	Int16 Nb_Identify=0 ;
	
	// Get the pointer of the red,green and blue video buffer
	FrameFromCam = GetRGBFrame();
	
	//Main loop for the timings and stuff
	while(1)
	{
		ClockStart();
		
		int clockVal = ClockGet();

		switch(clockVal)
		{
			case 0: //readPOBEye periodic, period =2s,  execution time= 0.2s
				// Grab the RGB components
				GrabRGBFrame();
				
				// Binary the RGB buffer
				BinaryRGBFrame(FrameFromCam);
				int symbol = 0;
				
				//stores the image in memory
				FrameFromCam = GetRGBFrame();

				break;
			case 400: //pobSymbolDetect periodic, period = 2s, execution time= 0.2s
				// Try to identify the forms and make a list of it
				Nb_Identify=IdentifyForm(FrameFromCam,ListOfForm,pattern);
				
				int i;
				//parses Nb_Identify for the right symbol
				for(i=0; i<Nb_Identify; i++)
				{
					switch(ListOfForm[i].id)
					{
						//Cases that tries to get matched
						case IDP_CROSS:
							symbol =2;
							break;
						case IDP_TOWER:
							symbol =3;
							break;
						case IDP_TRIANGLE:
							symbol =4;
							break;
						default:
						break;
					}
				}
				
				break;
			case 100:
				switch(symbol)
				{
					case 1:
						MoveBot(RUN);
						break;
					case 2:
						MoveBot(LEFT);
						break;
						
					case 3:
						MoveBot(RIGHT);
						break;
					default:
					break;
					
					
				}
			case 30:
				break;
			case 200:	
				break;
		}	
	}
	return 0;
}


void MoveBot(UInt8 Way)
{
	SetPortD(Way);
}
