#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// เพิ่ม prototypes เพื่อหลีกเลี่ยง implicit declaration / conflicting types
static void pattern_chase(const int leds[], int n, TickType_t step_delay, int steps);
static void pattern_all_blink(const int leds[], int n, TickType_t step_delay, int cycles);
static void pattern_pingpong(const int leds[], int n, TickType_t step_delay, int steps);
static void pattern_binary_count(const int leds[], int n, TickType_t step_delay, int rounds);

void app_main(void)
{
    const int leds[] = {4, 5, 18};
    const int n = sizeof(leds) / sizeof(leds[0]);

    for (int i = 0; i < n; ++i) {
        gpio_set_direction(leds[i], GPIO_MODE_OUTPUT);
        gpio_set_level(leds[i], 0); // init LOW
    }

    const TickType_t step = pdMS_TO_TICKS(200);
    const TickType_t mode_duration = pdMS_TO_TICKS(3000); // เวลาโดยรวมต่อโหมด
    while (1) {
        // โหมด 1: chase (run enough steps to fill mode_duration)
        pattern_chase(leds, n, step, (int)(mode_duration / (n * step)));
        // โหมด 2: all blink
        pattern_all_blink(leds, n, step, (int)(mode_duration / (2 * step)));
        // โหมด 3: ping-pong
        pattern_pingpong(leds, n, step, (int)(mode_duration / ( (2*n-2) * step )));
        // โหมด 4: binary counter
        pattern_binary_count(leds, n, step, (int)(mode_duration / ((1<<n) * step)));
    }
}

static void pattern_chase(const int leds[], int n, TickType_t step_delay, int steps)
{
    for (int s = 0; s < steps; ++s) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                gpio_set_level(leds[j], (j == i) ? 1 : 0);
            }
            vTaskDelay(step_delay);
        }
    }
}

static void pattern_all_blink(const int leds[], int n, TickType_t step_delay, int cycles)
{
    for (int c = 0; c < cycles; ++c) {
        for (int i = 0; i < n; ++i) gpio_set_level(leds[i], 1);
        vTaskDelay(step_delay);
        for (int i = 0; i < n; ++i) gpio_set_level(leds[i], 0);
        vTaskDelay(step_delay);
    }
}

static void pattern_pingpong(const int leds[], int n, TickType_t step_delay, int steps)
{
    for (int s = 0; s < steps; ++s) {
        // forward
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) gpio_set_level(leds[j], (j == i) ? 1 : 0);
            vTaskDelay(step_delay);
        }
        // backward (exclude ends to avoid duplicate)
        for (int i = n - 2; i > 0; --i) {
            for (int j = 0; j < n; ++j) gpio_set_level(leds[j], (j == i) ? 1 : 0);
            vTaskDelay(step_delay);
        }
    }
}

static void pattern_binary_count(const int leds[], int n, TickType_t step_delay, int rounds)
{
    int max = 1 << n;
    for (int r = 0; r < rounds; ++r) {
        for (int v = 0; v < max; ++v) {
            for (int b = 0; b < n; ++b) {
                gpio_set_level(leds[b], (v >> b) & 1);
            }
            vTaskDelay(step_delay);
        }
    }
}
