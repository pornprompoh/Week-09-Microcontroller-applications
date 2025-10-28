#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void app_main(void)
{
    gpio_set_direction(2, GPIO_MODE_OUTPUT);
    bool led_state = false;
    while(1)
    {
        
        gpio_set_level(2,led_state); // pin = 2 , logic level = 1 => ON
        vTaskDelay(pdMS_TO_TICKS(500));
        led_state = !led_state;
        
    }
}