/*
 * c_bit.cpp
 *
 *  Created on: Dec 2, 2018
 *      Author: Pankhard159
 */

#include "c_bit.h"

void C_Bit::set(uint8_t* data, int offset, bool on){
	if(on){
		*data |= 1<<offset;
	} else{
		*data &= ~(1<<offset);
	}
}

