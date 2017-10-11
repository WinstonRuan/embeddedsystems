///////////////////////////////////////////////// 
//Include
/////////////////////////////////////////////////
#include <pob-eye.h>
#include <timer.h>
#include "pattern.h"	//dictionary of forms
#include "bitmap.h"
#include "pad.h"
#include "EventManager\EventManager.h"
#include "logging.h"
#include "pobMover.h"

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
static UInt8 LCD_Buffer [LCD_WIDTH*LCD_HEIGHT*BITS] ;
static GraphicBuffer ScreenBuffer ;
	
int main(void)
{
	InitPobeye2();    	  //init POB-EYE
	InitUART0(115200);    //init UART for terminal serial I/O
	InitI2C(I2C_100_KHZ); //init I2C Bus
	InitLCD();
	InitPobProto();       //init pob-proto
	InitCameraPobeye2();
	
	// Get the pointer of the red,green and blue video buffer
	s_FrameFromCam = GetRGBFrame();
	
	// Init the Graphic buffer with 128 per 64, one pixel per bit and LCD_Buffer
	InitGraphicBuffer( &ScreenBuffer, LCD_WIDTH,LCD_HEIGHT,ONE_BIT,LCD_Buffer);

	// clear the graphic buffer
	ClearGraphicBuffer(&ScreenBuffer);

	
	SendBufferToUART0((unsigned char *)"Starting Program\n",17);
	
	while(1)
	{
		//EM_Run();
		//PobSymbolAction();
		ReadPobEye();
		PobSymbolDetect();
	}
	return 0;
}

void ReadPobEye()
{			
	// Grab the RGB components
	GrabRGBFrame();
	
	// Binary the RGB buffer
	BinaryRGBFrame(s_FrameFromCam);
}
void PobSymbolDetect()
{
	// Try to identify the forms and make a list of it
	s_Nb_Identify=IdentifyForm(s_FrameFromCam,s_ListOfForm,pattern);
												
	int i;
	//parses Nb_Identify for the right symbol
	for(i=0; i<s_Nb_Identify; i++)
	{
		switch(s_ListOfForm[i].id)
		{
#ifdef IDP_0_CROSS
		case IDP_0_CROSS:
			// Draw bitmap on the buffer and the LCD screen
			DrawBitmap(0,0,IDB_CROSS,bitmap,&ScreenBuffer);
			DrawLCD(&ScreenBuffer);
			MoveBot(RUN);
			WaitUs(500000);
			MoveBot(STOP);
			break;
#endif

#ifdef IDP_2_KING			
		case IDP_2_KING:
			DrawBitmap(0,0,IDB_KING,bitmap,&ScreenBuffer);
			DrawLCD(&ScreenBuffer);
			break;
#endif

#ifdef IDP_3_TOWER						
		case IDP_3_TOWER:
			DrawBitmap(0,0,IDB_TOWER,bitmap,&ScreenBuffer);
			DrawLCD(&ScreenBuffer);
			break;
#endif

#ifdef IDP_7_TRIANGLE45		
		case IDP_7_TRIANGLE45:
			DrawBitmap(0,0,IDB_TRIANGLE,bitmap,&ScreenBuffer);
			DrawLCD(&ScreenBuffer);
			break;
#endif

#ifdef IDP_5_TRIANGLE			
		case IDP_5_TRIANGLE:
			DrawBitmap(0,0,IDB_TRIANGLE,bitmap,&ScreenBuffer);
			DrawLCD(&ScreenBuffer);
			MoveBot(LEFT);
			WaitUs(500000);
			MoveBot(STOP);
			break;
#endif
		break;
#ifdef IDP_6_CIRCLE
		case IDP_6_CIRCLE:
			DrawBitmap(0,0,IDB_CIRCLE,bitmap,&ScreenBuffer);
			DrawLCD(&ScreenBuffer);
			MoveBot(RIGHT);
			WaitUs(1000000);
			MoveBot(STOP);
			break;
#endif
		default:
			break;
		}
	}
	if (s_Nb_Identify == 0)
		{
		DrawBitmap(0,0,IDB_NOFORMS,bitmap,&ScreenBuffer);
		DrawLCD(&ScreenBuffer);
		}
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


	//all pin of PORTC are configured to manage servomotors
	Proto.portc=RC7_AS_SERVO	| RC6_AS_SERVO |RC3_AS_SERVO |RC2_AS_SERVO|RC1_AS_SERVO |RC0_AS_SERVO;	
	//SetServoMotor(RC0_AS_SERVO, 100);

	//RD0 RD1 RD2 RD3 are configured as digitals output to gear DC motor, RD4 RD5 RD6 RD7 are configured as digitals input
	Proto.portd=RD7_AS_DI	| RD6_AS_DI	|RD5_AS_DI |RD4_AS_DI|RD3_AS_DO	|RD2_AS_DO	|RD1_AS_DO	|RD0_AS_DO;		
	
	//set the pob proto
	SetPobProto(&Proto);	
}

