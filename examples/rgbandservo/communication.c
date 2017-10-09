// Communication file:

#include <pob-eye.h>

#include "communication.h"


//send string and LF+CR
void send(const char *buffer)
{
	UInt32 i=0;

	for(i=0;buffer[i]!=0;i++)
	{
		SendByteToUART0(buffer[i]);
	}

	SendByteToUART0(LF);
	SendByteToUART0(CR);
}

//receive string with CR, return size of string
UInt8 receive(unsigned char *buffer,UInt8 len)
{
	UInt8 size=0;
	UInt8 byte;

	do
	{
		byte=GetByteFromUART0();

		if(byte==CR)
			break;

		buffer[size]=byte;
		size++;

		if(size>len)
		{
			return len;
		}

	}while(1);

	buffer[size]=0;

	return size;
}


Int8 extractString(String *str,int strNb, UInt8 *buffer,UInt8 size)
{
	UInt8 i=0,j=0;
	UInt8 str_index=0;

	for( ; i<size;i++)
	{
		if(buffer[i]!= ' ')
		{
			str[str_index].buffer[j]=buffer[i];
			j++;
		}
		else
		{
			if(j==0)
				continue;

			str[str_index].buffer[j]='\0';
			str[str_index].len=j;
			
//			SendByteToUART('0'+ str_index);
//			sendCr(str[str_index].buffer);

			str_index++;

			if(str_index>strNb)
				return -1;

			j=0;
		}

	}

	str[str_index].buffer[j]='\0';
	str[str_index].len=j;

//	SendByteToUART('0'+ str_index);
//	sendCr(str[str_index].buffer);

	return str_index+1;
}


//compate 2 strings, return 1 if egual.
UInt8 strCmp(const Int8 *str,UInt8 *cmp,UInt8 len)
{
	UInt8 i=0;

	for(i=0;i<len||str[i]!=0;i++){
		if(str[i]!=cmp[i])
			return 0;
	}

	if(i==len)
		return 1;

	return 0;
}