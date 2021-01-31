/*
 * myI2C.cpp
 *
 *  Created on: Nov 20, 2018
 *      Author: tomas
 */

#include <myI2C.h>

my_I2C::my_I2C() {
	// TODO Auto-generated constructor stub
    I2C_MasterGetDefaultConfig(&masterConf);
    I2C_MasterInit(I2C0, &masterConf, CLOCK_GetFreq(I2C0_CLK_SRC));
}

my_I2C::~my_I2C() {
	// TODO Auto-generated destructor stub
}

void my_I2C::write( int addr_slave,  uint8_t* data, int len)	{
	i2c_master_transfer_t masterXfer;
    memset(&masterXfer, 0, sizeof(masterXfer));

    masterXfer.slaveAddress = addr_slave;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = data[0];
    masterXfer.subaddressSize = 1;
    masterXfer.data = &data[1];
    masterXfer.dataSize = len;

    masterXfer.flags = kI2C_TransferDefaultFlag;
    I2C_MasterTransferBlocking(I2C0, &masterXfer);
    PRINTF("End write\n");
}

void my_I2C::read( int addr_slave, int addrReg, uint8_t* data, int len) {
	i2c_master_transfer_t masterXfer;
	memset(&masterXfer, 0, sizeof(masterXfer));

	masterXfer.slaveAddress = addr_slave;
	masterXfer.direction = kI2C_Read;
	masterXfer.subaddress = addrReg;
	masterXfer.subaddressSize = 1;
	masterXfer.data = data;
	masterXfer.dataSize = len;

	masterXfer.flags = kI2C_TransferDefaultFlag;
	I2C_MasterTransferBlocking(I2C0, &masterXfer);
}
