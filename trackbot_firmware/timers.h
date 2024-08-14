#ifndef TIMERS_H_
#define TIMERS_H_

#include "em_timer.h"
#include "em_letimer.h"
#include "stdint.h"

#define MEASURE_PERIOD		250							// measure period in ms
#define SOUND_VELOCITY  	34.322						// cm/ms
#define IRQF					1								// IRQ pin on PC.01
#define MOTOR_RIGHT			8								// motor control pins on PortA
#define MOTOR_LEFT			7								// 0: off;  1: on
#define MOTOR_DIR_RIGHT		2								// motor direction on PortD
#define MOTOR_DIR_LEFT		3								// 0: forwards;  1: backwards

extern uint8_t sensorF_ready;
extern uint32_t distanceF;
extern uint32_t timer0_freq;

void Timer0_setup(void);
void Letimer_setup(void);

#endif /* TIMERS_H_ */
