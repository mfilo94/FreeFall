/*
 * Copyright 2016-2018 NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    MKL25Z128xxx4_Project.cpp
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include "fsl_port.h"
#include "fsl_smc.h"
/* TODO: insert other include files here. */

#include "MMA8451Q.h"
#include "c_timers.h"

#undef PRINTF_FLOAT_ENABLE
#undef PRINTF_ADVANCED_ENABLE
#define PRINTF_FLOAT_ENABLE 1
#define PRINTF_ADVANCED_ENABLE 1



MMA8451Q* mq;


extern "C" {
	void PORTA_IRQHandler(){

		PORT_ClearPinsInterruptFlags(BOARD_INITPINS_ACCEL_INT2_PORT, 1<<BOARD_INITPINS_ACCEL_INT2_PIN);

		uint8_t status = 0;
		mq->readRegs(MMA8451_I2C_ADDRESS, INT_SOURCE, &status, sizeof(status));

		if((status & 0x04) == 0x04){
			mq->readRegs(MMA8451_I2C_ADDRESS, FF_MT_SRC, &status, sizeof(status));
			PRINTF("Free fall detected!\n\r");
		}

	}
}


/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();


    i2c_master_config_t masterConfig;

    //Ziskaj default nastavenie master configu
    I2C_MasterGetDefaultConfig(&masterConfig);
    //default Bdr pre UART komunikaciu debugu 115200

    //Vytvorenie objektu pre snimanie akcelerometra
    mq = new MMA8451Q(&masterConfig);

    C_Timers timers = C_Timers();
    timers.PIT_init();
    mq->setFreeFall();

    while(1){

    	PRINTF("Entering to sleep mode...\n\r");

    	SMC_PreEnterStopModes();

    	/* Enable the wakeup interrupt here. */
    	timers.EnableInterrupt();
    	mq->enableFreeFallInterrupt();

        SMC_SetPowerModeStop(SMC, kSMC_PartialStop);

        SMC_PostExitStopModes();
    }

    return 0 ;
}
