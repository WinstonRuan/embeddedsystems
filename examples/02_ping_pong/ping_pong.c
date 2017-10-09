/** \anchor pingpong
 *
 * @brief Ping Pong. 
 *
 * @file

 An example to use the UART: a simple ping pong between POBEYE 2 and computer.

 * @author Foucault Alisson
 */

#include <pob-eye.h>

int main(void)
{		
	//Initialize byte value
	UInt8 byte = 0;
	
	// Initialization of the POB-Eye and the Serial
	InitPobeye2();
	InitUART0(115200);	
	
	while (1)
	{
		//Get the byte from the UART
		byte = GetByteFromUART0();
		
		//Send the byte to the UART
		SendByteToUART0(byte);
	}
	return 0;
}
