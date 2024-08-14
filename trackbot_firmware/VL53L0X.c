#include "VL53L0X.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "sl_sleeptimer.h"

uint16_t distanceL, distanceR;
uint8_t active_sensor;										// 0: left is active; 1: right is active
static I2C_TransferSeq_TypeDef seq;
static uint8_t dataW[10];
static uint8_t dataR[6];

void VL53L0_write_byte (uint8_t reg_address, uint8_t write_data)
{
	dataW[0] = reg_address;
	dataW[1] = write_data;
	seq.flags = I2C_FLAG_WRITE;
	seq.buf[0].len = 2;
	if (active_sensor)
		I2CSPM_Transfer(I2C1, &seq);
	else
		I2CSPM_Transfer(I2C0, &seq);
}

uint8_t VL53L0_read_byte (uint8_t reg_address)
{
	dataW[0] = reg_address;
	seq.flags = I2C_FLAG_WRITE_READ;
	seq.buf[0].len = 1;
	seq.buf[1].len = 1;
	if (active_sensor)
		I2CSPM_Transfer(I2C1, &seq);
	else
		I2CSPM_Transfer(I2C0, &seq);
	return dataR[0];
}

void VL53L0_write_word (uint8_t reg_address, uint16_t write_data)
{
	dataW[0] = reg_address;
	dataW[1] = (write_data >> 8) & 0xFF;
	dataW[2] = write_data & 0xFF;
 	seq.flags = I2C_FLAG_WRITE;
 	seq.buf[0].len = 3;
	if (active_sensor)
		I2CSPM_Transfer(I2C1, &seq);
	else
		I2CSPM_Transfer(I2C0, &seq);
}

uint16_t VL53L0_read_word (uint8_t reg_address)
{
 	dataW[0] = reg_address;
 	seq.flags = I2C_FLAG_WRITE_READ;
 	seq.buf[0].len = 1;
 	seq.buf[1].len = 2;
	if (active_sensor)
		I2CSPM_Transfer(I2C1, &seq);
	else
		I2CSPM_Transfer(I2C0, &seq);
 	return (dataR[0] << 8) + dataR[1];
}

void VL53L0_write_bytes (uint8_t reg_address, uint8_t* write_data, uint8_t n_bytes)
{
	uint16_t cnt;
	dataW[0] = reg_address;

	for (cnt=0; cnt<n_bytes; cnt++) {
		dataW[cnt + 1] = write_data[cnt];
	}
 	seq.flags = I2C_FLAG_WRITE;
 	seq.buf[0].len = n_bytes+1;
	if (active_sensor)
		I2CSPM_Transfer(I2C1, &seq);
	else
		I2CSPM_Transfer(I2C0, &seq);
}

void VL53L0_read_bytes (uint8_t reg_address, uint8_t* read_data, uint8_t n_bytes)
{
	uint16_t cnt;
	dataW[0] = reg_address;
 	seq.buf[0].len = 1;
 	seq.buf[1].len = n_bytes;
 	seq.flags = I2C_FLAG_WRITE_READ;
	if (active_sensor)
		I2CSPM_Transfer(I2C1, &seq);
	else
		I2CSPM_Transfer(I2C0, &seq);
 	for (cnt=0; cnt<n_bytes; cnt++)
 		read_data[cnt] = dataR[cnt];
}

VL53L0_RETVAL VL53L0_init (void)
{
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(gpioPortB, IRQL, gpioModeInputPullFilter, 1);		// left sensor
//	GPIO_PinModeSet(gpioPortB, IRQR, gpioModeInputPullFilter, 1);		// right sensor
	GPIO_PinModeSet(gpioPortC, 7, gpioModeInputPullFilter, 1);		// right sensor
//	GPIO_ExtIntConfig(gpioPortB, IRQL, IRQL, false, true, true);
//	GPIO_ExtIntConfig(gpioPortB, IRQR, IRQR, false, true, true);

 	seq.addr = VL53L0_I2C_ADDR;														// sensor slave address
 	seq.flags = I2C_FLAG_WRITE;
 	seq.buf[0].data = dataW;
 	seq.buf[1].data = dataR;
 	active_sensor = 0;
 	VL53L0_default_cfg();
// 	VL53L0_start_continuous(0);
 	active_sensor = 1;
 	VL53L0_default_cfg();
// 	VL53L0_start_continuous(0);
   return VL53L0_OK;
}

VL53L0_RETVAL VL53L0_default_cfg (void)
{
	uint8_t spad_cnt;
	uint8_t spad_type_aperture;
	uint8_t first_spad_enable;
	uint8_t spads_enabled;
	uint8_t cnt;
	uint8_t tmp_buff;
	uint8_t stop_val;
	uint8_t ref_spad_map[6];

	sl_sleeptimer_delay_millisecond(100);
//	VL53L0_enable();
//	sl_sleeptimer_delay_millisecond(100);

//	VL53L0_hw_reset();
//	sl_sleeptimer_delay_millisecond(100);

	tmp_buff = VL53L0_read_byte(VL53L0_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV);
	tmp_buff |= 0x01;

	VL53L0_write_byte(VL53L0_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV, tmp_buff );

	VL53L0_write_byte(0x88, 0x00 );

	VL53L0_write_byte(0x80, 0x01 );
	VL53L0_write_byte(0xFF, 0x01 );
	VL53L0_write_byte(0x00, 0x00 );

	stop_val = VL53L0_read_byte(0x91 );

	VL53L0_write_byte(0x00, 0x01 );
	VL53L0_write_byte(0xFF, 0x00 );
	VL53L0_write_byte(0x80, 0x00 );

	tmp_buff = VL53L0_read_byte(VL53L0_MSRC_CONFIG_CONTROL );
	tmp_buff |= 0x12;

	VL53L0_write_byte(VL53L0_MSRC_CONFIG_CONTROL, tmp_buff );
	VL53L0_write_word(VL53L0_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT, 0x0010 );
	VL53L0_write_byte(VL53L0_SYSTEM_SEQUENCE_CONFIG, 0xFF );

	if (!VL53L0_get_spad_info(&spad_cnt, &spad_type_aperture ))
		return VL53L0_STATUS_ERR;

	VL53L0_read_bytes(VL53L0_GLOBAL_CONFIG_SPAD_ENABLES_REF_0, ref_spad_map, 6);

	VL53L0_write_byte(0xFF, 0x01 );
	VL53L0_write_byte(VL53L0_DYNAMIC_SPAD_REF_EN_START_OFFSET, 0x00 );
	VL53L0_write_byte(VL53L0_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD, 0x2C );
	VL53L0_write_byte(0xFF, 0x00 );
	VL53L0_write_byte(VL53L0_GLOBAL_CONFIG_REF_EN_START_SELECT, 0xB4 );

	first_spad_enable = spad_type_aperture ? 12 : 0;
	spads_enabled = 0;

	for (cnt=0; cnt<48; cnt++) {
		if ( cnt < first_spad_enable || spads_enabled == spad_cnt ) {
			ref_spad_map[cnt / 8] &= ~(1 << (cnt % 8));
		}
		else if ((ref_spad_map[cnt / 8] >> (cnt % 8)) & 0x01) {
			spads_enabled++;
		}
	}
	VL53L0_write_bytes(VL53L0_GLOBAL_CONFIG_SPAD_ENABLES_REF_0, ref_spad_map, 6);

	VL53L0_write_byte(0xFF, 0x01 );
	VL53L0_write_byte(0x00, 0x00 );
	VL53L0_write_byte(0xFF, 0x00 );
	VL53L0_write_byte(0x09, 0x00 );
	VL53L0_write_byte(0x10, 0x00 );
	VL53L0_write_byte(0x11, 0x00 );
	VL53L0_write_byte(0x24, 0x01 );
	VL53L0_write_byte(0x25, 0xFF );
	VL53L0_write_byte(0x75, 0x00 );
	VL53L0_write_byte(0xFF, 0x01 );
	VL53L0_write_byte(0x4E, 0x2C );
	VL53L0_write_byte(0x48, 0x00 );
	VL53L0_write_byte(0x30, 0x20 );
	VL53L0_write_byte(0xFF, 0x00 );
	VL53L0_write_byte(0x30, 0x09 );
	VL53L0_write_byte(0x54, 0x00 );
	VL53L0_write_byte(0x31, 0x04 );
	VL53L0_write_byte(0x32, 0x03 );
	VL53L0_write_byte(0x40, 0x83 );
	VL53L0_write_byte(0x46, 0x25 );
	VL53L0_write_byte(0x60, 0x00 );
	VL53L0_write_byte(0x27, 0x00 );
	VL53L0_write_byte(0x50, 0x06 );
	VL53L0_write_byte(0x51, 0x00 );
	VL53L0_write_byte(0x52, 0x96 );
	VL53L0_write_byte(0x56, 0x08 );
	VL53L0_write_byte(0x57, 0x30 );
	VL53L0_write_byte(0x61, 0x00 );
	VL53L0_write_byte(0x62, 0x00 );
	VL53L0_write_byte(0x64, 0x00 );
	VL53L0_write_byte(0x65, 0x00 );
	VL53L0_write_byte(0x66, 0xA0 );
	VL53L0_write_byte(0xFF, 0x01 );
	VL53L0_write_byte(0x22, 0x32 );
	VL53L0_write_byte(0x47, 0x14 );
	VL53L0_write_byte(0x49, 0xFF );
	VL53L0_write_byte(0x4A, 0x00 );
	VL53L0_write_byte(0xFF, 0x00 );
	VL53L0_write_byte(0x7A, 0x0A );
	VL53L0_write_byte(0x7B, 0x00 );
	VL53L0_write_byte(0x78, 0x21 );
	VL53L0_write_byte(0xFF, 0x01 );
	VL53L0_write_byte(0x23, 0x34 );
	VL53L0_write_byte(0x42, 0x00 );
	VL53L0_write_byte(0x44, 0xFF );
	VL53L0_write_byte(0x45, 0x26 );
	VL53L0_write_byte(0x46, 0x05 );
	VL53L0_write_byte(0x40, 0x40 );
	VL53L0_write_byte(0x0E, 0x06 );
	VL53L0_write_byte(0x20, 0x1A );
	VL53L0_write_byte(0x43, 0x40 );
	VL53L0_write_byte(0xFF, 0x00 );
	VL53L0_write_byte(0x34, 0x03 );
	VL53L0_write_byte(0x35, 0x44 );
	VL53L0_write_byte(0xFF, 0x01 );
	VL53L0_write_byte(0x31, 0x04 );
	VL53L0_write_byte(0x4B, 0x09 );
	VL53L0_write_byte(0x4C, 0x05 );
	VL53L0_write_byte(0x4D, 0x04 );
	VL53L0_write_byte(0xFF, 0x00 );
	VL53L0_write_byte(0x44, 0x00 );
	VL53L0_write_byte(0x45, 0x20 );
	VL53L0_write_byte(0x47, 0x08 );
	VL53L0_write_byte(0x48, 0x28 );
	VL53L0_write_byte(0x67, 0x00 );
	VL53L0_write_byte(0x70, 0x04 );
	VL53L0_write_byte(0x71, 0x01 );
	VL53L0_write_byte(0x72, 0xFE );
	VL53L0_write_byte(0x76, 0x00 );
	VL53L0_write_byte(0x77, 0x00 );
	VL53L0_write_byte(0xFF, 0x01 );
	VL53L0_write_byte(0x0D, 0x01 );
	VL53L0_write_byte(0xFF, 0x00 );
	VL53L0_write_byte(0x80, 0x01 );
	VL53L0_write_byte(0x01, 0xF8 );
	VL53L0_write_byte(0xFF, 0x01 );
	VL53L0_write_byte(0x8E, 0x01 );
	VL53L0_write_byte(0x00, 0x01 );
	VL53L0_write_byte(0xFF, 0x00 );
	VL53L0_write_byte(0x80, 0x00 );

	VL53L0_write_byte(VL53L0_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x04 );

	tmp_buff = VL53L0_read_byte(VL53L0_GPIO_HV_MUX_ACTIVE_HIGH );
	tmp_buff &= ~0x10;

	VL53L0_write_byte(VL53L0_GPIO_HV_MUX_ACTIVE_HIGH, tmp_buff );
	VL53L0_write_byte(VL53L0_SYSTEM_INTERRUPT_CLEAR, 0x01 );

//	m_timing_us = 10000;

	VL53L0_write_byte(VL53L0_SYSTEM_SEQUENCE_CONFIG, 0xE8 );
	VL53L0_write_byte(VL53L0_SYSTEM_SEQUENCE_CONFIG, 0x01 );

	if (!VL53L0_single_calibration(0x40 ))
		return VL53L0_STATUS_ERR;

	VL53L0_write_byte(VL53L0_SYSTEM_SEQUENCE_CONFIG, 0x02 );

	if (!VL53L0_single_calibration(0x00 ))
		return VL53L0_STATUS_ERR;

	VL53L0_write_byte(VL53L0_SYSTEM_SEQUENCE_CONFIG, 0xE8 );
	return VL53L0_STATUS_OK;
}

//void VL53L0_hw_reset (void)
//{
//	GPIO_PinOutClear(XSHport, XSHpin);
//	sl_sleeptimer_delay_millisecond(100);
//	GPIO_PinOutSet(XSHport, XSHpin);
//	sl_sleeptimer_delay_millisecond(100);
//}

void VL53L0_start_continuous (uint32_t period_ms)
{
	uint16_t osc_calibrate;
	uint8_t stop_val;

	VL53L0_write_byte(0x80, 0x01 );
	VL53L0_write_byte(0xFF, 0x01 );
	VL53L0_write_byte(0x00, 0x00 );

	stop_val = VL53L0_read_byte(0x91 );

	VL53L0_write_byte(0x91, stop_val );
	VL53L0_write_byte(0x00, 0x01 );
	VL53L0_write_byte(0xFF, 0x00 );
	VL53L0_write_byte(0x80, 0x00 );

	if (period_ms != 0) {
		osc_calibrate = VL53L0_read_word(VL53L0_OSC_CALIBRATE_VAL );
		if (osc_calibrate != 0) {
			period_ms *= osc_calibrate;
		}
		VL53L0_write_word(VL53L0_SYSTEM_INTERMEASUREMENT_PERIOD, 0x03E8 );
		VL53L0_write_byte(VL53L0_SYSRANGE_START, 0x04 );
	}
	else {
		VL53L0_write_byte(VL53L0_SYSRANGE_START, 0x02 );
	}
}

void VL53L0_stop_continuous (void)
{
	VL53L0_write_byte(VL53L0_SYSRANGE_START, 0x01 );
	VL53L0_write_byte(0xFF, 0x01 );
	VL53L0_write_byte(0x00, 0x00 );
	VL53L0_write_byte(0x91, 0x00 );
	VL53L0_write_byte(0x00, 0x01 );
	VL53L0_write_byte(0xFF, 0x00 );
}

VL53L0_RETVAL VL53L0_set_signal_rate_limit (float limit_mcps)
{
	uint16_t tmp_data;

	if (limit_mcps < 0.0 || limit_mcps > 511.99) {
		return VL53L0_STATUS_ERR;
	}
	tmp_data = ( uint16_t ) limit_mcps;
	tmp_data *= 0x80;
	VL53L0_write_word(VL53L0_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT, tmp_data );
	return VL53L0_OK;
}

float VL53L0_get_signal_rate_limit (void)
{
	float sr_limit;
	sr_limit = (float) VL53L0_read_word(VL53L0_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT );
	sr_limit /= 128.0;
	return sr_limit;
}

VL53L0_RETVAL VL53L0_get_spad_info (uint8_t* count_data, uint8_t* aperture_type)
{
	uint8_t tmp;

	VL53L0_write_byte(0x80, 0x01 );
	VL53L0_write_byte(0xFF, 0x01 );
	VL53L0_write_byte(0x00, 0x00 );
	VL53L0_write_byte(0xFF, 0x06 );

	tmp = VL53L0_read_byte(0x83 );
	tmp |= 0x04;

	VL53L0_write_byte(0x83, tmp );
	VL53L0_write_byte(0xFF, 0x07 );
	VL53L0_write_byte(0x81, 0x01 );

	VL53L0_write_byte(0x80, 0x01 );

	VL53L0_write_byte(0x94, 0x6b );
	VL53L0_write_byte(0x83, 0x00 );

	sl_sleeptimer_delay_millisecond(10);

	VL53L0_write_byte(0x83, 0x01 );

	tmp = VL53L0_read_byte(0x92 );

	*count_data = tmp & 0x7f;
	*aperture_type = ( tmp >> 7 ) & 0x01;

	VL53L0_write_byte(0x81, 0x00 );
	VL53L0_write_byte(0xFF, 0x06 );

	tmp = VL53L0_read_byte(0x83 & ( ~0x04 ) );

	VL53L0_write_byte(0x83, tmp );
	VL53L0_write_byte(0xFF, 0x01 );
	VL53L0_write_byte(0x00, 0x01 );

	VL53L0_write_byte(0xFF, 0x00 );
	VL53L0_write_byte(0x80, 0x00 );

	return VL53L0_STATUS_OK;
}

VL53L0_RETVAL VL53L0_single_calibration (uint8_t vhv_init_byte )
{
	vhv_init_byte |= 0x01;
	VL53L0_write_byte(VL53L0_SYSRANGE_START, vhv_init_byte );
	VL53L0_write_byte(VL53L0_SYSTEM_INTERRUPT_CLEAR, 0x01 );
	VL53L0_write_byte(VL53L0_SYSRANGE_START, 0x00 );
	return VL53L0_STATUS_OK;
}

uint16_t VL53L0_get_range_continuous (void)
{
	uint16_t range;
	uint16_t timeout = 1000;

	while ((VL53L0_read_byte(VL53L0_RESULT_RANGE_STATUS ) & 0x07) == 0) {
		if (timeout-- == 0)
			return 0;
		sl_sleeptimer_delay_millisecond(10);
	}
	range = VL53L0_read_word(VL53L0_RESULT_RANGE_STATUS + 10);
	VL53L0_write_byte(VL53L0_SYSTEM_INTERRUPT_CLEAR, 0x01 );
//	if (range > 2000)
//		return 0;
	return range;
}

uint16_t VL53L0_get_range_single (void)
{
	uint8_t stop_val;
	uint16_t range;

	VL53L0_write_byte(0x80, 0x01 );
	VL53L0_write_byte(0xFF, 0x01 );
	VL53L0_write_byte(0x00, 0x00 );

	stop_val = VL53L0_read_word(0x91 );
	VL53L0_write_byte(0x91, stop_val );

	VL53L0_write_byte(0x00, 0x01 );
	VL53L0_write_byte(0xFF, 0x00 );
	VL53L0_write_byte(0x80, 0x00 );

	VL53L0_write_byte(VL53L0_SYSRANGE_START, 0x01 );

	while (VL53L0_read_word(VL53L0_SYSRANGE_START) & 0x01) {
//		if (check_timeout_expired()) {
//			ctx->did_timeout = 1;
//			return 65535;
//		}
	}

	range = VL53L0_get_range_continuous();
//	if (range > 2000)
//		return 0;
	return range;
}


