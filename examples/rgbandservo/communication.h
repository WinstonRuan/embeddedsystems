/*
 * 
 * Copyright 2007 POB-Technology. All rights reserved.
 * POB-Technology PROPRIETARY/CONFIDENTIAL.
 * 
 * Communication include
 */
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#define LF				0x0A
#define CR				0x0D


typedef struct _string
{
	UInt8 len;
	UInt8 buffer[32];
} String;


/* extractString
 *
 */
extern Int8 extractString(String *str,int strNb, UInt8 *buffer,UInt8 size);


/* sendCr
 *
 */
extern void send(const char *buffer);

/* receiveCr
 *
 */
extern UInt8 receive(unsigned char *buffer,UInt8 len);

/* strCmp
 *
 */
extern UInt8 strCmp(const Int8 *str,UInt8 *cmp,UInt8 len);


#endif