#ifndef VL53L0X_H
#define VL53L0X_H

#include "stdint.h"
#include "sl_i2cspm.h"
extern uint16_t distanceL, distanceR;
extern uint8_t active_sensor;

//#define 	XSHport 	gpioPortD
//#define	XSHpin	2
//#define 	INTport	gpioPortB
//#define	INTpin	1
#define IRQL		1
#define IRQR		4

typedef	int8_t VL53L0_RETVAL;

#define VL53L0_OK           											 0x00
#define VL53L0_INIT_ERROR   											 0xFF
#define VL53L0_I2C_ADDR                                      0x52
#define VL53L0_SYSRANGE_START                                0x00
#define VL53L0_SYSTEM_THRESH_HIGH                            0x0C
#define VL53L0_SYSTEM_THRESH_LOW                             0x0E
#define VL53L0_SYSTEM_SEQUENCE_CONFIG                        0x01
#define VL53L0_SYSTEM_RANGE_CONFIG                           0x09
#define VL53L0_SYSTEM_INTERMEASUREMENT_PERIOD                0x04
#define VL53L0_SYSTEM_INTERRUPT_CONFIG_GPIO                  0x0A
#define VL53L0_GPIO_HV_MUX_ACTIVE_HIGH                       0x84
#define VL53L0_SYSTEM_INTERRUPT_CLEAR                        0x0B
#define VL53L0_RESULT_INTERRUPT_STATUS                       0x13
#define VL53L0_RESULT_RANGE_STATUS                           0x14
#define VL53L0_RESULT_CORE_AMBIENT_WINDOW_EVENTS_RTN         0xBC
#define VL53L0_RESULT_CORE_RANGING_TOTAL_EVENTS_RTN          0xC0
#define VL53L0_RESULT_CORE_AMBIENT_WINDOW_EVENTS_REF         0xD0
#define VL53L0_RESULT_CORE_RANGING_TOTAL_EVENTS_REF          0xD4
#define VL53L0_RESULT_PEAK_SIGNAL_RATE_REF                   0xB6
#define VL53L0_ALGO_PART_TO_PART_RANGE_OFFSET_MM             0x28
#define VL53L0_MSRC_CONFIG_CONTROL                           0x60
#define VL53L0_PRE_RANGE_CONFIG_MIN_SNR                      0x27
#define VL53L0_PRE_RANGE_CONFIG_VALID_PHASE_LOW              0x56
#define VL53L0_PRE_RANGE_CONFIG_VALID_PHASE_HIGH             0x57
#define VL53L0_PRE_RANGE_MIN_COUNT_RATE_RTN_LIMIT            0x64
#define VL53L0_FINAL_RANGE_CONFIG_MIN_SNR                    0x67
#define VL53L0_FINAL_RANGE_CONFIG_VALID_PHASE_LOW            0x47
#define VL53L0_FINAL_RANGE_CONFIG_VALID_PHASE_HIGH           0x48
#define VL53L0_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT   0x44
#define VL53L0_PRE_RANGE_CONFIG_SIGMA_THRESH_HI              0x61
#define VL53L0_PRE_RANGE_CONFIG_SIGMA_THRESH_LO              0x62
#define VL53L0_PRE_RANGE_CONFIG_VCSEL_PERIOD                 0x50
#define VL53L0_PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI            0x51
#define VL53L0_PRE_RANGE_CONFIG_TIMEOUT_MACROP_LO            0x52
#define VL53L0_SYSTEM_HISTOGRAM_BIN                          0x81
#define VL53L0_HISTOGRAM_CONFIG_INITIAL_PHASE_SELECT         0x33
#define VL53L0_HISTOGRAM_CONFIG_READOUT_CTRL                 0x55
#define VL53L0_FINAL_RANGE_CONFIG_VCSEL_PERIOD               0x70
#define VL53L0_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI          0x71
#define VL53L0_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_LO          0x72
#define VL53L0_CROSSTALK_COMPENSATION_PEAK_RATE_MCPS         0x20
#define VL53L0_MSRC_CONFIG_TIMEOUT_MACROP                    0x46
#define VL53L0_SOFT_RESET_GO2_SOFT_RESET_N                   0xBF
#define VL53L0_IDENTIFICATION_MODEL_ID                       0xC0
#define VL53L0_IDENTIFICATION_REVISION_ID                    0xC2
#define VL53L0_OSC_CALIBRATE_VAL                             0xF8
#define VL53L0_GLOBAL_CONFIG_VCSEL_WIDTH                     0x32
#define VL53L0_GLOBAL_CONFIG_SPAD_ENABLES_REF_0              0xB0
#define VL53L0_GLOBAL_CONFIG_SPAD_ENABLES_REF_1              0xB1
#define VL53L0_GLOBAL_CONFIG_SPAD_ENABLES_REF_2              0xB2
#define VL53L0_GLOBAL_CONFIG_SPAD_ENABLES_REF_3              0xB3
#define VL53L0_GLOBAL_CONFIG_SPAD_ENABLES_REF_4              0xB4
#define VL53L0_GLOBAL_CONFIG_SPAD_ENABLES_REF_5              0xB5
#define VL53L0_GLOBAL_CONFIG_REF_EN_START_SELECT             0xB6
#define VL53L0_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD           0x4E
#define VL53L0_DYNAMIC_SPAD_REF_EN_START_OFFSET              0x4F
#define VL53L0_POWER_MANAGEMENT_GO1_POWER_FORCE              0x80
#define VL53L0_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV             0x89
#define VL53L0_ALGO_PHASECAL_LIM                             0x30
#define VL53L0_ALGO_PHASECAL_CONFIG_TIMEOUT                  0x30

#define VL53L0_STATUS_ERR                                    0x00
#define VL53L0_STATUS_OK                                     0x01

/**
 * @brief Initialization function.
 * @param cfg Click configuration structure.
 * 
 * @description This function initializes all necessary pins and peripherals used for this click.
 */
VL53L0_RETVAL VL53L0_init (void);

/**
 * @brief Click Default Configuration function.
 * @description This function executes default configuration for LightRanger2 click.
 */
VL53L0_RETVAL VL53L0_default_cfg (void);

/**
 * @brief Click Default Configuration function.
 * @param reg_address    8-bit register address.
 * @param write_data     8-bit data to write.
 * @description This function writes a byte of data to the targeted 8-bit register
 * address of the VL53L0X ranging and gesture detection sensor on the LightRanger
 * 2 click board.
 */
void VL53L0_write_byte (uint8_t reg_address, uint8_t write_data);

/**
 * @brief Click Default Configuration function.
 * @param reg_address    8-bit register address.
 * @returns 8-bit read data.
 * @description Function reads a byte of data from the targeted 8-bit register 
 * address of the VL53L0X ranging and gesture detection sensor on the LightRanger
 * 2 click board.
 */
uint8_t VL53L0_read_byte (uint8_t reg_address);

/**
 * @brief Click Default Configuration function.
 * @param reg_address    8-bit register address.
 * @param write_data     16-bit data to write.
 * @description This function writes the 16-bit data to the targeted 8-bit register
 * address of the VL53L0X ranging and gesture detection sensor on the LightRanger
 * 2 click board.
 */
void VL53L0_write_word (uint8_t reg_address, uint16_t write_data);

/**
 * @brief Click Default Configuration function.
 * @param reg_address    8-bit register address.
 * @returns 16-bit read data.
 * @description Function reads the 16-bit data from the targeted 8-bit register
 * address of the VL53L0X ranging and gesture detection sensor
 */
uint16_t VL53L0_read_word (uint8_t reg_address);

/**
 * @brief Generic sequential data write function
 * @param reg_address                8-bit register address
 * @param write_data                 pointer to the data to be written
 * @param n_bytes                    number of bytes to be written
 * @description Function writes a sequential data starting to the
 * targeted 8-bit register address of the
 * VL53L0X ranging and gesture detection sensor
 */
void VL53L0_write_bytes (uint8_t reg_address, uint8_t* write_data, uint8_t n_bytes );

/**
 * @brief Generic sequential data write function
 * @param reg_address                8-bit register address
 * @param read_data                  pointer to the data to be read
 * @param n_bytes                    number of bytes to read
 * @description Function reads a sequential data starting from the
 * targeted 8-bit register address of the
 * VL53L0X ranging and gesture detection sensor
 */
void VL53L0_read_bytes (uint8_t reg_address, uint8_t* read_data, uint8_t n_bytes );

/**
 * @brief Hardware reset function
 * @description Function hardware reset the VL53L0X ranging and gesture detection sensor
 * by cleared to '0' states of RST pin, wait 100 ms,
 * sets to '1' states of RST pin and wait for another 100 ms.
 * @note
 * Delay is 200 ms.
 */
void VL53L0_hw_reset (void);

/**
 * @brief Enable function
 * @description Function enable VL53L0X ranging and gesture detection sensor
 * by sets to '1' states of RST pin
 * of the LightRanger 2 click board.
 */
void VL53L0_enable (void);

/**
 * @brief Disable function
 * @description Function disable VL53L0X ranging and gesture detection sensor
 * by cleared to '0' states of RST pin
 * of the LightRanger 2 click board.
 */
void VL53L0_disable (void);

/**
 * @brief Get interrupt state function
 * @return interrupt state
 * - 0 : not active
 * - 1 : active
 * @description The function gets interrupt state
 * by return status of INT pin of the
 * LightRanger 2 click board.
 */
uint8_t VL53L0_get_interrupt (void);

/**
 * @brief Start continuous ranging measurements function
 * @param period_ms                32-bit period in ms
 * @description Function start continuous ranging measurements of the
 * VL53L0X ranging and gesture detection sensor
 * on the LightRanger 2 click board.
 */
void VL53L0_start_continuous (uint32_t period_ms);

/**
 * @brief Stop continuous measurements function
 * @description Function stop continuous ranging measurements of the
 * VL53L0X ranging and gesture detection sensor
 * on the LightRanger 2 click board.
 */
void VL53L0_stop_continuous (void);

/**
 * @brief Set the signal rate limit check value function
 * @param period_ms      float signal rate limit check value
 * @return status
 * - 0 : ERROR;
 * - 1 : OK;
 * @description Function sets the limit presumably determines
 * the minimum measurement necessary for the sensor to report a valid reading
 * of the VL53L0X ranging and gesture detection sensor
 * on the LightRanger 2 click board.
 */
VL53L0_RETVAL VL53L0_set_signal_rate_limit (float limit_mcps);

/**
 * @brief Get the signal rate limit check value function
 * @return
 * float signal rate limit check value
 * @description Function gets the signal rate limit check value of the
 * VL53L0X ranging and gesture detection sensor
 * on the LightRanger 2 click board.
 */
float VL53L0_get_signal_rate_limit (void);

/**
 * @brief Get reference SPAD ( single photon avalanche diode ) function
 * @param count_data               pointer to the memory location where count data data be stored
 * @param aperture_Type            pointer to the memory location where aperture type data be stored
 * @return status
 * - 0 : ERROR;
 * - 1 : OK;
 * @description Function get reference SPAD ( single photon avalanche diode )
 * count and type of the
 * VL53L0X ranging and gesture detection sensor
 * on the LightRanger 2 click board.
 */
VL53L0_RETVAL VL53L0_get_spad_info (uint8_t* count_data, uint8_t* aperture_type);

/**
 * @brief Perform single reference calibration function
 * @param vhv_init_byte              8-bit vhv init byte
 * @return status
 * - 0 : ERROR;
 * - 1 : OK;
 * @description Function perform single reference calibration
 * of the VL53L0X ranging and gesture detection sensor
 * on the LightRanger 2 click board.
 */
VL53L0_RETVAL VL53L0_single_calibration (uint8_t vhv_init_byte);

/**
 * @brief Get a range measurement ( continuous mode ) function
 * @return
 * 16-bit range measurement in millimeters value
 * @description Function gets a range measurement in millimeters when continuous mode is active
 * of the VL53L0X ranging and gesture detection sensor
 * on the LightRanger 2 click board.
 */
uint16_t VL53L0_get_range_continuous (void);

/**
 * @brief Get a range measurement ( single mode ) function
 * @return
 * 16-bit range measurement in millimeters value
 * @description Function gets a range measurement in millimeters when single mode is active
 * of the VL53L0X ranging and gesture detection sensor
 * on the LightRanger 2 click board.
 */
uint16_t VL53L0_get_range_single (void);

#endif  // _VL53L0X_H_

