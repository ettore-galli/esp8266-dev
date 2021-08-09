/* View ADC Value via LED

   
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
#include "esp8266/gpio_struct.h"

#include "driver/pwm.h"

// Blinking LED pin
#define GPIO_OUTPUT_LED 14
#define GPIO_OUTPUT_PIN_SEL (1ULL << GPIO_OUTPUT_LED)

#define PWM_OUTPUT 12

// Timer
#define TIMER_RELOAD true
#define TIMER_ONCE false

volatile uint16_t current_adc_data = 100;
volatile uint32_t pwm_period = 1000;

uint32_t getPwmDuty(){
    return pwm_period / 3;
}

void setup_pwm()
{
    uint32_t duty[1] = {getPwmDuty()};
    const uint32_t pin[1] = {(uint32_t)PWM_OUTPUT};
    pwm_init(pwm_period, duty, 1, pin);
    pwm_start();
    pwm_stop(0);
}

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

void delayUntilMs(TickType_t *xLastWakeTime,
                  const int xTimeIncrementMs)
{
    vTaskDelayUntil(xLastWakeTime, xTimeIncrementMs / portTICK_PERIOD_MS);
}

void perform_flash(int pin, TickType_t *xLastWakeTime, int flashDelayMs)
{
    gpio_set_level(pin, true);
    pwm_start();
    delayUntilMs(xLastWakeTime, flashDelayMs);
    gpio_set_level(pin, false);
    pwm_stop(0);
    delayUntilMs(xLastWakeTime, flashDelayMs);
}

int calculate_adc_data_flashes(uint16_t current_adc_data)
{
    return 1 + current_adc_data / 103;
}

static void sense_task(void *arg)
{

    for (;;)
    {
        uint16_t adc_data;
        if (ESP_OK == adc_read_fast(&adc_data, 1))
        {
            current_adc_data = adc_data;
            // if (adc_data != pwm_period){
            //     pwm_period = adc_data;
            //     pwm_set_period(3*(20 + pwm_period));
            //     pwm_start();
            // }
            
        }
        vTaskDelay(30 / portTICK_PERIOD_MS);
    }
}

static void display_task(void *arg)
{
    int DELAY_BETWEEN_FLASHES = 300;            // ms
    int DELAY_BETWEEN_FLASH_DIGIT_GROUPS = 500; // ms
    int DELAY_BETWEEN_SIGNALS = 3000;           // ms

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        for (int i = 0; i < calculate_adc_data_flashes(current_adc_data); i++)
        {
            perform_flash(GPIO_OUTPUT_LED, &xLastWakeTime, DELAY_BETWEEN_FLASHES);
        }
        delayUntilMs(&xLastWakeTime, DELAY_BETWEEN_SIGNALS);
    }
}

void app_main()
{
    printf("Start GPIO Demo\n");

    printf("GPIO Setup...");
    setup_gpio();
    setup_pwm();
    printf("[DONE]\n");

    printf("Starting sense task...");
    xTaskCreate(sense_task, "sense_task", 2048, NULL, 10, NULL);
    printf("[DONE]\n");

    printf("Starting display task...");
    xTaskCreate(display_task, "sense_task", 2048, NULL, 5, NULL);
    printf("[DONE]\n");
}
