/*
 * Timers.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: Pankhard159
 */

#include "c_timers.h"

bool C_Timers::status_flag = false;

void PIT_IRQHandler() {
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
	C_Timers::status_flag = true;
}


void C_Timers::EnableInterrupt(){
	/* Enable timer interrupts for channel 0 */
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

	/* Enable interrupt at the NVIC - Nested vectored interrupt controller */
	EnableIRQ(PIT_IRQn);
}
void C_Timers::DisableInterrupt(){
	/* Disable timer interrupts for channel 0 */
	PIT_DisableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

	/* Disable interrupt at the NVIC - Nested vectored interrupt controller */
	DisableIRQ(PIT_IRQn);
}


C_Timers::C_Timers() {
	// TODO Auto-generated constructor stub
	C_Timers::status_flag = false;
}

C_Timers::~C_Timers() {
	// TODO Auto-generated destructor stub
}

void C_Timers::PIT_init(){

	pit_config_t pitConfig;

	PIT_GetDefaultConfig(&pitConfig);

	/* Init pit module */
	PIT_Init(PIT, &pitConfig);
}

void C_Timers::PIT_startTimer(int time_ms){
	/* Set timer period for channel 0 */
	//pit_status_flags_t
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, MSEC_TO_COUNT(time_ms, PIT_SOURCE_CLOCK));
	PIT_StartTimer(PIT, kPIT_Chnl_0);
}

void C_Timers::PIT_stopTimer(){
	PIT_StopTimer(PIT, kPIT_Chnl_0);
}

bool C_Timers::getStatusFlag(){
	return C_Timers::status_flag;
}
void C_Timers::clearStatusFlag(){
	C_Timers::status_flag = false;
}
void C_Timers::wait(int time_ms){

	this->PIT_startTimer(time_ms);

	while(!this->getStatusFlag());

	this->clearStatusFlag();
	this->PIT_stopTimer();

}
