#include "led.h"

void LED_PIN_Initialize(void) {
    gpio_config_t config = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&config);
}

void LED_On(void) {
    gpio_set_level(LED_PIN, 1);
}

void LED_Off(void) {
    gpio_set_level(LED_PIN, 0);
}
