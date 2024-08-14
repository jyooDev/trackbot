#include "timers.h"
#include "em_cmu.h"
#include "em_gpio.h"

uint8_t sensorF_ready;
uint32_t distanceF;
uint32_t timer0_freq;

void Timer0_setup(void)															// configure TIMER0 for measuring reaction time
{
	GPIO_PinModeSet(gpioPortC, 0, gpioModePushPull, 0);				// trig pin
	GPIO_PinModeSet(gpioPortC, IRQF, gpioModeInputPullFilter, 0);	// echo pin

	CMU_ClockEnable(cmuClock_TIMER0, true);
	timer0_freq = CMU_ClockFreqGet(cmuClock_TIMER0);
	TIMER_Init_TypeDef initTMR = TIMER_INIT_DEFAULT;
	initTMR.enable = false;														// do not start timer right after setup

	TIMER_InitCC_TypeDef initCC0 = TIMER_INITCC_DEFAULT;				// CC0 is used for generating 10us pulses with a certain period
	initCC0.mode = timerCCModePWM;											// this could be any value

	TIMER_InitCC_TypeDef initCC1 = TIMER_INITCC_DEFAULT;				// CC1 is used to start the echo width measurement
	initCC1.edge = timerEdgeRising;											// perform capture at rising edge
	initCC1.mode = timerCCModeCapture;

	TIMER_InitCC_TypeDef initCC2 = TIMER_INITCC_DEFAULT;				// CC2 is used to stop the echo width measurement
	initCC2.edge = timerEdgeFalling;											// perform capture at falling edge
	initCC2.mode = timerCCModeCapture;

	TIMER_Init(TIMER0, &initTMR);												// init and start TIMER0
	TIMER_InitCC(TIMER0, 0, &initCC0);
	TIMER_InitCC(TIMER0, 1, &initCC1);
	TIMER_InitCC(TIMER0, 2, &initCC2);

	TIMER0->TOP = (timer0_freq / 1000)*MEASURE_PERIOD;
	TIMER_CompareBufSet(TIMER0, 0, (timer0_freq/1000000)*11);		// set 11us pulse duration
	(GPIO->TIMERROUTE[0]).CC0ROUTE = (0<<16) + gpioPortC;				// set TRIG output
	(GPIO->TIMERROUTE[0]).CC1ROUTE = (IRQF<<16) + gpioPortC;			// set ECHO input
	(GPIO->TIMERROUTE[0]).CC2ROUTE = (IRQF<<16) + gpioPortC;			// set ECHO input
	(GPIO->TIMERROUTE[0]).ROUTEEN = 0x07;									// enable routing for CC0-CC2 channels

	TIMER_Enable(TIMER0, true);
	TIMER_IntEnable(TIMER0, TIMER_IEN_CC2);
	NVIC_EnableIRQ(TIMER0_IRQn);
}

void Letimer_setup(void)
{
	// pins for the motor driver
	GPIO_PinModeSet(gpioPortA, MOTOR_LEFT, gpioModePushPull, 1);				// PWM output left
	GPIO_PinModeSet(gpioPortA, MOTOR_RIGHT, gpioModePushPull, 1);				// PWM output right
	GPIO_PinModeSet(gpioPortD, MOTOR_DIR_RIGHT, gpioModeWiredAnd, 0);			// direction right
	GPIO_PinModeSet(gpioPortD, MOTOR_DIR_LEFT, gpioModeWiredAnd, 0);			// direction left

//	GPIO->LETIMERROUTE[0].OUT0ROUTE = gpioPortA | (7 << _GPIO_LETIMER_OUT0ROUTE_PIN_SHIFT);
//	GPIO->LETIMERROUTE[0].OUT1ROUTE = gpioPortA | (8 << _GPIO_LETIMER_OUT0ROUTE_PIN_SHIFT);
//	GPIO->LETIMERROUTE[0].ROUTEEN = GPIO_LETIMER_ROUTEEN_OUT0PEN + GPIO_LETIMER_ROUTEEN_OUT1PEN;
//
//	CMU_ClockEnable(cmuClock_LETIMER0, true);
//	LETIMER_Init_TypeDef letimerInit = LETIMER_INIT_DEFAULT;			// set up Letimer for 10ms interrupts when connected
//	letimerInit.enable = false;												// do not start Letimer right after initialization
//	letimerInit.comp0Top = true;												// Reload COMP0 on underflow
//	letimerInit.topValue = 327;												// 32768 pulses in 1 sec (100Hz PWM)
//	letimerInit.repMode = letimerRepeatFree;
//	letimerInit.ufoa0 = letimerUFOAPwm;
//	letimerInit.ufoa1 = letimerUFOAPwm;
//	LETIMER_CounterSet(LETIMER0, letimerInit.topValue);
//	LETIMER_CompareSet(LETIMER0, 0, 327/2);								// duty cycle
//	LETIMER_CompareSet(LETIMER0, 1, 327/2);								// duty cycle
//	LETIMER_Init(LETIMER0, &letimerInit);									// Initialize and enable LETIMER
}

void TIMER0_IRQHandler(void)
{
	TIMER_IntClear(TIMER0, TIMER_IF_CC2);
	sensorF_ready = 1;
}
