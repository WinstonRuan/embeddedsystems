/****************************************/
/* POB-TECHNOLOGY						*/
/* file menu.c							*/	
/* 16-02-2006							*/
/* Demo for PobBoTDemo					*/
/* author Pierre Seguin					*/ 
/* e-mail :pseguin@pob-technology.com	*/
/****************************************/

#include "POB-EYE.h"
#include "menu.h"






/********************globals to manage the cursor ***********************/
UInt8 X_Cursor=64;
UInt8 Y_Cursor=32;
/************************************************************************/




//Draw the menu : icons + title + cursor
void DrawMenu(Menu *MenuDef)
{
	UInt8 i=0;

	ClearGraphicBuffer(MenuDef->Lcd_Buffer);							//clear the graphic buffer
	
	if (MenuDef->Title) DrawBitmap(0,0,MenuDef->Title,MenuDef->Ressource,MenuDef->Lcd_Buffer);	//If the the menu has got a title, it's drawn

	//draw all icons
	for (i=0;i<MenuDef->Nb_Icon;i++)
	{
		DrawBitmap(MenuDef->TabOfIcon[i].X,MenuDef->TabOfIcon[i].Y,MenuDef->TabOfIcon[i].Id,MenuDef->Ressource,MenuDef->Lcd_Buffer);		
	}

	//draw the cursor
	DrawBitmap(X_Cursor,Y_Cursor,MenuDef->Arrow,MenuDef->Ressource,MenuDef->Lcd_Buffer);


	//draw the full LCD
	DrawLCD(MenuDef->Lcd_Buffer);	
}



// test if the cursor is on a icon
UInt8 IsCursorOnIcon(Menu *MenuDef,UInt8 *Icon)
{
	UInt8 i=0,icon=0,w=0,h=0;	



	//parse the array of icons and 
	for (i=0;i<MenuDef->Nb_Icon;i++)
	{		
		w=MenuDef->TabOfIcon[i].X+MenuDef->TabOfIcon[i].W;
		h=MenuDef->TabOfIcon[i].Y+MenuDef->TabOfIcon[i].H;


		//test if the cursor is on it
		if (X_Cursor>MenuDef->TabOfIcon[i].X&&X_Cursor<w&&Y_Cursor>MenuDef->TabOfIcon[i].Y&&Y_Cursor<h)
		{
			*Icon=i; //get the index of the icon in the array of icon
			return 1; // return 1 if the cursor is on a icon
		}
			
	}
	return 0; //return 0 if the cursor is not on a icon	
}

