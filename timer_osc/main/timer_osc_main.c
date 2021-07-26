/* Timer oscillator

   
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/hw_timer.h"
#include "esp8266/timer_struct.h"

// Blinking LED pin
#define GPIO_OUTPUT_LED 14
#define GPIO_OUTPUT_PIN_SEL (1ULL << GPIO_OUTPUT_LED)

// Timer
#define TIMER_RELOAD true
#define TIMER_ONCE false

uint32_t ticks = 100;

void setup_gpio()
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    adc_config_t adc_config;

    adc_config.mode = ADC_READ_TOUT_MODE;
    adc_config.clk_div = 8;
    adc_init(&adc_config);
}

void toggle_oscillator_state(void *arg)
{
    static uint32_t curticks = 0;
    static int state = 0;
    curticks++;
    if (curticks > ticks)
    {
        curticks = 0;
        gpio_set_level(GPIO_OUTPUT_LED, (state++) % 2);
    }
}

static void sense_task(void *arg)
{

    for (;;)
    {
        uint16_t adc_data;
        if (ESP_OK == adc_read_fast(&adc_data, 1))
        {
            if (adc_data != ticks)
            {
                ticks = adc_data;
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    printf("Start GPIO Demo\n");

    printf("GPIO Setup...");
    setup_gpio();
    printf("[DONE]\n");

    hw_timer_init(toggle_oscillator_state, NULL);
    hw_timer_alarm_us(100, TIMER_RELOAD);

    printf("Starting sense task...");
    xTaskCreate(sense_task, "sense_task", 2048, NULL, 5, NULL);
    printf("[DONE]\n");
}
