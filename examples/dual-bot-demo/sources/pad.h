//
// POB-Technology 2008
// Project: Dualbot demo program
//
#ifndef _PAD_H
#define _PAD_H


#define RIGHT_LEFT_AXE	7		/**< right/left analog button */
#define UP_DOWN_AXE		6		/**< up/down analog button */

/** @brief Manage analog pad.
 *
 * @return 1 if it has moved, else return 0.
 */
UInt8 GetPad(void);

/** @brief Manage button press.
 *
 * @return 1 if the button is pressed, else return 0.
 */
UInt8 GetButton (void);


#endif
