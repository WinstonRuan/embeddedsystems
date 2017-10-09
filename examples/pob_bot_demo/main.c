/****************************************/
/* POB-TECHNOLOGY						*/
/* file main.c							*/	
/* 16-02-2006							*/
/* Demo for PobBoTDemo					*/
/* author Pierre Seguin					*/ 
/* e-mail :pseguin@pob-technology.com	*/
/****************************************/

#include <pob-eye.h>

#include "intro.h"		//bitmaps for the POB-TECHNOLOG's introduction
#include "pattern.h"	//dictionnary of forms
#include "bitmap.h"	//bitmaps for the graphic interface
#include "pad.h"		//header for the pad functions
#include "menu.h"		//header to manage a menu






/**********************************************/
static UInt8 LCD_Buffer		[128*64*BITS];	//space memory to manage the graphique interface: 128*64 pixels, 1 bit per pixel
static UInt8 LCD_Fast_Buffer[64*64*BYTES];	//space memory to show the video from the CMOS sensor in real time : 64*64 pixels, 8 bytes per pixel

static GraphicBuffer LCD_Fast_Buffer_Video;		//graphic buffer for the graphic interface
static GraphicBuffer LCD_Buffer_Video;			//graphic buffer for the real time view 

//For more information, read the chapiter which talks about the POB-LCD128
/**********************************************/









/********************functions used in main.c **********/
void DrawVision (UInt8 *FrameFromCam);
void InitPobProto (void);
void IntroPOB(void);
void MoveBot(UInt8 Way);
/****************************************************************************/


/******Define of the value to set RD0 RD1 RD2 RD3 to gear the robot**********/
#define MOVE_RUN		0x05
#define MOVE_BACK		0x0A
#define MOVE_LEFT		0x09
#define MOVE_RIGHT		0x06
#define STOP_BOT		0
/**************************************************************************************************/




/**********Define to draw the graphic interface on the TestMenu (icon on the right)******************/
#define X_SERVO 15
#define Y_SERVO 5


#define X_ENGINE	30	
#define Y_ENGINE	32
/*****************************************************************************************************/


/******************** number of icon dmanaged on the main and test menu ***************************/
#define NB_ICON_MAIN_MENU 4
#define NB_ICON_TEST_MENU 18
#define NB_ICON_GUN_MENU  4
/***************************************************************************************************/


#define	ICON_CROSS		0
#define ICON_TRIANGLE	1
#define ICON_YOURTEST	2
#define ICON_GUN		3


#define	INIT_HEAD 135  // position of the head at the begining


/*** PobGun position ****/

#define POBGUN_SERVO1		3
#define POBGUN_SERVO2		6

#define POBGUN_POSITION_INIT	128
#define POBGUN_POSITION_FIRE	230

#define POBGUN_FIRE_INIT		40
#define POBGUN_FIRE_FIRE		128

#define POBGUN_ANA				2


int main (void)
{
	
	UInt8 i=0,k=0,out=0,WhichIcon=0;			
	UInt8 Nb_Identify=0;	//used to get the number of forms recognized
	UInt8 Head=INIT_HEAD;	//used to save the position of the head	
	UInt8 Value=0;			//used to follow the cross

	Icon MainIconList[NB_ICON_MAIN_MENU];		//array of icon used on the main menu (4 icons)
	Icon TestIconList[NB_ICON_TEST_MENU];	// array of icon used on the test menu 
	Icon GunIconList[NB_ICON_GUN_MENU];

	UInt8 *FrameFromCam;			//Struct to get the RGB components 
	Form  ListOfForm[MAX_OF_FORM];	//array used to save the ID of the forms which are recognized

	Menu	MainMenu,TestMenu,GunMenu;	//Struct to manage the main and the test menu	

	UInt8	ServPos[6];				//array used in the test menu, it used to save the position of the six servomotors

	UInt8	AnaGun;					//PobGun analog sensor
	UInt8	TriangleFound=0;

	Int8 debug[24];
	UInt32 debugLen;

	InitPobeye2();		//initialize the POB-EYE (is coming from the POB-LIBRARY)
	InitUART0(115200);
	InitI2C(I2C_100_KHZ);
	InitCameraPobeye2();
	InitPobProto();		//initialize POB-PROTO, the source code is at the end of this file
	InitLCD();
	
	FrameFromCam = GetRGBFrame();

	/***************************************************************************/
	InitGraphicBuffer(&LCD_Buffer_Video,128,64,ONE_BIT,LCD_Buffer); 
	InitGraphicBuffer(&LCD_Fast_Buffer_Video,64,64,EIGHT_BITS,LCD_Fast_Buffer);
	/***************************************************************************/
	
	
	/***********************************Definitions of the icon of the main menu ******************/
	//Icon of the cross
	MainIconList[0].X=10;	
	MainIconList[0].Y=20;
	MainIconList[0].Id=IDB_CROSS;		//ID of the bitmap genered by the  POB-BITMAP 
	MainIconList[0].GraphicRessource=bitmap;	//pointer on the array of the bitmaps
	MainIconList[0].W=GetBitmapWidth(MainIconList[0].Id,bitmap);		// width  of the bitmap
	MainIconList[0].H=GetBitmapHeight(MainIconList[0].Id,bitmap);	// height 

	//Icon of the triangle 
	MainIconList[1].X=52;
	MainIconList[1].Y=20;
	MainIconList[1].Id=IDB_TRIANGLE;
	MainIconList[1].GraphicRessource=bitmap;
	MainIconList[1].W=GetBitmapWidth(MainIconList[1].Id,bitmap);
	MainIconList[1].H=GetBitmapHeight(MainIconList[1].Id,bitmap);	


	//Icon of the test
	MainIconList[2].X=90;
	MainIconList[2].Y=14;
	MainIconList[2].Id=IDB_YOURTEST;
	MainIconList[2].GraphicRessource=bitmap;
	MainIconList[2].W=GetBitmapWidth(MainIconList[2].Id,bitmap);
	MainIconList[2].H=GetBitmapHeight(MainIconList[2].Id,bitmap);
	
	MainIconList[3].X=90;
	MainIconList[3].Y=41;
	MainIconList[3].Id=IDB_TARGET;
	MainIconList[3].GraphicRessource=bitmap;
	MainIconList[3].W=GetBitmapWidth(MainIconList[3].Id,bitmap);
	MainIconList[3].H=GetBitmapHeight(MainIconList[3].Id,bitmap);
	
							/***************************/
	
	//Creat the main menu	
	MainMenu.Arrow=IDB_ARROW;		//the ID of the bitmap used for the cursor
	MainMenu.Title=IDB_POBBOT;		//the ID of the title. If zero, no title wilbe drawn
	MainMenu.Lcd_Buffer=&LCD_Buffer_Video;	//pointer on the graphic buffer
	MainMenu.Nb_Icon=NB_ICON_MAIN_MENU;		//number of icons
	MainMenu.Ressource=bitmap;			//pointer on the array of the graphic ressources
	MainMenu.TabOfIcon=MainIconList;		//pointer on the array of icon


	/******************************************************************/	

	

	/***********************************Definitions of the icon of the test menu ******************/
	


	for (i=0;i<12;i+=2)		//this loop creats the +/- icons to manage the 6 servomotors
	{	
		TestIconList[i].X=X_SERVO+13*(i/2);
		TestIconList[i].Y=Y_SERVO;
		TestIconList[i].Id=IDB_MORE;
		TestIconList[i].GraphicRessource=bitmap;
		TestIconList[i].W=GetBitmapWidth(TestIconList[i].Id,bitmap);
		TestIconList[i].H=GetBitmapHeight(TestIconList[i].Id,bitmap);


		TestIconList[i+1].X=X_SERVO+13*(i/2);
		TestIconList[i+1].Y=Y_SERVO+TestIconList[i].H;
		TestIconList[i+1].Id=IDB_LESS;
		TestIconList[i+1].GraphicRessource=bitmap;
		TestIconList[i+1].W=GetBitmapWidth(TestIconList[i+1].Id,bitmap);
		TestIconList[i+1].H=GetBitmapHeight(TestIconList[i+1].Id,bitmap);
	
	}

	

	//icon to gear the left forward wheel
	TestIconList[13].X=X_ENGINE;
	TestIconList[13].Y=Y_ENGINE;
	TestIconList[13].Id=IDB_UP;
	TestIconList[13].GraphicRessource=bitmap;
	TestIconList[13].W=GetBitmapWidth(TestIconList[13].Id,bitmap);
	TestIconList[13].H=GetBitmapHeight(TestIconList[13].Id,bitmap);


	//icon to gear the left back wheel
	TestIconList[14].X=X_ENGINE;
	TestIconList[14].Y=Y_ENGINE+GetBitmapHeight(IDB_MOTOR,bitmap)-GetBitmapHeight(IDB_DOWN,bitmap);
	TestIconList[14].Id=IDB_DOWN;
	TestIconList[14].GraphicRessource=bitmap;
	TestIconList[14].W=GetBitmapWidth(TestIconList[14].Id,bitmap);
	TestIconList[14].H=GetBitmapHeight(TestIconList[14].Id,bitmap);


	//icon to gear the right forward wheel
	TestIconList[15].X=X_ENGINE+GetBitmapWidth(IDB_MOTOR,bitmap)-GetBitmapWidth(IDB_UP,bitmap);;
	TestIconList[15].Y=Y_ENGINE;
	TestIconList[15].Id=IDB_UP;
	TestIconList[15].GraphicRessource=bitmap;
	TestIconList[15].W=GetBitmapWidth(TestIconList[15].Id,bitmap);
	TestIconList[15].H=GetBitmapHeight(TestIconList[15].Id,bitmap);


	//icon to gear the right back wheel
	TestIconList[16].X=X_ENGINE+GetBitmapWidth(IDB_MOTOR,bitmap)-GetBitmapWidth(IDB_UP,bitmap);
	TestIconList[16].Y=Y_ENGINE+GetBitmapHeight(IDB_MOTOR,bitmap)-GetBitmapHeight(IDB_DOWN,bitmap);
	TestIconList[16].Id=IDB_DOWN;
	TestIconList[16].GraphicRessource=bitmap;
	TestIconList[16].W=GetBitmapWidth(TestIconList[16].Id,bitmap);
	TestIconList[16].H=GetBitmapHeight(TestIconList[16].Id,bitmap);
	

	//icon to manage the back icon, to exit from this menu
	TestIconList[12].X=95;
	TestIconList[12].Y=32;
	TestIconList[12].Id=IDB_BACK;
	TestIconList[12].GraphicRessource=bitmap;
	TestIconList[12].W=GetBitmapWidth(TestIconList[12].Id,bitmap);
	TestIconList[12].H=GetBitmapWidth(TestIconList[12].Id,bitmap);


	//icon to draw the to DC motor
	TestIconList[17].X=X_ENGINE;
	TestIconList[17].Y=Y_ENGINE;
	TestIconList[17].Id=IDB_MOTOR;
	TestIconList[17].GraphicRessource=bitmap;
	TestIconList[17].W=GetBitmapWidth(TestIconList[17].Id,bitmap);
	TestIconList[17].H=GetBitmapHeight(TestIconList[17].Id,bitmap);
		


	//Creats the TestMenu
	TestMenu.Arrow=IDB_ARROW;
	TestMenu.Title=0;
	TestMenu.Lcd_Buffer=&LCD_Buffer_Video;
	TestMenu.Nb_Icon=NB_ICON_TEST_MENU;
	TestMenu.Ressource=bitmap;
	TestMenu.TabOfIcon=TestIconList;



	//icon to manage the back icon, to exit from this menu
	GunIconList[0].X=95;
	GunIconList[0].Y=32;
	GunIconList[0].Id=IDB_BACK;
	GunIconList[0].GraphicRessource=bitmap;
	GunIconList[0].W=GetBitmapWidth(GunIconList[0].Id,bitmap);
	GunIconList[0].H=GetBitmapHeight(GunIconList[0].Id,bitmap);
	
	GunIconList[1].X=8;
	GunIconList[1].Y=16;
	GunIconList[1].Id=IDB_FIRE;
	GunIconList[1].GraphicRessource=bitmap;
	GunIconList[1].W=GetBitmapWidth(GunIconList[1].Id,bitmap);
	GunIconList[1].H=GetBitmapHeight(GunIconList[1].Id,bitmap);

	GunIconList[2].X=8;
	GunIconList[2].Y=32;
	GunIconList[2].Id=IDB_LOAD;
	GunIconList[2].GraphicRessource=bitmap;
	GunIconList[2].W=GetBitmapWidth(GunIconList[2].Id,bitmap);
	GunIconList[2].H=GetBitmapHeight(GunIconList[2].Id,bitmap);

	GunIconList[3].X=8;
	GunIconList[3].Y=50;
	GunIconList[3].Id=IDB_FIRE_FORM;
	GunIconList[3].GraphicRessource=bitmap;
	GunIconList[3].W=GetBitmapWidth(GunIconList[3].Id,bitmap);
	GunIconList[3].H=GetBitmapHeight(GunIconList[3].Id,bitmap);

	//Create the GunMenu
	GunMenu.Arrow=IDB_ARROW;
	GunMenu.Title=IDB_GUNTEST;
	GunMenu.Lcd_Buffer=&LCD_Buffer_Video;
	GunMenu.Nb_Icon=NB_ICON_GUN_MENU;
	GunMenu.Ressource=bitmap;
	GunMenu.TabOfIcon=GunIconList;


	//Launch the POB-TECHNOLOGY introduction
	IntroPOB();		


	//Set the Head
	SetServoMotor(0,Head);
	
	SetServoMotor(  POBGUN_SERVO1 ,POBGUN_POSITION_INIT);
	SetServoMotor(  POBGUN_SERVO2 ,POBGUN_FIRE_INIT);

	
	//Draw the Main menu for the first time
	DrawMenu(&MainMenu);



	//Main loop of the application
	while(1)
	{

		//if the cursor has moved, refresh the screen
		if(GetPad())
		{
			DrawMenu(&MainMenu);
		}

		
		//If the button is pressed
		if (GetButton())
		{

			//We have to test if we are on an icon
			if (IsCursorOnIcon(&MainMenu,&WhichIcon))
			{
				switch(WhichIcon) // test the index of the icon
				{
				case ICON_TRIANGLE : //We are on the triangle icon, the robot look to the triganle. Move the head, and turn to the left an the right
						/* The screen will be cut in to part. At the left, we will see the video on real time.
						At the right, you message witch ask you to press the button to return */
						
										
						LCD_Buffer_Video.width=64;				//change the width of the graphic interface's buffer
						ClearGraphicBuffer(&LCD_Buffer_Video);	//clear the graphic interface buffer
						DrawBitmap(5,20,IDB_TORETURN,bitmap,&LCD_Buffer_Video); //draw the bitmap "Press Ok to Return"
						DrawRightLCD(&LCD_Buffer_Video);	//draw the right LCD screen to show the message. After that, we never use it anymore, so we just hace to do it one time.
															
						

						while(GetButton());		//wait for the button is released

						Head=INIT_HEAD;	
						SetServoMotor(0,Head);	//set the head


						//loop until the button is pressed
						do
						{
							GrabRGBFrame();					//get a new Frame from the cmos sensor
							BinaryRGBFrame(FrameFromCam);	//binarise it

							DrawVision(FrameFromCam);		//draw the binarised frame on the left screen

							Nb_Identify=IdentifyForm(FrameFromCam,ListOfForm,pattern);	//identify the form on the picture from the dictionary of forms
							
							for(i=0;i<Nb_Identify;i++)				//parse the list of forms knowed
							{
								if(ListOfForm[i].id==IDP_TRIANGLE)	//if a form is a triangle, we hace to try to move the bot front of the triangle
								{														

									if (ListOfForm[i].y>65)		//if the triangle is too down 
									{
										if(Head<250) Head+=3;	//set the head under its current position
									}
									else if (ListOfForm[i].y<55) //if the triangle is too up
									{
										if(Head>3)	Head-=3;	//set the head upper than its current position		
									}

									SetServoMotor(0,Head);		//set the head with the new position




									if(ListOfForm[i].x>55)		//manage if the triangle is too on the right
									{										
																			
										MoveBot(MOVE_RIGHT);	//go to the right
										WaitMs(50);			//wait 0.05 sec
										MoveBot(STOP_BOT);		//stop the bot
									}
									else if(ListOfForm[i].x<35)	//if the triangle is too on the left
									{									
										MoveBot(MOVE_LEFT);		//go the to left
										WaitMs(50);			//wait 0.05 sec
										MoveBot(STOP_BOT);		//stop the bot
									}
								}
								

							}

						}while(!GetButton());		//if the button is pressed, get out from the loop


						LCD_Buffer_Video.width=128;	//we give the good side the the graphic interface buffer
						DrawMenu(&MainMenu);		//draw the main menu to refresh the full screen
						
						while(GetButton());			//wait for the button is released



					break;

				case ICON_CROSS:	
					
						/* it's the same as the Triangle. The difference it's the head doesn't move. But the bot is trying to run on the cross*/

						LCD_Buffer_Video.width=64;
						ClearGraphicBuffer(&LCD_Buffer_Video);
						DrawBitmap(5,20,IDB_TORETURN,bitmap,&LCD_Buffer_Video);
						DrawRightLCD(&LCD_Buffer_Video);

						while(GetButton());

						Head=INIT_HEAD;
						SetServoMotor(0,Head);
						do
						{
							GrabRGBFrame();	
							BinaryRGBFrame(FrameFromCam);						
							DrawVision(FrameFromCam);							

							Nb_Identify=IdentifyForm(FrameFromCam,ListOfForm,pattern);								
							
							Value=STOP_BOT;
							for(i=0;i<Nb_Identify;i++)
							{
								if(ListOfForm[i].id==IDP_CROSS)
								{								
									if(ListOfForm[i].x>55)				
									{										
																			
										Value=MOVE_RIGHT;
									}
									else if(ListOfForm[i].x<35)
									{									
										Value=MOVE_LEFT;									
									}
									else
									{
										Value=MOVE_RUN;
									}
								}							

								if (Value==MOVE_RIGHT||Value==MOVE_LEFT)
								{
									MoveBot(Value);
									WaitMs(50);
									Value=STOP_BOT;
								}
							}

							MoveBot(Value);

						}while(!GetButton());


						LCD_Buffer_Video.width=128;
						DrawMenu(&MainMenu);
						while(GetButton());
					break;



				
				case ICON_YOURTEST:
						
						/* this icon is to test your servomotors and DC motors*/

						out=0;
						DrawMenu(&TestMenu);	//draw the test menu
						while(GetButton());		//wait the button is released

						for (i=0;i<6;i++)		//initiate the array of servomotros position to default value (128)
						{
							ServPos[i]=128;
						}
						SetServoMotor(0,128);	//set all servomotors
						SetServoMotor(1,128);
						SetServoMotor(2,128);
						SetServoMotor(3,128);
						SetServoMotor(6,128);
						SetServoMotor(7,128);
						

						do
						{
							if(GetPad())	//if the cursor has moved, refresh the screen
							{
								DrawMenu(&TestMenu);
							}
							
							if (GetButton()) // if the button is pressed 
							{		
								if(IsCursorOnIcon(&TestMenu,&WhichIcon)) // test if the cursor is on an icon
								{
									
									switch (WhichIcon) // 
									{
									case 0:
									case 1:
									case 2:
									case 3:
									case 4:
									case 5:
									case 6:
									case 7:
									case 8:
									case 9:
									case 10:
									case 11:

										// if we are on an icon to managing the servomotors

										if (WhichIcon%2==0) //test if we are on a +
										{
									
											ServPos[WhichIcon/2]+=4; //increase the position with 4 step
										}
										else				//else we are on a -
										{
											ServPos[WhichIcon/2]-=4;//decrease the position with 4 step
										}						
										

//										PrintTextOnPobTerminal("Servo %d=%d",WhichIcon/2,ServPos[WhichIcon/2]); //print on the pob-terminal the new posistion of the servomotor

										WhichIcon = WhichIcon/2;

										debugLen = Bin2Dec(ServPos[WhichIcon],debug);
										debug[debugLen]=0;
										SendString(debug);
										SendString("\r\n");

										if (((WhichIcon)>=0)&&((WhichIcon)<=3))
										{
											SetServoMotor(WhichIcon,ServPos[WhichIcon]);
										}
										else
										{
											SetServoMotor((WhichIcon)+2,ServPos[WhichIcon]);
										}

									break;
									


									//gear the DC motor
									case 13:										
										MoveBot(0x04);
										WaitMs(100);
										while(GetButton());
										MoveBot(0);
									break;
								
									case 14:
										MoveBot(0x08);
										WaitMs(100);
										while(GetButton());
										MoveBot(0);
									break;
								

									case 15:
										MoveBot(0x01);
										WaitMs(100);
										while(GetButton());
										MoveBot(0);
									break;
								

									case 16:
										MoveBot(0x02);
										WaitMs(100);
										while(GetButton());
										MoveBot(0);
									break;
								
									case 12: // if the bck icon is pressed, to exit from the while loop we have to set the "out" varible to 1
										out=1; 
									break;
									}
								}
							}

						}while(!out);			// until the back icon is pressed
						DrawMenu(&MainMenu);	// draw the main menu
						while(GetButton());		// wait for the button is released

						
					break;

				case ICON_GUN:

					/* this icon is to test pobgun */

					out=0;
					DrawMenu(&GunMenu);	//draw the test menu
					while(GetButton());


					SetServoMotor(POBGUN_SERVO1,POBGUN_POSITION_INIT);
					SetServoMotor(POBGUN_SERVO2,POBGUN_FIRE_INIT);

					do
					{
						if(GetPad())	//if the cursor has moved, refresh the screen
						{
							DrawMenu(&GunMenu);
						}
							
						if (GetButton()) // if the button is pressed 
						{		
							if(IsCursorOnIcon(&GunMenu,&WhichIcon)) // test if the cursor is on an icon
							{
								if(WhichIcon==0)
								{
									out=1;
									while(GetButton());	
								}
								else if(WhichIcon==1)
								{
									//fire!
									SetServoMotor(POBGUN_SERVO1,POBGUN_POSITION_FIRE);
									
									for(i=0;i<10;i++)
									{
										WaitMs(100);
									}
									
									AnaGun = GetPortAnalog(POBGUN_ANA);
									if(AnaGun > 40)
									{
										SetServoMotor(POBGUN_SERVO2,POBGUN_FIRE_FIRE);
									}

									for(i=0;i<10;i++)
									{
										WaitMs(100);
									}

									SetServoMotor(POBGUN_SERVO1,POBGUN_POSITION_INIT);

									
								}
								else if(WhichIcon==2)
								{
									//load arrow
									SetServoMotor(POBGUN_SERVO1,POBGUN_POSITION_INIT);
									WaitMs(5);
									SetServoMotor(POBGUN_SERVO2,POBGUN_FIRE_INIT);
								}
								else if(WhichIcon==3)
								{
									LCD_Buffer_Video.width=64;				//change the width of the graphic interface's buffer
									ClearGraphicBuffer(&LCD_Buffer_Video);	//clear the graphic interface buffer
									DrawBitmap(5,20,IDB_TORETURN,bitmap,&LCD_Buffer_Video); //draw the bitmap "Press Ok to Return"
									DrawRightLCD(&LCD_Buffer_Video);	//draw the right LCD screen to show the message. After that, we never use it anymore, so we just hace to do it one time.

									while(GetButton());		//wait for the button is released

									Head=INIT_HEAD;	
									SetServoMotor(0,Head);	//set the head
									//fire!
									SetServoMotor(POBGUN_SERVO1,POBGUN_POSITION_FIRE);

									//loop until the button is pressed
									TriangleFound=0;
									do
									{
										Value=STOP_BOT;
										do
										{
											Value=STOP_BOT;
											GrabRGBFrame();	
											BinaryRGBFrame(FrameFromCam); 
											DrawVision(FrameFromCam);							
						
											Nb_Identify=IdentifyForm(FrameFromCam,ListOfForm,pattern);

											if (Nb_Identify)
											{
												for(k=0;k<Nb_Identify;k++)
												{
													if(ListOfForm[k].id==IDP_TRIANGLE)
													{								
														if(ListOfForm[k].x>55)				
														{									
															Value=MOVE_RIGHT;
														}
														else if(ListOfForm[k].x<35)
														{
															Value=MOVE_LEFT;									
														}
														else 
														{
															TriangleFound=1;
														}
													}							

													if (Value==MOVE_RIGHT||Value==MOVE_LEFT)
													{
														SetPortD(Value);
														WaitMs(50);
														SetPortD(STOP_BOT);
													}
												}							
											}
										}while(Value!=STOP_BOT);

										if(TriangleFound==1)
										{
											SetServoMotor(POBGUN_SERVO2,POBGUN_FIRE_FIRE);
											for(i=0;i<10;i++)
											{
												WaitMs(100);
											}
											SetServoMotor(POBGUN_SERVO1,POBGUN_POSITION_INIT);
										}

									}while(!GetButton());		//if the button is pressed, get out from the loop


									LCD_Buffer_Video.width=128;	//we give the good side the the graphic interface buffer
									DrawMenu(&GunMenu);		//draw the main menu to refresh the full screen
									
									while(GetButton());			//wait for the button is released

								}
							}
						}
				
					}while(!out);			// until the back icon is pressed
					
					DrawMenu(&MainMenu);	// draw the main menu
					while(GetButton());		// wait for the button is released

					break;
				}
			}
		}


	}
}



//Draw the frame binarised 
void DrawVision (UInt8 *FrameFromCam)
{	
	//draw the red buffer (88*120 pixels) on the Buffer for for the left screen (64*64 pixels)
	DrawComponentInABufferVideo(FrameFromCam,&LCD_Fast_Buffer_Video);
	DrawLeftLCD(&LCD_Fast_Buffer_Video); //refresh the left screen
}



//launch the POB-TECHNOLOGY introduction
void IntroPOB(void)
{
	UInt32 i=0;
	
	//clear the right screen
	ClearGraphicBuffer(&LCD_Buffer_Video);
	
	//Draw the 3 letters
	DrawBitmap(8,5,IDB_POB,intro,&LCD_Buffer_Video);
	DrawLCD(&LCD_Buffer_Video);
	WaitMs(1000);

	DrawBitmap(8,47,IDB_TECHNO,intro,&LCD_Buffer_Video);		
	DrawLCD(&LCD_Buffer_Video);	
	
	WaitMs(1000);
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
	SetPortD(Way);
}

