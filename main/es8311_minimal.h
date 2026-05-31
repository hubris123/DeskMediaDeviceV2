/*
 * ES8311 Minimal Codec Driver
 * Direct I2C register writes — no library dependency.
 *
 * Hardcoded for:
 *   Sample rate : 16000 Hz
 *   MCLK        : 16000 * 384 = 6144000 Hz
 *   Bit depth   : 16-bit
 *   Mode        : DAC only (playback), I2S Philips (standard) format, slave
 *
 * Register values derived from espressif/esp_codec_dev es8311.c source.
 */

#pragma once

#include "esp_err.h"
#include "driver/i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * ES8311 I2C address
 * CE pin pulled low on Waveshare ESP32-P4-WIFI6-Touch-LCD-4.3 → 7-bit addr 0x18
 * -------------------------------------------------------------------------*/
#define ES8311_I2C_ADDR         (0x18)

/* -------------------------------------------------------------------------
 * Register addresses
 * -------------------------------------------------------------------------*/
#define ES8311_REG00_RESET      0x00
#define ES8311_REG01_CLK1       0x01
#define ES8311_REG02_CLK2       0x02
#define ES8311_REG03_CLK3       0x03
#define ES8311_REG04_CLK4       0x04
#define ES8311_REG05_CLK5       0x05
#define ES8311_REG06_CLK6       0x06
#define ES8311_REG07_CLK7       0x07
#define ES8311_REG08_CLK8       0x08
#define ES8311_REG09_SDPIN      0x09  /* DAC serial port */
#define ES8311_REG0A_SDPOUT     0x0A  /* ADC serial port */
#define ES8311_REG0B_SYS        0x0B
#define ES8311_REG0C_SYS        0x0C
#define ES8311_REG0D_SYS        0x0D  /* power up/down */
#define ES8311_REG0E_SYS        0x0E  /* analog power */
#define ES8311_REG10_SYS        0x10
#define ES8311_REG11_SYS        0x11
#define ES8311_REG12_SYS        0x12  /* DAC enable */
#define ES8311_REG13_SYS        0x13
#define ES8311_REG14_SYS        0x14  /* analog PGA gain */
#define ES8311_REG15_ADC        0x15
#define ES8311_REG16_ADC        0x16  /* ADC PGA gain */
#define ES8311_REG17_ADC        0x17  /* ADC volume */
#define ES8311_REG1B_ADC        0x1B  /* ADC HPF */
#define ES8311_REG1C_ADC        0x1C  /* ADC equalizer */
#define ES8311_REG31_DAC        0x31  /* DAC mute */
#define ES8311_REG32_DAC        0x32  /* DAC volume */
#define ES8311_REG37_DAC        0x37  /* DAC ramp rate */
#define ES8311_REG44_GPIO       0x44  /* internal reference */
#define ES8311_REG45_GP         0x45

/* -------------------------------------------------------------------------
 * Precomputed register values for 16kHz / 16-bit / slave / MCLK=6144000 Hz
 *
 * Clock coefficients for mclk=6144000, rate=16000 (from coeff_div table):
 *   pre_div=3, pre_multi=2, adc_div=1, dac_div=1
 *   fs_mode=0, lrck_h=0, lrck_l=0xFF, bclk_div=4
 *   adc_osr=0x10, dac_osr=0x20
 * -------------------------------------------------------------------------*/

/* REG02: pre_div=(3-1)<<5=0x40 | pre_multi_enc(2)=1<<3=0x08 → 0x48 */
#define ES8311_VAL_REG02        0x48
/* REG03: fs_mode=0<<6 | adc_osr=0x10 → 0x10 */
#define ES8311_VAL_REG03        0x10
/* REG04: dac_osr=0x20 → 0x20 */
#define ES8311_VAL_REG04        0x20
/* REG05: (adc_div-1)<<4 | (dac_div-1) = 0 → 0x00 */
#define ES8311_VAL_REG05        0x00
/* REG06: (bclk_div-1) = 3 → 0x03 */
#define ES8311_VAL_REG06        0x03
/* REG07: lrck_h = 0 → 0x00 */
#define ES8311_VAL_REG07        0x00
/* REG08: lrck_l = 0xFF */
#define ES8311_VAL_REG08        0xFF

/* DAC volume: 0xBF ≈ 0 dB (roughly 75% of full scale register range) */
#define ES8311_DAC_VOLUME       0xBF

/* -------------------------------------------------------------------------
 * Public API
 * -------------------------------------------------------------------------*/

/**
 * @brief Initialize ES8311 codec via direct I2C register writes.
 *
 * Call AFTER bsp_i2c_init() (or bsp_display_start_with_config(), which
 * initialises I2C internally). Uses the BSP I2C bus handle.
 *
 * @param i2c_bus  BSP I2C master bus handle (from bsp_i2c_get_handle())
 * @return ESP_OK on success
 */
esp_err_t es8311_minimal_init(i2c_master_bus_handle_t i2c_bus);

#ifdef __cplusplus
}
#endif
