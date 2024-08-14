#include "em_common.h"
#include "app_assert.h"
#include "sl_bluetooth.h"
#include "app.h"

#include "gatt_db.h"
#include "printf.h"
#include "em_gpio.h"
#include "gpiointerrupt.h"
#include "timers.h"
#include "VL53L0X.h"

#define SIGNAL_LEFT	0
#define SIGNAL_RIGHT	1
#define SIGNAL_FRONT	2
#define SIDE_THRESHOLD	150
#define FRONT_THRESHOLD	20

#define BL_MODE_OFF 	0
#define BL_MODE_ON 	1
#define BL_FORWARD 	2
#define BL_BACKWARD 	3
#define BL_LEFT 		4
#define BL_RIGHT 		5

static void Timer_Handler(sl_sleeptimer_timer_handle_t *handle, void *data);

static sl_sleeptimer_timer_handle_t timer_handle;
static uint8_t timer_signal = 0;
static uint8_t command, isMoving;
static uint8_t advertising_set_handle = 0xff;
static uint8_t connection_id = 0xff;
static uint32_t capture1, capture2;
static int32_t duration;
static float pulseWidth;


SL_WEAK void app_init(void)
{
	printf("Trackbot is on\n");
	Letimer_setup();
	sl_sleeptimer_delay_millisecond(1000);
	sensorF_ready = command = 0;
	isMoving = 1;
	VL53L0_init();
	Timer0_setup();
}

SL_WEAK void app_process_action(void)
{
	if (sensorF_ready) {
		sensorF_ready = 0;
		capture1 = (TIMER0->CC[1]).ICOF;
		capture2 = (TIMER0->CC[2]).ICOF;
		duration = capture2 - capture1;
		if (duration < 0)
			duration += TIMER0->TOP;
		pulseWidth = (duration*1000.0)/timer0_freq;							// pulse width in ms
		distanceF = (uint32_t)((pulseWidth * SOUND_VELOCITY)/2.0 + 0.5);	// distance in cm
//		printf("capture1=%d  capture2=%d  duration=%d\n", capture1, capture2, duration);
//		printf("pulseWidth=%f\n", pulseWidth);
		if (command == 0) {
			if (distanceF <= FRONT_THRESHOLD) {							// stop motors
				GPIO_PinOutClear(gpioPortA, MOTOR_RIGHT);
				GPIO_PinOutClear(gpioPortA, MOTOR_LEFT);
			}
			else {
				GPIO_PinOutSet(gpioPortA, MOTOR_RIGHT);				// motors on
				GPIO_PinOutSet(gpioPortA, MOTOR_LEFT);
			}
		}
		sl_bt_external_signal(SIGNAL_FRONT);
		printf("distanceF=%dcm\n", distanceF);
	}
	if (timer_signal) {
		timer_signal = 0;
		active_sensor = 0;
		distanceL = VL53L0_get_range_single();
		sl_bt_external_signal(SIGNAL_LEFT);
		if (command == 0) {
			if (distanceL <= SIDE_THRESHOLD) {
				printf("\tDistanceL = %dmm\n", distanceL);
				GPIO_PinOutSet(gpioPortD, MOTOR_DIR_RIGHT);			// reverse motor direction
			}
			else
				GPIO_PinOutClear(gpioPortD, MOTOR_DIR_RIGHT);
		}
		active_sensor = 1;
		distanceR = VL53L0_get_range_single();
		sl_bt_external_signal(SIGNAL_RIGHT);
		if (command == 0) {
			if (distanceR <= SIDE_THRESHOLD) {
				printf("\t\tDistanceR = %dmm\n", distanceR);
				GPIO_PinOutSet(gpioPortD, MOTOR_DIR_LEFT);			// reverse motor direction
			}
			else
				GPIO_PinOutClear(gpioPortD, MOTOR_DIR_LEFT);
		}
	}
}

void sl_bt_on_event(sl_bt_msg_t *evt)
{
	switch (SL_BT_MSG_ID(evt->header)) {
		case sl_bt_evt_system_boot_id:
			sl_bt_advertiser_create_set(&advertising_set_handle);
			sl_bt_legacy_advertiser_generate_data(advertising_set_handle, sl_bt_advertiser_general_discoverable);
			sl_bt_advertiser_set_timing(advertising_set_handle, 160, 160, 0, 0);
			sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_advertiser_connectable_scannable);
			sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
			sl_sleeptimer_start_periodic_timer_ms(&timer_handle, 250, Timer_Handler, NULL, 0, 0);
			break;

		case sl_bt_evt_connection_opened_id:
			connection_id = evt->data.evt_connection_opened.connection;
			break;

		case sl_bt_evt_connection_closed_id:
			sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_advertiser_connectable_scannable);
			connection_id = 0xff;
			break;

		case sl_bt_evt_gatt_server_attribute_value_id:
			if (evt->data.evt_gatt_server_attribute_value.attribute == gattdb_command) {
				command = evt->data.evt_gatt_server_attribute_value.value.data[0];
				printf("Received command = %d\n", command);
				switch (command) {
					case BL_MODE_ON:	//temporarily stop motors to allow manual driving
						isMoving = 0;
						GPIO_PinOutClear(gpioPortA, MOTOR_RIGHT);
						GPIO_PinOutClear(gpioPortA, MOTOR_LEFT);
					break;

					case BL_LEFT:		// Turn left - reverse direction of the left motor
						if (isMoving) {
							GPIO_PinOutSet(gpioPortD, MOTOR_DIR_LEFT);
//							GPIO_PinOutToggle(gpioPortD, MOTOR_DIR_LEFT);
							printf("Turning left\n");
						}
				   break;

					case BL_RIGHT:		// Turn rigth - reverse direction of the right motor
						if (isMoving) {
							GPIO_PinOutSet(gpioPortD, MOTOR_DIR_RIGHT);
//							GPIO_PinOutToggle(gpioPortD, MOTOR_DIR_RIGHT);
							printf("Turning right\n");
						}
					break;

					case BL_FORWARD:	// Start moving forwards
						GPIO_PinOutClear(gpioPortD, MOTOR_DIR_LEFT);
						GPIO_PinOutClear(gpioPortD, MOTOR_DIR_RIGHT);
						GPIO_PinOutSet(gpioPortA, MOTOR_RIGHT);
						GPIO_PinOutSet(gpioPortA, MOTOR_LEFT);
						isMoving = 1;
					break;

					case BL_BACKWARD:	// Turn left - L motor backward, R motor forward
						GPIO_PinOutSet(gpioPortD, MOTOR_DIR_LEFT);
						GPIO_PinOutSet(gpioPortD, MOTOR_DIR_RIGHT);
						GPIO_PinOutSet(gpioPortA, MOTOR_RIGHT);
						GPIO_PinOutSet(gpioPortA, MOTOR_LEFT);
						isMoving = 1;
					break;

					default:
					break;
				}
			}
			break;

		case sl_bt_evt_system_external_signal_id:
			if (evt->data.evt_system_external_signal.extsignals == SIGNAL_LEFT) {
				sl_bt_gatt_server_send_notification(connection_id, gattdb_distance_left, sizeof(distanceL), (uint8_t*)&distanceL);
			}
			if (evt->data.evt_system_external_signal.extsignals == SIGNAL_RIGHT) {
				sl_bt_gatt_server_send_notification(connection_id, gattdb_distance_right, sizeof(distanceR), (uint8_t*)&distanceR);
			}
			if (evt->data.evt_system_external_signal.extsignals == SIGNAL_FRONT) {
				sl_bt_gatt_server_send_notification(connection_id, gattdb_distance_front, sizeof(distanceF), (uint8_t*)&distanceF);
			}
			break;


		default:
			break;
	}
}

static void Timer_Handler(sl_sleeptimer_timer_handle_t *handle, void *data)
{
	(void) handle;
	(void) data;
	timer_signal = 1;
}
