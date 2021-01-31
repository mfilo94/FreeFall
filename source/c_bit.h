/*
 * c_bit.h
 *
 *  Created on: Dec 2, 2018
 *      Author: Pankhard159
 */
#include "fsl_common.h"

#ifndef C_BIT_H_
#define C_BIT_H_



class C_Bit {
public:
	static void set(uint8_t* data, int offset, bool on_off);
};


#endif /* C_BIT_H_ */
