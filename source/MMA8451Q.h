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

#ifndef MMA8451Q_H
#define MMA8451Q_H


#include <cfloat>
#include <math.h>
#include "fsl_i2c.h"
#include "i2c.h"
#include "c_bit.h"
#include "fsl_debug_console.h"


/**
* MMA8451Q accelerometer example
*
* @code
* #include "mbed.h"
* #include "MMA8451Q.h"
* 
* #define MMA8451_I2C_ADDRESS (0x1d<<1)
* 
* int main(void) {
* 
* MMA8451Q acc(P_E25, P_E24, MMA8451_I2C_ADDRESS);
* PwmOut rled(LED_RED);
* PwmOut gled(LED_GREEN);
* PwmOut bled(LED_BLUE);
* 
*     while (true) {       
*         rled = 1.0 - abs(acc.getAccX());
*         gled = 1.0 - abs(acc.getAccY());
*         bled = 1.0 - abs(acc.getAccZ());
*         wait(0.1);
*     }
* }
* @endcode
*/

#define REG_WHO_AM_I      0x0D  //WHO_AM_I Device ID register - The device identification register identifies the part - Device ID (0x1A)
#define REG_CTRL_REG_1    0x2A //system control 1 register - ODR = 800 Hz, standby mode

//These registers contain the X-axis, Y-axis, and Z-axis, and 14-bit output sample data expressed as 2's complement numbers.
#define REG_OUT_X_MSB     0x01 // [7:0] are eight MSBs of 14-bit sample.
#define REG_OUT_Y_MSB     0x03  // [7:0] are eight MSBs of 14-bit realtime sample
#define REG_OUT_Z_MSB     0x05  //[7:0] are eight MSBs of 14-bit realtime sample

#define UINT14_MAX        16383


//Addressa akcelerometra
#define MMA8451_I2C_ADDRESS 0x1D


#define DEVICE_WEIGTH 135 //in grams
#define GRAVITY_ACCEL 9.81 //m_g/s^2

#define XYZ_DATA_CFG 0x0E
#define HPF_OUT_MASK 1<<4

#define HP_FILTER_CUTOFF 0x0F
#define PULSE_LPF_EN 1<<4
#define PULSE_HPF_BYP 1<<5
#define HP_FILTER_CUTOFF_SEL_0 1<<0
#define HP_FILTER_CUTOFF_SEL_1 1<<1
#define HP_FILTER_CUTOFF_SEL (HP_FILTER_CUTOFF_SEL_0 | HP_FILTER_CUTOFF_SEL_1)

#define CTRL_REG1 0x2A
#define ACTIVE_MASK 1<<0
#define ODR_MASK_0 1<<3
#define ODR_MASK_1 1<<4
#define ODR_MASK_2 1<<5
#define ASLP_RATE_0 1<<6
#define ASLP_RATE_1 1<<7
#define ODR_MASK (ODR_MASK_2 | ODR_MASK_1 | ODR_MASK_0)

#define CTRL_REG2 0x2B
#define MODS_MASK_1 1<<0
#define MODS_MASK_2 1<<1
#define MODS_MASK (MODS_MASK_2 | MODS_MASK_1)

#define FF_MT_THS 0x17
#define FF_MT_COUNT 0x18

#define FF_MT_CFG 0x15
#define FF_MT_CFG_ELE 1<<7
#define FF_MT_CFG_OAE 1<<6
#define FF_MT_CFG_ZEFE 1<<5
#define FF_MT_CFG_YEFE 1<<4
#define FF_MT_CFG_XEFE 1<<3

#define CTRL_REG4 0x2D
#define INT_EN_FF_MT 1<<2

#define CTRL_REG5 0x2E
#define INT_CFG_DRDY 1<<0
#define INT_SOURCE 0x0C
#define FF_MT_SRC 0x16


extern "C" {

void PORTA_IRQHandler(void);

}



class MMA8451Q
{
public:
  /**
  * MMA8451Q constructor
  *
  * @param master_config
  */
  MMA8451Q(i2c_master_config_t* master_config); //i2c_master_config_t* master_config

  /**
  * MMA8451Q destructor
  */
  ~MMA8451Q();

  /**
   * Get the value of the WHO_AM_I register
   *
   * @returns WHO_AM_I value
   */
  uint8_t getWhoAmI();

  /**
   * Get X axis acceleration
   *
   * @returns X axis acceleration
   */
  float getAccX();

  /**
   * Get Y axis acceleration
   *
   * @returns Y axis acceleration
   */
  float getAccY();

  /**
   * Get Z axis acceleration
   *
   * @returns Z axis acceleration
   */
  float getAccZ();

  /**
   * Get XYZ axis acceleration
   *
   * @param res array where acceleration data will be stored
   */
  void getAccAllAxis(float * res);

	/**
	* Initialization of accelerometer
	*
	*/
  void initAcceletometer();

  float getRoll(float* axis);

  float getPitch(float* axis);

  float radsToDegs(float rads);

  /**
   * axis_in - aktualne hodnoty
   * samplePeriod - perioda vzorkovania
   * axis_out - vyfiltrovane hodnoty
   */
  void lowPassFilter(float* axis_in, float samplePeriodInMs, float *axis_out);

  void readRegs(uint8_t slave_addr, uint8_t addr, uint8_t * data, int len);
  void writeRegs(uint8_t slave_addr, uint8_t addr, uint8_t * data, int len);

  void setFreeFall();
  void enableFreeFallInterrupt();

private:
  C_I2C m_i2c;
  float last_axies[3] = {0};

  int16_t getAccAxis(uint8_t addr);
  int sign(float rotation_point);
};

#endif
