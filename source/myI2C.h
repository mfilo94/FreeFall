/*
 * myI2C.h
 *
 *  Created on: Nov 20, 2018
 *      Author: tomas
 */

#ifndef MYI2C_H_
#define MYI2C_H_

#include "fsl_i2c.h"
#include "fsl_debug_console.h"

class my_I2C {
private:
	i2c_master_config_t masterConf;
public:
	my_I2C();
	void read( int addr_slave, int addrReg, uint8_t* data, int len);
	void write( int addr_slave,  uint8_t* data, int len);
	virtual ~my_I2C();
};

#endif /* MYI2C_H_ */
