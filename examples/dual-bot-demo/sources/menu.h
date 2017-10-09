//
// POB-Technology 2008
// Project: Dualbot demo program
//
#include <pob-eye.h>


#ifndef _MENU_H
#define _MENU_H

/** @brief Icon's structure
 */
typedef struct _Icon
{
	UInt8 const *GraphicRessource;	/**< pointer to bitmaps data */
	UInt8 Id;						/**< id of bitmap */
	UInt8 X;						/**< X position in screen */
	UInt8 Y;						/**< Y position in screen */
	UInt8 W;						/**< icon's width */
	UInt8 H;						/**< icon's height */
}Icon;

/** @brief Menu
 */
typedef struct _Menu
{
	Icon *TabOfIcon;				/**< Icon's array */
	UInt8 Title;					/**< title'identifier in bitmap memory */
	UInt8 Arrow;					/**< Arrow's identifier in bitmap memory */
	GraphicBuffer *Lcd_Buffer;		/**< pointer on the graphic buffer to draw the menu	*/
	UInt8 const* Ressource;			/**< pointer to the bitmap memory to draw the cursor and the title */
	UInt8 Nb_Icon;					/**< number of icons */
}Menu;




//test if the cursor is on the icon
UInt8 IsCursorOnIcon(Menu *MenuDef,UInt8 *Icon);		

//refresh the menu
void DrawMenu(Menu *MenuDef);


#endif
