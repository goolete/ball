#ifndef __KEY_H
#define __KEY_H

#include <stm32f10x.h>
#include "init.h"
#include "delay.h"


#define uint unsigned int 
#define uchar unsigned char
void mKey_Config(void);
int mKeyScan(void);
void GetKeyVal(void);

#endif

