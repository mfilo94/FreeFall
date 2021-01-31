/*
 * Timers.h
 *
 *  Created on: Nov 27, 2018
 *      Author: Pankhard159
 */

#include "fsl_pit.h"
#include "fsl_common.h"
#include "fsl_clock.h"

#ifndef C_TIMERS_H_
#define C_TIMERS_H_


#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)


extern "C" {

void PIT_IRQHandler(void);

}


class C_Timers {
private:

public:
	static bool status_flag;

	C_Timers();
	virtual ~C_Timers();

	void PIT_init();
	void EnableInterrupt();
	void DisableInterrupt();
	void PIT_startTimer(int time_ms);
	void PIT_stopTimer();
	bool getStatusFlag();
	void clearStatusFlag();
	void wait(int time_ms);

};

#endif /* TIMERS_H_ */
