/* GPIO Demo Example


   
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "driver/adc.h"

// Blinking LED pin
#define GPIO_OUTPUT_LED    14
#define GPIO_OUTPUT_PIN_SEL  (1ULL<<GPIO_OUTPUT_LED)

// ADC value
volatile uint16_t adc_input_value = 10;


void setup_gpio(){
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

static void blink_task(void *arg)
{
    TickType_t xLastWakeTime;
    bool led_on; 
    
    led_on = false;
    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        // vTaskDelay(adc_input_value / portTICK_PERIOD_MS);
        // vTaskDelay(adc_input_value / portTICK_PERIOD_MS);
        vTaskDelayUntil( &xLastWakeTime, 1 + adc_input_value / portTICK_PERIOD_MS );

        led_on = !led_on;
        gpio_set_level(GPIO_OUTPUT_LED, led_on);
    }
}

static void log_task(void *arg)
{

    for (;;) {
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}

static void sense_task(void *arg)
{

    for (;;) {
        uint16_t adc_data;
        if (ESP_OK == adc_read(&adc_data)) {
            adc_input_value = adc_data;
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    printf("Start GPIO Demo\n");

    printf("GPIO Setup...");
    setup_gpio();
    printf("[DONE]\n");

    printf("Starting blink task...");
    xTaskCreate(blink_task, "blink_task", 2048, NULL, 20, NULL);
    printf("[DONE]\n");

    printf("Starting log task...");
    xTaskCreate(log_task, "log_task", 2048, NULL, 5, NULL);
    printf("[DONE]\n");

    printf("Starting sense task...");
    xTaskCreate(sense_task, "sense_task", 2048, NULL, 10, NULL);
    printf("[DONE]\n");
}
