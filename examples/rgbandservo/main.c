

#include <stdlib.h>
#include <pob-eye.h>

#include "communication.h"

#define UART_BUFFER_SIZE 	255		/* uart buffer size */
#define STR_NUMBER			 4		/* string number */

#define CMD_POB		"POB"		/* simple test */
#define CMD_RGB		"RGB"		/* grab a rgb frame */
#define CMD_COMPRESS	"COMPRESS" 	/* compress frame component, parameters: ON/OFF */
#define CMD_SEND		"SEND"		/* send frame, parameters: RED/GREEN/BLUE/HUE/SATURATION/LIGHT */
#define CMD_RUN		"RUN"		/* pob-bot run */
#define CMD_BACK		"BACK"		/* pob-bot back */
#define CMD_LEFT		"LEFT"		/* pob-bot left */
#define CMD_RIGHT		"RIGHT"		/* pob-bot right */
#define CMD_STOP		"STOP"		/* pob-bot stop */
#define CMD_SERVO		"SERVO"		/* set servomotor position, parameters: servo_value position_value */
#define CMD_LCD		"LCD"		/* draw camera frame, parameters: ON */


#define OK			"OK"			/* last operation successfull */
#define NOK			"!OK"

#define ON			"ON"
#define OFF			"OFF"

//pob-proto init
static UInt8 pobProtoOk=0;
static void InitPobProto (void);

//pob-bot movement
#define MOVE_RUN		0x05
#define MOVE_BACK		0x0A
#define MOVE_LEFT		0x09
#define MOVE_RIGHT		0x06
#define STOP_BOT		0

//pob-bot move 
static void MoveBot(UInt8 Way);

static GraphicBuffer LCD_Fast_Buffer_Video;		//graphic buffer for the graphic interface

//set servomotor
static void SetServoRgb(UInt8 servo, UInt8 position);

static void DrawVision (UInt8 *FrameFromCam);

//lcd fast buffer video
static UInt8 LCD_Fast_Buffer[64*64*BYTES];
static GraphicBuffer LCD_Fast_Buffer_Video;

int main(void)
{
	UInt32 i,j;

	UInt32 time0,time1;

	Int32 cnt=0;

	UInt8 *ptr =0;
	int wh = 160*120;

	UInt8 green[160*120];
	UInt8 blue[160*120];

	UInt8 uartBufferSize=UART_BUFFER_SIZE;	//uart buffer size
	UInt8 uartBuffer[UART_BUFFER_SIZE];		//uart buffer
	UInt8 uartSize;							//number of byte receive from uart

	UInt8 stringMax=STR_NUMBER;	//string buffer size
	Int8 stringNumber=0;		//number of string in buffer
	String string[STR_NUMBER];	//string buffer

	int servo,position,temp;

	UInt8 *FrameFromCam;
	UInt8 PrintFrameOnLcd = 0;

	InitPobeye2();
	
	InitUART0(115200);
	InitI2C(I2C_100_KHZ);

//	SendString("Init pobeye2 uart/i2c ok\r\n");

	InitCameraPobeye2();

//	SendString("Init pobeye2 camera ok\r\n");

	FrameFromCam = GetRGBFrame();

	while(1)
	{
		//receive user command
		uartSize=receive(uartBuffer,uartBufferSize);

		//extract the user command and parameters from the uart buffer
		stringNumber=extractString(string,stringMax,uartBuffer,uartSize);

		//command switch:

		if(strCmp(CMD_POB,string[0].buffer,string[0].len)==1)
		{
			//simple test
			send(OK);
		}
		else if(strCmp(CMD_RGB,string[0].buffer,string[0].len)==1)
		{
			//grab a frame from camera sensor
			GrabRGBFrame();
			//GrabFullRGBFrame(green,blue);
			
			cnt=0;
			send(OK);
		}
		else if(strCmp(CMD_RUN,string[0].buffer,string[0].len)==1)
		{
			MoveBot(MOVE_RUN);
			send(OK);
		}
		else if(strCmp(CMD_LEFT,string[0].buffer,string[0].len)==1)
		{
			MoveBot(MOVE_LEFT);
			send(OK);
		}
		else if(strCmp(CMD_RIGHT,string[0].buffer,string[0].len)==1)
		{
			MoveBot(MOVE_RIGHT);
			send(OK);
		}
		else if(strCmp(CMD_BACK,string[0].buffer,string[0].len)==1)
		{
			MoveBot(MOVE_BACK);
			send(OK);
		}
		else if(strCmp(CMD_STOP,string[0].buffer,string[0].len)==1)
		{
			MoveBot(STOP_BOT);
			send(OK);
		}
		else if(strCmp(CMD_COMPRESS,string[0].buffer,string[0].len)==1)
		{
			send(OK);
		}
		else if(strCmp(CMD_SERVO,string[0].buffer,string[0].len)==1)
		{	
			if(stringNumber==3)
			{
				//get servo and position
				servo=(UInt8)strtol((const char*)string[1].buffer,(char **)'\0',10);
				position=(UInt8)strtol((const char*)string[2].buffer,(char **)'\0',10);

				//set servo
				SetServoRgb(servo, position);

				send(OK);
			}
			else
			{
				send(NOK);
			}
		}
		else if(strCmp(CMD_LCD,string[0].buffer,string[0].len)==1)
		{
			//init the pob-lcd
			InitLCD(); 
			//init the graphic buffer video: 64 x 64 pixels
			InitGraphicBuffer(&LCD_Fast_Buffer_Video,64,64,EIGHT_BITS,LCD_Fast_Buffer);
			//clear graphic buffer
			ClearGraphicBuffer(&LCD_Fast_Buffer_Video);
			while (GetByteFromUART0NoWait(&PrintFrameOnLcd)==0) 
			{
				GrabRGBFrame();					//get a new Frame from the cmos sensor
				BinaryRGBFrame(FrameFromCam);	
				DrawVision(FrameFromCam);
			}
			send(OK);
		}
		else if(strCmp(CMD_SEND,string[0].buffer,string[0].len)==1)
		{

			if(cnt==0)
			{
				ptr = GetRGBFrame();
			//	GetHSLFrame(ptr);
			//	GetHSVFrame(ptr);

				//GrayRGBFrame(ptr);
				//BinaryRGBFrame(ptr);
	
				cnt=1;
			}
			else if(cnt==1)
			{
				ptr= green;//GetRgbFrame();
				cnt=2;
			}
			else if(cnt==2)
			{
				ptr=blue; //GetRgbFrame();
			}

		//	ptr = GetRed();

			for (i=0,j=0; i < wh ; i++,j++)
			{
				SendByteToUART0( ptr[i] );
		
				//pob-eye crash if you send to many byte

				if(j==255)
				{
					j=0;
					GetByteFromUART0();
				}
			}

			send(OK);
		}
		else
		{
			send(NOK);
		}
	}


	return 0;
}

//Function to initialize the POB-PROTO board
void InitPobProto (void)
{
	PobProto	Proto; // struct to set the pob-proto
	
	Proto.porta=ALL_PORTA_AS_ANA;	//to get the position of the analogic joystick, you have to set the PORTA as analogic input
	Proto.portc=RC7_AS_SERVO	| RC6_AS_SERVO |RC3_AS_SERVO |RC2_AS_SERVO|RC1_AS_SERVO |RC0_AS_SERVO;	//all pin of PORTC are configured to manage servomotors
	Proto.portd=RD7_AS_DI	| RD6_AS_DI	|RD5_AS_DI |RD4_AS_DI|RD3_AS_DO	|RD2_AS_DO	|RD1_AS_DO	|RD0_AS_DO;		//RD0 RD1 RD2 RD3 are configured as digitals output to gear DC motor, RD4 RD5 RD6 RD7 are configured as digitals input
	SetPobProto(&Proto);	//set the pob proto
}


//function to move the bot
void MoveBot (UInt8 Way)
{	
	if(pobProtoOk==0)
	{
		InitPobProto();
		SwitchOnAllServo();
		pobProtoOk=1;
	}

	SetPortD(Way);
}


//set servo
void SetServoRgb(UInt8 servo, UInt8 position)
{
	if(pobProtoOk==0)
	{
		InitPobProto();
		SwitchOnAllServo();
		pobProtoOk=1;
	}


	if(position==4||position==5)
	{
		SetServoMotor(servo,position+2);
	}
	else
	{
		SetServoMotor(servo,position);
	}
}

void DrawVision (UInt8 *FrameFromCam)
{	
	//draw the red buffer (88*120 pixels) on the Buffer for for the left screen (64*64 pixels)
	DrawComponentInABufferVideo(FrameFromCam,&LCD_Fast_Buffer_Video);
	DrawLeftLCD(&LCD_Fast_Buffer_Video); //refresh the left screen
}