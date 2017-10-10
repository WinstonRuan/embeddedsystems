///////////////////////////////////////////////// 
//Include
/////////////////////////////////////////////////
#include <pob-eye.h>
#include "pattern.h"	//dictionary of forms
#include "pad.h"
#include "EventManager\EventManager.h"
#include "logging.h"
#include "pobMover.h"

/////////////////////////////////////////////////
//Macro Defines
/////////////////////////////////////////////////
#define READ_POB_EYE_PERIOD      2000
#define POB_SYMBOL_DETECT_PERIOD 2000
#define POB_SYMBOL_ACTION_PERIOD 100

/////////////////////////////////////////////////
//Structs and Enums
/////////////////////////////////////////////////

typedef enum   //Add new symbols here
{
	SYMBOL_CROSS,
	SYMBOL_TOWER,
	SYMBOL_TRIANGLE,
	NUM_SYMBOLS	
}
SYMBOL_TYPE_T;

typedef enum //IDs to be assigned to each function pointer used by the EventManager
{
	READ_POB_EYE_EVT_ID,
	POB_SYMBOL_DETECT_EVT_ID,
	POB_SYMBOL_ACTION_EVT_ID
}
EVENT_ID_T;

/////////////////////////////////////////////////
//Forward Declarations
/////////////////////////////////////////////////
void InitPobProto (void);
void MoveBot(UInt8 Way);
void PobSymbolAction();
void PobSymbolDetect();
void ReadPobEye();

/////////////////////////////////////////////////
//Local Data
/////////////////////////////////////////////////
static int    s_PobEyeSymbol=0;
static Int16  s_Nb_Identify=0;            //Number of form 
static Form   s_ListOfForm[MAX_OF_FORM];  //array used to save the ID of the forms which are recognized
static UInt8 *s_FrameFromCam;		       //Struct to get the RGB components 
	
int main(void)
{
	InitPobeye2();    	  //init POB-EYE
	InitUART0(115200);    //init UART for terminal serial I/O
	EM_Init();            //init EventManager
	InitI2C(I2C_100_KHZ); //init I2C Bus
	InitPobProto();       //init pob-proto
	InitCameraPobeye2();
	
	// Get the pointer of the red,green and blue video buffer
	s_FrameFromCam = GetRGBFrame();
	
	EM_StartPeriodicEvent(2,
	                      1000,
	                      PobSymbolAction);

	EM_StartPeriodicEvent(0,     
	                      2000,
	                      ReadPobEye);      	                      
	EM_StartPeriodicEvent(1,
					  2000,
					  PobSymbolDetect); 
	
	SendBufferToUART0((unsigned char *)"Starting Program\n",16);
	
	while(1)
	{
		EM_Run();
	}
	return 0;
}

void ReadPobEye()
{
	PrintToUart0( "Entered ReadPobEye\n");				

	// Grab the RGB components
	GrabRGBFrame();
	
	// Binary the RGB buffer
	BinaryRGBFrame(s_FrameFromCam);
		
}
void PobSymbolDetect()
{
	// Try to identify the forms and make a list of it
//	s_Nb_Identify=IdentifyForm(s_FrameFromCam,s_ListOfForm,pattern);
	
	PrintToUart0( "Entered PobSymbolDetect\n");				
										
//	int i;
	//parses Nb_Identify for the right symbol
//	for(i=0; i<s_Nb_Identify; i++)
//	{
//		switch(s_ListOfForm[i].id)
//		{
//		//Cases that tries to get matched
//		case IDP_CROSS:
//			s_PobEyeSymbol = SYMBOL_CROSS;
//			SendBufferToUART0((unsigned char *)"Got Symbol Cross",16);
//			break;
//		case IDP_TOWER:
//			s_PobEyeSymbol = SYMBOL_TOWER;
//			SendBufferToUART0((unsigned char *)"Got Symbol Tower",16);
//			break;
//		case IDP_TRIANGLE:
//			s_PobEyeSymbol = SYMBOL_TRIANGLE;
//			SendBufferToUART0((unsigned char *)"Got Symbol Triangle",19);
//			break;
//		default:
//			SendBufferToUART0((unsigned char *)"Got Symbol None",15);
//			break;
//		}
//	}
}

void PobSymbolAction()
{
	PrintToUart0( "Entered PobSymbolAction\n");				

//	switch(s_PobEyeSymbol)
//	{
//		case SYMBOL_CROSS:
//			MoveBot(RUN);
//			break;
//		case SYMBOL_TOWER:
//			MoveBot(LEFT);
//			break;			
//		case SYMBOL_TRIANGLE:
//			MoveBot(RIGHT);
//			break;
//		default:
//			break;					
//	}
}

void MoveBot(UInt8 Way)
{
	SetPortD(Way);
}

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

