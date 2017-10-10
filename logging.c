#include "logging.h"

void LG_Print(char *str_, int numChars)
{
	#ifdef ENABLE_LOGGING
	SendBufferToUART0(str,numChars);
	#endif
}

