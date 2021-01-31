

#include "i2c.h"


C_I2C::C_I2C(i2c_master_config_t* master_config){

	// Inicializuj I2C master,
	//zapnutie hodin a nastavenie ich
	I2C_MasterInit(I2C0, master_config, I2C_MASTER_CLK);
}


void C_I2C::read(uint8_t sl_addr, uint8_t sub_addr, uint8_t * data, int len){

	memset(&this->masterXfer, 0, sizeof(i2c_master_transfer_t));

	//Nastavenie I2C pre citanie zo slave
	this->masterXfer.slaveAddress = sl_addr;
	this->masterXfer.subaddress = sub_addr;
	this->masterXfer.subaddressSize = 1;
	this->masterXfer.direction = kI2C_Read;
	this->masterXfer.data = data;
	this->masterXfer.dataSize = len;
	this->masterXfer.flags = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &this->masterXfer);
}

void C_I2C::write(uint8_t sl_addr, uint8_t sub_addr, uint8_t * data, int len){

	memset(&masterXfer, 0, sizeof(i2c_master_transfer_t));

	 //Nastavenie I2C pre zapis do slave
	this->masterXfer.slaveAddress = sl_addr;
	this->masterXfer.subaddress = sub_addr;
	this->masterXfer.subaddressSize = sizeof(sub_addr);
	this->masterXfer.direction = kI2C_Write;
	this->masterXfer.data = data;
	this->masterXfer.dataSize = len;
	this->masterXfer.flags = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &this->masterXfer);
}

