/****************************************/
/* POB-TECHNOLOGY						*/
/* file menu.h							*/	
/* 16-02-2006							*/
/* Demo for PobBoTDemo					*/
/* author Pierre Seguin					*/ 
/* e-mail :pseguin@pob-technology.com	*/
/****************************************/
#include "POB-EYE.h"



#ifndef _MENU
#define _MENU

//struct of an icon
typedef struct _Icon
{
	UInt8 const *GraphicRessource;	//	pointer of the array of bitmaps to draw the icon
	UInt8 Id;						//	id of bitmap
	UInt8 X;						// 
	UInt8 Y;						// 
	UInt8 W;						// icon's width
	UInt8 H;						// icon's height

}Icon;





//struct of a manu
typedef struct _Menu
{
	Icon *TabOfIcon;				// array of icon 
	UInt8 Title;					// ID of the title in the array of bitmaps
	UInt8 Arrow;					// ID of the Arrow in the array of bitmaps
	GraphicBuffer *Lcd_Buffer;		// pointer on the graphic buffer to draw the menu	
	UInt8 const* Ressource;			// pointer of the array of bitmap to draw the cursor and the title
	UInt8 Nb_Icon;					// number of icons 
}Menu;







//test if the cursor is on the icon
UInt8 IsCursorOnIcon(Menu *MenuDef,UInt8 *Icon);		

//refresh the menu
void DrawMenu(Menu *MenuDef);






#endif
