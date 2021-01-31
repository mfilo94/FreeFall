/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "MMA8451Q.h"



MMA8451Q::MMA8451Q(i2c_master_config_t* master_config): m_i2c(master_config) {
	//Init accelerometer
	initAcceletometer();
}

MMA8451Q::~MMA8451Q() { }

void MMA8451Q::initAcceletometer(){
	uint8_t data[1] = {1};
	this->writeRegs(MMA8451_I2C_ADDRESS, REG_CTRL_REG_1, data, 1);
}

uint8_t MMA8451Q::getWhoAmI() {
    uint8_t who_am_i = 0;
    this->readRegs(MMA8451_I2C_ADDRESS, REG_WHO_AM_I, &who_am_i, 1);
    return who_am_i;
}

float MMA8451Q::getAccX() {
    return (float(getAccAxis(REG_OUT_X_MSB))/4096.0);
}

float MMA8451Q::getAccY() {
    return (float(getAccAxis(REG_OUT_Y_MSB))/4096.0);
}

float MMA8451Q::getAccZ() {
    return (float(getAccAxis(REG_OUT_Z_MSB))/4096.0);
}

void MMA8451Q::getAccAllAxis(float * res) {
    res[0] = getAccX();
    res[1] = getAccY();
    res[2] = getAccZ();
}

int16_t MMA8451Q::getAccAxis(uint8_t addr) {
    int16_t acc;
    uint8_t res[2] = {0};
    this->readRegs(MMA8451_I2C_ADDRESS, addr, res, 2);

    acc = (res[0] << 6) | (res[1] >> 2);
    if (acc > UINT14_MAX/2)
        acc -= UINT14_MAX;

    return acc;
}

void MMA8451Q::readRegs(uint8_t slave_addr, uint8_t addr, uint8_t * data, int len) {
	this->m_i2c.read(slave_addr, addr, data, len);
}

void MMA8451Q::writeRegs(uint8_t slave_addr, uint8_t addr, uint8_t * data, int len) {
	this->m_i2c.write(slave_addr, addr, data, len);
}


float MMA8451Q::getRoll(float* axis){
	return atanf(axis[1]/(this->sign(axis[2])*sqrt(powf(axis[2],2)+powf(axis[0],2))));
}

float MMA8451Q::getPitch(float* axis){
	return atanf(-axis[0]/sqrt(powf(axis[1],2)+powf(axis[2],2)));
}

float MMA8451Q::radsToDegs(float rads){
	return rads*180/M_PI;
}

int MMA8451Q::sign(float rotation_point){
	return rotation_point>0? 1 : -1;
}


void MMA8451Q::lowPassFilter(float* axis_in, float samplePeriodInMs, float *axis_out){
	uint8_t f_c = 5;
	samplePeriodInMs = samplePeriodInMs/1000;
	float alpha = (2*M_PI*samplePeriodInMs*f_c)/(2*M_PI*samplePeriodInMs*f_c+1);

	for (uint8_t i = 0; i < 3; ++i) {
		axis_out[i] = (1-alpha)*this->last_axies[i]+ alpha*axis_in[i];
		this->last_axies[i] = axis_out[i];
	}
}



void MMA8451Q::setFreeFall(){

	uint8_t data = 0x20;
    this->writeRegs(MMA8451_I2C_ADDRESS, CTRL_REG1, &data, sizeof(data));

    data = 0xb8;
    this->writeRegs(MMA8451_I2C_ADDRESS, FF_MT_CFG, &data, sizeof(data));

    data = 0x03;
    this->writeRegs(MMA8451_I2C_ADDRESS, FF_MT_THS, &data, sizeof(data));

    data = 0x06;
    this->writeRegs(MMA8451_I2C_ADDRESS, FF_MT_COUNT, &data, sizeof(data));

	data = 0x04;
	this->writeRegs(MMA8451_I2C_ADDRESS, CTRL_REG4, &data, sizeof(data));

    data = 0x00;
    this->writeRegs(MMA8451_I2C_ADDRESS, CTRL_REG5, &data, sizeof(data));

    data = 0x21;
    this->writeRegs(MMA8451_I2C_ADDRESS, CTRL_REG1, &data, sizeof(data));
}

void MMA8451Q::enableFreeFallInterrupt(){
	EnableIRQ(PORTA_IRQn);
}
