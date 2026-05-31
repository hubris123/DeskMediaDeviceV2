#include "es8311_minimal.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "ES8311";

#define ES8311_PA_PIN 53

inline static esp_err_t es8311_write_reg(i2c_master_dev_handle_t dev, uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = {reg, val};
    return i2c_master_transmit(dev, buf, 2, -1);
}

/**
 * Initialize ES8311 for DAC playback only.
 *
 * Configuration: 16kHz, 16-bit, stereo, I2S (Philips) format, slave mode.
 * MCLK = 6.144 MHz (384 × 16kHz) provided by ESP32 I2S on GPIO13.
 *
 * Register values derived from espressif/esp_codec_dev es8311 driver:
 *   Clock table entry {mclk=6144000, rate=16000}:
 *     pre_div=3, pre_multi=2, adc_div=1, dac_div=1,
 *     fs_mode=0, lrck_h=0x00, lrck_l=0xFF, bclk_div=4,
 *     adc_osr=0x10, dac_osr=0x20
 */
esp_err_t es8311_minimal_init(i2c_master_dev_handle_t i2c_dev)
{
    esp_err_t ret;

    // --- Step 1: Pre-reset defaults (mirrors es8311_open initial writes) ---
    uint8_t pre_reset[][2] = {
        {0x01, 0x30},  // CLK_MANAGER_REG01: initial before reset
        {0x02, 0x00},  // CLK_MANAGER_REG02
        {0x03, 0x10},  // CLK_MANAGER_REG03
        {0x16, 0x24},  // ADC_REG16
        {0x04, 0x10},  // CLK_MANAGER_REG04
        {0x05, 0x00},  // CLK_MANAGER_REG05
        {0x0B, 0x00},  // SYSTEM_REG0B
        {0x0C, 0x00},  // SYSTEM_REG0C
        {0x10, 0x1F},  // SYSTEM_REG10
        {0x11, 0x7F},  // SYSTEM_REG11
    };
    for (int i = 0; i < sizeof(pre_reset)/sizeof(pre_reset[0]); i++) {
        ret = es8311_write_reg(i2c_dev, pre_reset[i][0], pre_reset[i][1]);
        if (ret != ESP_OK) return ret;
    }

    // --- Step 2: Software reset ---
    ret = es8311_write_reg(i2c_dev, 0x00, 0x80);
    if (ret != ESP_OK) return ret;
    // Busy-wait 2ms — do NOT use vTaskDelay here. Yielding to the OS during
    // codec init allows LVGL to render a blank frame before create_ui() runs,
    // which leaves the display flush pipeline stuck and causes a blank screen.
    esp_rom_delay_us(2000);

    // --- Step 3: Slave mode, MCLK from input pin ---
    ret = es8311_write_reg(i2c_dev, 0x00, 0x00);  // slave mode (bit6=0), clear reset
    if (ret != ESP_OK) return ret;
    ret = es8311_write_reg(i2c_dev, 0x01, 0x3F);  // use MCLK pin (bit7=0), all clocks on
    if (ret != ESP_OK) return ret;
    ret = es8311_write_reg(i2c_dev, 0x06, 0x00);  // SCLK not inverted (bit5=0), bclk_div later
    if (ret != ESP_OK) return ret;

    // --- Step 4: Clock dividers for MCLK=6.144MHz, rate=16kHz ---
    // pre_div=3 → (3-1)<<5=0x40, pre_multi=2 → 0x08
    ret = es8311_write_reg(i2c_dev, 0x02, 0x48);  // CLK_MANAGER_REG02
    if (ret != ESP_OK) return ret;
    // adc_div=1-1=0, dac_div=1-1=0
    ret = es8311_write_reg(i2c_dev, 0x05, 0x00);  // CLK_MANAGER_REG05
    if (ret != ESP_OK) return ret;
    // fs_mode=0, adc_osr=0x10
    ret = es8311_write_reg(i2c_dev, 0x03, 0x10);  // CLK_MANAGER_REG03
    if (ret != ESP_OK) return ret;
    // dac_osr=0x20
    ret = es8311_write_reg(i2c_dev, 0x04, 0x20);  // CLK_MANAGER_REG04
    if (ret != ESP_OK) return ret;
    // lrck_h=0x00, lrck_l=0xFF → LRCK divider = 255+1 = 256
    // codec_clock = 6.144MHz / 3 * 2 = 4.096MHz; 4.096MHz / 256 = 16kHz ✓
    ret = es8311_write_reg(i2c_dev, 0x07, 0x00);  // CLK_MANAGER_REG07: lrck_h
    if (ret != ESP_OK) return ret;
    ret = es8311_write_reg(i2c_dev, 0x08, 0xFF);  // CLK_MANAGER_REG08: lrck_l
    if (ret != ESP_OK) return ret;
    // bclk_div=4 → (4-1)=3 in bits[3:0]
    ret = es8311_write_reg(i2c_dev, 0x06, 0x03);  // CLK_MANAGER_REG06: bclk_div
    if (ret != ESP_OK) return ret;

    // --- Step 5: Serial digital port — I2S format, 16-bit, DAC enabled ---
    // REG09 (SDPIN): bits[1:0]=00 (I2S/Philips), bits[4:3]=00 (16-bit), bit6=0 (DAC active)
    ret = es8311_write_reg(i2c_dev, 0x09, 0x00);  // SDPIN_REG09: DAC serial port
    if (ret != ESP_OK) return ret;
    // REG0A (SDPOUT): bit6=1 mutes ADC (we only need DAC)
    ret = es8311_write_reg(i2c_dev, 0x0A, 0x40);  // SDPOUT_REG0A: ADC muted
    if (ret != ESP_OK) return ret;

    // --- Step 6: System and analog power-on ---
    ret = es8311_write_reg(i2c_dev, 0x17, 0xBF);  // ADC_REG17: ADC volume
    if (ret != ESP_OK) return ret;
    ret = es8311_write_reg(i2c_dev, 0x0E, 0x02);  // SYSTEM_REG0E: enable DAC output path
    if (ret != ESP_OK) return ret;
    ret = es8311_write_reg(i2c_dev, 0x12, 0x00);  // SYSTEM_REG12: enable DAC
    if (ret != ESP_OK) return ret;
    ret = es8311_write_reg(i2c_dev, 0x14, 0x1A);  // SYSTEM_REG14: analog PGA gain
    if (ret != ESP_OK) return ret;
    ret = es8311_write_reg(i2c_dev, 0x0D, 0x01);  // SYSTEM_REG0D: power on analog module
    if (ret != ESP_OK) return ret;
    ret = es8311_write_reg(i2c_dev, 0x15, 0x40);  // ADC_REG15: ADC ramp rate
    if (ret != ESP_OK) return ret;
    ret = es8311_write_reg(i2c_dev, 0x37, 0x08);  // DAC_REG37: DAC ramp rate enable
    if (ret != ESP_OK) return ret;
    ret = es8311_write_reg(i2c_dev, 0x45, 0x00);  // GP_REG45: GP control
    if (ret != ESP_OK) return ret;
    ret = es8311_write_reg(i2c_dev, 0x44, 0x50);  // GPIO_REG44: DAC→ADC internal reference
    if (ret != ESP_OK) return ret;

    // --- Step 7: Additional system registers (from es8311_open) ---
    ret = es8311_write_reg(i2c_dev, 0x13, 0x10);  // SYSTEM_REG13
    if (ret != ESP_OK) return ret;
    ret = es8311_write_reg(i2c_dev, 0x1B, 0x0A);  // ADC_REG1B: EQ
    if (ret != ESP_OK) return ret;
    ret = es8311_write_reg(i2c_dev, 0x1C, 0x6A);  // ADC_REG1C: EQ/HPF
    if (ret != ESP_OK) return ret;

    // --- Step 8: DAC unmute and volume ---
    ret = es8311_write_reg(i2c_dev, 0x31, 0x00);  // DAC_REG31: unmute DAC (bits[6:5]=00)
    if (ret != ESP_OK) return ret;
    ret = es8311_write_reg(i2c_dev, 0x32, 0xBF);  // DAC_REG32: volume (~0dB)
    if (ret != ESP_OK) return ret;

    // --- Step 9: Enable PA amplifier ---
    gpio_config_t pa_cfg = {.pin_bit_mask = 1ULL << ES8311_PA_PIN, .mode = GPIO_MODE_OUTPUT};
    gpio_config(&pa_cfg);
    gpio_set_level(ES8311_PA_PIN, 1);

    ESP_LOGI(TAG, "ES8311 initialized (16kHz/16-bit/I2S/DAC)");
    return ESP_OK;
}
