/* View ADC Value via LED

   
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/pwm.h"

// Blinking LED pin
#define GPIO_OUTPUT_LED 14
#define GPIO_OUTPUT_PIN_SEL (1ULL << GPIO_OUTPUT_LED)

#define PWM_OUTPUT 12

// Timer
#define TIMER_RELOAD true
#define TIMER_ONCE false

volatile uint32_t pwm_period = 1000;

uint32_t getPwmDuty()
{
    return pwm_period / 3;
}

void setup_pwm()
{
    uint32_t duty[1] = {getPwmDuty()};
    const uint32_t pin[1] = {(uint32_t)PWM_OUTPUT};
    pwm_init(pwm_period, duty, 1, pin);
    pwm_set_phase(0, 0);
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

uint32_t getPwmPeriod(uint16_t adc_data)
{
    return 3 * (20 + 10 * (adc_data / 10));
}

static void sense_task(void *arg)
{
    for (;;)
    {
        uint16_t adc_data;
        if (ESP_OK == adc_read_fast(&adc_data, 1))
        {

            uint32_t current_pwm_period = getPwmPeriod(adc_data);

            if (current_pwm_period != pwm_period)
            {
                pwm_period = current_pwm_period;
                printf("adc %u period %u \n", adc_data, pwm_period);
                pwm_set_period(pwm_period);
                pwm_start();
            }
        }
        vTaskDelay(30 / portTICK_PERIOD_MS);
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
}
