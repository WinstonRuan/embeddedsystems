/** \anchor hello0
 *
 * @brief Hello world. 
 *
 * @file

 An example to print Hello World in the UART.

 * @author Foucault Alisson
 */

#include <pob-eye.h>

int main (void)
{	
	//Initialize POB-Eye and Serial
	InitPobeye2();
	InitUART0(115200);

	//Send "Hello World" to the UART0
	SendBufferToUART0("Hello World", 11);

	return 0;
}
