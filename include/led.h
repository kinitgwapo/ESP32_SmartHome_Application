#ifndef LED_H
#define LED_H

// Macro (Pin Definitions)
#define LED_PIN GPIO_NUM_23 // GPIO Pin 23 

// Libraries
#include <driver/gpio.h>

// Public Function Declarations
void LED_PIN_Initialize(void); // Configures LED to Pin 23
void LED_On(void); // Turns On LED
void LED_Off(void); // Turns Off LED

#endif
